//
// Created by yifat on 01/12/16.
//

#include "BfsGrid.h"
//
// Created by yifat on 23/11/16.
//

#include "BfsGrid.h"
#include "Grid.h"
#include "Point.h"
#include <stdio.h>
#include <iostream>

list<pPoint> * BfsGrid::getShortestPath(Grid * grid, Point * start, Point * end) {
    queue<pPointNode > * que;
    PointNode * currNode, * newNode;
    Point * tempPoint;
    list<pPoint> * lstNeibors;
    list<pPoint> * chekedData = new list<pPoint>();
    list<pPointNode> * checkedNodes = new list<pPointNode>();
    list<pPoint> * tempList;

    que = new queue<pPointNode>();
    currNode = new PointNode(*start);
    que->push(currNode);
    // While the queue is not empty
    while(!que->empty()) {
        currNode = que->front();
        // Set the node as marked.
        grid->setObstical(currNode->getData()->getX(),currNode->getData()->getY());
        que->pop();

        // If we have rached the end point
        if(*(Point *)currNode->getData() == *end) {
            tempList = BfsGrid::makeList(currNode);
            delete(currNode);
            BfsGrid::deleteAllNodesList(checkedNodes);
            delete(checkedNodes);
            BfsGrid::removeNodesFromeList(chekedData);
            delete(chekedData);
            BfsGrid::deleteAllNodesQueue(que);
            delete(que);
            return tempList;
        }

        lstNeibors = grid->getNeighborsOfPoint((Point *)currNode->getData());
        while(!lstNeibors->empty()) {
            tempPoint = lstNeibors->back();
            newNode = new PointNode(*tempPoint);
            lstNeibors->pop_back();
            delete(tempPoint);

            // If the value was not checked allready.
            //if(!BfsGrid::isFound(chekedData, (Point *)newNode->getData())) {
                newNode->setNext(currNode);
                que->push(newNode);
            //} else {
            //    delete(newNode);
            //}
        }
        delete(lstNeibors);

        //cout <<
        chekedData->push_front((Point *)currNode->getData());
        checkedNodes->push_front(currNode);
    }

    BfsGrid::deleteAllNodesList(checkedNodes);
    delete(checkedNodes);
    BfsGrid::removeNodesFromeList(chekedData);
    delete(chekedData);
    BfsGrid::deleteAllNodesQueue(que);
    delete(que);

    return new list<pPoint>();
}

list<pPoint> * BfsGrid::makeList(PointNode * node) {
    list<pPoint> * lst = new list<pPoint>();
    PointNode * copyNode = node;
    Point * tempPoint;
//    PointNode * pointer;

    // Put the current value in the list.
    tempPoint = new Point (copyNode->getData()->getX(),copyNode->getData()->getY());
    lst->push_front(tempPoint);

    // While this SmartNode Points to another node;
    while(copyNode->isNextDifined()) {
        // Moves to the next node.
        copyNode = copyNode->getNext();
        tempPoint = new Point (copyNode->getData()->getX(),copyNode->getData()->getY());
        lst->push_front(tempPoint);
    }
    return lst;
}

bool BfsGrid::isFound(list <pPoint>* lst, Point *item) {
    list<pPoint> * templst= new list<pPoint>();
    bool isFound = false;
    Point * temp;

    while(!lst->empty()) {
        temp = lst->front();
        lst->pop_front();
        templst->push_front(temp);
    }

    while(!templst->empty()) {
        temp = templst->front();
        templst->pop_front();
        if(*item == *temp) {
            isFound = true;
        }
        lst->push_front(temp);
    }

    delete(templst);
    return isFound;
}

void BfsGrid::deleteAllNodesQueue(queue<pPointNode> *que) {
    PointNode * item;
    while(!que->empty()){
        item = que->front();
        que->pop();
        delete(item);
    }
}

void BfsGrid::deleteAllNodesList(list <pPointNode> *lst) {
    PointNode * item;
    while(!lst->empty()){
        item = lst->front();
        lst->pop_front();
        delete(item);
    }
}

void BfsGrid::removeNodesFromeList(list <pPoint> *lst) {
    while(!lst->empty()) {
        lst->pop_front();
    }
}