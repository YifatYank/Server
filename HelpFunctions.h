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
    static list <pstring> * split(string * str,char ch);
    static int stringToInt(string str);

    static string serialize(Point * point);
    static string serialize(Trip * trp);
    static string serialize(Cab * cab);
    static string serialize(Driver * driver);

    static Point * deserializePoint(char * str);
    static Trip * deserializeTrip(char * str);
    static Cab * desrializeCab(char * str);
    static Driver * desrializeDriver(char * str);
};

#endif //SERVER_HELPFUNCTIONS_H
