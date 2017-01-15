//
// Created by yifat on 01/12/16.
//
//
// Created by yifat on 26/11/16.
//

#include "GridNode.h"

GridNode::GridNode(int x, int y) {
    this->place = new Point(x,y);
    this->obstical = false;
}

GridNode::~GridNode() {
    delete (this->place);
}

Point * GridNode::getPlace() {
    return this->place;
}

void GridNode::setAsOnbltical() {
    this->obstical = true;
}

void GridNode::removeObstical() {
    this->obstical = false;
}

bool GridNode::isObstical() {
    return this->obstical;
}