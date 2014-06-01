#pragma once
#include "material.h"
#include "StringHelperFunctions.h"
#include "vector4.h"

#include <string>
#include <typeinfo>

class tinyxml2::XMLElement;

class ISettingBase
{
public:
    enum SettingType
    {
        efloat = 0,
        eint,
        eBool,
        eString,
        eUserType,
    };
    ISettingBase() : m_settingName(""), m_settingNameHash(hashString("")) {}
    ISettingBase(const std::string& name) : m_settingName(name), m_settingNameHash(hashString(name)) {}
    virtual ~ISettingBase() {}

    const std::string& getSettingName() const { return m_settingName; }
    const unsigned int getHashValue() const { return m_settingNameHash; }
    const SettingType getSettingType() const { return m_type; }
protected:	
    std::string m_settingName;
    unsigned int m_settingNameHash;
    SettingType m_type;
private:
};

template <class T>
class ISetting : public ISettingBase
{
public:
    ISetting() : ISettingBase(), m_data() {}
    ISetting(const std::string& name, const T& data) : ISettingBase(name), m_data(data) 
    {
        if (typeid(T) == typeid(int))
        {
            m_type = eint;
        }
        else if (typeid(T) == typeid(float))
        {
            m_type = efloat;
        }
        else if (typeid(T) == typeid(bool))
        {
            m_type = eBool;
        }
        else if (typeid(T) == typeid(std::string))
        {
            m_type = eString;
        }
        else
        {
            MSG_TRACE_CHANNEL("SETTING", "Warning: UserType Setting detected with type T defined as: %s", typeid(T).name() )
            m_type = eUserType;
        }
    }
    ~ISetting() {}

    const T& getData() const { return m_data; }
protected:	
    T     m_data;
private:
};

class DeserialisableSetting : public ISettingBase
{
public:
    DeserialisableSetting() {}
    virtual ~DeserialisableSetting() {}

    //Implement this function in non primitive type settings to initialise them
    virtual void deserialise( const tinyxml2::XMLElement* element);
protected:
private:
};

class RenderSetting : public DeserialisableSetting
{
public:

    enum RendererType
    {
        D3D9 = 0,
        D3D10,
        D3D11,
        OGL,
    };

    RenderSetting() { m_type = eUserType; }
    ~RenderSetting() {}

    void deserialise( const tinyxml2::XMLElement* element);

    const unsigned int resolutionWidth() const { return m_resolutionWidth; }
    const unsigned int resolutionHeight() const { return m_resolutionHeight; }
    RendererType getRenderType() const { return m_rendererType; }
    const bool getUseCG() const { return m_useCG; }
protected:
private:
    unsigned int m_resolutionWidth;
    unsigned int m_resolutionHeight;
    bool m_useCG;
    RendererType m_rendererType;
    std::string m_windowName;
};

class MaterialSetting : public DeserialisableSetting
{
public:
    MaterialSetting() {}
    ~MaterialSetting() {}

    void deserialise( const tinyxml2::XMLElement* element);
    const Material& getMaterial() const { return m_material; }
protected:
private:
    Material m_material;
};

class VectorSetting : public DeserialisableSetting
{
public:
    VectorSetting() {}
    ~VectorSetting() {}

    void deserialise( const tinyxml2::XMLElement* element);
    const Vector4& getVector() const { return m_vector; }
    int getNumberOfElements() const { return m_numberElements; }
protected:
private:
    Vector4 m_vector;
    int m_numberElements;
};