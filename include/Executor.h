#ifndef EXECUTOR_H__
#define EXECUTOR_H__

#include "Work.h"
#include "ExecutorInterface.h"

#include <thread>
#include <functional>
#include <exception>

namespace task {
namespace internal {

class WorkReader {
    public:
        virtual Work get(std::function<bool(void)> pred = []() {return false; }) = 0;
        virtual void notifyPred() = 0;
        virtual ~WorkReader() = default;

        class PredicateIsTrue : public std::exception { };
    protected:
        WorkReader() = default;
};

class Executor : public ExecutorInterface {
    public:
        Executor(WorkReader &channel);
        ~Executor();
    private:
        bool terminated;
        WorkReader &channel;
        std::thread t;

        void stop();
};

}
}

#endif