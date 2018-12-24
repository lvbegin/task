#include "Task.h"
#include "WorkQueue.h"
#include "Executor.h"
#include "ExecutorPool.h"

#include <mutex>

namespace task {

static unsigned int numberExecutor = 5;

static std::unique_ptr<internal::ExecutorPool> pool;

static void initialize() {
    auto p = std::make_unique<internal::WorkQueue>();
    internal::Executors executors;
    for (unsigned int i = 0; i < numberExecutor; i++)
        executors.emplace_back(std::make_unique<internal::Executor>(*p));

    pool.reset(new internal::ExecutorPool(std::move(p), std::move(executors)));
}

void sendWorkToPool(std::function<void(void)> &&w) {
    static std::once_flag flag;
    std::call_once(flag, initialize);
    pool->newWork(w);
}

void setWorkPoolSize(unsigned int size) {
    if (nullptr != pool)
        throw std::runtime_error("worker pool already initialized.");
    numberExecutor = size;
}

}