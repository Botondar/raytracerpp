#include "image.hpp"

#include <stdexcept>

CImage::CImage(s32 Width, s32 Height) :
    m_Width(Width),
    m_Height(Height)
{
    if(Width <= 0 || Height <= 0)
    {
        throw std::invalid_argument("Invalid image width or height");
    }

    m_pPixels = new UColor[m_Width*m_Height];
}

CImage::CImage(const CImage& Other) :
    m_Width(Other.m_Width),
    m_Height(Other.m_Height)
{
    m_pPixels = new UColor[m_Width*m_Height];

    //std::copy(Other.m_pPixels, Other.m_pPixels + m_Width * m_Height, m_pPixels);
    memcpy(m_pPixels, Other.m_pPixels, sizeof(UColor) * size_t(m_Width*m_Height));
}

CImage::~CImage()
{
    delete[] m_pPixels;
}

CImage& CImage::operator=(const CImage& Other)
{
    if(this != &Other)
    {
        m_Width = Other.m_Width;
        m_Height = Other.m_Height;

        delete[] m_pPixels;
        m_pPixels = new UColor[m_Width*m_Height];

        //std::copy(Other.m_pPixels, Other.m_pPixels + m_Width * m_Height, m_pPixels);
        memcpy(m_pPixels, Other.m_pPixels, sizeof(UColor) * size_t(m_Width*m_Height));
    }
    return *this;
}

UColor& CImage::operator()(s32 X, s32 Y)
{
    if(X < 0 || X >= m_Width ||
       Y < 0 || Y >= m_Height)
    {
        throw std::out_of_range("Invalid pixel range");
    }

    return m_pPixels[X + Y * m_Width];
}

UColor CImage::operator()(s32 X, s32 Y) const
{
    if(X < 0 || X >= m_Width ||
       Y < 0 || Y >= m_Height)
    {
        throw std::out_of_range("Invalid pixel range");
    }

    return m_pPixels[X + Y * m_Width];
}

s32 CImage::Width() const
{
    return m_Width;
}
s32 CImage::Height() const
{
    return m_Height;
}

UColor* CImage::Pixels()
{
    return m_pPixels;
}

const UColor* CImage::Pixels() const
{
    return m_pPixels;
}

void CImage::Resize(s32 Width, s32 Height)
{
    if(Width <= 0 || Height <= 0)
    {
        throw std::invalid_argument("Invalid image width or height");
    }

    m_Width = Width;
    m_Height = Height;

    delete[] m_pPixels;
    m_pPixels = new UColor[Width*Height];
}

Vec3 CImage::Sample(r32 U, r32 V) const
{
    U = U - floorf(U);
    V = V - floorf(V);

    s32 PixelX = lroundf(U * (m_Width - 1));
    s32 PixelY = lroundf(V * (m_Height - 1));
    return U32ToVec3((*this)(PixelX, PixelY).Color);
}

void CImage::Blit(const CImage& Image, s32 OffsetX, s32 OffsetY)
{
    s32 StartX = std::max(OffsetX, 0);
    s32 StartY = std::max(OffsetY, 0);

    s32 EndX = std::min(OffsetX + Image.Width(), m_Width);
    s32 EndY = std::min(OffsetY + Image.Height(), m_Height);

    for(s32 DestY = StartY; DestY < EndY; DestY++)
    {
        for(s32 DestX = StartX; DestX < EndX; DestX++)
        {
            s32 SrcX = DestX - OffsetX;
            s32 SrcY = DestY - OffsetY;

            (*this)(DestX, DestY) = Image(SrcX, SrcY);
        }
    }
}

std::istream& CImage::Read(std::istream& Stream)
{
    // TODO(boti)
    SBitmapFileHeader FileHeader = {};
    SBitmapInfoHeader InfoHeader = {};

    Stream.read(reinterpret_cast<char*>(&FileHeader.Type), sizeof(FileHeader.Type));
    // 0x4D42 = 'MB'
    if(FileHeader.Type != 0x4D42)
    {
        throw std::invalid_argument("Invalid .bmp file");
    }

    Stream.read(reinterpret_cast<char*>(&FileHeader.Size), sizeof(FileHeader) - sizeof(FileHeader.Type));
    Stream.read(reinterpret_cast<char*>(&InfoHeader), sizeof(InfoHeader));

    if(InfoHeader.BitCount != 24)
    {
        throw std::invalid_argument("Invalid bit count in .bmp");
    }

    m_Width = InfoHeader.Width;
    m_Height = InfoHeader.Height;

    // Some .bmp files are bottom-up
    if(m_Height < 0)
        m_Height = -m_Height;

    delete[] m_pPixels;
    m_pPixels = new UColor[m_Width*m_Height];

    for(s32 i = 0; i < m_Width*m_Height; i++)
    {
        u8 R, G, B;
        Stream.read(reinterpret_cast<char*>(&B), sizeof(u8));
        Stream.read(reinterpret_cast<char*>(&G), sizeof(u8));
        Stream.read(reinterpret_cast<char*>(&R), sizeof(u8));
        m_pPixels[i].Color = RGBToU32(R, G, B);
    }

    return Stream;
}

std::ostream& CImage::Write(std::ostream& Stream) const
{
    SBitmapFileHeader FileHeader = {};
    FileHeader.Type = 0x4D42; // 'MB'
    FileHeader.Size = sizeof(SBitmapFileHeader) + sizeof(SBitmapInfoHeader) + 3 * m_Width * m_Height;
    FileHeader.Offset = sizeof(SBitmapFileHeader) + sizeof(SBitmapInfoHeader);
    SBitmapInfoHeader InfoHeader = {};
    InfoHeader.Size = sizeof(SBitmapInfoHeader);
    InfoHeader.Width = m_Width;
    InfoHeader.Height = m_Height;
    InfoHeader.Planes = 1;
    InfoHeader.BitCount = 24; // .bmp files don't handle Alpha well, so we just skip it
    InfoHeader.Compression = 0; // BI_RGB, uncompressed
    InfoHeader.ImageSize = 0; // Uncompressed files can set the ImageSize to 0
    InfoHeader.PixelsPerMeterX = 0;
    InfoHeader.PixelsPerMeterY = 0;
    InfoHeader.ClrUsed = 0;
    InfoHeader.ClrImportant = 0;

    Stream.write(reinterpret_cast<char*>(&FileHeader), sizeof(FileHeader));
    Stream.write(reinterpret_cast<char*>(&InfoHeader), sizeof(InfoHeader));
    for(s32 i = 0; i < m_Width*m_Height; i++)
    {
        UColor Pixel = m_pPixels[i];

        // .bmp files are in BGR format
        Stream.write(reinterpret_cast<char*>(&Pixel.Components.Blue), sizeof(u8));
        Stream.write(reinterpret_cast<char*>(&Pixel.Components.Green), sizeof(u8));
        Stream.write(reinterpret_cast<char*>(&Pixel.Components.Red), sizeof(u8));
    }

    return Stream;
}
