#pragma once

#include "common.hpp"
#include "ray.hpp"
#include "material.hpp"

class IShape;

/** Stores the hit information of a ray-shape collision. */
struct SHitInfo
{
    r32 tVal; /**< The t parameter of the ray where the collision occured. */
    Vec3 Point; /**< The world coordinate where the collision occured. */
    Vec3 Normal; /**< The surface normal of the object where the collision occured. */
    const IShape* Shape; /**< Pointer to the object which the ray collided with. **/
};

/** Generic shape interface. */
class IShape : public IStringSerializable
{
public:
    /** Constructor 
        \param Material pointer to the shape's material.
    */
    IShape(std::shared_ptr<IMaterial> Material);

    virtual ~IShape() = 0;

    /** Returns the shape's material pointer. */
    const std::shared_ptr<IMaterial>& GetMaterial() const;

    /** Returns the UV coordinates of the object at a given point. */
    virtual void GetUV(Vec3 Point, r32& U, r32& V) const = 0;

    /** Checks whether a ray intersects with the shape. 
        \param Ray the ray to check the intersection with.
        \param tMin the minimum t parameter of the ray to consider for intersection.
        \param tMax the maximum t paramter of the ray to consided for intersection.
        \param HitInfo reference to the object which will store the collision information.
    */
    virtual bool Intersect(const CRay& Ray, r32 tMin, r32 tMax, SHitInfo& HitInfo) const = 0;

protected:
    std::shared_ptr<IMaterial> m_Material; /**< Pointer to the shape's material */
};