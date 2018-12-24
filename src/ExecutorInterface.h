#ifndef EXECUTOR_INTERFACE_H__
#define EXECUTOR_INTERFACE_H__

namespace task {
namespace internal {

class ExecutorInterface {
    public:
        virtual ~ExecutorInterface() = default;
    protected:
        ExecutorInterface() = default;
};

}
}

#endif