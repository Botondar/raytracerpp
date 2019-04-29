#include "camera.hpp"

CCamera::CCamera(r32 FieldOfView,
                 Vec3 Position,
                 Vec3 Forward,
                 Vec3 Up) :
    m_FieldOfView(FieldOfView),
    m_Position(Position)
{
    m_Forward = Normalize(Forward);
    m_Up = Normalize(Up);
}

void CCamera::SetPosition(Vec3 Position)
{
    m_Position = Position;
}

void CCamera::SetForward(Vec3 Forward)
{
    m_Forward = Normalize(Forward);
}

void CCamera::SetUp(Vec3 Up)
{
    m_Up = Normalize(Up);
}

Vec3 CCamera::GetPosition() const
{
    return m_Position;
}
Vec3 CCamera::GetForward() const
{
    return m_Forward;
}
Vec3 CCamera::GetUp() const
{
    return m_Up;
}

CRay CCamera::RayFromUV(r32 U, r32 V, r32 AspectRatio) const
{
    r32 TanHalfFov = tanf(m_FieldOfView / 2.0f);

    Vec3 Right = Cross(m_Forward, m_Up);
    Vec3 Up = Cross(Right, m_Forward);
    Vec3 Direction;
    Direction += m_Forward;
    Direction += TanHalfFov*Up * V;
    Direction += TanHalfFov*AspectRatio * Right * U;

    Direction = Normalize(Direction);

    return CRay(m_Position, Direction);
}

std::string& CCamera::ReadFromString(std::string& String)
{
    while(!String.empty() && String[0] != '\0')
    {
        std::string Name = ExtractQuote(String);

        if(Name == "position")
        {
            std::string Contents = ExtractBraceContents(String);
            m_Position = ExtractVec3(Contents);
        }
        else if(Name == "forward")
        {
            std::string Contents = ExtractBraceContents(String);
            m_Forward = Normalize(ExtractVec3(Contents));
        }
        else if(Name == "up")
        {
            std::string Contents = ExtractBraceContents(String);
            m_Up = Normalize(ExtractVec3(Contents));
        }
        else if(Name == "fov")
        {
            String.erase(0, 1);
            std::string Fov = String.substr(0, String.find_first_of(','));
            String.erase(0, Fov.size());

            m_FieldOfView = DegreeToRadian(std::stof(Fov));
        }
        else
        {
            throw;
        }
    }
    return String;
}

void CCamera::WriteToString(std::string& String) const
{
    std::string StrPosition;
    WriteVec3(StrPosition, m_Position);
    std::string StrForward;
    WriteVec3(StrForward, m_Forward);
    std::string StrUp;
    WriteVec3(StrUp, m_Up);

    std::stringstream Stream;
    Stream << "\"camera\" {\n";
    Stream << "\t\"position\":" << StrPosition << ",\n";
    Stream << "\t\"forward\":" << StrForward << ",\n";
    Stream << "\t\"up\":" << StrUp << ",\n";
    Stream << "\t\"fov\":" << RadianToDegree(m_FieldOfView) << "\n" <<
        "}\n";

    String += Stream.str();
}

