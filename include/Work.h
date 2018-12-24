#ifndef WORK_H__
#define WORK_H__

#include <functional>

namespace task {
namespace internal {

typedef std::function<void(void)> Work;

}
}

#endif