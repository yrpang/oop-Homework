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
    User(std::string userName, std::string passwd, std::string name, std::string phoneNum);

    std::string getUserName();
    int getNo();
    bool login(std::string passwd);
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
    Admin(std::string userName, std::string passwd, std::string name, std::string phoneNum, Hotel &h);

    void showstatus();
    void setPrice(int roomNo, float newprice);
    void setUserDiscount();
    void setDaysDiscount();
    void addRoom();
    void delRoom();
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
    Waiter(std::string userName, std::string passwd, std::string name, std::string phoneNum, Hotel &h);

    void showstatus();
    void checkIn();
    void changeRoom();
    void checkOut();
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
    Customer(std::string userName, std::string passwd, std::string name, std::string phoneNum, Hotel &h);

    void checkIn(int roomNo);
    int getRoomNo();
    void changeRoomNo(int roomNo);
    int checkOut();
    bool ifIn();
    bool ifBook();
    int getStar();
    void book();
    void cancel();
    void comment();
};

#endif
