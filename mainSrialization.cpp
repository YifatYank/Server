//
// Created by yifat on 29/12/16.
//

#include <stdio.h>
#include <iostream>
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
#include "Point.h"
#include "Trip.h"
#include "Cab.h"
#include "Driver.h"

using namespace std;
using namespace boost::archive;
std::stringstream ss;

int main() {
    //GridPoint *gp = new GridPoint(new Point(1,5));

    Trip * t = new Trip(0,1,Point(0,0), Point(1,1), 5);
    Cab * cab = new Cab(0, HONDA, WHITE);
    cab->setTrip(t);
    Driver * driver = new Driver(0,22,single,2);
    driver->setCab(cab);
    Driver * copyDriver;

    std::string serial_str;
    boost::iostreams::back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive oa(s);

    oa << driver;
    s.flush();

    //cout << serial_str << endl;

    boost::iostreams::basic_array_source<char> device(serial_str.c_str(), serial_str.size());
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
    boost::archive::binary_iarchive ia(s2);
    ia >> copyDriver;

    delete(cab);
    delete(t);
    delete(driver);
    delete(copyDriver);
  //  string * stringP = p->getString();
  //  cout << stringP;
//    delete(stringP);

   // stringP = p1->getString();
   // cout << stringP;
   // delete(stringP);


    // Srialize trip.
//    oa << t;
 //   s.flush();

    // Deserialize trip
//    ia >> copyTrip;

    return 0;
}