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
    template <typename Return>
    class ThreadPoolBase
    {
    public:
        virtual ~ThreadPoolBase()
        {
            {
                std::lock_guard<std::mutex> Lock(this->QueueMutex);
                this->ShouldStop = true;
            }
            
            this->Condition.notify_all();
            this->Threads.clear();
        }

        void Enqueue(std::function<Return()> Task)
        {
            {
                std::lock_guard<std::mutex> Lock(this->QueueMutex);
                this->TaskQueue.emplace(std::move(Task));
            }

            this->Condition.notify_one();
        }

    protected:
        ThreadPoolBase() = default;

        std::vector<std::jthread> Threads;
        std::condition_variable Condition;
        

        std::queue<std::function<Return()>> TaskQueue;
        std::mutex QueueMutex;

        bool ShouldStop{};
    };
}

