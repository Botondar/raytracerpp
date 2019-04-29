#pragma once

#include "common.hpp"

/** Serializable interface that supports reading from and writing to streams. */
class ISerializable
{
public:
    virtual ~ISerializable();

    /** Reads the object from a stream. */
    virtual std::istream& Read(std::istream& Stream) = 0;
    /** Writes the object to a stream. */
    virtual std::ostream& Write(std::ostream& Stream) const = 0;
};

/** Stream operator overload to read the object from a stream. */
std::istream& operator>>(std::istream& Stream, ISerializable* Var);
/** Stream operator overload to write the object to a stream. */
std::ostream& operator<<(std::ostream& Stream, const ISerializable* Var);

/** Serializable interface that supports reading from and writing to strings */
class IStringSerializable
{
public:
    virtual ~IStringSerializable();

    /** Reads the object from a string, possibly removing contents from the string. */
    virtual std::string& ReadFromString(std::string& String) = 0;
    /** Writes the object to a string. */
    virtual void WriteToString(std::string& String) const = 0;
};