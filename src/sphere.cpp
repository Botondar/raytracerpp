#include "sphere.hpp"

CSphere::CSphere(Vec3 Center, r32 Radius, std::shared_ptr<IMaterial> Material) :
    IShape(Material),
    m_Center(Center),
    m_Radius(Radius)
{
}

CSphere::~CSphere()
{

}

void CSphere::GetUV(Vec3 Point, r32& U, r32& V) const
{
    Vec3 RelativePosition = Normalize(m_Center - Point);
    U = 0.5f + atan2f(RelativePosition.Z, RelativePosition.X) / (2*Pi32);
    V = 0.5f - asinf(RelativePosition.Y) / Pi32;
}

bool CSphere::Intersect(const CRay& Ray, r32 tMin, r32 tMax, SHitInfo& HitInfo) const
{
    Vec3 CenterToOrigin = Ray.Origin() - m_Center;
    r32 a = Dot(Ray.Direction(), Ray.Direction());
    r32 b = 2.0f * Dot(CenterToOrigin, Ray.Direction());
    r32 c = Dot(CenterToOrigin, CenterToOrigin) - m_Radius * m_Radius;

    r32 D = b * b - 4 * a*c;
    if(D < 0.0f)
    {
        return false;
    }

    // We only need the smaller solution
    r32 tVal = (-b - sqrtf(D)) / (2 * a);

    if(tVal < tMin || tVal > tMax)
    {
        return false;
    }

    HitInfo.tVal = tVal;
    HitInfo.Point = Ray.PointAt(tVal);
    HitInfo.Normal = Normalize(HitInfo.Point - m_Center);
    HitInfo.Shape = this;
    return true;
}

std::string& CSphere::ReadFromString(std::string& String)
{
    while(!String.empty() && String[0] != '\0')
    {
        std::string Name = ExtractQuote(String);

        if(Name == "position")
        {
            std::string Contents = ExtractBraceContents(String);
            m_Center = ExtractVec3(Contents);
        }
        else if(Name == "radius")
        {
            String.erase(0, 1);
            std::string Offset = String.substr(0, String.find_first_of(','));
            String.erase(0, Offset.size());

            m_Radius = std::stof(Offset);
        }
        else if(Name == "materialdiffuse")
        {
            m_Material = std::make_shared<CMaterialDiffuse>(Vec3(0.0f, 0.0f, 0.0f));

            std::string Contents = ExtractBraceContents(String);
            m_Material->ReadFromString(Contents);
        }
        else if(Name == "materialmetal")
        {
            m_Material = std::make_shared<CMaterialMetal>(Vec3(0.0f, 0.0f, 0.0f));

            std::string Contents = ExtractBraceContents(String);
            m_Material->ReadFromString(Contents);
        }
        else if(Name == "materialdielectric")
        {
            m_Material = std::make_shared<CMaterialDielectric>(Vec3(0.0f, 0.0f, 0.0f), 1.0f);

            std::string Contents = ExtractBraceContents(String);
            m_Material->ReadFromString(Contents);
        }
        else
        {
            throw;
        }
    }
    return String;
}

void CSphere::WriteToString(std::string& String) const
{
    std::string StrPosition;
    WriteVec3(StrPosition, m_Center);
    std::stringstream Stream;
    Stream << "\"sphere\" {\n";
    Stream << "\t\"position\":" << StrPosition << ",\n";
    Stream << "\t\"radius\":" << m_Radius << ",\n";
    std::string StrMaterial;
    m_Material->WriteToString(StrMaterial);
    Stream << StrMaterial;
    Stream << "}\n";

    String += Stream.str();
}
