#pragma once

#include <unordered_map>
#include <vector>

#include "dfl/task/ChaseLevDeque.h"
#include "dfl/task/Worker.h"

namespace dfl::task {
  class WorkerGroup {
    friend class Worker;

    size_t _worker_count;
    std::vector<ChaseLevDeque> _queues;
    FAAQueue _global;
    std::unordered_map<std::thread::id, Worker*> _worker_map;
    Worker* _workers;

    static void set_current(WorkerGroup &wg);

  public:
    static WorkerGroup *get_current();

    WorkerGroup(const WorkerGroup &) = delete;
    WorkerGroup &operator=(const WorkerGroup &) = delete;

    explicit WorkerGroup(size_t size);
    ~WorkerGroup();

    void stop();
    void push(const Job &job);
  };
}
