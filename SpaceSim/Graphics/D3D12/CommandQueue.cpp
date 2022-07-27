#include "Graphics/D3D12/CommandQueue.h"

#include "Core/Resource/Resourceable.h"
#include "Core/Resource/RenderResource.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Graphics/D3D12/DeviceManagerD3D12.h"
#include <sstream>

static size_t clCounter = 0;

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
size_t CommandQueue::CreateCommandList()
{
    CommandList commandList;
    if (m_commandListHighIndex < m_commandLists.size())
    {
        HRESULT hr = m_deviceManager.GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandList.m_alloctor));
        if (hr != S_OK)
        {
            MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to create command allocator with error: 0x%x, %s", hr, getLastErrorMessage(hr));
            return std::numeric_limits<size_t>::max();
        }

#ifdef DEBUG
        std::wstringstream str;
        str << m_name.c_str() << L":CommandAllocator" << clCounter;
        commandList.m_alloctor->SetName(str.str().c_str());
#endif

        hr = m_deviceManager.GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandList.m_alloctor, nullptr, IID_PPV_ARGS(&commandList.m_list));
        if (hr != S_OK)
        {
            MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to create closed command list with error: 0x%x, %s", hr, getLastErrorMessage(hr));
            return std::numeric_limits<size_t>::max();
        }

#ifdef DEBUG
        str = std::wstringstream();
        str << m_name.c_str() << L":CommandList" << clCounter;
        commandList.m_alloctor->SetName(str.str().c_str());
        ++clCounter;
#endif

        m_commandLists[m_commandListHighIndex++] = commandList;
    }
    return m_commandListHighIndex - 1;
}


///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void CommandQueue::SetName(std::string commandQueueName)
{
#ifdef DEBUG
    std::wstring temp;
    convertToWideString(commandQueueName, temp);
    m_name += temp;
    //m_queue->SetName(m_name.c_str());
#endif
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
size_t CommandQueueManager::CreateCommandQueue()
{
    RenderResourceHelper renderResource = RenderResourceHelper(m_resource);
    auto& deviceManager = renderResource.getWriteableResource().getDeviceManager();

    CommandQueue queue(deviceManager);

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    HRESULT hr = deviceManager.GetDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue.m_queue));
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to create a command queue with error: 0x%x, %s", hr, getLastErrorMessage(hr));
        return std::numeric_limits<size_t>::max();
    }

#ifdef _DEBUG
    std::wstringstream str;
    str << L"CommandQueue" << m_commandQueues.size();
    queue.m_queue->SetName(str.str().c_str());
    queue.m_name = str.str();
#endif

    hr = deviceManager.GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&queue.m_fence));
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to create a GPU fence with error: 0x%x, %s", hr, getLastErrorMessage(hr));
        return std::numeric_limits<size_t>::max();
    }
#ifdef _DEBUG
    str << L"Fence";
    queue.m_queue->SetName(str.str().c_str());
#endif
    queue.m_fenceValue = 1;

    queue.m_fenceEvent = CreateEvent(nullptr, false, false, nullptr);
    if (queue.m_fenceEvent == 0)
    {
        hr = GetLastError();
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to create an Event with error: 0x%x, %s", hr, getLastErrorMessage(hr));
        return std::numeric_limits<size_t>::max();
    }

    m_commandQueues.push_back(queue);
    return m_commandQueues.size() - 1;
}
