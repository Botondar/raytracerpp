#pragma once

#include "shape.hpp"
/** Shape class that represents a sphere in the world. */
class CSphere : public IShape
{
public:
    /** Constructor 
        \param Center the location of the sphere.
        \param Radius the radius of the sphere.
        \param Material the material of the sphere.
    */
    CSphere(Vec3 Center, r32 Radius, std::shared_ptr<IMaterial> Material = nullptr);
    virtual ~CSphere();

    virtual void GetUV(Vec3 Point, r32& U, r32& V) const;

    virtual bool Intersect(const CRay& Ray, r32 tMin, r32 tMax, SHitInfo& HitInfo) const;

    virtual std::string& ReadFromString(std::string& String);
    virtual void WriteToString(std::string& String) const;

private:
    Vec3 m_Center; /**< The location of the sphere. */
    r32 m_Radius; /**< The radius of the sphere. */
};
