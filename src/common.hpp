#pragma once

#include <cinttypes>
#include <cfloat>
#include <cmath>
#include <random>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <atomic>
#include <mutex>
#include <sstream>

typedef uint8_t u8; /**< Unsigned 8 bit integer. */
typedef int8_t s8; /**< Signed 8 bit integer. */
typedef uint16_t u16; /**< Unsigned 16 bit integer. */
typedef int16_t s16; /**< Signed 16 bit integer. */
typedef uint32_t u32; /**< Unsigned 32 bit integer. */
typedef int32_t s32; /**< Signed 32 bit integer. */
typedef uint64_t u64; /**< Unsigned 64 bit integer. */
typedef int64_t s64; /**< Signed 64 bit integer. */

typedef float r32; /**< 32 bit floating point value. */
typedef double r64; /**< 64 bit floating point value. */

const r32 Pi32 = 3.1415927f; /**< 32 bit constant for pi. */

/** Converts degrees to radians. */
r32 DegreeToRadian(r32 Degree);
/** Converts radians to degrees. */
r32 RadianToDegree(r32 Radian);

/** Returns a random number between [0..1). */
r32 RandomNormalized();
/** Returns a random number between (-1..1). */
r32 RandomNormalizedNeg();

/** Clamps a value between a range .
    \param Val value to clamp.
    \param Min lower bound of the clamp.
    \param Max upper bound of the clamp.
*/
template<class T>
T Clamp(T Val, T Min, T Max)
{
    return std::max(std::min(Val, Max), Min);
}

/** Mathematical 3D vector.
*/
struct Vec3
{
    r32 X; /**< X coordinate of the vector. */
    r32 Y; /**< Y coordinate of the vector. */
    r32 Z; /**< Z coordinate of the vector. */

    /** Empty constructor. Initializes to 0.*/
    Vec3();
    /** Sets the coordinates to the appropriate parameters. */
    Vec3(r32 X, r32 Y, r32 Z);

    /** Returns the negated version of the vector. */
    Vec3 operator-() const;

    /** Checks if all coordinates are equal. */
    bool operator==(const Vec3& Other) const;

    /** Adds a vector to another (component-wise). */
    Vec3& operator+=(const Vec3& Other);
    /** Subtracts a vector from another (component-wise). */
    Vec3& operator-=(const Vec3& Other);
    /** Multiplies a vector with another (component-wise). */
    Vec3& operator*=(const Vec3& Other);

    /** Adds a vector to another (component-wise). */
    Vec3 operator+(const Vec3& Other) const;
    /** Subtracts a vector from another (component-wise). */
    Vec3 operator-(const Vec3& Other) const;
    /** Multiplies a vector with another (component-wise). */
    Vec3 operator*(const Vec3& Other) const;

    /** Multiplies a vector with a scalar. */
    Vec3& operator*=(r32 S);
    /** Divides a vector by a scalar. */
    Vec3& operator/=(r32 S);

    /** Multiplies a vector with a scalar. */
    Vec3 operator*(r32 S) const;
    /** Divides a vector by a scalar. */
    Vec3 operator/(r32 S) const;

    /** Returns the square of the Pythagorean length. */
    r32 LengthSq() const;
    /** Returns the Pythagorean length. */
    r32 Length() const;
};

/** Multiplies a vector with a scalar. */
Vec3 operator*(r32 S, const Vec3& V);

/** Returns the vector divided by its length. */
Vec3 Normalize(const Vec3& V);

/** Returns two vectors' dot product. */
r32 Dot(const Vec3& A, const Vec3& B);
/** Returns two vectors' cross product. */
Vec3 Cross(const Vec3& A, const Vec3& B);

/** Returns vector A's projection to B. */
Vec3 Project(const Vec3& A, const Vec3& B);
/** Returns vector A's rejection from B. */
Vec3 Reject(const Vec3& A, const Vec3& B);

/** Returns a vector's reflection given the surface normal. */
Vec3 Reflect(Vec3 Incident, Vec3 Normal);
/** Returns a vector's refraction given a surface normal and the refractive index ratio between the medium.
    Returns a null vector if no refraction is possible.
*/
Vec3 Refract(Vec3 Incident, Vec3 Normal, r32 RefractiveRatio);

/** Linearly interpolates between to vectors given a t value. */
Vec3 Lerp(const Vec3& A, const Vec3& B, r32 t);

/** Returns a unit vector pointing in a random direction */
Vec3 RandomInUnitSphere();

/** Extracts a quote from a string.
    Extracts characters between '"' characters, removing them from the string.
    Quotes are also removed.
*/
std::string ExtractQuote(std::string& String);
/** Extracts content from a string between curly {} braces.
    The braces and the characters between are removed from the string.
*/
std::string ExtractBraceContents(std::string& String);
/** Returns a Vec3 from a string. 
    String must be {x, y, z} format.
*/
Vec3 ExtractVec3(std::string String);
/** Writes a Vec3 to a string in {x, y, z} format. */
void WriteVec3(std::string& String, Vec3 V);
