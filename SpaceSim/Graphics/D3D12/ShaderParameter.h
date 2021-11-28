#pragma once

#include "Graphics/ShaderDataStructures.h"
#include "Math/matrix44.h"

#include <vector>
#include <variant>
#include "ConstantBuffer.h"

class DeviceManager;
struct DescriptorHeap;



struct ShaderParameter
{    void CreateConstantBuffer(const DeviceManager& deviceManager, DescriptorHeap& heap);
    template <class T>
    void UploadDataToContentBuffer(const T& data)
    {
        if (std::holds_alternative<ConstantBuffer<T>>())
        {
            auto constantBuffer = std::get<ConstantBuffer<T>>(m_constantBuffer);
            constantBuffer.UpdateCPUData(data);
        }
    }

    size_t m_rootParamIndex;
    size_t m_nameHash;
    std::variant<ConstantBuffer<WVPBufferContent>, ConstantBuffer<MaterialContent>, ConstantBuffer<PerFrameConstants>> m_constantBuffer;

};

using ShaderParameters = std::vector<ShaderParameter>;