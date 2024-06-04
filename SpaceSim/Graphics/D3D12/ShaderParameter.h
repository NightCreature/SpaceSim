#pragma once

#include "..\bin\Shaders\Shaders\Shared\CommonStructures.h"
#include "Math/matrix44.h"

#include <vector>
#include <variant>
#include "ConstantBuffer.h"

class DeviceManager;
struct DescriptorHeap;

#include "Core/Types/TemplateTypeMagic.h"

struct TextureData
{
    size_t m_startingSlot;
    size_t m_numberOfTextures;
};

struct ShaderParameter
{
    ShaderParameter()
    {
    }

    ~ShaderParameter()
    {
    }

    void Print() const;

    size_t m_rootParamIndex;
    size_t m_nameHash;
    using ShaderParameterData = std::variant<WVPData, MaterialConstants, LightParameters, TextureData>;
    ShaderParameterData m_data;
};

using ShaderParameters = std::vector<ShaderParameter>;


void PrintParameters(const ShaderParameters& parameters);

#define ECHO_NAME(Type) #Type
//template <class T>
//struct 
//static constexpr std::array<FunctionPointer, sizeof...(Ts)> ids = { nameof<Ts>... }; //This is a hack so we can do static pointer compares in the functions below

//Gameplay side
struct ShaderParameterData
{
    size_t m_nameHash;
    std::variant<WVPData, MaterialConstants, LightParameters> m_data;
};

using ShaderParamersData = std::vector<ShaderParameterData>;

size_t GetVariantSize(size_t index);
ShaderParameter::ShaderParameterData GetVariant(const std::string& name);