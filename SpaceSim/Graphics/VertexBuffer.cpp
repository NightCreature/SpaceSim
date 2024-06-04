#include "Graphics/VertexBuffer.h"
#include "Graphics/DeviceManager.h"

#include <sstream>
#include <iostream>

#include "Core/StringOperations/StringHelperFunctions.h"
#include "Logging/LoggingMacros.h"

#include "Graphics/D3DDebugHelperFunctions.h"

#include <mutex>

#include "Graphics/D3D12/D3D12X.h"
#include "Graphics/D3D12/CommandQueue.h"
#include "Core/Types/Types.h"

#ifdef _DEBUG
static size_t vbCounter = 0;
#endif

///-----------------------------------------------------------------------------
///! @brief   
///! @remark This should be a normal buffer more like how our constant buffers work
///-----------------------------------------------------------------------------
void VertexBuffer2::Create(const DeviceManager& deviceManager, CommandList& commandList, size_t bufferSize, void* data, size_t vertexStride)
{
    //D3D12 resource creation
    D3D12_HEAP_PROPERTIES defaultHeap;
    defaultHeap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    defaultHeap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    defaultHeap.Type = D3D12_HEAP_TYPE_DEFAULT;
    defaultHeap.CreationNodeMask = 1;
    defaultHeap.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC resourceDefaultDescriptor;
    resourceDefaultDescriptor.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceDefaultDescriptor.Alignment = 0;
    resourceDefaultDescriptor.Width = bufferSize; //data size
    resourceDefaultDescriptor.Height = 1;
    resourceDefaultDescriptor.DepthOrArraySize = 1;
    resourceDefaultDescriptor.MipLevels = 1;
    resourceDefaultDescriptor.Format = DXGI_FORMAT_UNKNOWN;
    resourceDefaultDescriptor.SampleDesc.Count = 1;
    resourceDefaultDescriptor.SampleDesc.Quality = 0;
    resourceDefaultDescriptor.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceDefaultDescriptor.Flags = D3D12_RESOURCE_FLAG_NONE;


    D3D12_RESOURCE_DESC resourceUploadDescriptor;
    resourceUploadDescriptor.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    resourceUploadDescriptor.Alignment = 0;
    resourceUploadDescriptor.Width = bufferSize; //data size
    resourceUploadDescriptor.Height = 1;
    resourceUploadDescriptor.DepthOrArraySize = 1;
    resourceUploadDescriptor.MipLevels = 1;
    resourceUploadDescriptor.Format = DXGI_FORMAT_UNKNOWN;
    resourceUploadDescriptor.SampleDesc.Count = 1;
    resourceUploadDescriptor.SampleDesc.Quality = 0;
    resourceUploadDescriptor.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    resourceUploadDescriptor.Flags = D3D12_RESOURCE_FLAG_NONE;

    D3D12_HEAP_PROPERTIES uploadHeap;
    uploadHeap.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    uploadHeap.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    uploadHeap.Type = D3D12_HEAP_TYPE_UPLOAD;
    uploadHeap.CreationNodeMask = 1;
    uploadHeap.VisibleNodeMask = 1;

    HRESULT hr = deviceManager.GetDevice()->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &resourceDefaultDescriptor, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_defaultResource));
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("VertexBuffer", "Failed to create the GPU vertex buffer resource");
    }

    hr = deviceManager.GetDevice()->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &resourceUploadDescriptor, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_uploadResource));
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("VertexBuffer", "Failed to create the CPU/Upload vertex buffer resource");
    }
    //Would need to fix this
    //D3DDebugHelperFunctions::SetDebugChildName(m_defaultResource, FormatString("Vertex Buffer with size: %d, no. vertices: %d", bufferSize, m_vertexCount));

    D3D12_SUBRESOURCE_DATA initData = {};
    //ZeroMemory(&initData, sizeof(D3D12_SUBRESOURCE_DATA));
    initData.pData = data;
    initData.RowPitch = bufferSize;
    initData.SlicePitch = bufferSize;

    //First arg is commandlist

    UpdateSubresources<1>(commandList.m_list, m_defaultResource, m_uploadResource, 0, 0, 1, &initData);
    
    //CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
    //uint8* vertexData = nullptr;
    //hr = m_uploadResource->Map(0, &readRange, reinterpret_cast<void**>(&vertexData));
    //memcpy(vertexData, &data, bufferSize);
    //m_uploadResource->Unmap(0, nullptr);

    //commandList.m_list->CopyResource(m_defaultResource, m_uploadResource);

    //Need to create a desciptor again here
    auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_defaultResource, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
    commandList.m_list->ResourceBarrier(1, &barrier);
    m_bufferView.BufferLocation = m_defaultResource->GetGPUVirtualAddress();
    m_bufferView.StrideInBytes = static_cast<unsigned int>(vertexStride);
    m_bufferView.SizeInBytes = static_cast<unsigned int>(bufferSize);

#ifdef _DEBUG
    std::wstringstream str;
    str << L"VertexBuffer" << vbCounter++;
    m_defaultResource->SetName(str.str().c_str());
    str << L"Upload";
    m_uploadResource->SetName(str.str().c_str());
#endif
    //HRESULT hr = deviceManager.GetDevice()->CreateCommittedResource(
    //    &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
    //    D3D12_HEAP_FLAG_NONE,
    //    &CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
    //    D3D12_RESOURCE_STATE_COPY_DEST,
    //    nullptr,
    //    IID_PPV_ARGS(&m_defaultResource));

    ////NAME_D3D12_OBJECT(m_vertexBuffer);

    //{
    //    hr = deviceManager.GetDevice()->CreateCommittedResource(
    //        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
    //        D3D12_HEAP_FLAG_NONE,
    //        &CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
    //        D3D12_RESOURCE_STATE_GENERIC_READ,
    //        nullptr,
    //        IID_PPV_ARGS(&m_uploadResource));

    //    // Copy data to the upload heap and then schedule a copy 
    //    // from the upload heap to the vertex buffer.
    //    D3D12_SUBRESOURCE_DATA vertexData = {};
    //    vertexData.pData = data;
    //    vertexData.RowPitch = bufferSize;
    //    vertexData.SlicePitch = vertexData.RowPitch;

    //    //PIXBeginEvent(commandList.Get(), 0, L"Copy vertex buffer data to default resource...");

    //    UpdateSubresources<1>(commandList.m_list, m_defaultResource, m_uploadResource, 0, 0, 1, &vertexData);
    //    commandList.m_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_defaultResource, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

    //    //PIXEndEvent(commandList.Get());
    //}

    // Initialize the vertex buffer view.
    //m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
    //m_vertexBufferView.SizeInBytes = SampleAssets::VertexDataSize;
    //m_vertexBufferView.StrideInBytes = SampleAssets::StandardVertexStride;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void VertexDeclarationDescriptor::Deserialise(const tinyxml2::XMLElement* node)
{
    //position_dimension = "3" normal = "true" tangent = "true" vertex_color = "false" textureCoordsDimensions = "2"

    auto* attribute = node->FindAttribute("position_dimension");
    if (attribute != nullptr)
    {
        position = attribute->IntValue();
    }
    attribute = nullptr;

    attribute = node->FindAttribute("normal");
    if (attribute != nullptr)
    {
        normal = attribute->BoolValue();
    }
    attribute = nullptr;

    attribute = node->FindAttribute("tangent");
    if (attribute != nullptr)
    {
        tangent = attribute->BoolValue();
    }
    attribute = nullptr;

    attribute = node->FindAttribute("vertex_color");
    if (attribute != nullptr)
    {
        vertexColor = attribute->BoolValue();
    }
    attribute = nullptr;

    attribute = node->FindAttribute("textureCoordsDimensions");
    if (attribute != nullptr)
    {
        const std::string attributeString = attribute->Value();
        //Split this on , for tokens
        auto textureDimensions = tokeniseString(attributeString, ',');
        for (const std::string& texCoordDimension : textureDimensions)
        {
            textureCoordinateDimensions.push_back(strToint(texCoordDimension));
        }
    }
    attribute = nullptr;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
MeshResourceIndices VertexBuffer::CreateBuffer(const DeviceManager& deviceManager, CommandList& commandList, DescriptorHeap& heap, const VertexDataStreams& data)
{
    MeshResourceIndices resourceIndices;

    //Figure out how
    size_t numberOfDataStreams = data.m_streams.size();
    m_data.resize(numberOfDataStreams);

    //Copy data from our inbound data to the GPU here
    auto iterator = data.m_streams.begin();
    auto endIterator = data.m_streams.end();
    size_t strucutredBufferArrayIndex = 0;
    resourceIndices.numberOfTextureStreams = 0;
    for (;iterator != endIterator; ++iterator)
    {
        const VertexStreamType streamType = iterator->first;
        m_data[strucutredBufferArrayIndex] = std::make_pair(streamType, StructuredBuffer());
        const auto& dataVariantArray = iterator->second;
        switch (dataVariantArray.index())
        {
        case 0:
            m_data[strucutredBufferArrayIndex].second.Create(deviceManager, commandList, heap, std::get<0>(dataVariantArray), "Float Data");
            break;
        case 1:
            m_data[strucutredBufferArrayIndex].second.Create(deviceManager, commandList, heap, std::get<1>(dataVariantArray), "Vector2 Data");
            break;
        case 2:
            m_data[strucutredBufferArrayIndex].second.Create(deviceManager, commandList, heap, std::get<2>(dataVariantArray), "Vector3 Data");
            break;
        case 3:
            m_data[strucutredBufferArrayIndex].second.Create(deviceManager, commandList, heap, std::get<3>(dataVariantArray), "Vector4 Data");
            break;
        case 4:
            m_data[strucutredBufferArrayIndex].second.Create(deviceManager, commandList, heap, std::get<4>(dataVariantArray), "Color Data");
            break;
        default:
            MSG_TRACE_CHANNEL("VertexBuffer", "Sending unknonwn data to a vertex buffer, known types are float, Vector2, Vector3, Vector4 and Color");
            break;
        }

        switch (streamType)
        {
        case VertexStreamType::Position:
            resourceIndices.posBufferIndex = static_cast<uint>(m_data[strucutredBufferArrayIndex].second.GetDescriptorIndex());
            break;
        case VertexStreamType::Normal:
            resourceIndices.normalBufferIndex = static_cast<uint>(m_data[strucutredBufferArrayIndex].second.GetDescriptorIndex());
            break;
        case VertexStreamType::Tangent:
            resourceIndices.tangentBufferIndex = static_cast<uint>(m_data[strucutredBufferArrayIndex].second.GetDescriptorIndex());
            break;
        case VertexStreamType::Color:
            resourceIndices.vertexColorBufferIndex = static_cast<uint>(m_data[strucutredBufferArrayIndex].second.GetDescriptorIndex());
            break;
        default:
            break;
        }

        if (streamType >= VertexStreamType::UVStart)
        {
            resourceIndices.textureBufferIndex = static_cast<uint>(m_data[strucutredBufferArrayIndex].second.GetDescriptorIndex());
            ++resourceIndices.numberOfTextureStreams;
        }

        ++strucutredBufferArrayIndex;
    }
    
    return resourceIndices;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void VertexBuffer::Destroy()
{
    for (auto structuredBuffer : m_data)
    {
        structuredBuffer.second.Destroy();
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
VertexDataStreams CreateDataStreams(const VertexDeclarationDescriptor& descriptor)
{
    VertexDataStreams dataStreams;
    switch (descriptor.position)
    {
    case 2:
        dataStreams.m_streams[VertexStreamType::Position] = std::vector<Vector2>();
        break;
    case 3:
    default:
        dataStreams.m_streams[VertexStreamType::Position] = std::vector<Vector3>();
        break;
    case 4:
        dataStreams.m_streams[VertexStreamType::Position] = std::vector<Vector4>();
        break;
    }
    
    if (descriptor.normal)
    {
        dataStreams.m_streams[VertexStreamType::Normal] = std::vector<Vector3>();
    }

    if (descriptor.tangent)
    {
        dataStreams.m_streams[VertexStreamType::Tangent] = std::vector<Vector3>();
    }

    if (descriptor.vertexColor)
    {
        dataStreams.m_streams[VertexStreamType::Color] = std::vector<Color>();
    }

    if (!descriptor.textureCoordinateDimensions.empty())
    {
        //Add UV Streams
        dataStreams.m_uvStreamCount = 0;
        size_t uvStreamStart = static_cast<std::underlying_type_t<VertexStreamType>>(VertexStreamType::UVStart);
        for (const auto& uvDimensions : descriptor.textureCoordinateDimensions)
        {
            switch (uvDimensions)
            {
            case 1:
                dataStreams.m_streams[static_cast<VertexStreamType>(uvStreamStart + dataStreams.m_uvStreamCount)] = std::vector<float>();
            default:
            case 2:
                dataStreams.m_streams[static_cast<VertexStreamType>(uvStreamStart + dataStreams.m_uvStreamCount)] = std::vector<Vector2>();
                break;
            case 3:
                dataStreams.m_streams[static_cast<VertexStreamType>(uvStreamStart + dataStreams.m_uvStreamCount)] = std::vector<Vector3>();
                break;
            case 4:
                dataStreams.m_streams[static_cast<VertexStreamType>(uvStreamStart + dataStreams.m_uvStreamCount)] = std::vector<Vector4>();
                break;
            }
            ++dataStreams.m_uvStreamCount;
        }

    }

    return dataStreams;
}
