#pragma once

#include "shape.hpp"


/** Shape that represents a plane in the world.
    
    Equation for the plane is Nx*x + Ny*y +Nz*z = offset
*/
class CPlane : public IShape
{
public:
    /** Constructor 
        \param Normal the surface normal of the plane.
        \param Offset the value how far along the plane is on the normal relative to the world origin.
        \param TextureUp world vector which defines which axis corresponds to the texture's vertical axis.
        \param Material the material of the plane.
    */
    CPlane(Vec3 Normal, 
           r32 Offset,
           Vec3 TextureUp = Vec3(0.0f, 0.0f, -1.0f), 
           CSharedPointer<IMaterial> Material = nullptr);
    virtual ~CPlane();

    virtual void GetUV(Vec3 Point, r32& U, r32& V) const;

    virtual bool Intersect(const CRay& Ray, r32 tMin, r32 tMax, SHitInfo& HitInfo) const;

    virtual std::string& ReadFromString(std::string& String);
    virtual void WriteToString(std::string& String) const;
private:
    Vec3 m_Normal;
    r32 m_Offset;

    Vec3 m_TextureUp;
    Vec3 m_TextureRight;
};
