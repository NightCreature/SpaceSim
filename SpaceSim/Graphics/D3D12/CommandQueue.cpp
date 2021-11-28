#include "Graphics/D3D12/CommandQueue.h"

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

#ifdef _DEBUG
        std::wstringstream str;
        str << L"CommandAllocator" << clCounter;
        commandList.m_alloctor->SetName(str.str().c_str());
#endif

        hr = m_deviceManager.GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandList.m_alloctor, nullptr, IID_PPV_ARGS(&commandList.m_list));
        if (hr != S_OK)
        {
            MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to create closed command list with error: 0x%x, %s", hr, getLastErrorMessage(hr));
            return std::numeric_limits<size_t>::max();
        }

#ifdef _DEBUG
        str = std::wstringstream();
        str << L"CommandList" << clCounter;
        commandList.m_alloctor->SetName(str.str().c_str());
#endif

        m_commandLists[m_commandListHighIndex++] = commandList;
    }
    return m_commandListHighIndex - 1;
}
