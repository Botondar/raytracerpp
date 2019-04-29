#pragma once

#include "common.hpp"
#include "color.hpp"
#include "serializable.hpp"

#pragma pack(push, 1)
/** Struct which holds the .bmp file information. 
    See https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/ns-wingdi-tagbitmapfileheader
*/
struct SBitmapFileHeader
{
    u16 Type;
    u32 Size;
    u16 Reserved1;
    u16 Reserved2;
    u32 Offset;
};

/** Struct which holds the .bmp image information. 
    See https://docs.microsoft.com/en-us/windows/desktop/api/wingdi/ns-wingdi-tagbitmapinfoheader
*/
struct SBitmapInfoHeader
{
    u32 Size;
    s32 Width;
    s32 Height;
    u16 Planes;
    u16 BitCount;
    u32 Compression;
    u32 ImageSize;
    s32 PixelsPerMeterX;
    s32 PixelsPerMeterY;
    u32 ClrUsed;
    u32 ClrImportant;
};
#pragma pack(pop)

/** Image class that stores pixel data and allows for sampling. */
class CImage : public ISerializable
{
public:
    /** Constructor
        \param Width width of the image. Must be greater than 0.
        \param Height height of the image. Must be greate than 0.
    */
    CImage(s32 Width = 1, s32 Height = 1);
    CImage(const CImage& Other);
    virtual ~CImage();

    CImage& operator=(const CImage& Other);

    /** Returns the pixel at (X, Y).
        Throws out_of_range exception if X or Y are invalid.
    */
    UColor& operator()(s32 X, s32 Y);
    /** Returns the pixel at (X, Y).
        Throws out_of_range exception if X or Y are invalid.
    */
    UColor operator()(s32 X, s32 Y) const;

    s32 Width() const;
    s32 Height() const;

    /** Returns the raw pointer to the pixel data. */
    UColor* Pixels();
    /** Returns the raw pointer to the pixel data. */
    const UColor* Pixels() const;

    /** Resizes the image to a new resolution 
        \param Width the new width of the image. Must be greater than 0.
        \param Height the new height of the iamge. Must be reater than 0.
    */
    void Resize(s32 Width, s32 Height);
    /** Returns a normalized color from the normalized image coordinates.
        \param U normalized horizontal coordinate ranging from [0..1).
        \param V normalized vertical coordinate ranging from [0..1).
    */
    Vec3 Sample(r32 U, r32 V) const;

    /** Copies an image to another at a given location.
        \param Image the image to copy to this image.
        \param OffsetX the X coordinate at which to start the copy.
        \param OffsetY the Y coordinate at which to start the copy.
    */
    void Blit(const CImage& Image, s32 OffsetX, s32 OffsetY);

    /** Reads the image in .bmp format from a stream.
        \param Stream the stream from which to read. Must be binary.
    */
    virtual std::istream& Read(std::istream& Stream);
    /** Writes the image in .bmp format to a stream.
        \param Stream the stream to write. Must be binary.
    */
    virtual std::ostream& Write(std::ostream& Stream) const;
private:
    s32 m_Width; /**< Width of the image in pixels. */
    s32 m_Height; /**< Height of the image in pixels. */
    UColor* m_pPixels; /**< Pixel/color data of the image. */
};
