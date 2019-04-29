#pragma once

#include "common.hpp"
#include "sharedpointer.hpp"
#include "ray.hpp"
#include "image.hpp"

/** Generic material interface. 
    Stores an object's color, texture and the reflection function.
*/
class IMaterial : public IStringSerializable
{
public:
    /** Constructor
        \param Color the material's diffuse color.
        \param Texture pointer to a CImage which contains additional color information.
    */
    IMaterial(Vec3 Color, CSharedPointer<CImage> Texture = nullptr);

    /** Returns the material's color. */
    Vec3 GetColor() const;
    
    /** Returns a pointer to material's texture. */
    const CSharedPointer<CImage>& GetTexture() const;

    /** Reflects the vector given the surface coordinate and its normal.
        \param Ray the ray to reflect.
        \param Position the coordinate where the collision occured.
        \param Normal the surface normal of the shape where the collision occured.
    */
    virtual CRay Scatter(const CRay& Ray, Vec3 Position, Vec3 Normal) const = 0;
protected:
    Vec3 m_Color;
    CSharedPointer<CImage> m_Texture;
};

/** Diffuse (matte) material */
class CMaterialDiffuse : public IMaterial
{
public:
    /** See IMaterial */
    CMaterialDiffuse(Vec3 Color, CSharedPointer<CImage> Texture = nullptr);

    /** Scatters the ray in a random direction. */
    virtual CRay Scatter(const CRay& Ray, Vec3 Position, Vec3 Normal) const;

    virtual std::string& ReadFromString(std::string& String);
    virtual void WriteToString(std::string& String) const;
};

/** Metallic material which reflects rays in a mirror-life fashion. */
class CMaterialMetal : public IMaterial
{
public:
    /** See IMaterial
        \param Color the color of the material.
        \param Fuzziness randomizes the direction of the reflection. 0 means perfectly clear metal.
        \param Texture the texture of the material.
    */
    CMaterialMetal(Vec3 Color, r32 Fuzziness = 0.0f, CSharedPointer<CImage> Texture = nullptr);

    /** Mathematically reflects ray, randomizing it by the metal's fuzziness. */
    virtual CRay Scatter(const CRay& Ray, Vec3 Position, Vec3 Normal) const;

    virtual std::string& ReadFromString(std::string& String);
    virtual void WriteToString(std::string& String) const;
protected:
    r32 m_Fuzziness; /**< Parameter which controls how fuzzy the metal is. 0 means perfectly clear. */
};

/** Dielectric material which rays pass through. */
class CMaterialDielectric : public IMaterial
{
public:
    /** See IMaterial
        \param Color the color of the material.
        \param RefractiveIndex the physical refractive index of the material.
        \param Texture the texture of the material.
    */
    CMaterialDielectric(Vec3 Color, r32 RefractiveIndex, CSharedPointer<CImage> Texture = nullptr);

    /** Returns the refraction/reflection of the ray depending on the angle. */
    virtual CRay Scatter(const CRay& Ray, Vec3 Position, Vec3 Normal) const;

    virtual std::string& ReadFromString(std::string& String);
    virtual void WriteToString(std::string& String) const;
private:
    r32 m_RefractiveIndex; /**< Physical refractive index of the object. */
};