#pragma once

#include <stdint.h>
#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>

namespace SnazzCraft
{
    class ThreadPoolBase
    {
    public:
        virtual ~ThreadPoolBase();

        void Enqueue(std::function<void()> Task);

    protected:
        ThreadPoolBase() = default;

        std::vector<std::jthread> Threads;
        std::condition_variable Condition;
        
        std::queue<std::function<void()>> TaskQueue;
        std::mutex QueueMutex;

        bool ShouldStop{};
    };
}

