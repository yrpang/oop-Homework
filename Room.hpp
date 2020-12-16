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
    Room(){}
    Room(int starLevel, float price, int bedNum)
    {
        this->No = ++maxNo;
        this->starLevel = starLevel;
        this->price = price;
        this->bedsNum = bedNum;
        this->userName = "";
        status = 0;
    }

    void showInfo()
    {
        std::cout << No << " " << starLevel << " " << price << " " << bedsNum << " " << userName << std::endl;
    }

    bool ifFull()
    {
        return status == 2;
    }

    bool ifSchedual()
    {
        return status == 1;
    }

    bool ifEmpty()
    {
        return status == 0;
    }

    void setPrice(float newprice)
    {
        price = newprice;
    }

    int getNo()
    {
        return No;
    }

    void checkIn(std::string userName)
    {
        status = 2;
        this->userName = userName;
    }

    // 退房或取消
    void checkOut()
    {
        status = 0;
        std::string tmp("");
        this->userName = tmp;
    }

    float getPrice()
    {
        return price;
    }

    int book(std::string userName)
    {
        if (status != 0)
            return -1;
        status = 1;
        this->userName = userName;
        return 0;
    }
};

int Room::maxNo = -1;

#endif
