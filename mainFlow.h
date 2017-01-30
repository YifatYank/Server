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
    list<Trip *> *tripsToCalcPath;
    list<int> *tripsToAssignDriver;
    list <pthread_t> *deadPool;

    void assignTrips();

    void tellDriversToGo();

public:
    /**
   * mainFlow
   * constructor
   * @param height
     * @param width
     */
    mainFlow(int height, int width);

/**
   * mainFlow
   * destructor
  */
    ~mainFlow();

/**
   * addTaxiToCenter
   * adding a taxi to center
   * @param id
   * @param mf manufacturer
   * @param color
   * @param type
     * @return true\false
  */
    bool addTaxiToCenter(int id, Manufacturer mf, Color color, int type);

/**
   * askDriverLocation
   * returns the driver's location
   * @param id
     * @return point
  */
    Point *askDriverLocation(int id);

/**
   * updateTime
   * updating the time
  */
    void updateTime();

/**
   * sayByeToDrivers
   * deleting the drivers
  */
    void sayByeToDrivers();

/**
   * setObstical
   * setting an obstical
   * @param x
     * @param y
  */
    void setObstical(int x, int y);

/**
   * addTrip
   * adding a trip
   * @param id
     * @param taarif
     * @param start
     * @param end
     * @param startTime
     * @param numOfPassangers
  */
    bool addTrip(int id, int taarif, Point *start, Point *end, int numOfPassangers, int startTime);

/**
   * addDriver
   * adding a driver
   * @param id
     * @param ms marital status
     * @param age
     * @param yearsOfExp
     * @param vehicle_id
  */
    void addDriver(int id, Marital_Status ms, int age, int yearsOfExp, int vehicle_id);

/**
   * addTaxi
   * adding a taxi
   * @param x id
   * @param c color
     * @param mf manufacturer
     * @param type
  */
    bool addTaxi(int x, Manufacturer mf, Color c, int type);

/**
   * connectToClient
   * connecting to client
   * @param param
  */
    static void *connectToClient(void *params);

/**
   * getClients
   *
   * @param params
  */
    static void *getClients(void *params);
};

struct threadArgs {
    mainFlow *flow;
    int socketD;
};

#endif //SERVER_MAINFLOW_H
