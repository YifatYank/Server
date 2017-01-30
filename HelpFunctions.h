//
// Created by yifat on 21/01/17.
//

#ifndef SERVER_HELPFUNCTIONS_H
#define SERVER_HELPFUNCTIONS_H

#include <iostream>
#include "Enums.h"
#include "Driver.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "Socket.h"
#include "Tcp.h"
#include "Cab.h"
#include "Driver.h"
#include "Definitions.h"

using namespace boost::archive;using namespace std;

class HelpFunctions {
public:
    /**
   * split
   *splits a string by a char
   * @param str a string
     * @param ch the char
     * @return a list
  */
    static list <pstring> *split(string *str, char ch);

/**
   * stringToInt
   * converts string to int
   * @param str string
     * @return integer
  */
    static int stringToInt(string str);

/**
   * serialize
   * serialize a point to string
   * @param point
     * @return a string
  */
    static string serialize(Point *point);

/**
   * serialize
   * serialize a trip to string
   * @param trip
     * @return a string
  */
    static string serialize(Trip *trp);

/**
   * serialize
   * serialize a cab to string
   * @param cab
     * @return a string
  */
    static string serialize(Cab *cab);

/**
   * serialize
   * serialize a driver to string
   * @param driver
     * @return a string
  */
    static string serialize(Driver *driver);

/**
   * deserialize
   * deserialize a string to pointer
   * @param str a string
     * @return point
  */
    static Point *deserializePoint(char *str);

/**
   * deserialize
   * deserialize a string to trip
   * @param str a string
     * @return trip
  */
    static Trip *deserializeTrip(char *str);

/**
   * deserialize
   * deserialize a string to cab
   * @param str a string
     * @return cab
  */
    static Cab *desrializeCab(char *str);

/**
   * deserialize
   * deserialize a string to driver
   * @param str a string
     * @return driver
  */
    static Driver *desrializeDriver(char *str);
};

#endif //SERVER_HELPFUNCTIONS_H
