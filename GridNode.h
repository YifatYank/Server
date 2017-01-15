//
// Created by yifat on 01/12/16.
//

#ifndef UNITTEST_GRIDNODE_H
#define UNITTEST_GRIDNODE_H

#include "Point.h"
#include "PointNode.h"
using namespace std;
class GridNode {
private:
    Point * place;
    bool obstical;
public:
    /**
    * GridNode.
    * <p>
    * constructor.
    * @param x x's coordinate
    * @param y y's coordinate
    */
    GridNode(int x,int y);
    /**
    * GridNode.
    * <p>
    * Destructor.
    */
    ~GridNode();
    /**
    * getPlace.
    * <p>
    * return the current place point.
    * @return place a Point
    */
    Point * getPlace();
    /**
    * SetAsOnbltical.
    * <p>
    * setting the obstacle.
    */
    void setAsOnbltical();
    /**
    * removeObstical.
    * <p>
    * removing the obstacle.
    */
    void removeObstical();
    /**
     * isObstical.
     * <p>
     * return true if it is an obstacle.
     * @return true if it is an obstacle,false otherwise.
     */
    bool isObstical();

};

#endif //UNITTEST_POINTNODE_H
