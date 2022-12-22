#pragma once
//#include "Graphics/material.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Math/vector4.h"

#include <string>
#include <typeinfo>

//Testing
#include "Core/Serialization/Archive.h"
#include "Core/Serialization/ISerializable.h"

class tinyxml2::XMLElement;

//This might need to be a serialise based object
class ISettingBase : public ISerializable
{
public:
    enum class SettingType {
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
    const size_t getHashValue() const { return m_settingNameHash; }
    const SettingType getSettingType() const { return m_type; }

    void Serialize(Archive& archive) override
    {
        MSG_TRACE_CHANNEL("SETTING", "Reading From archive");
        archive.Read(m_settingName);
        archive.Read(m_settingNameHash);
        archive.Read(m_type);
    }

    void Serialize(Archive& archive) const override
    {
        MSG_TRACE_CHANNEL("SETTING", "Writing to archive");
        archive.Write(m_settingName);
        archive.Write(m_settingNameHash);
        archive.Write(m_type);
    }

    REGISTER_SERIALIZATION_OBJECT(ISettingBase);
protected:
    std::string m_settingName;
    size_t m_settingNameHash;
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
            m_type = ISettingBase::SettingType::eint;
        }
        else if (typeid(T) == typeid(float))
        {
            m_type = ISettingBase::SettingType::efloat;
        }
        else if (typeid(T) == typeid(bool))
        {
            m_type = ISettingBase::SettingType::eBool;
        }
        else if (typeid(T) == typeid(std::string))
        {
            m_type = ISettingBase::SettingType::eString;
        }
        else
        {
            MSG_TRACE_CHANNEL("SETTING", "Warning: UserType Setting detected with type T defined as: %s", typeid(T).name() )
            m_type = ISettingBase::SettingType::eUserType;
        }
    }
    ~ISetting() {}

    const T& getData() const { return m_data; }

    ///-----------------------------------------------------------------------------
    ///! @brief   
    ///! @remark
    ///-----------------------------------------------------------------------------
    void Serialize(Archive& archive) override
    {
        ISettingBase::Serialize(archive);
    }


    ///-----------------------------------------------------------------------------
    ///! @brief   
    ///! @remark
    ///-----------------------------------------------------------------------------
    void Serialize(Archive& archive) const override
    {
        ISettingBase::Serialize(archive);

        archive.Write(m_data);
    }

    REGISTER_SERIALIZATION_TEMPLATE(ISetting<T>);
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

    void Serialize(Archive& archive) override { ISettingBase::Serialize(archive); }
    void Serialize(Archive& archive) const override { ISettingBase::Serialize(archive); }

    REGISTER_SERIALIZATION_OBJECT(DeserialisableSetting);
protected:
private:
};

class RenderSetting : public DeserialisableSetting
{
public:

    enum class RendererType
    {
        D3D9 = 0,
        D3D10,
        D3D11,
        OGL,
    };

    RenderSetting() { m_type = ISettingBase::SettingType::eUserType; }
    ~RenderSetting() {}

    void deserialise( const tinyxml2::XMLElement* element);

    const unsigned int resolutionWidth() const { return m_resolutionWidth; }
    const unsigned int resolutionHeight() const { return m_resolutionHeight; }
    RendererType getRenderType() const { return m_rendererType; }
    const bool getUseCG() const { return m_useCG; }

    void Serialize(Archive& archive) override { DeserialisableSetting::Serialize(archive); }
    void Serialize(Archive& archive) const override
    {
        DeserialisableSetting::Serialize(archive);

        archive.Write(m_resolutionWidth);
        archive.Write(m_resolutionHeight);
        archive.Write(m_useCG);
        archive.Write(m_rendererType);
        archive.Write(m_windowName);
    }

    REGISTER_SERIALIZATION_OBJECT(RenderSetting)
protected:
private:
    unsigned int m_resolutionWidth;
    unsigned int m_resolutionHeight;
    bool m_useCG;
    RendererType m_rendererType;
    std::string m_windowName;
};

//class MaterialSetting : public DeserialisableSetting
//{
//public:
//    MaterialSetting() {}
//    ~MaterialSetting() {}
//
//    void deserialise( const tinyxml2::XMLElement* element);
//    const Material& getMaterial() const { return m_material; }
//protected:
//private:
//    Material m_material;
//};

class VectorSetting : public DeserialisableSetting
{
public:
    VectorSetting() {}
    ~VectorSetting() {}

    void deserialise( const tinyxml2::XMLElement* element);
    const Vector4& getVector() const { return m_vector; }
    int getNumberOfElements() const { return m_numberElements; }

    void Serialize(Archive& archive) override { DeserialisableSetting::Serialize(archive); }
    void Serialize(Archive& archive) const override 
    {
        DeserialisableSetting::Serialize(archive); 

        archive.Write(m_vector);
        archive.Write(m_numberElements);
    }

    REGISTER_SERIALIZATION_OBJECT(VectorSetting)
protected:
private:
    Vector4 m_vector;
    int m_numberElements;
};