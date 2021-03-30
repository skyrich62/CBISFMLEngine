/**
 * The MIT License (MIT)
 *
 * @copyright
 * Copyright (c) 2020 Rich Newman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file
 * @brief Interface for ThreadPool
*/

#ifndef COMPUBRITE_SFML_THREADPOOL_H
#define COMPUBRITE_SFML_THREADPOOL_H

#include <CompuBrite/SFML/TaskQueue.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

namespace CompuBrite::SFML
{

/**
 * Manage a pool of threads and tasks for the threads to perform.
 */
class ThreadPool
{
public:
    using Task = TaskQueue::Task;

    ThreadPool() = default;
    ~ThreadPool();

    ThreadPool(const ThreadPool &) = delete;
    ThreadPool& operator=(const ThreadPool &) = delete;

    /// Add a task to the TaskQueue for this pool. Free threads will take
    /// Tasks from the front of the queue and execute them.
    /// @see TaskQueue
    void addTask(Task &&task);

    /// Add a task with a promise.
    /// @overload
    template <typename Ret>
    std::promise<Ret> addTask(std::function<Ret ()> work)
    {
        std::promise<Ret> promise;
        auto task = [work, &promise] ()
        {
            try {
                auto r = work();
                promise.set_value(r);
            } catch (...) {
                promise.set_exception(std::current_exception());
            }
        };
        addTask(std::move(task));
        return promise;
    }

    /// Activate the ThreadPool and add active threads to it.
    /// This can be called many times.  Each time, it will add new threads
    /// to the pool.
    /// @param n The number of threads to add to the pool.
    void activate(std::size_t n = 1);

    /// Shutdown the ThreadPool.  All waiting threads will wake up and terminate.
    /// All running threads will finish their tasks and then terminate.
    void shutdown();

private:
    using ThreadPtr = std::unique_ptr<std::thread>;
    using Pool      = std::list<ThreadPtr>;
    using Mutex     = std::mutex;
    using Lock      = std::unique_lock<Mutex>;
    using Cond      = std::condition_variable;

    /// This is the function that all threads in the pool will execute.
    /// Each thread will wait for the TaskQueue to be non-empty or the
    /// ThreadPool to be shutdown.  If shutdown, then the thread will terminate.
    /// If the TaskQueue is non-empty, then the thread will take the next
    /// task from the TaskQueue and execute it.
    void svc();

    /// Lock the mutex and return a std::unique_lock wrapper.
    Lock lock()                             { return Lock(_mutex); }

private:
    TaskQueue         _queue;
    Pool              _pool;
    mutable Mutex     _mutex;
    Cond              _empty;
    bool              _shutdown{false};
};

ThreadPool&
operator<<(ThreadPool &pool, TaskQueue::Task &&task);
} // namespace CompuBrite::SFML

#endif // COMPUBRITE_SFML_THREADPOOL_H
