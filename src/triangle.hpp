#pragma once

#include "shape.hpp"

class CTriangle : public IShape
{
public:
    CTriangle(CSharedPointer<IMaterial> Material,
              Vec3 P0, Vec3 P1, Vec3 P2,
              bool bCustomNormals = false,
              Vec3 N0 = Vec3(), Vec3 N1 = Vec3(), Vec3 N2 = Vec3());

    virtual ~CTriangle();

    virtual void GetUV(Vec3 Point, r32& U, r32& V) const;

    virtual bool Intersect(const CRay& Ray, r32 tMin, r32 tMax, SHitInfo& HitInfo) const;

    virtual std::string& ReadFromString(std::string& String);
    virtual void WriteToString(std::string& String) const;
private:
    Vec3 m_P0, m_P1, m_P2;
    Vec3 m_N0, m_N1, m_N2;
};