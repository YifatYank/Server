//
// Created by yifat on 01/12/16.
//

#ifndef UNITTEST_BFSGRID_H
#define UNITTEST_BFSGRID_H

#include <list>
#include "queue"
#include "Point.h"
#include "Grid.h"
#include "Definitions.h"

using namespace std;

#include "Definitions.h"
class BfsGrid {
private:
    /* isFound.
    * <p>
    * return true if the point is found.
    * @param lst list of points
    * @param item the point required
    * @isFound true or false
    */
    static bool isFound(list <pPoint>* lst, Point * item);
    /* GridNode.
    * <p>
    * constructor.
    * @param x x's position
    * @param y y's position
    */
    static void removeNodesFromeList(list<pPoint> * lst);
    /* deleteAllNodesList
    * <p>
    * deleting all the nodes in the list.
    * @param lst a list of nodes
    */
    static void deleteAllNodesList(list<pPointNode> * lst);
    /* makeList.
    * <p>
    * making a list of points
    * @param node a node
    * @return lst a list of points
    */
    static list<pPoint> * makeList(PointNode * node);
    /* deleteAllNodesQueue.
    * <p>
    * deleting all the nodes in the queue.
    * @param que a queue of nodes
    */
    static void deleteAllNodesQueue(queue<pPointNode> * que);
public:
    /* getShortestPath.
    * <p>
    * return the shortest path on the grid.
    * @param grid a grid
    * @param start starting point
    * @param end ending point
    * @return pPoint a list of point
    */
    static list<pPoint > * getShortestPath(Grid * grid, Point * start, Point * end);
};


#endif //UNITTEST_BFSGRID_H
