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
 * @brief Implementation for TaskQueue
*/

#include "CompuBrite/SFML/TaskQueue.h"

namespace CompuBrite::SFML {

void
TaskQueue::add(TaskQueue::Task &&task)
{
    _queue.emplace_back(std::move(task));
}

TaskQueue::Task
TaskQueue::take()
{
    if (_queue.empty()) {
        return Task();
    }
    auto ret = std::move(_queue.back());
    _queue.pop_back();
    return ret;
}

TaskQueue&
operator<<(TaskQueue &queue, TaskQueue::Task &&task)
{
    queue.add(std::move(task));
    return queue;
}

} // namespace CompuBrite::SFML
