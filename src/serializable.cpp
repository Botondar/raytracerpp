#include "serializable.hpp"

ISerializable::~ISerializable()
{

}

std::istream& operator>>(std::istream& Stream, ISerializable* Var)
{
    return Var->Read(Stream);
}
std::ostream& operator<<(std::ostream& Stream, const ISerializable* Var)
{
    return Var->Write(Stream);
}

IStringSerializable::~IStringSerializable()
{

}