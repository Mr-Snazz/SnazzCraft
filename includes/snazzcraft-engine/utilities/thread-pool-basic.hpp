#pragma once

#include "snazzcraft-engine/utilities/thread-pool-base.hpp"

namespace SnazzCraft
{
    template <typename Return>
    class ThreadPoolBasic : public SnazzCraft::ThreadPoolBase<Return>
    {
    public:
        ThreadPoolBasic(size_t ThreadCount)
        {
            auto ThreadFunction = [this]() -> void
            {
                while (true)
                {
                    std::function<Return()> Task;

                    {
                        std::unique_lock<std::mutex> Lock(this->QueueMutex);

                        // Wait until a task is availuble or the pool is stopping
                        this->Condition.wait(Lock, [this]{ return this->ShouldStop || !this->TaskQueue.empty(); });
                        if (this->ShouldStop && this->TaskQueue.empty()) return;

                        Task = std::move(this->TaskQueue.front());
                        this->TaskQueue.pop();
                    }

                    if (Task) Task();
                }
            };

            for (size_t I{}; I < ThreadCount; I++) this->Threads.emplace_back(ThreadFunction);
        }
    };
}