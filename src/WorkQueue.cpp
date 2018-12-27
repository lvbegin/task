#include "WorkQueue.h"

namespace task {
namespace internal {

WorkQueue::WorkQueue() = default;

WorkQueue::~WorkQueue() = default;

void WorkQueue::put(Work w) {
    std::unique_lock<std::mutex> l(m);
    q.push(std::move(w));
    condition.notify_one();
}

Work WorkQueue::get(std::function<bool(void)> pred) {
    std::unique_lock<std::mutex> l(m);
    bool predicate_is_true = false;
    condition.wait(l, [&predicate_is_true, pred, this]() {  
        if (pred()) {
            predicate_is_true = true;
            return true;
        } 
        return !q.empty(); 
    });
    if (predicate_is_true)
        throw PredicateIsTrue();
    auto w = q.front();
    q.pop();
    return w;
}

void WorkQueue::notifyPred() { condition.notify_all(); }

}
}