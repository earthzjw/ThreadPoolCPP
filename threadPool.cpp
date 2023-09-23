
#include "threadPool.h"
#include <cassert>
#include <iostream>

ThreadPool::ThreadPool(int numThreads)
    : run_(false), threadNum(numThreads), maxThreadNum(numThreads), workingThreadCount(0) {}

bool ThreadPool::isRunning() {
    return run_;
}

ThreadPool::~ThreadPool() {
    assert (run_);
    run_ = false;
    cv.notify_all();
    for (std::thread& thread : pool_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void ThreadPool::run() {
    run_ = true;
    for (int i = 0; i < threadNum; ++i) {
        pool_.emplace_back([this] {
            while (run_) {
                ThreadFunc task;
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this] { return !run_ || !taskFuncQueue.empty(); });
                    if (!run_ && taskFuncQueue.empty()) {
                        return;
                    }
                    task = std::move(taskFuncQueue.front());
                    taskFuncQueue.pop();
                    ++workingThreadCount;
                }
                task();
                --workingThreadCount;
            }
        });
    }
}

void ThreadPool::enqueue(ThreadFunc task){
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !run_ || taskFuncQueue.size() < threadNum; });
        taskFuncQueue.push(task);
    }
    cv.notify_all();

}

