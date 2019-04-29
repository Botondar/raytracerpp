#include "ray.hpp"

CRay::CRay(Vec3 Origin, Vec3 Direction) :
    m_Origin(Origin),
    m_Direction(Normalize(Direction))
{

}

Vec3& CRay::Origin()
{
    return m_Origin;
}
Vec3 CRay::Origin() const
{
    return m_Origin;
}

Vec3& CRay::Direction()
{
    return m_Direction;
}
Vec3 CRay::Direction() const
{
    return m_Direction;
}

Vec3 CRay::PointAt(r32 tVal) const
{
    return m_Origin + tVal * m_Direction;
}
