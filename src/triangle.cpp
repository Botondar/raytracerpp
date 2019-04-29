#include "triangle.hpp"


CTriangle::CTriangle(CSharedPointer<IMaterial> Material,
                     Vec3 P0, Vec3 P1, Vec3 P2,
                     bool bCustomNormals,
                     Vec3 N0, Vec3 N1, Vec3 N2) :
    IShape(Material),
    m_P0(P0), m_P1(P1), m_P2(P2)

{
    if(bCustomNormals)
    {
        m_N0 = N0;
        m_N1 = N1;
        m_N2 = N2;
    }
    else
    {
        Vec3 Edge0 = P1 - P0;
        Vec3 Edge1 = P2 - P0;

        Vec3 Normal = Normalize(Cross(Edge0, Edge1));
        m_N0 = Normal;
        m_N1 = Normal;
        m_N2 = Normal;
    }
}

CTriangle::~CTriangle()
{

}

void CTriangle::GetUV(Vec3 Point, r32& U, r32& V) const
{
    // TODO(boti)
}

// https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool CTriangle::Intersect(const CRay& Ray, r32 tMin, r32 tMax, SHitInfo& HitInfo) const
{
    const r32 Bias = 0.0001f;
    const Vec3 Edge0 = m_P1 - m_P0;
    const Vec3 Edge1 = m_P2 - m_P0;

    const Vec3 CrossRayEdge1 = Cross(Ray.Direction(), Edge1);
    const r32 CosTheta = Dot(Edge0, CrossRayEdge1);
    if(CosTheta > -Bias && CosTheta < Bias)
    {
        return false;
    }

    const Vec3 P0ToRay = Ray.Origin() - m_P0;

    r32 U = Dot(P0ToRay, CrossRayEdge1) / CosTheta;
    if(U < 0.0f || U > 1.0f)
    {
        return false;
    }

    r32 V = Dot(Ray.Direction(), Cross(P0ToRay, Edge0)) / CosTheta;
    if(V < 0.0f || U + V > 1.0f)
    {
        return false;
    }

    r32 tVal = Dot(Edge1, Cross(P0ToRay, Edge0)) / CosTheta;

    if(tVal < tMin || tVal > tMax)
    {
        return false;
    }


    HitInfo.tVal = tVal;
    HitInfo.Point = Ray.PointAt(tVal);
    HitInfo.Normal = m_N0; // TODO(boti): normal interpolation
    HitInfo.Shape = this;
    return true;
}

std::string& CTriangle::ReadFromString(std::string& String)
{
    return String;
}

void CTriangle::WriteToString(std::string& String) const
{

}