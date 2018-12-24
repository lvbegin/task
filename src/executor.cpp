#include "executor.h"

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

executor::executor(WorkReader &channel) : terminated(false), channel(channel), t(thread_body, &channel, &this->terminated) { }

executor::~executor() { 
    stop();
    t.join(); 
}

void executor::stop() { 
    terminated = true; 
    channel.notifyPred();
}

}
}