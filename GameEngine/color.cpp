#include "color.h"
#include "..\SpaceSim\tinyxml2.h"

HASH_ELEMENT_IMPLEMENTATION(Color)

Color::Color()
{
    m_rgba[0] = 0.0f;
    m_rgba[1] = 0.0f;
    m_rgba[2] = 0.0f;
    m_rgba[3] = 1.0f;
}

Color::Color(float r, float g, float b)
{
    m_rgba[0] = r;
    m_rgba[1] = g;
    m_rgba[2] = b;
    m_rgba[3] = 1.0f;
}

Color::Color(float r, float g, float b, float a)
{
    m_rgba[0] = r;
    m_rgba[1] = g;
    m_rgba[2] = b;
    m_rgba[3] = a;
}

Color::Color(const float rgba[4])
{
    m_rgba[0] = rgba[0];
    m_rgba[1] = rgba[1];
    m_rgba[2] = rgba[2];
    m_rgba[3] = rgba[3];
}

Color::~Color()
{
}

void Color::write(std::ostream &out) const
{
    out << "(" << m_rgba[0] << ", " << m_rgba[1] << ", " << m_rgba[2] << ", " << m_rgba[3] << ")" << std::endl;
}

void Color::read(std::istream& in)
{
    char temp;
    in >> temp >> m_rgba[0] >> temp >> m_rgba[1] >> temp >> m_rgba[2] >> temp >> m_rgba[3] >> temp;
}

Color Color::black()
{
    return Color(0.0, 0.0, 0.0);
}

Color Color::white()
{
    return Color(1.0, 1.0, 1.0);
}

Color Color::red()
{
    return Color(1.0, 0.0, 0.0);
}

Color Color::green()
{
    return Color(0.0, 1.0, 0.0);
}

Color Color::blue()
{
    return Color(0.0, 0.0, 1.0);
}

Color Color::cyan()
{
    return Color(0.0, 1.0, 1.0);
}

Color Color::yellow()
{
    return Color(1.0, 1.0, 0.0);
}

Color Color::magenta()
{
    return Color(1.0, 0.0, 1.0);
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void Color::deserialise( const tinyxml2::XMLElement* node )
{
    const tinyxml2::XMLAttribute* attribute = node->FindAttribute("r");
    if (attribute)
    {
        m_rgba[0] = attribute->FloatValue();
    }
    attribute = node->FindAttribute("g");
    if (attribute)
    {
        m_rgba[1] = attribute->FloatValue();
    }
    attribute = node->FindAttribute("b");
    if (attribute)
    {
        m_rgba[2] = attribute->FloatValue();
    }
    attribute = node->FindAttribute("a");
    if (attribute)
    {
        m_rgba[3] = attribute->FloatValue();
    }
}
