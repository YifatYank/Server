//
// Created by yifat on 28/01/17.
//

#include "mainFlow.h"

mainFlow::mainFlow(int height, int width) {
    int index, status;

    this->grid = new Grid(height, width);
    this->center = new TaxiCenter(this->grid);
    this->time = 0;
    this->driversThatHasATrip = new list<Driver *>();
    this->tripsToAssignDriver = new list<int>();
    this->tcp = Tcp::getTcp(true, 0);
    this->deadPool = new list<pthread_t>();
    this->tripsToCalcPath = new list<Trip *>();

    struct calcPathParams *  params = (struct calcPathParams *)malloc(sizeof(struct calcPathParams)) ;
    params->map = this->grid->copyGrid();
    params->pathQueue = this->tripsToCalcPath;

    for(index = 0;index < 5; ++ index) {
        pthread_t t;
        status = pthread_create(&t, NULL, calcPathTask::threadFunction, (void *)params);//<--- NULL??
    }
}

mainFlow::~mainFlow() {
    delete(this->center);
    delete(this->grid);

    // Todo : delete lists
}

void mainFlow::addTaxiToCenter(int id, Manufacturer mf, Color color, int type) {
    center->addTaxi(id, mf, color, type);
}

Point * mainFlow::askDriverLocation(int id) {
    string sendMessage;
    char askLocation[] = "location";
    Point *location;
    char *recivedMassage = new char[4096];
    string *tempStr;

    // Send the driver massage and ask for its location.
    sendMessage = askLocation;
    this->tcp->sendDataByMapping(sendMessage, id);

    // Revice its location
    tcp->reciveDataByMapping(recivedMassage, 4096, id);
    location = HelpFunctions::deserializePoint(recivedMassage);
    return location;
}

void mainFlow::tellDriversToGo() {
    char go[] = "go";
    Driver *tempDriver;
    string sendMessage;
    char *recivedMassage = new char[4096];
    list<pDriver> * tempDriversList = new list<Driver *>();

    sendMessage = go;
    // While there are driver that need to drive.
    while(!this->driversThatHasATrip->empty()) {
        tempDriver = this->driversThatHasATrip->front();
        this->driversThatHasATrip->pop_front();

        // Tell the current driver to drive.
        this->tcp->sendDataByMapping(sendMessage, tempDriver->getID());

        // Gets if the driver has reched hiss destination.
        this->tcp->reciveDataByMapping(recivedMassage, 4096, tempDriver->getID());

        // checks if the driver reached its destination
        if (strcmp(recivedMassage, "yes") == 0) {
            // Updates the driver location in the taxicenter.
            tempDriver->driveToDestination();
        } else {
            tempDriversList->push_front(tempDriver);
        }
    }

    while (!tempDriversList->empty()) {
        this->driversThatHasATrip->push_front(tempDriversList->front());
        tempDriversList->pop_front();
    }

    delete[](recivedMassage);
    delete(tempDriversList);
}

void mainFlow::assignTrips() {
    Driver *tempDriver;
    Trip *tempTrp;
    list <int> * tempTripIdList = new list<int>();
    string sendMessage;
    char *recivedMassage = new char[4096];

    // While there are trips to assin drivers to.
    while(this->tripsToAssignDriver->empty()) {
        tempTrp = center->findTrip(this->tripsToAssignDriver->front());
        this->tripsToAssignDriver->pop_front();

        // If the trip was found in the taxiCenter(id != -1).
        // And it is the time to find a driver to take the trip.
        if (tempTrp->getID() != -1 && tempTrp->getStartTime() <= this->time)  {
            // While the trip path was not calculated yet (By the threads that calculates the paths).
            while(tempTrp->getTrip_path() == NULL) {
                sleep(1);
            }

            // Find a driver to take the trip.
            tempDriver = this->center->AssignTripToDriver(tripsToAssignDriver->front());

            // If there was a driver to take the trip(The driver's id is -1).
            if (tempDriver->getID() != -1) {
                // Send the driver the trip details.
                sendMessage = HelpFunctions::serialize(tempTrp);
                this->tcp->sendDataByMapping(sendMessage, tempDriver->getID());
                // Add the driver two the list of drivers that has a trip.
                this->driversThatHasATrip->push_front(tempDriver);
            } else {
                // There was not a driver to take the trip.
                tempTripIdList->push_front(tempTrp->getID());
            }
        } else {
            // If it is was not the time to assigned driver to the trip.
            if (tempTrp->getStartTime() > time) {
                tempTripIdList->push_front(tempTrp->getID());
            }
        }
    }

    // Return all the trips the need to be taken by a driver, to the whating list.
    while (!tempTripIdList->empty()) {
        this->tripsToAssignDriver->push_front(tempTripIdList->front());
        tempTripIdList->pop_front();
    }

    delete[](recivedMassage);
    delete (tempTripIdList);
}

void mainFlow::updateTime() {
    ++(this->time);
    this->assignTrips();
    this->tellDriversToGo();
}

void mainFlow::sayByeToDrivers() {
    list <pDriver> *tempDriversList = new list<pDriver>();
    list <pDriver> *driversList = this->center->getDriver();
    Driver *tempDriver;
    char bye[] = "bye";
    string sendMessage;

    while (!driversList->empty()) {
        tempDriver = driversList->front();
        driversList->pop_front();
        tempDriversList->push_front(tempDriver);

        sendMessage = bye;
        this->tcp->sendDataByMapping(sendMessage, tempDriver->getID());
    }

    while (!tempDriversList->empty()) {
        driversList->push_front(tempDriversList->front());
        tempDriversList->pop_front();
    }

    delete (tempDriversList);
}

void mainFlow::setObstical(int x, int y) {
    this->grid->setObstical(x,y);
}

void mainFlow::addTrip(int id, int taarif, Point *start, Point *end, int numOfPassangers, int startTime) {
    Trip * trp;
    trp = this->center->answerCalls(id, taarif, *start, *end, numOfPassangers, startTime);
    if(trp != NULL) {
        this->tripsToCalcPath->push_front(trp);
    }
}

void mainFlow::addDriver(int id, Marital_Status ms, int age, int yearsOfExp, int vehicle_id) {
    this->center->addDriver(id, ms, age, yearsOfExp, vehicle_id);
}

void * mainFlow::connectToClient(void *params) {
    int vehicle_id;
    int socketD;
    Cab *tempCab;
    Driver *tempDriver;
    char *recivedMassage = new char[4096];
    char emptyMassage[] = "0";
    string sendMessage;
    mainFlow * flow;
    list<int> *ids;
    struct threadArgs *args = (struct threadArgs *) params;

    // Get the parameters from the params list.
    flow = args->flow;
    socketD = args->socketD;
    free(args);

    flow->tcp->reciveData(recivedMassage, 4096, socketD);
    tempDriver = HelpFunctions::desrializeDriver(recivedMassage);
    sendMessage = emptyMassage;
    flow->tcp->sendData(sendMessage, socketD);

    // Get the cab request
    flow->tcp->reciveData(recivedMassage, 4096, socketD);
    vehicle_id = HelpFunctions::stringToInt(recivedMassage);

    // Add the driver to the texi center drivers list
    flow->addDriver(tempDriver->getID(), tempDriver->getMlStatus(), tempDriver->getAge(),
                      tempDriver->getExp(), vehicle_id);

    // Send the driver the cab he reqected.
    tempCab = flow->center->findCab(vehicle_id);
    sendMessage = HelpFunctions::serialize(tempCab);
    flow->tcp->sendData(sendMessage, socketD);
    // White for the client to tell he recived the message
    flow->tcp->reciveData(recivedMassage, 4096, socketD);

    // Add the driver id to the socket mapping of th tcp.
    flow->tcp->addMapping(socketD, tempDriver->getID());
    ids->push_front(tempDriver->getID());
    delete (tempDriver);
}

void * mainFlow::getClients(void *params) {
    struct threadArgs *args = (struct threadArgs *) params;
    int numOfDrivers, index, socketD;
    int status;
    Tcp *tcp;
    mainFlow * flow;
    list<int> *ids;

    tcp = Tcp::getTcp(true, 0);
    flow = args->flow;
    numOfDrivers = args->socketD;
    free(args);

    // Get the number of drivers

    for (index = 0; index < numOfDrivers; ++index) {
        socketD = tcp->acceptOneClient();
        pthread_t t1;
        args = (struct threadArgs *) malloc(sizeof(struct threadArgs));
        args->socketD = socketD;
        args->flow = flow;

        status = pthread_create(&t1, NULL, connectToClient, args);//<--- NULL??
        //threadNo = pthread_create(thread,NULL,threadFunction,parametersToThread);
        if (status) {
            cout << "Error creating thread";
        }
    }
}

void mainFlow::addTaxi(int x, Manufacturer mf, Color c, int type) {
    this->center->addTaxi(x, mf, c, type);
}
