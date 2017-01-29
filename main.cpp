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
#include "calcPathTask.h"
#include "mainFlow.h"

using namespace std;


bool isNumber(string str);

int getNumber(string str);

bool numberOfInputs(int numInputExpected);

void addTripToCenter(mainFlow *flow, int height, int width);

void addTaxiToCenter(mainFlow *flow);

void getObsticals(mainFlow *flow);

int main(int argc, char *argv[]) {
    int task;
    int height, width;
    int time = 0;
    int port;
    list<void *> *parameterToThread;
    bool continueProg = true;
    Tcp *tcp;
    struct threadArgs *args;
    int numOfDrivers;
    mainFlow *flow;

    if (argc != 2) {
        return 0;
    }

    // Initiate the tcp
    port = HelpFunctions::stringToInt(argv[1]);
    tcp = Tcp::getTcp(true, port);
    tcp->initialize();
    string tempstr;
    // Getting the grid size and creates the grid.
    //cin >> height >> width;
    char *hw;
    getline(cin, tempstr);
    while (std::count(tempstr.begin(), tempstr.end(), ' ') != 1) {
        //while ((numberOfInputs(2) == false) || (height < 0 || width < 0)) {
        cout << "-1" << endl;
       break;// getline(cin, tempstr);
    }

    //hw = strtok(tempstr," ");
    //height = hw[0];
    //width  = hw[1];
    flow = new mainFlow(height, width);
    getObsticals(flow);

    cin >> task;

    while (continueProg) {
        switch (task) {
            // Gets a new driver.
            case 1: {
                //checks if the input us of correct type - in this case: int
                while (!(cin >> numOfDrivers) || (numberOfInputs(1) == false) || (numOfDrivers < 0)) {
                    cout << "-1" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                pthread_t t;
                args = (struct threadArgs *) malloc(sizeof(struct threadArgs));
                args->flow = flow;
                args->socketD = numOfDrivers;
                int status = pthread_create(&t, NULL, mainFlow::getClients, args);
                if (status) {
                    cout << "Error creating thread";
                }
                break;
            }
                // Gets a new trip
            case 2: {
                addTripToCenter(flow, height, width);
                break;
            }
                // Gets a new cab
            case 3: {
                addTaxiToCenter(flow);
                break;
            }
                // Gets  a driver location.
            case 4: {
                int id;
                cin >> id;
                while ((numberOfInputs(1) == false) || (id < 0)) {
                    cout << "-1" << endl;
                    cin >> id;
                }

                flow->askDriverLocation(id);
                break;
            }
            case 9: {
                flow->updateTime();
                break;
            }
            case 7: {

                continueProg = false;
                // Tell the driver the program has ended
                flow->sayByeToDrivers();
                break;
            }
            default: {
                cout << "-1" << endl;
                continueProg = true;
                break;
            }
        }
        if (continueProg) {
            cin >> task;
        }
    }

    delete (Tcp::getTcp(true, 0));

    //pthread_join();
    return 0;
}

void addTaxiToCenter(mainFlow *flow) {
    int id, type;
    char dummy, c, man;
    Color color;
    Manufacturer mf;

    cin >> id >> dummy >> type >> dummy >> man >> dummy >> c;

    while ((numberOfInputs(4) == false) || (id < 0 || type < 0 || type > 1)) {
        cout << "-1" << endl;
        cin >> id >> dummy >> type >> dummy >> man >> dummy >> c;
    }

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

    flow->addTaxi(id, mf, color, type);
}

void addTripToCenter(mainFlow *flow, int height, int width) {
    int id, startx, starty, endx, endy, numOfPassangers, taarif, startTime;
    char dummy;
    Point *start;
    Point *end;
    Driver *tempDriver;
    pthread_t thread;

    // Gets the trip details.
    cin >> id >> dummy >> startx >> dummy >> starty >> dummy >> endx >> dummy >> endy >> dummy
        >> numOfPassangers >> dummy >> taarif >> dummy >> startTime;

    while ((numberOfInputs(8) == false) || (startx > width || starty > height) ||
           (id < 0 || startx < 0 || starty < 0 || endx < 0 || endy < 0 || numOfPassangers < 0 || taarif < 0 ||
            startTime < 0)) {
        cout << "-1" << endl;
        cin >> id >> dummy >> startx >> dummy >> starty >> dummy >> endx >> dummy >> endy >> dummy
            >> numOfPassangers >> dummy >> taarif >> dummy >> startTime;
    }

    start = new Point(startx, starty);
    end = new Point(endx, endy);
    flow->addTrip(id, taarif, start, end, numOfPassangers, startTime);

    delete (end);
    delete (start);
}

void getObsticals(mainFlow *flow) {
    int index, obstacleNum;
    list <pstring> *seperatedListcoma, *seperatedList;
    string strInput, *tempStrx, *tempStry;
    cin >> obstacleNum;

    while ((numberOfInputs(1) == false) || (obstacleNum < 0)) {
        cout << "-1" << endl;
        cin >> obstacleNum;
    }

    for (index = 0; index < obstacleNum; ++index) {
        cin >> strInput;
        seperatedListcoma = HelpFunctions::split(&strInput, ',');
        tempStry = seperatedListcoma->back();
        seperatedListcoma->pop_back();
        tempStrx = seperatedListcoma->back();
        seperatedListcoma->pop_back();
        int x = HelpFunctions::stringToInt(tempStrx[0]);
        int y = HelpFunctions::stringToInt(tempStry[0]);
        flow->setObstical(x, y);
        delete (seperatedListcoma);
        delete (tempStrx);
        delete (tempStry);
    }
}


bool isNumber(string s) {
    char *p;
    strtol(s.c_str(), &p, 10);
    return *p == 0;
}

int getNumber(string s) {
    char *p;
    return strtol(s.c_str(), &p, 10);
}

bool numberOfInputs(int numInputExpected) {
    string strInput;
    int index = 0;
    do {
        //index = 0;
        string s;
        //gets a line from stream into string
        getline(cin, strInput, '\n');
        stringstream ss(strInput);
        while (getline(ss, s, ' ')) {
            if (isNumber(s)) {
                index++;
            }
        }
        if (index != numInputExpected) {
            return false;
        }
    } while (index != numInputExpected);
    //return strInput;
    return true;
}