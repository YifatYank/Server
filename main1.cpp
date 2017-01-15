//
// Created by adi on 13/12/16.
//

//
// Created by adi on 13/12/16.
//

#include <iostream>
#include "Driver.h"
#include "Definitions.h"
#include "TaxiCenter.h"

using namespace std;


list<pstring> * split(string * str,char ch);
int stringToInt(string str);

int main(int argc, char* argv[]) {
    int task;// = 0;
    int id;
    int index;
    int age;
    int type;
    int vehicle_id;
    int experience;
    char status, man, c, dummy;
    int height, width, obstacleNum;
    int startx, endx, starty, endy, taarif, NOP;
    string strInput, *tempStrx, *tempStry;
    Marital_Status ms;
    Manufacturer mf;
    Color color;
    TaxiCenter * center;
    Grid* grid;
    Driver* d;
    Point* obPoint;
    list <pstring> *seperatedListcoma, *seperatedList;
    list <pDriver> * drivers = new list<pDriver >();
    bool continueProg = true;
    string * tempStr;


    // Getting the grid size and creates thee grid.
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

            case 1: {
                cin >> id >> dummy >> age >> dummy >> status >> dummy >> experience >> dummy
                    >> vehicle_id;

                if (status == 'M') { ms = married; }
                else if (status == 'W') { ms = widowed; }
                else if (status == 'S') { ms = single; }
                else if (status == 'D') { ms = divorced; }

                center->addDriver(id, ms, age, experience, vehicle_id);
            }
                break;
            case 2: {
                cin >> id >> dummy >> startx >> dummy >> starty >> dummy >> endx >> dummy >> endy >> dummy >> NOP >> dummy >> taarif;
                Point *start = new Point(startx, starty);
                Point *end = new Point(endx, endy);
                drivers->push_front(center->answerCalls(id, taarif, *start, *end, NOP));
                delete (end);
                delete(start);
                break;
            }
            case 3: {
                cin >> id >> dummy >> type >> dummy >> man >> dummy >> c;
                if (man == 'H') { mf = HONDA; }
                else if (man == 'T') { mf = TESLA; }
                else if (man == 'S') { mf = SUBARO; }
                else if (man == 'D') { mf = FIAT; }

                if (c == 'R') { color = RED; }
                else if (c == 'B') { color = BLUE; }
                else if (c == 'G') { color = GREEN; }
                else if (c == 'P') { color = PINK; }
                else if (c == 'W') { color = WHITE; }

                center->addTaxi(id, mf, color, type);
                break;
            }
            case 4: {
                cin >> id;
                obPoint = center->getsDriverLocation(id);
                tempStr = obPoint->getString();
                cout << *tempStr << endl;
                delete(obPoint);
                delete(tempStr);
                break;
            }
            case 6: {
                while (!drivers->empty()) {
                    d = drivers->back();
                    d->drive();
                    drivers->pop_back();
                }
                break;
            }
            case 7: {
                while(!drivers->empty()) {
                    drivers->pop_front();
                }
                delete(drivers);
                delete (center);
                delete (grid);
                continueProg = false;
                break;
            }
        }
        if(continueProg) {
            cin >> task;
        }
    }
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