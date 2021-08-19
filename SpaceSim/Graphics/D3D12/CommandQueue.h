#pragma once

#include <array>
#include <assert.h>
#include <d3d12.h>
#include <vector>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

class DeviceManager;

struct CommandList
{
    ID3D12CommandAllocator* m_alloctor = nullptr;
    ID3D12GraphicsCommandList* m_list = nullptr;
};

struct CommandQueue
{
    using CommandLists = std::array<CommandList, 16>;

    CommandQueue(const DeviceManager& deviceManager) : m_deviceManager(deviceManager) {}

    CommandLists& GetCommandLists() { return m_commandLists; }
    size_t CreateCommandList();
    CommandList& GetCommandList(size_t commandListHandle) { assert(!m_commandLists.empty() && commandListHandle < m_commandLists.size()); return m_commandLists[commandListHandle]; }

    CommandLists m_commandLists;

    const DeviceManager& m_deviceManager; //So we can get the device from this
    ID3D12CommandQueue* m_queue = nullptr;
    ID3D12Fence* m_fence = nullptr;
    HANDLE m_fenceEvent = nullptr;

    size_t m_commandListHighIndex = 0;
};