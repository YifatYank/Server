//
// Created by yifat on 21/01/17.
//

#include "HelpFunctions.h"
std::stringstream ss;
list<pstring> * HelpFunctions::split(string * str,char ch) {
    string * newStr = new string();
    int index;
    char tempChar;
    list<pstring> *  lst = new list<pstring>();
    for(index = 0; index < str->length(); ++index) {
        tempChar = str->at(index);
        if(tempChar != ch) {
            newStr->push_back(tempChar);
        } else {
            lst->push_front(newStr);
            newStr = new string();
        }
    }

    if(newStr->length() != 0) {
        lst->push_front(newStr);
    } else {
        delete(newStr);
    }
    return lst;
}
int HelpFunctions::stringToInt(string str){
    char ch;
    int index, num = 0, digit;

    for(index = 0; index < str.size(); ++index) {
        ch = str.at(index);
        digit = ch - 48;
        num = num * 10 + digit;
    }
    return num;
}
string HelpFunctions::serialize(Point * point) {
    std::string serial_str;
    boost::iostreams::back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive oa(s);

    oa << point;
    s.flush();

    return serial_str;
}
string HelpFunctions::serialize(Trip * trp){
    std::string serial_str;
    boost::iostreams::back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive oa(s);

    oa << trp;
    s.flush();

    return serial_str;
}
string HelpFunctions::serialize(Cab * cab) {
    std::string serial_str;
    boost::iostreams::back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive oa(s);

    oa << cab;
    s.flush();

    return serial_str;
}
string HelpFunctions::serialize(Driver * driver) {
    int index;
    int lenth;

    std::string serial_str;
    boost::iostreams::back_insert_device<std::string> inserter(serial_str);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
    boost::archive::binary_oarchive oa(s);

    oa << driver;
    s.flush();

    //lenth = serial_str.length();
    //for(index = 0;index < lenth - 1; ++ index) {
    //    if(serial_str.at(index) == '\0') {
    //        serial_str.replace(index,1,"|");
    //    }
    //}

    //serial_str.replace('\0','|');
    return serial_str;
}
Point * HelpFunctions::deserializePoint(char * str){
    Point * point;
    boost::iostreams::basic_array_source<char> device(str, 4096);
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
    boost::archive::binary_iarchive ia(s2);
    ia >> point;

    return point;
}
Trip * HelpFunctions::deserializeTrip(char * str){
    Trip * trp;

    boost::iostreams::basic_array_source<char> device(str, 2048);
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
    boost::archive::binary_iarchive ia(s2);
    ia >> trp;

    return trp;
}
Cab * HelpFunctions::desrializeCab(char * str){
    Cab * cab;
    boost::iostreams::basic_array_source<char> device(str, 2048);
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
    boost::archive::binary_iarchive ia(s2);
    ia >> cab;

    return cab;
}
Driver * HelpFunctions::desrializeDriver(char * str) {
    Driver * driver;
    int index;
    //int lenth = str.length();
    //for(index = 0;index < lenth - 1; ++ index) {
    //     if(str.at(index) == '|') {
    //         str.replace(index,1,"\0");
    //     }
    // }

    boost::iostreams::basic_array_source<char> device(str, 4096);
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
    boost::archive::binary_iarchive ia(s2);
    ia >> driver;

    return driver;
}
