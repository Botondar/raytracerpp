#include "plane.hpp"

CPlane::CPlane(Vec3 Normal, 
               r32 Offset,
               Vec3 TextureUp, 
               std::shared_ptr<IMaterial> Material) :
    IShape(Material),
    m_Normal(Normalize(Normal)),
    m_Offset(Offset)
{
    m_TextureRight = Cross(Normalize(TextureUp), Normal);
    m_TextureUp = Cross(Normal, m_TextureRight);
}

CPlane::~CPlane()
{

}

void CPlane::GetUV(Vec3 Point, r32& U, r32& V) const
{
    U = Project(Point, m_TextureRight).Length();
    V = Project(Point, m_TextureUp).Length();
}

bool CPlane::Intersect(const CRay& Ray, r32 tMin, r32 tMax, SHitInfo& HitInfo) const
{
    r32 Denominator = Dot(m_Normal, Ray.Direction());
    if(Denominator == 0.0f)
    {
        return false;
    }

    r32 tVal = (m_Offset - Dot(m_Normal, Ray.Origin())) / Denominator;
    if(tVal < tMin || tVal > tMax)
    {
        return false;
    }

    HitInfo.tVal = tVal;
    HitInfo.Point = Ray.PointAt(tVal);
    HitInfo.Normal = m_Normal;
    HitInfo.Shape = this;
    return true;
}

std::string& CPlane::ReadFromString(std::string& String)
{
    while(!String.empty() && String[0] != '\0')
    {
        std::string Name = ExtractQuote(String);

        if(Name == "normal")
        {
            std::string Contents = ExtractBraceContents(String);

            std::string StrX = Contents.substr(0, Contents.find_first_of(','));
            Contents.erase(0, StrX.size() + 1);
            std::string StrY = Contents.substr(0, Contents.find_first_of(','));
            Contents.erase(0, StrX.size() + 1);
            std::string StrZ = Contents.substr(0, Contents.find_first_of(','));
            //Contents.erase(0, StrX.size() + 1);

            m_Normal.X = std::stof(StrX);
            m_Normal.Y = std::stof(StrY);
            m_Normal.Z = std::stof(StrZ);
        }
        else if(Name == "offset")
        {
            String.erase(0, 1);
            std::string Offset = String.substr(0, String.find_first_of(','));
            String.erase(0, Offset.size());

            m_Offset = std::stof(Offset);
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

void CPlane::WriteToString(std::string& String) const
{
    std::stringstream Stream;

    std::string StrNormal;
    WriteVec3(StrNormal, m_Normal);
    
    Stream << "\"plane\" {\n";
    Stream << "\t\"normal\":" << StrNormal << ",\n";
    Stream << "\t\"offset\":" << m_Offset << ",\n";
    std::string StrMaterial;
    m_Material->WriteToString(StrMaterial);
    Stream << StrMaterial;
    Stream << "}\n";

    String += Stream.str();
}