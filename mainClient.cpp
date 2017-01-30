#include <iostream>
#include "Enums.h"
#include "Driver.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "Socket.h"
#include "Tcp.h"
#include "HelpFunctions.h"

using namespace boost::archive;
using namespace std;

inline bool StringisInt(const std::string &str);

int nmain(int argc, char *argv[]) {
    int id, experience, vehicleId, age;
    //int currTime;
    char status, dummy;
    bool endOfProg = false;
    bool reachedEnd;
    int port;
    Marital_Status ms;
    string serialized;
    char emptyMassage[] = "0";
    char *messageRecived = new char[2048];
    list <pTrip> *tripList = new list<pTrip>();
    Driver *driver;
    Cab *cab;
    Trip *trp;
    Point *tempPoint;
    Tcp *tcp;
    string s;
    char *params[5];
    int k = 0;
    char *split;

    if (argc != 3) {
        delete[](messageRecived);
        delete (tripList);
        return 0;
    }

    port = HelpFunctions::stringToInt(argv[2]);
    // Initieize the comunication.
    tcp = Tcp::getTcp(false, port);
    tcp->initialize();

    // Gets the driver and creates it.
    // cin >> id >> dummy >> age >> dummy >> status >> dummy >> experience >> dummy
    //    >> vehicleId;
    bool again = true;
    while (again == true) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, s);
        if (count(s.begin(), s.end(), ',') != 4) {
            again = true;
            break;
        }

        char *secondS = (char *) s.c_str();
        split = strtok(secondS, ",");
        while (k < 5 && split != NULL) {
            params[k] = split;
            ++k;
            split = strtok(NULL, ",");
        }

        for (int i = 0; i < 5; i++) {
            if (!StringisInt(params[i])) {
                again = true;
                break;
            }
            if (i == 2) {
                continue;
            }
        }

        // Gets the trip details.
        id = stoi(params[0]);
        age = stoi(params[1]);
        //status = stoi(params[2]);
        experience = stoi(params[3]);
        vehicleId = stoi(params[4]);

        ms = married;
        if (*params[2] == 'M') { ms = married; }
        else if (*params[2] == 'W') { ms = widowed; }
        else if (*params[2] == 'S') { ms = single; }
        else if (*params[2] == 'D') { ms = divorced; }
        else {
            again = true;
            break;
        }
        //again = false;
    }

    driver = new Driver(id, age, ms, experience);
    serialized = HelpFunctions::serialize(driver);

    // Send the driver to the server
    tcp->sendData(serialized, 0);

    // Waite for the sever tell that it got the massage.
    tcp->reciveData(messageRecived, 2048, 0);

    // Send the requested vehicleId to the server
    serialized = std::to_string(vehicleId);
    tcp->sendData(serialized, 0);

    // Recive the cab
    tcp->reciveData(messageRecived, 2048, 0);
    cab = HelpFunctions::desrializeCab(messageRecived);
    driver->setCab(cab);
    // Send the server that the client got the message
    tcp->sendData(emptyMassage, 0);

    // While this is not the end of the program.
    while (!endOfProg) {
        // Waite for the trip
        tcp->reciveData(messageRecived, 2048, 0);
        if (strcmp(messageRecived, "bye") == 0) {
            endOfProg = true;
        } else if (strcmp(messageRecived, "location") == 0) {
            tempPoint = driver->getPlace();
            serialized = HelpFunctions::serialize(tempPoint);
            tcp->sendData(serialized, 0);
            delete (tempPoint);
        } else if (strcmp(messageRecived, "go") == 0) {
            serialized = "no";
            // Send the server if the driver reached his destination.
            tcp->sendData(serialized, 0);
        } else {
            // Means that this is a trip.
            trp = HelpFunctions::deserializeTrip(messageRecived);
            driver->setTrip(trp);

            while (!driver->isAvalable() && (!endOfProg)) {
                // Waite for 'go'
                tcp->reciveData(messageRecived, 2048, 0);
                if (strcmp(messageRecived, "go") == 0) {
                    reachedEnd = driver->drive();

                    if (reachedEnd) {
                        serialized = "yes";
                    } else {
                        serialized = "no";
                    }

                    // Send the server if the driver reached his destination.
                    tcp->sendData(serialized, 0);
                } else if (strcmp(messageRecived, "location") == 0) {
                    tempPoint = driver->getPlace();
                    serialized = HelpFunctions::serialize(tempPoint);
                    tcp->sendData(serialized, 0);
                    delete (tempPoint);
                } else if (strcmp(messageRecived, "bye") == 0) {
                    endOfProg = true;
                    driver->setTrip(NULL);
                }
            }
            tripList->push_front(trp);
        }
    }

    while (!tripList->empty()) {
        trp = tripList->front();
        tripList->pop_front();
        delete (trp);
    }

    delete (tripList);
    delete (driver);
    delete (cab);
    delete (tcp);
    delete[](messageRecived);
    return 0;
}

inline bool StringisInt(const std::string &str) {
    if (str.empty() || ((!isdigit(str[0])) && (str[0] != '-') && (str[0] != '+'))) return false;
    char *temp;
    strtol(str.c_str(), &temp, 10);
    return (*temp == 0);
}