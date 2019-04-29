#include "common.hpp"

r32 DegreeToRadian(r32 Degree)
{
    return Pi32 * Degree / 180.0f;
}

r32 RadianToDegree(r32 Radian)
{
    return 180.0f * Radian / Pi32;
}

r32 RandomNormalized()
{
    thread_local std::random_device RandomDevice;
    thread_local std::mt19937 MersenneTwister(RandomDevice());
    return MersenneTwister() / r32(MersenneTwister.max());
}

r32 RandomNormalizedNeg()
{
    return 2.0f * RandomNormalized() - 1.0f;
}

std::string ExtractQuote(std::string& String)
{
    String.erase(0, String.find_first_of('\"') + 1);
    std::string Quote = String.substr(0, String.find_first_of('\"'));
    String.erase(0, Quote.size() + 1);
    return Quote;
}

std::string ExtractBraceContents(std::string& String)
{
    String.erase(0, String.find_first_of('{') + 1);

    u32 BraceCount = 1;
    size_t EndIndex;
    for(EndIndex = 1; BraceCount > 0; EndIndex++)
    {
        if(String[EndIndex] == '{') BraceCount++;
        else if(String[EndIndex] == '}') BraceCount--;
    }

    std::string Contents = String.substr(0, EndIndex - 1);
    String.erase(0, Contents.size() + 1);

    return Contents;
}

Vec3 ExtractVec3(std::string String)
{
    std::string StrX = String.substr(0, String.find_first_of(','));
    String.erase(0, StrX.size() + 1);
    std::string StrY = String.substr(0, String.find_first_of(','));
    String.erase(0, StrY.size() + 1);
    std::string StrZ = String.substr(0, String.find_first_of(','));
    //Contents.erase(0, StrX.size() + 1);

    return Vec3(std::stof(StrX), std::stof(StrY), std::stof(StrZ));
}

void WriteVec3(std::string& String, Vec3 V)
{
    std::stringstream Stream(String);
    Stream << "{" << V.X << "," << V.Y << "," << V.Z << "}";
    String = Stream.str();
}

Vec3::Vec3() :
    X(0.0f),
    Y(0.0f),
    Z(0.0f)
{
}

Vec3::Vec3(r32 X_, r32 Y_, r32 Z_) :
    X(X_),
    Y(Y_),
    Z(Z_)
{
}

Vec3 Vec3::operator-() const
{
    return Vec3(-X, -Y, -Z);
}

bool Vec3::operator==(const Vec3& Other) const
{
    return
        (X == Other.X) &&
        (Y == Other.Y) &&
        (Z == Other.Z);
}

Vec3& Vec3::operator+=(const Vec3& Other)
{
    X += Other.X;
    Y += Other.Y;
    Z += Other.Z;
    return *this;
}

Vec3& Vec3::operator-=(const Vec3& Other)
{
    X -= Other.X;
    Y -= Other.Y;
    Z -= Other.Z;
    return *this;
}

Vec3& Vec3::operator*=(const Vec3& Other)
{
    X *= Other.X;
    Y *= Other.Y;
    Z *= Other.Z;
    return *this;
}

Vec3 Vec3::operator*(const Vec3& Other) const
{
    Vec3 V = *this;
    V *= Other;
    return V;
}

Vec3 Vec3::operator+(const Vec3& Other) const
{
    Vec3 V = *this;
    V += Other;
    return V;
}

Vec3 Vec3::operator-(const Vec3& Other) const
{
    Vec3 V = *this;
    V -= Other;
    return V;
}

Vec3& Vec3::operator*=(r32 S)
{
    X *= S;
    Y *= S;
    Z *= S;
    return *this;
}

Vec3& Vec3::operator/=(r32 S)
{
    if(S == 0.0f)
    {
        throw std::invalid_argument("Divide by 0");
    }
    X /= S;
    Y /= S;
    Z /= S;

    return *this;
}

Vec3 Vec3::operator*(r32 S) const
{
    Vec3 V = *this;
    V *= S;
    return V;
}
Vec3 Vec3::operator/(r32 S) const
{
    Vec3 V = *this;
    V /= S;
    return V;
}

Vec3 operator*(r32 S, const Vec3& V)
{
    return V * S;
}

r32 Vec3::LengthSq() const
{
    return X * X + Y * Y + Z * Z;
}

r32 Vec3::Length() const
{
    return sqrtf(X * X + Y * Y + Z * Z);
}

Vec3 Normalize(const Vec3& V)
{
    r32 Length = V.Length();
    if(Length != 0.0f)
    {
        return (V / Length);
    }

    return V;
}

r32 Dot(const Vec3& A, const Vec3& B)
{
    return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
}

Vec3 Cross(const Vec3& A, const Vec3& B)
{
    return Vec3(+(A.Y*B.Z - A.Z*B.Y),
                -(A.X*B.Z - A.Z*B.X),
                +(A.X*B.Y - A.Y*B.X));
}

Vec3 Project(const Vec3& A, const Vec3& B)
{
    return B * Dot(A, B) / B.Length();
}

Vec3 Reject(const Vec3& A, const Vec3& B)
{
    return A - Project(A, B);
}

Vec3 Reflect(Vec3 Incident, Vec3 Normal)
{
    return Incident - 2.0f * Dot(Incident, Normal) * Normal;
}

Vec3 Refract(Vec3 Incident, Vec3 Normal, r32 RefractiveRatio)
{
    Vec3 NormIncident = Normalize(Incident);
    r32 dt = Dot(Normalize(NormIncident), Normal);
    r32 Discriminant = 1.0f - RefractiveRatio * RefractiveRatio * (1.0f - dt * dt);
    if(Discriminant <= 0)
    {
        return Vec3(0.0f, 0.0f, 0.0f);
    }

    return RefractiveRatio * (NormIncident - dt * Normal) - Normal * sqrtf(Discriminant);
}

Vec3 Lerp(const Vec3& A, const Vec3& B, r32 t)
{
    return (1.0f - t) * A + t * B;
}

Vec3 RandomInUnitSphere()
{
    return Normalize(Vec3(RandomNormalizedNeg(), RandomNormalizedNeg(), RandomNormalizedNeg()));
}