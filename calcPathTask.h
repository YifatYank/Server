//
// Created by yifat on 28/01/17.
//

#ifndef SERVER_CALCPATHTASK_H
#define SERVER_CALCPATHTASK_H

#include <list>
#include "Grid.h"
#include <pthread.h>
#include "Trip.h"

using namespace std;

struct calcPathParams {
    Grid *map;
    list<Trip *> *pathQueue;
};

class calcPathTask {
private:
    Grid *grid;
    list<Trip *> *jobQueue;
    pthread_mutex_t jobQueueLock;

    /**
   * calcPathTask
   *constructor
   * @param map a grid
     * @param pathQueue a queue
  */
    calcPathTask(Grid *map, list<Trip *> *pathQueue);

/**
   * calcPathTask
   *destructor
  */
    ~calcPathTask();

/**
   * calcPaths
   *calculates the paths
  */
    void calcPaths();

public:
    /**
   * threadFunction
   * @param map args
  */
    static void *threadFunction(void *args);
};

#endif //SERVER_CALCPATHTASK_H
