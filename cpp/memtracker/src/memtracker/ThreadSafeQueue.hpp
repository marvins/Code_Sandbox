/**
 * @file    ThreadSafeQueue.hpp
 * @author  Marvin Smith
 * @date    11/3/2024
 */
#pragma once

// C++ Standard Libraries
#include <condition_variable>
#include <mutex>
#include <queue>


namespace mem {

/**
 * @class ThreadSafeQueue
 */
template <typename T>
class ThreadSafeQueue {

    public:

        void push(T value) {
            std::lock_guard<std::mutex> lock(mtx);
            queue.push(std::move(value));
            cv.notify_one();
        }

        bool pop(T& result) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]{ return !queue.empty(); });
            result = std::move(queue.front());
            queue.pop();
            return true;
        }

        bool try_pop(T& result) {
            std::lock_guard<std::mutex> lock(mtx);
            if (queue.empty()) {
                return false;
            }
            result = std::move(queue.front());
            queue.pop();
            return true;
        }

        bool empty() const {
            std::lock_guard<std::mutex> lock(mtx);
            return queue.empty();
        }

    private:
        std::queue<T> queue;
        std::mutex mtx;
        std::condition_variable cv;

}; // End of ThreadSafeQueue class

} // End of mem namespace