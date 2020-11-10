//
//  parallel.cpp
//  src.common
//
//  Created by 杨丰 on 2020/11/3.
//

#include "parallel.h"

#include <memory>
#include <thread>

#if defined(JET_TASKING_TBB)
# include <tbb/task_arena.h>
# include <tbb/task_scheduler_init.h>
#elif defined(JET_TASKING_OPENMP)
# include <omp.h>
#endif

static unsigned int sMaxNumberOfThreads = std::thread::hardware_concurrency();

void setMaxNumberOfThreads(unsigned int numThreads) {
#if defined(JET_TASKING_TBB)
    static std::unique_ptr<tbb::task_scheduler_init> tbbInit;
    if (!tbbInit.get())
      tbbInit.reset(new tbb::task_scheduler_init(numThreads));
    else {
      tbbInit->terminate();
      tbbInit->initialize(numThreads);
    }
#elif defined(JET_TASKING_OPENMP)
    omp_set_num_threads(numThreads);
#endif
    sMaxNumberOfThreads = std::max(numThreads, 1u);
}

unsigned int maxNumberOfThreads() { return sMaxNumberOfThreads; }
