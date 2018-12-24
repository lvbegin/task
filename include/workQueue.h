#ifndef WORK_QUEUE_H__
#define WORK_QUEUE_H__

#include "executor.h"
#include "executorPool.h"
#include <queue>
#include <mutex>
#include <condition_variable>

namespace task {
namespace internal {

class WorkQueue : public WorkReader, public WorkWriter {
    public:
        WorkQueue();
        ~WorkQueue();

        void put(work t);
        work get(std::function<bool(void)> pred = [] (){ return false;});
        void notifyPred();
    private:
        std::condition_variable condition;
        std::mutex m;
        std::queue<work> q;
};

}
}

#endif