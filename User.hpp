#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>

#ifndef USER_H
#define USER_H

#include "Hotel.hpp"

class User
{
protected:
    int No;
    std::string userName;
    std::string passwd;
    std::string name;
    std::string phoneNum;
    time_t lastLogin;

    static int maxNo;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &No;
        ar &userName;
        ar &passwd;
        ar &name;
        ar &phoneNum;
        ar &lastLogin;
        ar &maxNo;
    }

public:
    User() {}
    User(std::string userName, std::string passwd, std::string name, std::string phoneNum)
    {
        this->No = ++maxNo;
        this->userName = userName;
        this->passwd = passwd;
        this->name = name;
        this->phoneNum = phoneNum;
        lastLogin = time(0);
    }
    std::string getUserName()
    {
        return userName;
    }
    int getNo()
    {
        return No;
    }
    bool login(std::string passwd)
    {
        if (this->passwd == passwd)
        {
            lastLogin = time(0);
        }
        return passwd == this->passwd;
    }
};

class Admin : public User
{
private:
    Hotel *h;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &boost::serialization::base_object<User>(*this);
        ar &h;
    }

public:
    Admin() {}
    Admin(std::string userName, std::string passwd, std::string name, std::string phoneNum, Hotel &h) : User(userName, passwd, name, phoneNum)
    {
        this->h = &h;
    }
    void showstatus()
    {
        h->showStatus();
    }
    void setPrice(int roomNo, float newprice)
    {
        int i;
        for (i = 0; i < h->rooms.size(); i++)
        {
            if (roomNo == h->rooms[i].getNo())
                break;
        }
        h->rooms[i].setPrice(newprice);
        h->rooms[i].showInfo();
    }
    void setUserDiscount()
    {
        h->setUserDiscount();
    }
    void setDaysDiscount()
    {
        h->setDaysDiscount();
    }
    void addRoom()
    {
        h->addRoom();
    }
    void delRoom()
    {
        h->delRoom();
    }
};

class Waiter : public User
{
private:
    Hotel *h;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &boost::serialization::base_object<User>(*this);
        ar &h;
    }

public:
    Waiter() {}
    Waiter(std::string userName, std::string passwd, std::string name, std::string phoneNum, Hotel &h) : User(userName, passwd, name, phoneNum)
    {
        this->h = &h;
    }
    void showstatus()
    {
        h->showStatus();
    }
    void checkIn()
    {
        h->checkin();
    }
    void changeRoom()
    {
        h->changeRoom();
    }
    void checkOut()
    {
        h->checkOut();
    }
};

class Customer : public User
{
private:
    int starLevel;
    int status; // 0: just resigin  1: schedual  2: in  3: finished

    time_t startTime;
    time_t endTime;
    time_t schedualTime;
    std::vector<std::string> comments;
    int roomNo;

    Hotel *h;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &boost::serialization::base_object<User>(*this);
        ar &h;

        ar &starLevel;
        ar &status;
        ar &startTime;
        ar &endTime;
        ar &schedualTime;
        ar &comments;
        ar &roomNo;
    }

public:
    Customer() {}
    Customer(std::string userName, std::string passwd, std::string name, std::string phoneNum, Hotel &h) : User(userName, passwd, name, phoneNum)
    {
        starLevel = 0;
        status = 0;
        roomNo = -1;
        this->h = &h;
    }
    void checkIn(int roomNo)
    {
        status = 2;
        startTime = time(0);
        this->roomNo = roomNo;
    }

    int getRoomNo()
    {
        return roomNo;
    }

    void changeRoomNo(int roomNo)
    {
        this->roomNo = roomNo;
    }

    int checkOut()
    {
        status = 3;
        endTime = time(0);
        roomNo = -1;

        long remainSec = difftime(endTime, startTime);
        int days = (remainSec / (60 * 60 * 24));
        return days;
        // return 3;
    }

    bool ifIn()
    {
        return status == 2;
    }

    int getStar()
    {
        return starLevel;
    }

    void book()
    {
        if (status == 1 || status == 2)
        {
            std::cout << "当前状态不可预定，请退房或取消已有预定！" << std::endl;
            return;
        }

        time_t now = time(0);
        std::string s = std::ctime(&now);
        std::cout << "当前时间为" + s << "请问要预定几天后的房间:";
        int n;
        std::cin >> n;

        if (n < 1 || n > 7)
        {
            std::cout << "请输入大于0小于7的数字!" << std::endl;
        }

        int roomNo = h->book(this->No);
        if (roomNo != -1)
        {
            status = 1;
            this->roomNo = roomNo;
            this->schedualTime = now += n * 24 * 3600;
            std::cout << "预定成功！" << std::endl;
        }
    }

    void cancel()
    {
        if (status != 1)
        {
            std::cout << "当前状态无法取消，请联系前台！" << std::endl;
            return;
        }
        time_t now = time(0);
        long day_now = now / 3600 / 24;
        long day_schedual = schedualTime / 3600 / 24;
        if (day_schedual - day_now < 1)
        {
            std::cout << "距离预定时间太近了，不能取消，抱歉！" << std::endl;
            return;
        }
        h->cancel(this->roomNo);
        status = 0;
        std::cout << "预定取消成功！" << std::endl;
    }

    void comment()
    {
        if (status != 3)
        {
            std::cout << "退房后才能评论哦,期待您的好评～" << std::endl;
        }
        else
        {
            std::cout << "请输入您的评价：";
            std::string com;
            std::cin >> com;
            this->comments.emplace_back(com);
        }
    }
};

int User::maxNo = -1;

#endif
