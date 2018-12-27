#ifndef EXECUTOR_POOL_H__
#define EXECUTOR_POOL_H__

#include "Work.h"
#include "ExecutorInterface.h"

#include <vector>
#include <memory>

namespace task {
namespace internal {

class WorkWriter {
    public: 
        virtual void put(Work w) = 0;
        virtual ~WorkWriter() = default;
    protected:
        WorkWriter() = default;
};

typedef std::vector<std::unique_ptr<ExecutorInterface>> Executors;

class ExecutorPool {
    public:
        ExecutorPool(std::unique_ptr<WorkWriter> queue, Executors executors);
        ExecutorPool(ExecutorPool &&pool);
        ~ExecutorPool();
        
        void newWork(Work w) const;
    private:
        std::unique_ptr<WorkWriter> queue;
        Executors executors;
};

}
}

#endif