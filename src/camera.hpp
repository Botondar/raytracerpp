#pragma once

#include "common.hpp"
#include "ray.hpp"
#include "serializable.hpp"

/** Camera class */
class CCamera : public IStringSerializable
{
public:
    /** Constructor
        \param FieldOfView the camera's field of view on its vertical axis. Must be in radians.
        \param Position the camera's position in the world.
        \param Forward the direction where the camera's facing.
        \param Up the up axis of the world.
    */
    CCamera(r32 FieldOfView = (Pi32 / 4.0f),
            Vec3 Position   = Vec3(0.0f, 0.0f, 0.0f),
            Vec3 Forward    = Vec3(0.0f, 0.0f, -1.0f),
            Vec3 Up         = Vec3(0.0f, 1.0f, 0.0f));

    void SetPosition(Vec3 Position);
    void SetForward(Vec3 Forward);
    void SetUp(Vec3 Up);

    Vec3 GetPosition() const;
    Vec3 GetForward() const;
    Vec3 GetUp() const;

    /** Creates a ray from the camera to a given pixel.
        \param U normalized horizontal pixel coordinate (0 means left, 1 means the right edge).
        \param V normalized vertical pixel coordinate (0 means bottom, 1 means upper edge).
        \param AspectRatio the image's width divided by its height.
    */
    CRay RayFromUV(r32 U, r32 V, r32 AspectRatio = 1.0f) const;

    /** Serializes the camera from a string (json format). */
    virtual std::string& ReadFromString(std::string& String) override;
    /** Serlializes the camera to a string (json format). */
    virtual void WriteToString(std::string& String) const override;
private:
    r32 m_FieldOfView; /**< The camera's vertical field of view. */

    Vec3 m_Position; /**< The camera's position in the world. */
    Vec3 m_Forward; /**< The direction where the camera's pointing. */
    Vec3 m_Up; /**< The world's up axis. */
};
