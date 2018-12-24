#include "Executor.h"
#include "ExecutorPool.h"
#include "WorkQueue.h"
#include "Task.h"

#include <stdlib.h>
#include <future>
#include <functional>
#include <algorithm>
#include <iostream>

template<class o>
std::future<o> create_future(o v) {
    std::promise<o> p;
    p.set_value(v);
    return p.get_future();
}

template<class o>
void print(std::function <o(void)> arg) {
    std::cout << arg() << std::endl;
}

static int test_promise() {
    auto f = create_future<int>(3);
    print<int>([&f]() { return f.get(); });
    return 0;
}

static int test_work() {
    task::internal::WorkQueue c;
    task::internal::Executor e(c);
    return 0;
}

static int test_WorkQueue() {
    task::internal::WorkQueue p;
    bool called = false;
    p.put([&called ](void) { called = true; std::cout << "hello again!"<< std::endl; } );
    auto t = p.get();
    t();
    if (called)
        std::cout << "function called!" << std::endl;
    else
        std::cout << "function not called!" << std::endl;
    return 0;
}

static int test_ExecutorPool() {
    auto p = std::make_unique<task::internal::WorkQueue>();
    task::internal::Executors executors;
    executors.emplace_back(std::make_unique<task::internal::Executor>(*p));
    executors.emplace_back(std::make_unique<task::internal::Executor>(*p));
    executors.emplace_back(std::make_unique<task::internal::Executor>(*p));

    task::internal::ExecutorPool pool(std::move(p), std::move(executors));
    bool called = false;
    pool.newWork([&called]() {called = true; });

    return 0;
}

static int test_task() {
    auto f = task::TASK<int>([]() { 
        return 1; 
    });
    f.wait();
    std::cout << "value = " << f.get() << std::endl;
    return 0;
}

static int test_task_that_raise_exception() {
    auto f = task::TASK<int>([]() {
        throw std::runtime_error("This is an exception"); 
        return 1; 
    });
    f.wait();

    try {
        std::cout << "value = " << f.get() << std::endl;
    }
    catch (std::runtime_error &e) {
        std::cout << "exception caught" << std::endl;
    }
    return 0;
}

static int test_set_worker_pool_size_fails() {
    try {
        task::setWorkPoolSize(50);
        std::cout << "ERROR: no exception caugh" << std::endl;
    }
    catch (std::runtime_error &e) {
        std::cout << "exception caugh" << std::endl;
    }
    return 0;
}

int main() {
    test_work();
    test_promise();
    test_WorkQueue();
    test_ExecutorPool();
    test_task();
    test_task_that_raise_exception();
    test_set_worker_pool_size_fails();
    return EXIT_SUCCESS;
}