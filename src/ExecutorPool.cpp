#include "ExecutorPool.h"
#include <algorithm>

namespace task {
namespace internal {

ExecutorPool::ExecutorPool(std::unique_ptr<WorkWriter> queue, Executors executors) : queue(std::move(queue)), executors(std::move(executors)) { }

ExecutorPool::ExecutorPool(ExecutorPool &&pool) {
    queue = std::move(pool.queue);
    executors = std::move(pool.executors);
}

ExecutorPool::~ExecutorPool() = default;     

void ExecutorPool::newWork(Work t) { queue->put(t); }

}
}