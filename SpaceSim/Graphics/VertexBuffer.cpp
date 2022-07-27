#include "Graphics/VertexBuffer.h"
#include "Graphics/DeviceManager.h"

#include <sstream>
#include <iostream>

#include "Core/StringOperations/StringHelperFunctions.h"

#include "Graphics/D3DDebugHelperFunctions.h"

#include <mutex>

#include "Graphics/D3D12/D3D12X.h"
#include "Graphics/D3D12/CommandQueue.h"
#include "Core/Types/Types.h"

static size_t vbCounter = 0;

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void VertexBuffer::Create(const DeviceManager& deviceManager, CommandList& commandList, size_t bufferSize, void* data, size_t vertexStride)
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
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
const std::vector<D3D12_INPUT_ELEMENT_DESC>& VertexDeclarationDescriptor::createInputElementLayout(size_t& vertexStride)
{
    //Create the buffer layout elements
    unsigned int numberOfElements = 0;
    if (position > 0)
    {
        ++numberOfElements;
    }
    if (normal)
    {
        ++numberOfElements;
    }
    numberOfElements += (unsigned int)textureCoordinateDimensions.size();
    m_vertexDataLayoutElements.reserve(numberOfElements);//New overwrites data on the stack
    vertexStride = 0;
    if (position > 0)
    {
        D3D12_INPUT_ELEMENT_DESC layout;
        layout.SemanticName = "POSITION";
        layout.SemanticIndex = 0; 
        switch (position)
        {
        case 1:
            layout.Format = DXGI_FORMAT_R32_FLOAT;
            break;
        case 2:
            layout.Format = DXGI_FORMAT_R32G32_FLOAT;
            break;
        default:
        case 3:
            layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
            break;
        case 4:
            layout.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            break;
        }
        layout.InputSlot = 0;
        layout.AlignedByteOffset = static_cast<unsigned int>(vertexStride);
        layout.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        layout.InstanceDataStepRate = 0;
        m_vertexDataLayoutElements.push_back(layout);
        vertexStride += sizeof(float) * position;
    }

    if (normal)
    {
        D3D12_INPUT_ELEMENT_DESC layout;
        layout.SemanticName = "NORMAL";
        layout.SemanticIndex = 0; 
        layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        layout.InputSlot = 0;
        layout.AlignedByteOffset = static_cast<unsigned int>(vertexStride);
        layout.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        layout.InstanceDataStepRate = 0;
        m_vertexDataLayoutElements.push_back(layout);
        vertexStride += 12;
    }

    if (tangent)
    {
        D3D12_INPUT_ELEMENT_DESC layout;
        layout.SemanticName = "TANGENT";
        layout.SemanticIndex = 0;
        layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        layout.InputSlot = 0;
        layout.AlignedByteOffset = static_cast<unsigned int>(vertexStride);
        layout.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        layout.InstanceDataStepRate = 0;
        m_vertexDataLayoutElements.push_back(layout);
        vertexStride += 12;
    }

    if (vertexColor)
    {
        D3D12_INPUT_ELEMENT_DESC layout;
        layout.SemanticName = "COLOR";
        layout.SemanticIndex = 0; 
        layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        layout.InputSlot = 0;
        layout.AlignedByteOffset = static_cast<unsigned int>(vertexStride);
        layout.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        layout.InstanceDataStepRate = 0;
        m_vertexDataLayoutElements.push_back(layout);
        vertexStride += 16;
    }

    for (unsigned int counter = 0; counter < textureCoordinateDimensions.size(); ++counter)
    {
        D3D12_INPUT_ELEMENT_DESC layout;
        layout.SemanticName = "TEXCOORD";
        layout.SemanticIndex = counter; 
        layout.InputSlot = 0;
        layout.AlignedByteOffset = static_cast<unsigned int>(vertexStride);
        layout.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        layout.InstanceDataStepRate = 0;
        if (textureCoordinateDimensions[counter] == 2)
        {
            layout.Format = DXGI_FORMAT_R32G32_FLOAT;
            vertexStride += 8;
        }
        else if (textureCoordinateDimensions[counter] == 3)
        {
            layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
            vertexStride += 12;
        }
        else if (textureCoordinateDimensions[counter] == 4)
        {
            layout.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
            vertexStride += 16;
        }
        else
        {
            //Assume 2D texcoords
            layout.Format = DXGI_FORMAT_R32G32_FLOAT;
            vertexStride += 8;
        }
        m_vertexDataLayoutElements.push_back(layout);
        
    }

    return m_vertexDataLayoutElements;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
size_t VertexDeclarationDescriptor::GetVertexStride() const
{
    size_t stride = 0;
    stride = position * sizeof(float);
    stride += normal ? 3 * sizeof(float) : 0;
    stride += tangent ? 3 * sizeof(float) : 0;
    stride += vertexColor ? 4 * sizeof(float) : 0;
    
    for (unsigned int counter = 0; counter < textureCoordinateDimensions.size(); ++counter)
    {
        stride += textureCoordinateDimensions[counter] * sizeof(float);
    }

    return stride;
}

