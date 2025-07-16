#include "task/Job.h"

#include "Types.h"

namespace dfl::task {
  void Job::operator()() const {
    dfl_assert(_fn && "_fn cannot be null");
    _fn(_data);
  }
}
