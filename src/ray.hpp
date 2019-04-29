#pragma once

#include "common.hpp"

/** Class that represents a physical ray, that has a starting point and a direction. */
class CRay
{
public:
    /** Constructor
        \param Origin starting location of the ray in the world.
        \param Direction direction vector of the ray.
    */
    CRay(Vec3 Origin, Vec3 Direction);

    Vec3& Origin();
    Vec3 Origin() const;

    Vec3& Direction();
    Vec3 Direction() const;

    /** Returns a position along the ray given a parameter.
        
        The position returned is P(t) = A+V*t.
        
        \param tVal the parameter.
    */
    Vec3 PointAt(r32 tVal) const;
private:
    Vec3 m_Origin;
    Vec3 m_Direction;
};
