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

struct calcPathParams{
    Grid * map;
    list <Trip * > * pathQueue;
};

class calcPathTask {
private:
    Grid * grid;
    list <Trip * > * jobQueue;
    pthread_mutex_t jobQueueLock;
    calcPathTask(Grid * map, list <Trip * > * pathQueue);
    ~calcPathTask();
    void calcPaths();

public:
    static void * threadFunction(void * args);
};

#endif //SERVER_CALCPATHTASK_H
