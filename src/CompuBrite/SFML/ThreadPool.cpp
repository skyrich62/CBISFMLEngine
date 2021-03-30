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
 * @brief Implementation for ThreadPool
*/

#include "CompuBrite/SFML/ThreadPool.h"

namespace CompuBrite::SFML {

ThreadPool::~ThreadPool()
{
    shutdown();
    for (auto &ptr : _pool) {
        ptr->join();
    }
}

void
ThreadPool::shutdown()
{
    auto l = lock();
    _shutdown = true;
    _empty.notify_all();
}

void
ThreadPool::activate(std::size_t n)
{
    auto l = lock();
    for (auto i = 0u; i < n; ++i) {
        _pool.emplace_back(new std::thread([this]() { svc(); } ));
    }
}

void
ThreadPool::addTask(Task &&task)
{
    auto l = lock();
    _queue.add(std::move(task));
    _empty.notify_all();
}

void
ThreadPool::svc()
{
    while (true) {
        auto l = lock();
        _empty.wait(l, [this]() { return !_queue.empty() || _shutdown; });
        if (_shutdown) {
            return;
        }
        if (_queue.empty()) {
            continue;
        }

        auto task = std::move(_queue.take());
        if (task) {
            l.unlock();
            task();
        }
    }
}

ThreadPool&
operator<<(ThreadPool &pool, ThreadPool::Task &&task)
{
    pool.addTask(std::move(task));
    return pool;
}

} // namespace CompuBrite::SFML
