#include "Executor.h"

namespace task {
namespace internal {

static void thread_body(WorkReader *channel, bool *terminated) noexcept {
    for ( ; ; ) {
        try {
            channel->get([terminated](){ return *terminated; })();
        } 
        catch (WorkReader::PredicateIsTrue &e) {
            return ;
        }   
        catch (...) { }
    }
}

Executor::Executor(WorkReader &channel) : terminated(false), channel(channel), t(thread_body, &channel, &this->terminated) { }

Executor::~Executor() { 
    stop();
    t.join(); 
}

void Executor::stop() { 
    terminated = true; 
    channel.notifyPred();
}

}
}