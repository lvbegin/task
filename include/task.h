#ifndef TASK_H__
#define TASK_H__

#include "executorPool.h"

#include <future>
#include <iostream>

namespace task {

void setWorkPoolSize(unsigned int size);

extern void sendWorkToPool(std::function<void(void)> &&w);

template <class t>
std::future<t> TASK(std::function<t (void)> task) {
    auto  promise = std::make_shared<std::promise<t>>();
    auto w = [ promise, task ](void) {
        try {
            promise->set_value(task()); 
        }
        catch (...) {
            try {
                promise->set_exception(std::current_exception());
            }
            catch (...) { 
                std::cerr << "ERROR: cannot set exception as task answer." << std::endl;  
            } 
        }
    }; 
    sendWorkToPool(std::move(w));
    return promise->get_future();
}

}

#endif