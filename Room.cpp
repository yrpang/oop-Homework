#include "Room.hpp"

Room::Room(int starLevel, float price, int bedNum)
{
    this->No = ++maxNo;
    this->starLevel = starLevel;
    this->price = price;
    this->bedsNum = bedNum;
    this->userName = "";
    status = 0;
}

void Room::showInfo()
{
    std::cout << No << " " << starLevel << " " << price << " " << bedsNum << " " << userName << std::endl;
}

bool Room::ifFull()
{
    return status == 2;
}

bool Room::ifSchedual()
{
    return status == 1;
}

bool Room::ifEmpty()
{
    return status == 0;
}

void Room::setPrice(float newprice)
{
    price = newprice;
}

int Room::getNo()
{
    return No;
}

void Room::checkIn(std::string userName)
{
    status = 2;
    this->userName = userName;
}

// 退房或取消
void Room::checkOut()
{
    status = 0;
    std::string tmp("");
    this->userName = tmp;
}

float Room::getPrice()
{
    return price;
}

int Room::book(std::string userName)
{
    if (status != 0)
        return -1;
    status = 1;
    this->userName = userName;
    return 0;
}

int Room::maxNo = -1;