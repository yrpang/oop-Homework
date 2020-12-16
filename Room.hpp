#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>
#include <boost/serialization/vector.hpp>

#ifndef ROOM_H
#define ROOM_H

class Room
{
private:
    int No;
    int starLevel; // 0: normal, 1: stadard, 2: good
    float price;   // RMB
    int bedsNum;
    int status; // 0: empty, 1: schedual, 2: full
    std::string userName;

    static int maxNo;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &No;
        ar &starLevel;
        ar &price;
        ar &bedsNum;
        ar &status;
        ar &userName;
        ar &maxNo;
    }

public:
    Room() {}
    Room(int starLevel, float price, int bedNum);

    void showInfo();
    bool ifFull();
    bool ifSchedual();
    bool ifEmpty();
    void setPrice(float newprice);
    int getNo();
    void checkIn(std::string userName);
    // 退房或取消
    void checkOut();
    float getPrice();
    int book(std::string userName);
};
#endif
