#include "PointNode.h"
//
// Created by yifat on 27/11/16.
//


PointNode::PointNode(Point data) {
    this->isNextSet = false;
    this->data = new Point(data.getX(),data.getY());
}

PointNode::~PointNode() {
    delete(this->data);
}

Point *PointNode::getData() {
    return this->data;
}

PointNode *PointNode::getNext() {
    return this->next;
}

bool PointNode::isNextDifined() {
    return this->isNextSet;
}

void PointNode::setNext(PointNode *node) {
    this->next = node;
    this->isNextSet = true;
}
