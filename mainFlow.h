//
// Created by yifat on 28/01/17.
//

#ifndef SERVER_MAINFLOW_H
#define SERVER_MAINFLOW_H

#include "TaxiCenter.h"
#include "Grid.h"
#include "Tcp.h"
#include "Enums.h"
#include "HelpFunctions.h"
#include "calcPathTask.h"
#include <pthread.h>

using namespace std;

class mainFlow {
private:
    int time;
    TaxiCenter *center;
    Grid *grid;
    Tcp *tcp;
    list<Driver *> *driversThatHasATrip;
    list<Trip *> * tripsToCalcPath;
    list<int> *tripsToAssignDriver;
    list<pthread_t> * deadPool;

    void assignTrips();
    void tellDriversToGo();

public:
    mainFlow(int height, int width);
    ~mainFlow();
    void addTaxiToCenter(int id, Manufacturer mf, Color color, int type);
    Point * askDriverLocation(int id);
    void updateTime();
    void sayByeToDrivers();
    void setObstical (int x, int y);
    void addTrip(int id, int  taarif, Point *start,Point *end, int numOfPassangers, int startTime);
    void addDriver(int id, Marital_Status ms, int age, int yearsOfExp, int vehicle_id);
    void addTaxi(int x, Manufacturer mf, Color c, int type);
    static void *connectToClient(void *params);
    static void *getClients(void *params);
};

struct threadArgs {
    mainFlow * flow;
    int socketD;
};

#endif //SERVER_MAINFLOW_H
