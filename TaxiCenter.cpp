//
// Created by yifat on 01/12/16.
//
//
// Created by adi on 29/11/16.
//
#include <queue>
#include "TaxiCenter.h"
#include "Standart.h"
#include "Luxury.h"
TaxiCenter::TaxiCenter(Grid *grid) {
    this->grid = grid;
    this->drivers = new list<pDriver>;
    this->trips = new list<pTrip>;
    this->cabs = new list<pCab>;
    this->dummyCab = new Cab(-1, HONDA, RED);
    this->dummtTrip = new Trip (-1,-1, Point(-1,-1) , Point(-1,-1),-1, -1);
    this->dummyDriver = new Driver (-1, -1, single,-1);
}
TaxiCenter::~TaxiCenter(){
    this->deleteCabsList();
    this->deleteDriversList();
    this->deleteTripsList();
    delete(this->cabs);
    delete(this->drivers);
    delete(this->trips);
    delete(this->dummyCab);
    delete(this->dummyDriver);
    delete(this->dummtTrip);
};


Driver * TaxiCenter::answerCalls(int id, int taarif, Point start, Point end, int numOfPassangers, int startTime){
    Trip * trip;


    // Checkd if there is already trip with the same id in the taxi center.
    trip = this->findTrip(id);
    if(trip->getID() == -1) {
        trip = new Trip(id, taarif, start, end, numOfPassangers, startTime);
        trip->setTrip_path(this->grid->getSortesrPath(&start, &end));
        this->trips->push_front(trip);
        return this->dummyDriver;//return this->AssignTripToDriver(trip);

    }
    return NULL;
}

void TaxiCenter::addDriver(int id, Marital_Status ms, int age, int yearsOfExp) {
    Driver * driver = this->findDriver(id);
    // If he driver is not exist in the center
    if (driver->getID() == - 1) {
        driver = new Driver(id, age, ms, yearsOfExp);
        this->drivers->push_front(driver);
    }
}

void TaxiCenter::addTaxi(int id, Manufacturer mf, Color c, int type){
    Cab * cab;

    cab = findCab(id);
    if(cab->getId() == -1) {
        if (type == 1) {
            cab = new Standart(id, mf, c);
        } else if (type == 2) {
            cab = new Luxury(id, mf, c);
        }

        this->cabs->push_front(cab);
    }
}

list <pDriver> *TaxiCenter::getDriver() {
    return this->drivers;
}

list<Cab *> *TaxiCenter::getCabs() {
    return this->cabs;
}

list<Trip *> *TaxiCenter::getTrips() {
    return this->trips;
}

Driver *TaxiCenter::findDriver(int id) {
    list <pDriver> * templst = new list<pDriver>();
    Driver * driver;
    Driver * found  = this->dummyDriver;

    while(!this->drivers->empty()) {
        driver = this->drivers->front();
        this->drivers->pop_front();
        templst->push_front(driver);
    }

    while(!templst->empty()) {
        driver = templst->front();
        templst->pop_front();
        if(driver->getID() == id) {
            found = driver;
        }

        this->drivers->push_front(driver);
    }

    delete(templst);
    return found;
}

Cab *TaxiCenter::findCab(int id) {
    list <pCab> * templst = new list<pCab>();
    Cab * cab;
    Cab * found = this->dummyCab;

    while(!this->cabs->empty()) {
        cab = this->cabs->front();
        this->cabs->pop_front();
        templst->push_front(cab);
    }

    while(!templst->empty()) {
        cab = templst->front();
        templst->pop_front();
        if(cab->getId() == id) {
            found = cab;
        }

        this->cabs->push_front(cab);
    }

    delete(templst);
    return found;
}

Trip * TaxiCenter::findTrip(int id) {
    list <pTrip> * templst = new list<pTrip>();
    Trip * trip;
    Trip * found = this->dummtTrip;

    while(!this->trips->empty()) {
        trip = this->trips->front();
        this->trips->pop_front();
        templst->push_front(trip);
    }

    while(!templst->empty()) {
        trip = templst->front();
        templst->pop_front();
        if(trip->getID() == id) {
            found = trip;
        }

        this->trips->push_front(trip);
    }

    delete(templst);
    return found;

}

Point *TaxiCenter::getsDriverLocation(int id) {
    Driver * driver = this->findDriver(id);
    Point * location = driver->getPlace();
    return location;
}

void TaxiCenter::addDriver(int id, Marital_Status ms, int age, int yearsOfExp, int cabId) {
    Driver *driver;
    Cab * cab;

    driver = this->findDriver(id);
    // If he driver is not exist in the center
    if (driver->getID() == -1) {
        driver = new Driver(id, age, ms, yearsOfExp);
        this->drivers->push_front(driver);

        cab = this->findCab(id);
        if(cab->getId() != -1) {
            driver->setCab(cab);
        }
    }
}

void TaxiCenter::SetDriversCab(int driverId, int cabId) {
    Driver * driver;
    Cab * cab;
    driver = this->findDriver(driverId);
    cab = this->findCab(cabId);
    if(driver != NULL && cab != NULL) {
        driver->setCab(cab);
    }
}

void TaxiCenter::deleteDriversList() {
    Driver * driver;
    while(!this->drivers->empty()) {
        driver = this->drivers->front();
        this->drivers->pop_front();
        delete(driver);
    }
}

void TaxiCenter::deleteCabsList() {
    Cab * cab;
    while(!this->cabs->empty()) {
        cab = this->cabs->front();
        this->cabs->pop_front();
        delete(cab);
    }
}

void TaxiCenter::deleteTripsList() {
    Trip * trip;
    while(!this->trips->empty()) {
        trip = this->trips->front();
        this->trips->pop_front();
        delete(trip);
    }
}

Driver *TaxiCenter::AssignTripToDriver(int tripId) {
    Trip * trip = this->findTrip(tripId);
    if(trip->getID() != -1) {
        return this->AssignTripToDriver(trip);
    }
    return NULL;
}

Driver *TaxiCenter::AssignTripToDriver(Trip * trip) {
    list <pDriver > * tempList = new list<pDriver> ();
    Driver * chosenDriver = this->dummyDriver;
    Driver * driver;
    Point * tempPoint;
    bool isAssined = false;

    // Sreach for a driver to take the passanger.

    // Put all the drivers in a temporary place.
    while (!this->drivers->empty()) {
        driver = this->drivers->front();
        this->drivers->pop_front();
        tempList->push_front(driver);
    }

    // Returns the drivers to the permanent place and find a driver to take the trip.
    while (!tempList->empty() && !isAssined) {
        driver = tempList->front();
        tempList->pop_front();
        // If the driver is in avaliable and he is in the right place.
        tempPoint = driver->getPlace();
        if (*tempPoint == *trip->getSP() && driver->isAvalable()) {
            chosenDriver = driver;
            chosenDriver->setTrip(trip);
            isAssined = true;
        }
        delete(tempPoint);
        this->drivers->push_front(driver);
    }

    while (!tempList->empty()) {
        driver = tempList->front();
        tempList->pop_front();
        this->drivers->push_front(driver);
    }
    delete(tempList);

    return chosenDriver;

}
