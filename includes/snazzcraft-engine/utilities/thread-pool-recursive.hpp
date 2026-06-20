#pragma once

#include "snazzcraft-engine/utilities/thread-pool-base.hpp"

namespace SnazzCraft
{
    // Tasks should return true if they should be removed
    class ThreadPoolRecursive : public SnazzCraft::ThreadPoolBase<bool>
    {
    public:
        ThreadPoolRecursive(size_t ThreadCount)
        {
            auto ThreadFunction = [this]() -> void
            {
                while (true)
                {
                    std::function<bool()> Task;

                    {
                        std::unique_lock<std::mutex> Lock(this->QueueMutex);

                        // Wait until a task is availuble or the pool is stopping
                        this->Condition.wait(Lock, [this]{ return this->ShouldStop || !this->TaskQueue.empty(); });
                        if (this->ShouldStop && this->TaskQueue.empty()) return;

                        Task = std::move(this->TaskQueue.front());
                        this->TaskQueue.pop();
                    }

                    if (Task()) continue;
                        
                    {
                        std::unique_lock<std::mutex> Lock(this->QueueMutex);
                        this->TaskQueue.push(Task);
                    }
                }
            };

            for (size_t I{}; I < ThreadCount; I++) this->Threads.emplace_back(ThreadFunction);
        }
    };
}


