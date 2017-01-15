//
// Created by yifat on 01/12/16.
//

#ifndef UNITTEST_GRID_H
#define UNITTEST_GRID_H

#include "Point.h"
#include "GridNode.h"
#include <list>
using namespace std;

class Grid {
private:
    int height;
    int width;
    GridNode *** grideWorld;
    GridNode *** tempGride;
    /* initiateGrid.
    * <p>
    * initiates grid's cells.
    */
    void initiateGrid();
    /** createTempGrid
     * Creates a copy of the gridWorld.
     *
     */
    void createTempGrid();
    /** deleteTempGrid
     * Deletes the copy of the gridWolrd.
     */
    void deleteTempGrid();

public:
    /**
    * GridNode.
    *
    * constructor.
    * @param x x's position
    * @param y y's position
    */
    Grid(int x, int y);
    /* Grid.
    * <p>
    * destructor.
    */
    ~Grid();
    /* isObstical.
    * <p>
    * return true if the point is an obstical.
    * @param x x's position
    * @param y y's position
    * @return true\false
    */
    bool isObstical(int x, int y);
    /* isObstical.
    * <p>
    * return true if the point is an obstical.
    * @param p a point
    * @return true\false
    */
    bool isObstical(Point p);
    /* setObstacle.
    * <p>
    * setting the obstical.
    * @param x x's position
    * @param y y's position
    */
    void setObstical(int x, int y);
    /* setObstical.
    * <p>
    * setting the obstical.
    * @param p a point
    */
    void setObstical(Point p);
    /* removeObstical.
    * <p>
    * .removeing the Obstical
    * @param x x's position
    * @param y y's position
    */
    void removeObstical (int x, int y);
    /* removeObstical.
    * <p>
    * removeing the Obstical.
    * @param p a point
    */
    void removeObstical(Point p);
    /* getNeighborsOfPoint.
    * <p>
    * return the list of the neighbors of point p.
    * @param p a point
    * @return lst a list of points\neighbors
    */
    list <Point *> * getNeighborsOfPoint(Point * p);
    /* getSortesrPath.
    * <p>
    * returning the shortest path.
    * @param start starting point
    * @param end ending point
    * @return lstToReturn list of points
    */
    list <Point *> * getSortesrPath(Point * start, Point * end);
};

#endif //UNITTEST_GRID_H
