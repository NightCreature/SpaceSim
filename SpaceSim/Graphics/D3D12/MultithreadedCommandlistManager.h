#pragma once

#include "Graphics\D3D12\CommandQueue.h"

#include <array>
#include <atomic>
#include <functional>
#include <mutex>
#include <string>

class Resource;

class MultithreadedCommandlistManager
{
public:
    MultithreadedCommandlistManager() = default;
    ~MultithreadedCommandlistManager() = default;

    void Cleanup();
    void Initialise(Resource* resource, const std::string_view& name);

    bool GetCommandListHandleForThreadIndex(size_t threadIndex, CommandList& commandList);
    void ReturnCommandListForThreadIndex(size_t threadIndex);

    void Update();

    template<class T>
    void SetCallbackOnCommandlistFinished(T func) { m_func = func; }
private:

    HRESULT ResetCommandListAndAllocator(auto& resourceCommandQueue, size_t index);

    Resource* m_resource;
    std::mutex m_mutex;
    struct CommandListStatus
    {
        CommandListStatus() : m_semaphore(1) {}

        std::atomic<int> m_semaphore; // -1 finished recording, 0 recording on list, 1 can be used to start recording commands
        size_t m_fenceValue = static_cast<size_t>(-1); //This tracks the fence value the GPU needs to signal before we can reset it.
    };
    std::array< CommandListStatus, CommandQueue::MaxCommandLists> m_commandListStatus;

    size_t m_queueHandle = ~0ull; //Handle to the actual command queue object

    std::function<void(size_t)> m_func;
};