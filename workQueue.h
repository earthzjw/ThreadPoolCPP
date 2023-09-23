#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

#include <cassert>
#include <condition_variable>
#include <mutex>
#include <queue>

template<typename T>
class workQueue{
    public:
        workQueue(int num = 8): nums(num) {};
        ~workQueue(){};
        workQueue(const workQueue&) = delete;
        void operator=(const workQueue&) = delete;
        T top(){
            std::unique_lock<std::mutex> lock(qMutex);
            return queue_.front();
        }
        T pop(){
            std::unique_lock<std::mutex> lock(qMutex);
            T temp = queue_.front();
            queue_.pop();
            return temp;
        }
        void push(T& something){
            std::unique_lock<std::mutex> lock(qMutex);
            cv.wait(lock, [this]{ return nums > queue_.size(); });
            queue_.push(something);
            cv.notify_one();
        }
    private:
        int nums;
        std::queue<T> queue_;
        std::mutex qMutex;
        std::condition_variable cv;
};

#endif