#include "shape.hpp"

IShape::IShape(std::shared_ptr<IMaterial> Material) :
    m_Material(Material)
{

}

IShape::~IShape()
{

}

const std::shared_ptr<IMaterial>& IShape::GetMaterial() const
{
    return m_Material;
}