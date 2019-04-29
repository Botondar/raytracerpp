#pragma once

#include "common.hpp"

/** Stores color information in 0xRRGGBBAA format. */
union UColor
{
    u32 Color; /**< 32 bit unsigned integer holding the color information */
    /** Struct to access the components separately. */
    struct
    {
        u8 Alpha; /**< Alpha channel of the color. */
        u8 Blue; /**< BLue channel of the color. */
        u8 Green; /**< Green channel of the color. */
        u8 Red; /**< Red channel of the color. */
    } Components;
};

/** Combines RGB color values to a single u32. */
u32 RGBToU32(u32 R, u32 G, u32 B);
/** Combines RGBA color values to a single u32 */
u32 RGBAToU32(u32 R, u32 G, u32 B, u32 A);

/** Converts a normalized color (values rangin from 0-1) to a single u32. */
u32 Vec3ToU32(const Vec3& V);
/** Converts a color stored in a u32 to a normalized color vector. */
Vec3 U32ToVec3(u32 Color);