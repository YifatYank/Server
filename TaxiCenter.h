//
// Created by yifat on 01/12/16.
//


#ifndef UNITTEST_TAXICENTER_H
#define UNITTEST_TAXICENTER_H

#include <list>
#include "Grid.h"
#include "Point.h"
#include "Cab.h"
#include "Driver.h"
#include "Definitions.h"
#include <pthread.h>

using namespace std;

class TaxiCenter {
private:
    Driver *dummyDriver;
    Cab *dummyCab;
    Trip *dummtTrip;
    list<Driver *> *drivers;
    list<Cab *> *cabs;
    list<Trip *> *trips;
    Grid *grid;
    pthread_mutex_t lockDrivers;

/**
 * deleteDriversList
 * delete all the drivers in the list
*/
    void deleteDriversList();

/**
 * deleteCabsList
 * delete all the cabs in the list
*/
    void deleteCabsList();

/**
 * deleteTripsList
 * delete all the trips in the list
*/
    void deleteTripsList();

    Driver *AssignTripToDriver(Trip *trip);

public:
/**
 * TaxiCenter
 * constructor
 * @param grid a grid
*/
    TaxiCenter(Grid *grid);

/**
 * TaxiCenter
 * destructor
*/
    virtual ~TaxiCenter();

/**
 * answerCalls
 * @param id id of a driver
 * @param taarif taarif
 * @param end ending point
 * @param start starting point
 * @param numOfPassangers number of passengers
*/
    Trip *answerCalls(int id, int taarif, Point start, Point end, int numOfPassangers, int startTime);

/**
 * addDriver
 * @param id id of a driver
 * @param ms marital status
 * @param age the age of the driver
 * @param yearsOfExp years of experience
*/
    void addDriver(int id, Marital_Status ms, int age, int yearsOfExp);

/**
 * addDriver
 * @param id id of a driver
 * @param ms marital status
 * @param age the age of the driver
 * @param yearsOfExp years of experience
 * @param cabId id of a cab
*/
    void addDriver(int id, Marital_Status ms, int age, int yearsOfExp, int cabId);

    /**
 * addTaxi
 * @param c color
 * @param mf manufacturer
 * @param b boolean
 * @param id id value
*/
    bool addTaxi(int x, Manufacturer mf, Color c, int type);

/**
 * getDriver
 * returns a list of drivers
 * @return list of drivers
*/
    list <pDriver> *getDriver();

/**
 * getCabs
 * returns a list of cabs
 * @return list of cabs
*/
    list<Cab *> *getCabs();

/**
 * getTrips
 * returns a list of trips
 * @return list of trips
*/
    list<Trip *> *getTrips();

    /**
 * getsDriverLocation
 * The function gets a driver id and returns its location.
 * The function returns a new instance of a point.
 * @param id The driver id.
 * @return The drivers location.
 */
    Point *getsDriverLocation(int id);

    /**
 * setDriversCab
 * set a driver to a cab
 * @param cabId id of a cab
 * @param driverId id of a driver
*/
    void SetDriversCab(int driverId, int cabId);

    /**
 * findDriver
 * @param id driver's id
 * @return Driver
*/
    Driver *findDriver(int id);

    /**
* findCab
* @param id cab's id
* @return Cab
*/
    Cab *findCab(int id);

    /**
* findTrip
* @param id trip's id
* @return Trip
*/
    Trip *findTrip(int id);

    Driver *AssignTripToDriver(int tripId);


    static void *threadFunction(void *parameters);
};


#endif //UNITTEST_TAXICENTER_H