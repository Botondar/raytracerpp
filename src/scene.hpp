#pragma once

#include "heterostore.hpp"
#include "ray.hpp"
#include "image.hpp"
#include "camera.hpp"

#include "shape.hpp"
#include "plane.hpp"
#include "sphere.hpp"

/** A region of the final render. */
struct SRegion
{
    CImage Image; /**< The image of the region. */
    s32 OffsetX; /**< Horizontal coordinate in the final image. */
    s32 OffsetY; /**< Vertical coordinate in the final image. */
};

/** The parameters of the render. */
struct SRenderParams
{
    u32 SampleCount; /**< The number of pixels to shoot per pixel. */
    u32 MaxDepth; /**< The maximum number of times a ray can bounce. */
    u32 MaxThreadCount; /**< The maximum number of threads the app can create.*/

    // Internal for CScene::Render, only used for CScene::RenderRegion:
    r32 AspectRatio; /**< The aspect ratio of the output image. Internal use. */
    s32 FullRenderWidth; /**< The width of the final image. Internal use. */
    s32 FullRenderHeight; /**< The height of the final image. Internal use. */
};

/** Multithread data of the render. */
struct SSharedRenderData
{
    std::atomic<u32> PixelsProcessed; /**< The number of pixels processed by the threads. */
    std::mutex PrintMutex; /**< Lock for the output stream. */
};

/** Scene class that contains the objects, camera and renders the image. */
class CScene : public ISerializable
{
public:
    CScene();
    ~CScene();

    void SetCamera(CCamera Camera);

    /** Adds a shape to the scene. */
    void AddShape(std::shared_ptr<IShape> Shape);

    /** Renders a subregion of the final image.
        \param Region the region to render.
        \param Params the render parameters.
        \param Shared the data shared between threads.
    */
    void RenderRegion(SRegion& Region, SRenderParams& Params, SSharedRenderData& Shared) const;
    /** Renders the scene to an image. 
        \param Image the image to render to.
        \param Params the render parameters.
    */
    void Render(CImage& Image, SRenderParams& Params) const;

    /** Checks whether a ray intersects any objects in the scene.  */
    bool Intersect(const CRay& Ray, r32 tMin, r32 tMax, SHitInfo& HitInfo) const;

    /** Reads the scene from a stream in json format. */
    virtual std::istream& Read(std::istream& Stream);
    /** Writes the scene to a stream in json format. */
    virtual std::ostream& Write(std::ostream& Stream) const;
private:
    CCamera m_Camera; /**< The camera whose point of view to render the image from. */
    CHeteroStore<IShape> m_ShapeList; /**< The container that holds the shapes in the scene. */
    
    /** Returns the color from a ray simulation. */
    Vec3 TraceRay(CRay Ray, u32 MaxDepth = UINT32_MAX) const;
};
