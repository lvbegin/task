#ifndef WORK_QUEUE_H__
#define WORK_QUEUE_H__

#include "Executor.h"
#include "ExecutorPool.h"
#include <queue>
#include <mutex>
#include <condition_variable>

namespace task {
namespace internal {

class WorkQueue : public WorkReader, public WorkWriter {
    public:
        WorkQueue();
        ~WorkQueue();

        void put(Work t);
        Work get(std::function<bool(void)> pred = [] (){ return false;});
        void notifyPred();
    private:
        std::condition_variable condition;
        std::mutex m;
        std::queue<Work> q;
};

}
}

#endif