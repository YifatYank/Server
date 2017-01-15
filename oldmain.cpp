//
// Created by yifat on 27/12/16.
//
/**
#include <iostream>
#include <string>
#include "Point.h"
#include <list>
#include "Grid.h"
#include "Definitions.h"
using namespace std;


list<string * > * split(string * str,char ch);
int stringToInt(string str);

int main() {
    int heigth, width, x, y;
    Point * start, * end, *tempPoint;
    list <pstring> * seperatedListcoma, * seperatedList;
    string strInput, * tempStr, * stringToPrint;// * tempString;
    Grid * grid;
    list <pPoint>* path;


    // Get the parameters
    cin >> strInput;
    seperatedListcoma = split(&strInput, ',');

    // Get the string represents the width and the hieght
    tempStr = seperatedListcoma->back();
    seperatedListcoma->pop_back();
    seperatedList = split(tempStr,'_');
    delete(tempStr);

    // Get the height and the width.
    tempStr = seperatedList->back();
    heigth = stringToInt(*tempStr);
    seperatedList->pop_back();
    delete(tempStr);

    tempStr = seperatedList->back();
    width = stringToInt(*tempStr);
    seperatedList->pop_back();
    delete(tempStr);
    delete(seperatedList);

    // Get the string represents the startPoint
    tempStr = seperatedListcoma->back();
    seperatedListcoma->pop_back();
    seperatedList = split(tempStr,'_');
    delete(tempStr);

    // Get the startPoint y coord
    tempStr = seperatedList->back();
    x = stringToInt(*tempStr);
    delete(tempStr);
    seperatedList->pop_back();

    // Get the startPoint x coord
    tempStr = seperatedList->back();
    y = stringToInt(*tempStr);
    seperatedList->pop_back();
    delete(tempStr);

    delete(seperatedList);
    start = new Point(x,y);

    // Get the string represents the endPoint
    tempStr = seperatedListcoma->back();
    seperatedListcoma->pop_back();
    seperatedList = split(tempStr,'_');
    delete(tempStr);

    // Get the endPoint
    tempStr = seperatedList->back();
    x = stringToInt(*tempStr);
    seperatedList->pop_back();
    delete(tempStr);

    tempStr = seperatedList->back();
    y = stringToInt(*tempStr);
    seperatedList->pop_back();
    delete(tempStr);

    delete(seperatedList);
    end = new Point(x,y);

    delete(seperatedListcoma);

    // Creates the grid
    grid = new Grid(heigth,width);

    // Get the path.
    path = grid->getSortesrPath(start, end);

    // Prints the path.
    while (!path->empty()) {
        tempPoint = path->front();
        path->pop_front();
        stringToPrint = tempPoint->getString();
        cout << *stringToPrint << endl;
        delete (stringToPrint);
        delete (tempPoint);
    }
    delete(path);

    delete(start);
    delete(end);
    delete(grid);

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
}*/