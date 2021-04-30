#include "Graphics/D3D12/CommandQueue.h"

#include "Core/StringOperations/StringHelperFunctions.h"
#include "Graphics/D3D12/DeviceManagerD3D12.h"

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
        hr = m_deviceManager.GetDevice()->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&commandList.m_list));
        if (hr != S_OK)
        {
            MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to create closed command list with error: 0x%x, %s", hr, getLastErrorMessage(hr));
            return std::numeric_limits<size_t>::max();
        }

        m_commandLists[m_commandListHighIndex++] = commandList;
    }
    return m_commandListHighIndex - 1;
}
