#pragma once

#include <array>
#include <assert.h>
#include <d3d12.h>
#include <string>
#include <vector>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

class DeviceManager;
class Resource;

struct CommandList
{
    ID3D12CommandAllocator* m_alloctor = nullptr;
    ID3D12GraphicsCommandList* m_list = nullptr;
};

struct CommandQueue
{
    constexpr static size_t MaxCommandLists = 32;
    constexpr static size_t InvalidCommandListHandle = MaxCommandLists;
    using CommandLists = std::array<CommandList, MaxCommandLists>;
    

    CommandQueue(const DeviceManager& deviceManager) : m_deviceManager(deviceManager) {}
    CommandQueue(const CommandQueue&) = default;

    CommandQueue& operator =(const CommandQueue&) = default;

    CommandLists& GetCommandLists() { return m_commandLists; }
    size_t CreateCommandList();
    CommandList& GetCommandList(size_t commandListHandle) { assert(!m_commandLists.empty() && commandListHandle < m_commandLists.size()); return m_commandLists[commandListHandle]; }

    void SetName(const std::string_view& commandListName);
    void Cleanup();

    CommandLists m_commandLists;

//#ifdef DEBUG
    std::wstring m_name;
//#endif

    const DeviceManager& m_deviceManager; //So we can get the device from this
    ID3D12CommandQueue* m_queue = nullptr;
    ID3D12Fence* m_fence = nullptr;
    HANDLE m_fenceEvent = nullptr;

    size_t m_commandListHighIndex = 0;
    size_t m_fenceValue = ~0ull;
};

class CommandQueueManager
{
public:
    CommandQueueManager() {}
    ~CommandQueueManager() {}

    void Initialise(Resource* resource) { m_resource = resource; }
    void Cleanup();

    std::vector<CommandQueue>& GetCommandQueues() { return m_commandQueues; }
    size_t CreateCommandQueue();
    CommandQueue& GetCommandQueue(size_t commandQueueHandle)
    {
        assert(!m_commandQueues.empty() && commandQueueHandle < m_commandQueues.size());
        return m_commandQueues[commandQueueHandle];
    }
private:
    std::vector<CommandQueue> m_commandQueues;

    Resource* m_resource = nullptr;
};