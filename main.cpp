//
// Created by adi on 13/12/16.
//

//
// Created by
#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "Driver.h"
#include "Definitions.h"
#include "TaxiCenter.h"
#include "Socket.h"
#include "Tcp.h"
#include "Grid.h"
#include <pthread.h>
#include <map>
#include "HelpFunctions.h"

using namespace std;

struct threadArgs {
    TaxiCenter * center;
    list<int>  * driversIds;
    int socketD;
};

void * connectToClient(void * params);
void addTaxiToCenter(TaxiCenter * center);
void addTripToCenter(TaxiCenter * center, list <pthread_t> *  calcTripThread, list<int> * tripsToAssignDriver);
void askDriverLocation();
void getObsticals(Grid * grid);
void * getClients(void * params);
void tellDriversToGo(int time, TaxiCenter * center, list<int> * tripsToAssignDriver,
                     list <pthread_t> *  calcTripThread, list <pDriver> * drivers);
void sayByeToDrivers(TaxiCenter * center);

int main(int argc, char* argv[]) {
    int task;
    int height, width;
    int time = 0;
    int port;
    TaxiCenter * center;
    Grid* grid;
    list<int>  * driversIds = new list<int>();
    list <pDriver> * drivers = new list<pDriver >();
    list<int> * tripsToAssignDriver = new list<int>();
    list <pthread_t> *  calcTripThread = new list<pthread_t>();
    list <void *> * parameterToThread;
    bool continueProg = true;
    Tcp * tcp;
    struct threadArgs * args;
    int numOfDrivers;

    if(argc != 2) {
        delete(tripsToAssignDriver);
        delete(drivers);
        return 0;
    }

    // Initiate the tcp
    port = HelpFunctions::stringToInt(argv[1]);
    tcp = Tcp::getTcp(true, port);
    tcp->initialize();

    // Getting the grid size and creates the grid.
    cin >> height >> width;
    grid = new Grid(width, height);
    center = new TaxiCenter(grid);
    getObsticals(grid);

    cin>>task;
    while (continueProg) {
        switch (task) {
            // Gets a new driver.
            case 1: {

                cin >> numOfDrivers;
                pthread_t t;
                args = (struct threadArgs *)malloc (sizeof(struct threadArgs));
                args->center = center;
                args->socketD = numOfDrivers;
                args->driversIds = driversIds;
                int status = pthread_create(&t,NULL,getClients,args);//(&t, NULL, getClients  ,parameterToThread);//<--- NULL??
                if (status) {
                    cout << "Error creating thread";
                }
              //  getClients((void *)center);
                break;
            }
                // Gets a new trip
            case 2: {
                addTripToCenter(center, calcTripThread, tripsToAssignDriver);
                break;
            }
                // Gets a new cab
            case 3: {
                addTaxiToCenter(center);
                break;
            }
                // Gets  a driver location.
            case 4: {
                askDriverLocation();
                break;
            }
            case 9: {
                ++time;
                tellDriversToGo(time,center, tripsToAssignDriver,calcTripThread,drivers);
                break;
            }
            case 7: {

                continueProg = false;
                // Tell the driver the program has ended
                sayByeToDrivers(center);
                break;
            }
        }
        if (continueProg) {
            cin >> task;
        }
    }

    while (!drivers->empty()) {
        drivers->pop_front();
    }
    delete (drivers);
    while(!calcTripThread->empty()) {
        calcTripThread->pop_front();
    }
    delete (calcTripThread);
    while (!tripsToAssignDriver->empty()) {
        tripsToAssignDriver->pop_front();
    }
    delete (tripsToAssignDriver);

    while(!driversIds->empty()) {
        driversIds->pop_front();
    }
    delete(driversIds);
    delete (center);
    delete (grid);
    delete(Tcp::getTcp(true,0));
    return 0;
}

void * connectToClient(void * params) {
    int vehicle_id;
    int socketD;
    Cab * tempCab;
    Driver * tempDriver;
    char * recivedMassage = new char[4096];
    char emptyMassage[] = "0";
    string sendMessage;
    //list <void * > * lst;
    Tcp * tcp = Tcp::getTcp(true, 0);
    TaxiCenter * center;
    list <int> * ids;
    struct threadArgs * args = (struct threadArgs *)params;

    // Get the parameters from the params list.
    ids = args->driversIds;
    center = args->center;
    socketD = args->socketD;
    free(args);

    tcp->reciveData(recivedMassage, 4096, socketD);
    tempDriver = HelpFunctions::desrializeDriver(recivedMassage);
    sendMessage = emptyMassage;
    tcp->sendData(sendMessage,socketD);

    // Get the cab request
    tcp->reciveData(recivedMassage, 4096, socketD);
    vehicle_id = HelpFunctions::stringToInt(recivedMassage);

    // Add the driver to the texi center drivers list
    center->addDriver(tempDriver->getID(), tempDriver->getMlStatus(), tempDriver->getAge(),
                      tempDriver->getExp(), vehicle_id);


    // Send the driver the cab he reqected.
    tempCab = center->findCab(vehicle_id);
    sendMessage = HelpFunctions::serialize(tempCab);
    tcp->sendData(sendMessage, socketD);
    // White for the client to tell he recived the message
    tcp->reciveData(recivedMassage, 4096, socketD);

    // Add the driver id to the socket mapping of th tcp.
    tcp->addMapping(socketD, tempDriver->getID());
    ids->push_front(tempDriver->getID());
    delete (tempDriver);
}
void * getClients(void * params){
    struct threadArgs * args = (struct threadArgs *)params;
    int numOfDrivers, index, socketD;
    int status;
    Tcp * tcp;
    TaxiCenter * center;
    list <int> * ids;


    tcp = Tcp::getTcp(true,0);
    ids = args->driversIds;
    center =  args->center;
    numOfDrivers = args->socketD;
    free (args);

    // Get the number of drivers


    for (index = 0; index < numOfDrivers; ++index) {
        socketD = tcp->acceptOneClient();
        pthread_t t1;
        args = (struct threadArgs *)malloc (sizeof(struct threadArgs));
        args->socketD = socketD;
        args->center = center;
        args->driversIds = ids;

        status = pthread_create(&t1, NULL, connectToClient, args);//<--- NULL??
        //threadNo = pthread_create(thread,NULL,threadFunction,parametersToThread);
        if (status) {
            cout << "Error creating thread";
        }
    }
}
void addTaxiToCenter(TaxiCenter * center){
    int id, type;
    char dummy, c, man;
    Color color;
    Manufacturer  mf;

    cin >> id >> dummy >> type >> dummy >> man >> dummy >> c;
    mf = TESLA;
    if (man == 'H') { mf = HONDA; }
    else if (man == 'T') { mf = TESLA; }
    else if (man == 'S') { mf = SUBARO; }
    else if (man == 'D') { mf = FIAT; }

    color = RED;
    if (c == 'R') { color = RED; }
    else if (c == 'B') { color = BLUE; }
    else if (c == 'G') { color = GREEN; }
    else if (c == 'P') { color = PINK; }
    else if (c == 'W') { color = WHITE; }

    center->addTaxi(id, mf, color, type);
}
void addTripToCenter(TaxiCenter * center, list <pthread_t> *  calcTripThread, list<int> * tripsToAssignDriver) {
    int id, startx, starty, endx, endy ,numOfPassangers, taarif, startTime;
    char dummy;
    Point * start;
    Point * end;
    Driver * tempDriver;
    pthread_t thread;

    // Gets the trip ditails.
    cin >> id >> dummy >> startx >> dummy >> starty >> dummy >> endx  >> dummy >> endy >> dummy
        >> numOfPassangers >> dummy >> taarif >> dummy >> startTime;
    start = new Point(startx, starty);
    end = new Point(endx, endy);

    // Find a driver th take the trip.
    tempDriver = center->answerCalls(id, taarif, *start, *end, numOfPassangers, startTime + 1,&thread);

    // If there was not a problem with the trip information.
    if (tempDriver != NULL) {
        // Puts the trip in the whaiting list. And the thrad in the thread list.
        tripsToAssignDriver->push_front(id);
        calcTripThread->push_front(thread);
    }

    delete (end);
    delete (start);
}
void askDriverLocation() {
    int id;
    string sendMessage;
    char askLocation[] = "location";
    Point * location;
    char * recivedMassage = new char[4096];
    string * tempStr;
    // The parameters does not metter, the singketone is lready initiated;
    Tcp * tcp = Tcp::getTcp(true,0);

    cin >> id;
    sendMessage = askLocation;
    tcp->sendDataByMapping(sendMessage,id);
    tcp->reciveDataByMapping(recivedMassage, 4096, id);
    location = HelpFunctions::deserializePoint(recivedMassage);
    tempStr = location->getString();
    cout << *tempStr << endl;
    delete (location);
    delete (tempStr);
}
void getObsticals(Grid * grid) {
    int index, obstacleNum;
    list <pstring> *seperatedListcoma, *seperatedList;
    string strInput, *tempStrx, *tempStry;
    cin >> obstacleNum;

    for(index = 0; index < obstacleNum; ++index) {
        cin >> strInput;
        seperatedListcoma = HelpFunctions::split(&strInput, ',');
        tempStry = seperatedListcoma->back();
        seperatedListcoma->pop_back();
        tempStrx = seperatedListcoma->back();
        seperatedListcoma->pop_back();
        int x = HelpFunctions::stringToInt(tempStrx[0]);
        int y = HelpFunctions::stringToInt(tempStry[0]);
        grid->setObstical(x,y);
        delete(seperatedListcoma);
        delete (tempStrx);
        delete (tempStry);
    }
}
void tellDriversToGo(int time, TaxiCenter * center, list<int> * tripsToAssignDriver,
                     list <pthread_t> *  calcTripThread, list <pDriver> * drivers){
    pthread_t thread;
    char go[] = "go";
    list <pthread_t> *  tempThreadList = new list<pthread_t>();
    list<int> * tempTripsIdList = new list<int>();
    list<pDriver> * tempDriversList = new list<pDriver>();
    Driver * tempDriver;
    Trip * tempTrp;
    Tcp * tcp = Tcp::getTcp(true, 0);
    string sendMessage;
    char * recivedMassage = new char[4096];

    // If there are trips wauting to be assigned to driver.
    while (!tripsToAssignDriver->empty()) {
        //Gets the trip.
        tempTrp = center->findTrip(tripsToAssignDriver->front());
        thread = calcTripThread->front();

        // If the trip was found(id != -1) and it is the time to find a driver to take the trip.
        if(tempTrp->getID() != -1 && tempTrp->getStartTime() <= time) {
            pthread_join(thread, NULL);
            tempDriver = center->AssignTripToDriver(tripsToAssignDriver->front());
            // If there was a driver to take th trip(The driver's id is -1).
            if (tempDriver->getID() != -1) {
                drivers->push_front(tempDriver);
                // Send the driver the massage.
                sendMessage = HelpFunctions::serialize(tempTrp);
                tcp->sendDataByMapping(sendMessage,tempDriver->getID());

                // Take out the trip from the list of trips needed to be taken by a driver.
                tripsToAssignDriver->pop_front();
                calcTripThread->pop_front();
            } else {
                // There is no driver to take the trip so the trip stays
                // in the wating list.
                tempTripsIdList->push_front(tripsToAssignDriver->front());
                tripsToAssignDriver->pop_front();
                tempThreadList->push_front(calcTripThread->front());
                calcTripThread->pop_front();
            }
        } else {
            // If it is was not the time to assigned driver to the trip.
            if(tempTrp->getStartTime() > time) {
                // Take the trip out of the list
                tripsToAssignDriver->pop_front();
                // Put the trip back in the list
                tempTripsIdList->push_front(tempTrp->getID());

                tempThreadList->push_front(calcTripThread->front());
                calcTripThread->pop_front();
            }
            else {
                tripsToAssignDriver->pop_front();
                calcTripThread->pop_front();
            }
        }
    }

    // Return all the trips the need to be taken by a driver, to the whating list.
    while(!tempTripsIdList->empty()) {
        tripsToAssignDriver->push_front(tempTripsIdList->front());
        tempTripsIdList->pop_front();
    }

    while(!tempThreadList->empty()) {
        calcTripThread->push_front(tempThreadList->front());
        tempThreadList->pop_front();
    }


    // Tell the driver to go.
    sendMessage = go;
    while(!drivers->empty()) {
        tempDriver = drivers->front();
        drivers->pop_front();
        tcp->sendDataByMapping(sendMessage,tempDriver->getID());
        tcp->reciveDataByMapping(recivedMassage, 4096, tempDriver->getID());

        // checks if the driver reached its destination
        if (strcmp(recivedMassage,"yes") == 0) {
            tempDriver->driveToDestination();
        } else {
            tempDriversList->push_front(tempDriver);
        }
    }


    while(!tempDriversList->empty()) {
        drivers->push_front(tempDriversList->front());
        tempDriversList->pop_front();
    }

    delete(tempThreadList);
    delete [](recivedMassage);
    delete(tempTripsIdList);
    delete(tempDriversList);
}
void sayByeToDrivers(TaxiCenter * center) {
    list<pDriver> * driversList = center->getDriver();
    list<pDriver> * tempDriversList = new list<pDriver>();
    Driver * tempDriver;
    Tcp * tcp = Tcp::getTcp(true, 0);
    char bye[] = "bye";
    string sendMessage;


    while(!driversList->empty()) {
        tempDriver = driversList->front();
        driversList->pop_front();
        tempDriversList->push_front(tempDriver);

        sendMessage = bye;
        tcp->sendDataByMapping(sendMessage,tempDriver->getID());
    }

    while(!tempDriversList->empty()) {
        driversList->push_front(tempDriversList->front());
        tempDriversList->pop_front();
    }

    delete(tempDriversList);
}