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

bool addTripToCenter(mainFlow *flow, int height, int width);

bool addTaxiToCenter(mainFlow *flow);

bool getObsticals(mainFlow *flow);

list <pstring> *split(string *str, char ch);

inline bool StringisInt(const std::string &str);


int main(int argc, char *argv[]) {
    int task;
    int height = 0;
    int width = 0;
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

    int arr[2];
    // Getting the grid size and creates the grid.
    cin >> arr[0] >> arr[1];
    //cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (cin.fail() || arr[0] < 0 || arr[1] < 0) {
        cout << "-1" << endl;
        break;
    }
    height = arr[0];
    width = arr[1];

    // while (getObsticals(flow)) {
    //      flow = new mainFlow(height, width);
    // }
    cin >> task;
    while (continueProg) {
        switch (task) {
            // Gets a new driver.
            case 1: {
                string s;
                //checks if the input us of correct type - in this case: int
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, s);
                if (cin.fail() || !StringisInt(s) || (count(s.begin(), s.end(), ' ') > 0)) {
                    cout << "-1" << endl;
                    break;
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
                if (addTripToCenter(flow, height, width) == false) {
                    cout << "-1" << endl;
                    continueProg = true;
                }
                break;
            }
                // Gets a new cab
            case 3: {
                if (addTaxiToCenter(flow) == false) {
                    cout << "-1" << endl;
                    continueProg = true;
                }
                break;
            }
                // Gets  a driver location.
            case 4: {
                int id;
                cin >> id;
                if (cin.fail()) {//|| !StringisInt(s)) {
                    cout << "-1" << endl;
                    break;
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

bool addTaxiToCenter(mainFlow *flow) {
    int id, type;
    char dummy, c, man;
    Color color;
    Manufacturer mf;
    string s;
    char *params[4];
    int k = 0;
    char *split;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // cin >> id >> dummy >> type >> dummy >> man >> dummy >> c;
    getline(cin, s);
    if (count(s.begin(), s.end(), ',') != 3) {
        return false;
    }

    char *secondS = (char *) s.c_str();
    split = strtok(secondS, ",");
    while (k < 4 && split != NULL) {
        params[k] = split;
        ++k;
        split = strtok(NULL, ",");
    }

    if (!StringisInt(params[0]) || !StringisInt(params[1])) {
        return false;
    }
    id = stoi(params[0]);
    type = stoi(params[1]);
    if ((id < 0 || type < 0 || type > 1)) {
        cout << "-1" << endl;
        return false;
    }

    mf = TESLA;
    if (*params[2] == 'H') { mf = HONDA; }
    else if (*params[2] == 'T') { mf = TESLA; }
    else if (*params[2] == 'S') { mf = SUBARO; }
    else if (*params[2] == 'D') { mf = FIAT; }
    else { return false; }

    color = RED;
    if (*params[3] == 'R') { color = RED; }
    else if (*params[3] == 'B') { color = BLUE; }
    else if (*params[3] == 'G') { color = GREEN; }
    else if (*params[3] == 'P') { color = PINK; }
    else if (*params[3] == 'W') { color = WHITE; }
    else { return false; }

    flow->addTaxi(id, mf, color, type);
}

bool addTripToCenter(mainFlow *flow, int height, int width) {
    int id, startx, starty, endx, endy, numOfPassangers, taarif, startTime;
    char dummy;
    Point *start;
    Point *end;
    Driver *tempDriver;
    pthread_t thread;
    string s;
    char *params[8];
    int k = 0;
    char *split;
// cin >> id >> dummy >> startx >> dummy >> starty >> dummy >> endx >> dummy >>
// endy >> dummy>> numOfPassangers >> dummy >> taarif >> dummy >> startTime;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, s);
    if (count(s.begin(), s.end(), ',') != 7) {
        return false;
    }

    char *secondS = (char *) s.c_str();
    split = strtok(secondS, ",");
    while (k < 8 && split != NULL) {
        params[k] = split;
        if (!StringisInt(params[k]) || params[k] < 0) {
            return false;
        }
        ++k;
        split = strtok(NULL, ",");
    }
    // Gets the trip details.
    id = stoi(params[0]);
    startx = stoi(params[1]);
    starty = stoi(params[2]);
    endx = stoi(params[3]);
    endy = stoi(params[4]);
    numOfPassangers = stoi(params[5]);
    //taarif = (int) *params[6]-'0';
    taarif = stoi(params[6]);
    startTime = stoi(params[7]);

    if (startx > width || starty > height || endx > width || endy > height) {
        return false;
    }
    if ((startx == endx) && (starty == endy)) {
        return false;
    }

    start = new Point(startx, starty);
    end = new Point(endx, endy);
    flow->addTrip(id, taarif, start, end, numOfPassangers, startTime);

    delete (end);
    delete (start);
}

bool getObsticals(mainFlow *flow) {
    int index, obstacleNum;
    list <pstring> *seperatedListcoma, *seperatedList;
    string strInput, *tempStrx, *tempStry;
    cin >> obstacleNum;

    while (cin.fail() || (obstacleNum < 0)) {
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


list <pstring> *split(string *str, char ch) {
    string *newStr = new string();
    int index;
    char tempChar;
    list <pstring> *lst = new list<pstring>();
    for (index = 0; index < str->length(); ++index) {
        tempChar = str->at(index);
        if (tempChar != ch) {
            newStr->push_back(tempChar);
        } else {
            lst->push_front(newStr);
            newStr = new string();
        }
    }

    if (newStr->length() != 0) {
        lst->push_front(newStr);
    } else {
        delete (newStr);
    }
    return lst;
}

inline bool StringisInt(const std::string &str) {
    if (str.empty() || ((!isdigit(str[0])) && (str[0] != '-') && (str[0] != '+'))) return false;
    char *temp;
    strtol(str.c_str(), &temp, 10);
    return (*temp == 0);
}

