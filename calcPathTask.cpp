//
// Created by yifat on 28/01/17.
//

#include "calcPathTask.h"

calcPathTask::calcPathTask(Grid *map, list<Trip *> *pathQueue) {
    this->grid = map;
    this->jobQueue = pathQueue;
    pthread_mutex_init(&this->jobQueueLock, 0);
}

void * calcPathTask::threadFunction(void *args) {
    struct calcPathParams * parametets = (struct calcPathParams *)args;
    calcPathTask * task = new calcPathTask(parametets->map,parametets->pathQueue);
    task->calcPaths();
    delete(task);
}

calcPathTask::~calcPathTask() {
    pthread_mutex_destroy(&this->jobQueueLock);
}

void calcPathTask::calcPaths() {
    Trip * trip;
    // While the jobQueue was destrord by the father thread.
    while(this->jobQueue != NULL) {
        // If there are paths to calculate
        if(this->jobQueue->size() != 0) {
            // Lock  -> prepare for take out a trip from the trips list.
            pthread_mutex_lock(&this->jobQueueLock);
            // Double check: If there are paths to calculate
            if(this->jobQueue->size()!= 0) {
                trip = this->jobQueue->front();
                this->jobQueue->pop_front();
            } else {
                trip = NULL;
            }
            pthread_mutex_lock(&this->jobQueueLock);

            // If there was a trip to take out of the job list.
            if(trip != NULL) {
                trip->setTrip_path(this->grid->getSortesrPath(trip->getSP(), trip->getEP()));
            }
        }
    }
}