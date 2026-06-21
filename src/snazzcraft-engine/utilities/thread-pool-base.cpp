#include "snazzcraft-engine/utilities/thread-pool-base.hpp"

SnazzCraft::ThreadPoolBase::~ThreadPoolBase()
{
    {
        std::lock_guard<std::mutex> Lock(this->QueueMutex);
        this->ShouldStop = true;
    }
    
    this->Condition.notify_all();
    this->Threads.clear();
}

void SnazzCraft::ThreadPoolBase::Enqueue(std::function<void()> Task)
{
    {
        std::lock_guard<std::mutex> Lock(this->QueueMutex);
        this->TaskQueue.emplace(std::move(Task));
    }

    this->Condition.notify_one();
}