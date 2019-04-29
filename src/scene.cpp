#include "scene.hpp"

#include "common.hpp"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

CScene::CScene()
{
}
CScene::~CScene()
{

}

void CScene::SetCamera(CCamera Camera)
{
    m_Camera = Camera;
}

void CScene::AddShape(IShape* Shape)
{
    m_ShapeList.PushBack(Shape);
}

bool CScene::Intersect(const CRay& Ray, r32 tMin, r32 tMax, SHitInfo& HitInfo) const
{
    bool bIntersect = false;
    for(size_t i = 0; i < m_ShapeList.Size(); i++)
    {
        if(m_ShapeList[i]->Intersect(Ray, tMin, tMax, HitInfo))
        {
            tMax = std::min(HitInfo.tVal, tMax);
            bIntersect = true;
        }
    }
    return bIntersect;
}

Vec3 CScene::TraceRay(CRay Ray, u32 MaxDepth) const
{
    Vec3 Color(1.0f, 1.0f, 1.0f);

    SHitInfo HitInfo = {};
    for(u32 Depth = 0; Depth < MaxDepth; Depth++)
    {
        if(Intersect(Ray, 0.001f, FLT_MAX, HitInfo))
        {
            Vec3 ObjectColor = HitInfo.Shape->GetMaterial()->GetColor();

            Vec3 TextureColor = Vec3(1.0f, 1.0f, 1.0f);
            if(!HitInfo.Shape->GetMaterial()->GetTexture().IsNull())
            {
                r32 U, V;
                HitInfo.Shape->GetUV(HitInfo.Point, U, V);
                TextureColor = HitInfo.Shape->GetMaterial()->GetTexture()->Sample(U, V);
            }
            ObjectColor *= TextureColor;

            Color *= ObjectColor;

            Ray = HitInfo.Shape->GetMaterial()->Scatter(Ray, HitInfo.Point, HitInfo.Normal);
        }
        else
        {
            r32 SkyLerpVal = 0.5f*(Dot(Normalize(Ray.Direction()), m_Camera.GetUp()) + 1.0f);
            Color *= Lerp(Vec3(1.0f, 1.0f, 1.0f), Vec3(0.5f, 0.7f, 1.0f), SkyLerpVal);
            break;
        }
    }

    return Color;
}


void CScene::RenderRegion(SRegion& Region, SRenderParams& Params, SSharedRenderData& Shared) const
{   
    // we cache these, because our parameters are not const
    const s32 RenderWidth = Region.Image.Width();
    const s32 RenderHeight = Region.Image.Height();

    u32 PixelsProcessed = 0;
    for(s32 Y = 0; Y < RenderHeight; Y++)
    {
        for(s32 X = 0; X < RenderWidth; X++)
        {
            r32 RX = r32(X + Region.OffsetX);
            r32 RY = r32(Y + Region.OffsetY);

            Vec3 PixelColor;
            for(u32 Sample = 0; Sample < Params.SampleCount; Sample++)
            {
                r32 U = 2.0f * (RX + RandomNormalized()) / Params.FullRenderWidth - 1.0f;
                r32 V = 2.0f * (RY + RandomNormalized()) / Params.FullRenderHeight - 1.0f;

                CRay Ray = m_Camera.RayFromUV(U, V, Params.AspectRatio);
                PixelColor += TraceRay(Ray, Params.MaxDepth);

            }
            PixelColor /= r32(Params.SampleCount);

            PixelColor.X = Clamp(PixelColor.X, 0.0f, 1.0f);
            PixelColor.Y = Clamp(PixelColor.Y, 0.0f, 1.0f);
            PixelColor.Z = Clamp(PixelColor.Z, 0.0f, 1.0f);

            // Gamma correction
            PixelColor.X = pow(PixelColor.X, 1.0f / 2.2f);
            PixelColor.Y = pow(PixelColor.Y, 1.0f / 2.2f);
            PixelColor.Z = pow(PixelColor.Z, 1.0f / 2.2f);

            Region.Image(X, Y).Color = Vec3ToU32(PixelColor);
        }

        PixelsProcessed = Shared.PixelsProcessed.load();
        PixelsProcessed += RenderWidth;
        Shared.PixelsProcessed.store(PixelsProcessed);

        Shared.PrintMutex.lock();
        std::cout << 
            "\rRendering: " <<
            std::setprecision(0) << std::fixed <<
            100.0f*PixelsProcessed / (Params.FullRenderWidth*Params.FullRenderHeight) << "%";
        Shared.PrintMutex.unlock();
    }
}

void CScene::Render(CImage& Image, SRenderParams& Params) const
{
    Params.FullRenderWidth = Image.Width();
    Params.FullRenderHeight = Image.Height();
    Params.AspectRatio = r32(Image.Width()) / r32(Image.Height());

    SSharedRenderData Shared;
    Shared.PixelsProcessed.store(0);

    s32 RegionWidth = Image.Width();
    s32 RegionHeight = Image.Height();
    s32* RegionToDivide = &RegionWidth;

    u32 RegionCount = 1;
    for(u32 Div = Params.MaxThreadCount; 
        Div > 1 && 
        RegionWidth > 1 && 
        RegionHeight > 1; 
        Div /= 2)
    {
        *RegionToDivide /= 2;
        RegionToDivide = (RegionToDivide == &RegionWidth) ? &RegionHeight : &RegionWidth;
        RegionCount *= 2;
    }

    std::cout << "Starting Render:\n" <<
        " - ThreadCount: " << RegionCount << '\n' <<
        " - SampleCount: " << Params.SampleCount << '\n' <<
        " - Depth: " << Params.MaxDepth << '\n' <<
        " - Resolution: " << Params.FullRenderWidth << 'x' << Params.FullRenderHeight << "\n\n";

    auto StartTime = std::chrono::high_resolution_clock::now();

#ifdef MULTITHREAD
    std::thread* ThreadPool = new std::thread[RegionCount];
    SRegion* Regions = new SRegion[RegionCount];
    s32 OffsetX = 0;
    s32 OffsetY = 0;
    for(u32 i = 0; i < RegionCount; i++)
    {
        Regions[i].Image.Resize(RegionWidth, RegionHeight);
        Regions[i].OffsetX = OffsetX;
        Regions[i].OffsetY = OffsetY;
        ThreadPool[i] = std::thread(&CScene::RenderRegion, this,
                                    std::ref(Regions[i]), 
                                    std::ref(Params),
                                    std::ref(Shared));

        OffsetX += RegionWidth;
        if(OffsetX >= Image.Width())
        {
            OffsetX = 0;
            OffsetY += RegionHeight;
        }
    }

    for(u32 i = 0; i < RegionCount; i++)
    {
        ThreadPool[i].join();
        Image.Blit(Regions[i].Image, Regions[i].OffsetX, Regions[i].OffsetY);
    }
    


    delete[] ThreadPool;
    delete[] Regions;
#else
    SRegion Region;
    Region.Image = Image;
    Region.OffsetX = 0;
    Region.OffsetY = 0;

    RenderRegion(Region, Params, Shared);
    Image.Blit(Region.Image, Region.OffsetX, Region.OffsetY);
    
#endif
    auto EndTime = std::chrono::high_resolution_clock::now();

    std::cout << "\rRendering: 100%\n\n";

    std::cout << "Render duration: " << 
        std::setprecision(2) << std::fixed <<
        std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime).count() / 1000.0 << "s\n";
}

std::istream& CScene::Read(std::istream& Stream)
{
    std::string Json;

    std::streampos Pos = Stream.tellg();
    Stream.seekg(0, std::ios_base::end);
    std::streamoff Size = Stream.tellg() - Pos;
    Stream.seekg(Pos);

    Json.resize(size_t(Size));
    Stream.read(&Json[0], Size);

    Json.erase(std::remove_if(Json.begin(), Json.end(), isspace), Json.end());

    while(!Json.empty() && Json[0] != '\0')
    {
        std::string Name = ExtractQuote(Json);
        std::string Contents = ExtractBraceContents(Json);

        if(Name == "sphere")
        {
            CSphere* Sphere = new CSphere(Vec3(0.0f, 0.0f, 0.0f), 0.0f);
            Sphere->ReadFromString(Contents);
            AddShape(Sphere);
        }
        else if(Name == "plane")
        {
            CPlane* Plane = new CPlane(Vec3(0.0f, 0.0f, 0.0f), 0.0f);
            Plane->ReadFromString(Contents);
            AddShape(Plane);
        }
        else if(Name == "camera")
        {
            m_Camera.ReadFromString(Contents);
        }
        else
        {
            throw;
        }
    }

    return Stream;
}
std::ostream& CScene::Write(std::ostream& Stream) const
{
    std::string String;
    m_Camera.WriteToString(String);
    for(size_t i = 0; i < m_ShapeList.Size(); i++)
    {
        m_ShapeList[i]->WriteToString(String);
    }
    Stream << String;
    return Stream;
}