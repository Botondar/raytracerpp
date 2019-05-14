#include "material.hpp"

IMaterial::IMaterial(Vec3 Color, std::shared_ptr<CImage> Texture) :
    m_Color(Color),
    m_Texture(Texture)
{
}

Vec3 IMaterial::GetColor() const
{
    return m_Color;
}

const std::shared_ptr<CImage>& IMaterial::GetTexture() const
{
    return m_Texture;
}

CMaterialDiffuse::CMaterialDiffuse(Vec3 Color, std::shared_ptr<CImage> Texture) :
    IMaterial(Color, Texture)
{

}

CRay CMaterialDiffuse::Scatter(const CRay& Ray, Vec3 Position, Vec3 Normal) const
{
    return CRay(Position, Normal + RandomInUnitSphere());
}

std::string& CMaterialDiffuse::ReadFromString(std::string& String)
{
    while(!String.empty() && String[0] != '\0')
    {
        std::string Name = ExtractQuote(String);

        if(Name == "color")
        {
            std::string Contents = ExtractBraceContents(String);
            m_Color = ExtractVec3(Contents);
        }
        else if(Name == "texture")
        {
            std::string Path = ExtractQuote(String);

            std::fstream File(Path, std::ios_base::binary | std::ios_base::in);
            if(File.is_open())
            {
                m_Texture = std::make_shared<CImage>();
                m_Texture->Read(File);
            }
        }
        else
        {
            throw;
        }
    }
    return String;
}

void CMaterialDiffuse::WriteToString(std::string& String) const
{
    std::stringstream Stream;
    
    std::string StrColor;
    WriteVec3(StrColor, m_Color);

    Stream << "\"materialdiffuse\" : {\n";
    Stream << "\t\"color\":" << StrColor << ",\n";
    Stream << "\t\"texture\":\"null\"\n";
    Stream << "}\n";

    String += Stream.str();
}

CMaterialMetal::CMaterialMetal(Vec3 Color, r32 Fuzziness, std::shared_ptr<CImage> Texture) :
    IMaterial(Color, Texture),
    m_Fuzziness(Fuzziness)
{

}

CRay CMaterialMetal::Scatter(const CRay& Ray, Vec3 Position, Vec3 Normal) const
{
    return CRay(Position, 
                Reflect(Ray.Direction(), 
                Normal + m_Fuzziness * RandomInUnitSphere()));
}

std::string& CMaterialMetal::ReadFromString(std::string& String)
{
    while(!String.empty() && String[0] != '\0')
    {
        std::string Name = ExtractQuote(String);

        if(Name == "color")
        {
            std::string Contents = ExtractBraceContents(String);
            m_Color = ExtractVec3(Contents);
        }
        else if(Name == "fuzziness")
        {
            String.erase(0, 1);
            std::string Fuzziness = String.substr(0, String.find_first_of(','));
            String.erase(0, Fuzziness.size());

            m_Fuzziness = std::stof(Fuzziness);
        }
        else if(Name == "texture")
        {
            std::string Path = ExtractQuote(String);

            std::fstream File(Path, std::ios_base::binary | std::ios_base::in);
            if(File.is_open())
            {
                m_Texture = std::make_shared<CImage>();
                m_Texture->Read(File);
            }
        }
        else
        {
            throw;
        }
    }
    return String;
}

void CMaterialMetal::WriteToString(std::string& String) const
{
    std::stringstream Stream;

    std::string StrColor;
    WriteVec3(StrColor, m_Color);

    Stream << "\"materialmetal\" : {\n";
    Stream << "\t\"color\":" << StrColor << ",\n";
    Stream << "\t\"fuzziness\":" << m_Fuzziness << ",\n";
    Stream << "\t\"texture\":\"null\"\n";
    Stream << "}\n";

    String += Stream.str();
}

CMaterialDielectric::CMaterialDielectric(Vec3 Color, r32 RefractiveIndex, 
                                         std::shared_ptr<CImage> Texture) :
    IMaterial(Color, Texture),
    m_RefractiveIndex(RefractiveIndex)
{
    
}

CRay CMaterialDielectric::Scatter(const CRay& Ray, Vec3 Position, Vec3 Normal) const
{
    Vec3 Reflected = Reflect(Ray.Direction(), Normal);

    r32 RefractiveRatio;
    if(Dot(Ray.Direction(), Normal) > 0)
    {
        Normal = -Normal;
        RefractiveRatio = m_RefractiveIndex;
    }
    else
    {
        RefractiveRatio = 1.0f / m_RefractiveIndex;
    }

    Vec3 Direction = Refract(Ray.Direction(), Normal, RefractiveRatio);
    if(Direction == Vec3(0.0f, 0.0f, 0.0f))
    {
        Direction = Reflected;
    }

    return CRay(Position, Direction);
}

std::string& CMaterialDielectric::ReadFromString(std::string& String)
{
    while(!String.empty() && String[0] != '\0')
    {
        std::string Name = ExtractQuote(String);

        if(Name == "color")
        {
            std::string Contents = ExtractBraceContents(String);
            m_Color = ExtractVec3(Contents);
        }
        else if(Name == "refract")
        {
            String.erase(0, 1);
            std::string Refract = String.substr(0, String.find_first_of(','));
            String.erase(0, Refract.size());

            m_RefractiveIndex = std::stof(Refract);
        }
        else if(Name == "texture")
        {
            std::string Path = ExtractQuote(String);

            std::fstream File(Path, std::ios_base::binary | std::ios_base::in);
            if(File.is_open())
            {
                m_Texture = std::make_shared<CImage>();
                m_Texture->Read(File);
            }
        }
        else
        {
            throw;
        }
    }
    return String;
}

void CMaterialDielectric::WriteToString(std::string& String) const
{
    std::stringstream Stream;

    std::string StrColor;
    WriteVec3(StrColor, m_Color);

    Stream << "\"materialdielectric\" : {\n";
    Stream << "\t\"color\":" << StrColor << ",\n";
    Stream << "\t\"refract\":" << m_RefractiveIndex << ",\n";
    Stream << "\t\"texture\":\"null\"\n";
    Stream << "}\n";

    String += Stream.str();
}