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

        void Enqueue(std::function<void(void*)> Task, void* Argument);

    private:
        std::queue<std::function<void(void*)>> TaskQueue;
        std::queue<void*> ArgumentQueue;
        std::mutex QueueMutex;

        std::vector<std::jthread> Threads;
        std::condition_variable Condition;
        bool ShouldStop;
    };
}

