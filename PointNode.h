//
// Created by yifat on 01/12/16.
//

#ifndef UNITTEST_POINTNODE_H
#define UNITTEST_POINTNODE_H

#include "Point.h"


class PointNode {
private:
    Point * data;
    PointNode * next;
    bool isNextSet;
public:
/* PointNode.
    * <p>
    * constructor.
    * @param data a point
    */
    PointNode(Point p);
/* PointNode.
    * <p>
    * destructor.
    */
    ~PointNode();
/* getData.
    * <p>
    * returns the data of a point.
    * @return data the data of a point
    */
    Point * getData();
/* getNext.
    * <p>
    * returns the next point.
    * @return next next point
    */
    PointNode * getNext();
/* setNext.
    * <p>
    * sets the next node.
    * @param node a node
    */
    void setNext(PointNode * node);
/* isNextDifined.
    * <p>
    * returns true if the next point is difined.
    * @return isNextSet true\false
    */
    bool isNextDifined();

};


#endif //UNITTEST_POINTNODE_H
