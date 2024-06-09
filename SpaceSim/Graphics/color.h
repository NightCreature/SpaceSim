#ifndef COLOR_H
#define COLOR_H

#include "Core/StringOperations/StringHelperFunctions.h"
#include <array>
#include <ostream>

namespace tinyxml2
{
    class XMLElement;
}

class Color
{
public:
    Color();
    Color(float r, float g, float b);
    Color(float r, float g, float b, float a);
    Color(const float rgba[4]);
    Color(const std::array<float, 4>& rgba);
    ~Color();

    float a() const {return m_rgba[3];}
    float r() const {return m_rgba[0];}
    float g() const {return m_rgba[1];}
    float b() const {return m_rgba[2];}
    void setAlpha(float alpha) { m_rgba[3] = alpha;}
    void setRed(float redcomponent) { m_rgba[0] = redcomponent;}
    void setGreen(float bluecomponent) { m_rgba[1] = bluecomponent;}
    void setBlue(float greencomponent) { m_rgba[2] = greencomponent;}
    const float* rgba() const {return m_rgba;}

    void deserialise(const tinyxml2::XMLElement* node);
    void read(std::istream& in);
    void write(std::ostream& out) const;
    
    void OnDebugImgui(const std::string_view& name);

    static Color black();
    static Color white();
    static Color red();
    static Color green();
    static Color blue();
    static Color cyan();
    static Color yellow();
    static Color magenta();
    static Color darkgray() { return Color(0.25f, 0.25f, 0.25f, 1.0f); }

    HASH_ELEMENT_DEFINITION(Color)
protected:
private:
    float m_rgba[4];
};

inline std::ostream& operator<< (std::ostream& out, const Color& c)
{
    c.write(out);
    return out;
}

inline std::istream& operator>> (std::istream& in, Color& c)
{
    c.read(in);
    return in;
}
#endif