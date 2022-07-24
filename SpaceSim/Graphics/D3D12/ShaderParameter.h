#pragma once

#include "Graphics/ShaderDataStructures.h"
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
        MSG_TRACE_CHANNEL("ShaderParameter", "construction");
    }

    ~ShaderParameter()
    {
        MSG_TRACE_CHANNEL("ShaderParameter", "destruction");
    }

    void CreateConstantBuffer(const DeviceManager& deviceManager, DescriptorHeap& heap);
    template <class T>
    void UploadDataToContentBuffer(const T& data)
    {
        if (std::holds_alternative<T>())
        {
            auto constantBuffer = std::get<T>>(m_data);
            constantBuffer.UpdateCpuData(static_cast<void*>(&data));
        }
    }


    void Print() const;

    size_t m_rootParamIndex;
    size_t m_nameHash;
    using ShaderParameterData = std::variant<WVPBufferContent, MaterialContent, PerFrameConstants, TextureData>;
    ShaderParameterData m_data;
    ConstantBuffer m_cbData;
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
    std::variant<WVPBufferContent, MaterialContent, PerFrameConstants> m_data;
};

using ShaderParamersData = std::vector<ShaderParameterData>;