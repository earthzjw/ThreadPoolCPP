#ifndef THREAD_POOL_H
#define THREAD_POOL_H


#include "workQueue.h"
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <string>
#include <vector>

class ThreadPool{
    public:
        using ThreadFunc = std::function<void()>;
        ThreadPool(int threadNum);
        ThreadPool(const ThreadPool&) = delete;
        void operator=(const ThreadPool&) = delete;
        bool isRunning();
        ~ThreadPool();
        void run();
        void enqueue(ThreadFunc task);

    private:
        bool run_;
        int threadNum;
        int maxThreadNum;
        std::atomic<int> workingThreadCount;  // Number of threads currently working
        std::condition_variable cv;
        std::vector<std::thread> pool_;
        std::mutex mtx;
        std::string name;
        std::queue<ThreadFunc> taskFuncQueue;
};

#endif