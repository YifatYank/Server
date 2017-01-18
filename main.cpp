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
using namespace std;

list <pstring> * split(string * str,char ch);
int stringToInt(string str);
//void send(string str);
//char * recive();
string serialize(Cab * cab);
string serialize(Trip * trp);
Driver * desrializeDriver(char * str);
Point * deserializePoint(char * str);

int main(int argc, char* argv[]) {
    int task;
    int numOfDrivers;
    int id;
    int index;
    int type;
    int vehicle_id;
    int height, width, obstacleNum;
    int startx, endx, starty, endy, taarif, NOP;
    int time = 0;
    int startTime;
    char status, man, c, dummy;
    int port;
    string strInput, *tempStrx, *tempStry;
    pthread_t thread;
    Marital_Status ms;
    Manufacturer mf;
    Color color;
    TaxiCenter * center;
    Grid* grid;
    Driver* tempDriver;
    Cab * tempCab;
    Point* obPoint;
    Trip * tempTrp;
    list <pstring> *seperatedListcoma, *seperatedList;
    list <pDriver> * drivers = new list<pDriver >();
    list<int> * tripsToAssignDriver = new list<int>();
    list<int> * tempList= new list<int>();
    list <pthread_t> *  calcTripThread = new list<pthread_t>();
    list <pthread_t> *  tempThreadList = new list<pthread_t>();
    bool continueProg = true;
    string sendMessage;
    char * recivedMassage = new char[4096];
    string * tempStr;
    char emptyMassage[] = "0";
    char askLocation[] = "location";
    char go[] = "go";
    char bye[] = "bye";
    Tcp * tcp;

    if(argc != 2) {
        delete[](recivedMassage);
        delete(tempList);
        delete(tripsToAssignDriver);
        delete(drivers);
        return 0;
    }

    port = stringToInt(argv[1]);

    // Initialized comunication;
//    tcp = new Tcp(true, port);
//    tcp->initialize();

    // Getting the grid size and creates the grid.
    cin >> height >> width;
    grid = new Grid(width, height);
    center = new TaxiCenter(grid);
    cin >> obstacleNum;

    for(index = 0; index < obstacleNum; ++index) {
        cin >> strInput;
        seperatedListcoma = split(&strInput, ',');
        tempStry = seperatedListcoma->back();
        seperatedListcoma->pop_back();
        tempStrx = seperatedListcoma->back();
        seperatedListcoma->pop_back();
        int x = stringToInt(tempStrx[0]);
        int y = stringToInt(tempStry[0]);
        grid->setObstical(x,y);
        delete(seperatedListcoma);
        delete (tempStrx);
        delete (tempStry);
    }
    cin>>task;
    while (continueProg) {
        switch (task) {
            // Gets a new driver.
            case 1: {
                // Get the number of drivers
                cin >> numOfDrivers;

                for (index = 0; index < numOfDrivers; ++index) {
                    int socketD;
//                    socketD = tcp->acceptOneClient();
         //               pthread_t t1, t2;
       //                 int status = pthread_create(&t1, NULL, connectToClient, (void *) socketD);
       //             if (status) {
//                        cout << "Error creating thread";
         //           }

/**static void connectToClient() {
    tcp->reciveData(recivedMassage, 4096, socketD);
    tempDriver = desrializeDriver(recivedMassage);
    sendMessage = emptyMassage;
    tcp->sendData(sendMessage);

    // Get the cab request
    tcp->reciveData(recivedMassage, 4096, socketD);
    vehicle_id = stringToInt(recivedMassage);

    // Add the driver to the texi center drivers list
    center->addDriver(tempDriver->getID(), tempDriver->getMlStatus(), tempDriver->getAge(),
                      tempDriver->getExp(), vehicle_id);
    delete (tempDriver);

    // Send the driver the cab he reqected.
    tempCab = center->findCab(vehicle_id);
    sendMessage = serialize(tempCab);
    tcp->sendData(sendMessage);
    // White for the client to tell he recived the message
    tcp->reciveData(recivedMassage, 4096, socketD);

}*/
                }

                break;
            }
                // Gets a new trip
            case 2: {
                cin >> id >> startx >> starty >> endx  >> endy >> NOP >> taarif >> startTime;
                Point *start = new Point(startx, starty);
                Point *end = new Point(endx, endy);

                // Find a driver th take the trip.
                tempDriver = center->answerCalls(id, taarif, *start, *end, NOP, startTime + 1,&thread);

                // If there was not a problem with the trip information.
                if (tempDriver != NULL) {
                    // Puts the trip in the whaiting list. And the thrad in the thread list.
                    tripsToAssignDriver->push_front(id);
                    calcTripThread->push_front(thread);
                }

                delete (end);
                delete (start);
                break;
            }
                // Gets a new cab
            case 3: {
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
                break;
            }
                // Gets  a driver location.
            case 4: {
                cin >> id;
                sendMessage = askLocation;
//                tcp->sendData(sendMessage);
                tcp->reciveData(recivedMassage, 4096, 0);
                obPoint = deserializePoint(recivedMassage);
                tempStr = obPoint->getString();
                cout << *tempStr << endl;
                delete (obPoint);
                delete (tempStr);
                break;
            }
            case 9: {
                ++time;
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
                            tempTrp = center->findTrip(id);
                            // Send the driver the massage.
                            sendMessage = serialize(tempTrp);
//                                tcp->sendData(sendMessage);

                            // Take out the trip from the list of trips needed to be taken by a driver.
                            tripsToAssignDriver->pop_front();
                            calcTripThread->pop_front();
                        } else {
                            // There is no driver to take the trip so the trip stays
                            // in the wating list.
                            tempList->push_front(tripsToAssignDriver->front());
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
                            tempList->push_front(tempTrp->getID());

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
                while(!tempList->empty()) {
                    tripsToAssignDriver->push_front(tempList->front());
                    tempList->pop_front();
                }

                while(!tempThreadList->empty()) {
                    calcTripThread->push_front(tempThreadList->front());
                    tempThreadList->pop_front();
                }

                // Tell the driver to go.
                sendMessage = go;
//                tcp->sendData(sendMessage);

                // checks if the driver reached its destination
                tcp->reciveData(recivedMassage, 4096, 0);
                // If the driver reached his destination
                if (strcmp(recivedMassage,"yes") == 0) {
                    // There is only one driver.
                    tempDriver = drivers->front();
                    tempDriver->drive();
                    drivers->pop_front();
                }
                break;
            }
            case 7: {
                while (!drivers->empty()) {
                    drivers->pop_front();
                }
                delete (drivers);

                while(!calcTripThread->empty()) {
                    calcTripThread->pop_front();
                }

                delete (calcTripThread);
                delete(tempThreadList);

                while (!tripsToAssignDriver->empty()) {
                    tripsToAssignDriver->pop_front();
                }
                delete (tripsToAssignDriver);
                delete (tempList);
                delete (center);
                delete (grid);
                continueProg = false;
                // Tell the driver the program has ended
                sendMessage = bye;
//                tcp->sendData(sendMessage);
  //              delete (tcp);
                break;
            }
        }
        if (continueProg) {
            cin >> task;
        }
    }

    delete[](recivedMassage);
    return 0;
}

list<pstring> * split(string * str,char ch) {
    string * newStr = new string();
    int index;
    char tempChar;
    list<pstring> *  lst = new list<pstring>();
    for(index = 0; index < str->length(); ++index) {
        tempChar = str->at(index);
        if(tempChar != ch) {
            newStr->push_back(tempChar);
        } else {
            lst->push_front(newStr);
            newStr = new string();
        }
    }

    if(newStr->length() != 0) {
        lst->push_front(newStr);
    } else {
        delete(newStr);
    }
    return lst;
}
int stringToInt(string str){
    char ch;
    int index, num = 0, digit;

    for(index = 0; index < str.size(); ++index) {
        ch = str.at(index);
        digit = ch - 48;
        num = num * 10 + digit;
    }
    return num;
}
void send(string str) {
    const char* ip_address = "127.0.0.1";
    const int port_no = 5555;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("error creating socket");
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip_address);
    sin.sin_port = htons(port_no);

    char tempData[4096];
    strcpy(tempData,str.c_str());
    const char * data = tempData;
    int size_data = str.size();

    int sent_bytes = sendto(sock, data, size_data, 0, (struct sockaddr *) &sin, sizeof(sin));
    if (sent_bytes < 0) {
        perror("error writing to socket");
    }
    close(sock);
}
char * recive() {
    const int server_port = 5555;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("error creating socket");
    }

    struct sockaddr_in sin;

    memset(&sin, 0, sizeof(sin));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(server_port);

    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("error binding to socket");
    }

    struct sockaddr_in from;
    unsigned int from_len = sizeof(struct sockaddr_in);


    char * buffer = new char[4096];
    int bytes = recvfrom(sock, buffer, 4096 , 0, (struct sockaddr *) &from, &from_len);
    if (bytes < 0) {
        perror("error reading from socket");
    }

    close(sock);
    return buffer;
}

string serialize(Cab * cab) {
    std::string serial_str;
    boost::iostreams::back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive oa(s);

    oa << cab;
    s.flush();

    return serial_str;
}
string serialize(Trip * trp){
    std::string serial_str;
    boost::iostreams::back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive oa(s);

    oa << trp;
    s.flush();

    return serial_str;
}
/**void ConnectToClient(){
    // Get the driver
    tcp->reciveData(recivedMassage, 4096);
    tempDriver = desrializeDriver(recivedMassage);
    sendMessage = emptyMassage;
    tcp->sendData(sendMessage);

    // Get the cab request
    tcp->reciveData(recivedMassage, 4096);
    vehicle_id = stringToInt(recivedMassage);

    // Add the driver to the texi center drivers list
    center->addDriver(tempDriver->getID(), tempDriver->getMlStatus(), tempDriver->getAge(),
                      tempDriver->getExp(), vehicle_id);
    delete (tempDriver);

    // Send the driver the cab he reqected.
    tempCab = center->findCab(vehicle_id);
    sendMessage = serialize(tempCab);
    tcp->sendData(sendMessage);
    // White for the client to tell he recived the message
    tcp->reciveData(recivedMassage,4096);
}*/

Driver * desrializeDriver(char * str) {
    Driver * driver;
    int index;
    //int lenth = str.length();
   //for(index = 0;index < lenth - 1; ++ index) {
   //     if(str.at(index) == '|') {
   //         str.replace(index,1,"\0");
   //     }
   // }

    boost::iostreams::basic_array_source<char> device(str, 4096);
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
    boost::archive::binary_iarchive ia(s2);
    ia >> driver;

    return driver;
}
Point * deserializePoint(char * str){
    Point * point;
    boost::iostreams::basic_array_source<char> device(str, 4096);
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
    boost::archive::binary_iarchive ia(s2);
    ia >> point;

    return point;
}
