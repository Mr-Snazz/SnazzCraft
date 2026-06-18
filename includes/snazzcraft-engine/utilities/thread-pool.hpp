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
    class ThreadPool
    {
    public:
        ThreadPool(size_t ThreadCount);

        ~ThreadPool();

        void Enqueue(std::function<void()> Task);

    private:
        std::queue<std::function<void()>> TaskQueue;
        std::mutex QueueMutex;

        std::vector<std::jthread> Threads;
        std::condition_variable Condition;
        bool ShouldStop;
    };
}

