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