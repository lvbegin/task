#ifndef EXECUTOR_H__
#define EXECUTOR_H__

#include "work.h"
#include "ExecutorInterface.h"

#include <thread>
#include <functional>
#include <exception>

namespace task {
namespace internal {

class WorkReader {
    public:
        virtual work get(std::function<bool(void)> pred = []() {return false; }) = 0;
        virtual void notifyPred() = 0;
        virtual ~WorkReader() = default;

        class PredicateIsTrue : public std::exception { };
    protected:
        WorkReader() = default;
};

class executor : public ExecutorInterface {
    public:
        executor(WorkReader &channel);
        ~executor();
    private:
        bool terminated;
        WorkReader &channel;
        std::thread t;

        void stop();
};

}
}

#endif