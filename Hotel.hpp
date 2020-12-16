#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>
#include <boost/serialization/vector.hpp>

#include "Room.hpp"
#include "User.hpp"

#ifndef HOTEL_H
#define HOTEL_H

class Room;
class Admin;
class Waiter;
class Customer;
class Hotel
{
private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &freeRoom;
        ar &UserToDiscount;
        ar &DaysToDiscount;
        ar &totalIncome;
        ar &rooms;
        ar &admin;
        ar &waiter;
        ar &customer;
    }

protected:
    int freeRoom;
    std::vector<Room> rooms;
    std::vector<Admin> admin;
    std::vector<Waiter> waiter;
    std::vector<Customer> customer;
    float UserToDiscount[10];
    float DaysToDiscount[366];
    float totalIncome;

    // 仅管理员可设置的部分
    void addRoom();
    void delRoom();
    void setUserDiscount();
    void setDaysDiscount();

    // getIndex工具函数
    int getRoomIndex(int roomNo);
    int getCustomIndex(int userNo);

public:
    friend Admin; // 设置成为友元 让Admin可以完全控制酒店
    std::string name = "yrpang's hotel";

    Hotel();

    // 各种信息显示函数
    void showFree();
    void showUser(bool availableOnly = false);
    void showStatus();

    // 一次性初始化所用的函数
    void setAdmin(Admin &a);
    void setWaiter(Waiter &w);

    // 酒店管理函数
    void checkin();
    void changeRoom();
    void checkOut();
    void reg(Hotel &h);
    int book(int cusNo);
    void cancel(int roomNo);

    // 登陆函数
    Admin *adminLogin(std::string user, std::string passwd);
    Waiter *waiterLogin(std::string user, std::string passwd);
    Customer *customerLogin(std::string user, std::string passwd);
};

#endif
