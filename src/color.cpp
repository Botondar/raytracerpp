#include "color.hpp"

u32 RGBToU32(u32 R, u32 G, u32 B)
{
    return
        (0xFF000000 & (R << 24)) |
        (0x00FF0000 & (G << 16)) |
        (0x0000FF00 & (B << 8)) |
        0xFF;
}

u32 RGBAToU32(u32 R, u32 G, u32 B, u32 A)
{
    return
        (0xFF000000 & (R << 24)) |
        (0x00FF0000 & (G << 16)) |
        (0x0000FF00 & (B << 8)) |
        (0x000000FF & A);
}

u32 Vec3ToU32(const Vec3& V)
{
    return RGBToU32(lround(255.0f*V.X), lround(255.0f*V.Y), lround(255.0f*V.Z));
}

Vec3 U32ToVec3(u32 Color)
{
    u32 R = 0xFF & (Color >> 24);
    u32 G = 0xFF & (Color >> 16);
    u32 B = 0xFF & (Color >> 8);
    return Vec3(R / 255.0f, G / 255.0f, B / 255.0f);
}