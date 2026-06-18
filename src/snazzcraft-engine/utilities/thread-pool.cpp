#include "snazzcraft-engine/utilities/thread-pool.hpp"

SnazzCraft::ThreadPool::ThreadPool(size_t ThreadCount)
    : ShouldStop(false)
{
    auto ThreadFunction = [this]() -> void
    {
        while (true)
        {
            std::function<void()> Task;

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

    for (size_t I = 0u; I < ThreadCount; I++) this->Threads.emplace_back(ThreadFunction);
}

SnazzCraft::ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> Lock(this->QueueMutex);
        this->ShouldStop = true;
    }
    
    this->Condition.notify_all();
}

void SnazzCraft::ThreadPool::Enqueue(std::function<void()> Task)
{
    {
        std::lock_guard<std::mutex> Lock(this->QueueMutex);
        this->TaskQueue.emplace(std::move(Task));
    }

    this->Condition.notify_one();
}


