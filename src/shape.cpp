#include "shape.hpp"

IShape::IShape(CSharedPointer<IMaterial> Material) :
    m_Material(Material)
{

}

IShape::~IShape()
{

}

const CSharedPointer<IMaterial>& IShape::GetMaterial() const
{
    return m_Material;
}