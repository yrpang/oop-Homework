#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/program_options.hpp>

#include "Room.hpp"

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
    void addRoom()
    {
        std::cout << "请输入starLevel price bedsNum" << std::endl;
        int starLevel; // 0: normal, 1: stadard, 2: good
        float price;   // RMB
        int bedsNum;
        int status; // 0: empty, 1: schedual, 2: full
        std::cin >> starLevel >> price >> bedsNum;
        rooms.emplace_back(Room(starLevel, price, bedsNum));
        freeRoom++;
    }

    void delRoom()
    {
        std::cout << "请输入要删除的房间编号:";
        int no;
        std::cin >> no;
        int i = 0;
        while (rooms[i].getNo() != no && i < rooms.size())
        {
            i++;
        };
        rooms.erase(rooms.begin() + i);
        freeRoom--;
    }

    void setUserDiscount()
    {
        std::cout << "请输入顾客星级数目：";
        int n;
        std::cin >> n;
        std::cout << "请依次输入顾客星级和对应优惠(star discount格式):";
        int star, dis;
        for (int i = 0; i < n; i++)
        {
            std::cin >> star >> dis;
            UserToDiscount[star] = dis;
        }
    }

    void setDaysDiscount()
    {
        std::cout << "请输入日期优惠种类：";
        int n;
        std::cin >> n;
        std::cout << "请依次起止天数和对应优惠[start, end) e.g. 2 4 0.8 表示住2、3天的为8折:";
        int start, end, dis;
        for (int i = 0; i < n; i++)
        {
            std::cin >> start >> end >> dis;
            for (int i = start; i < end; i++)
            {
                DaysToDiscount[i] = dis;
            }
        }
    }

    // getIndex工具函数
    int getRoomIndex(int roomNo)
    {
        if (rooms.size() == 0)
        {
            return -1;
        }
        int i = 0;
        while (rooms[i].getNo() != roomNo && i < rooms.size())
        {
            i++;
        };
        if (i >= rooms.size())
            return -1;
        else
            return i;
    }

    int getCustomIndex(int userNo);

public:
    friend Admin; // 设置成为友元 让Admin可以完全控制酒店
    std::string name = "yrpang's hotel";

    Hotel()
    {
        freeRoom = 0;
        for (int i = 0; i < 10; i++)
        {
            UserToDiscount[i] = 1.0;
        }
        for (int i = 0; i < 366; i++)
        {
            DaysToDiscount[i] = 1.0;
        }
        totalIncome = 0.0;
    }

    // 各种信息显示函数
    void showFree()
    {
        std::cout << std::setw(10) << std::setiosflags(std::ios::left) << "空房间为:" << std::endl;
        std::cout << "房间编号 星级 价格 床位数" << std::endl;
        for (int i = 0; i < rooms.size(); i++)
        {
            if (rooms[i].ifEmpty())
            {
                rooms[i].showInfo();
            }
        }
    }

    void showUser();

    void showStatus()
    {
        std::cout << "欢迎使用酒店管理系统" << std::endl;
        std::cout << "#############酒店基本情况介绍#############" << std::endl;
        std::cout << std::setw(10) << std::setiosflags(std::ios::left) << "酒店名称:" << std::setw(20) << std::setiosflags(std::ios::right) << name << std::endl;
        std::cout << std::setw(10) << std::setiosflags(std::ios::left) << "空房数目:" << std::setw(20) << std::setiosflags(std::ios::right) << freeRoom << std::endl;
        std::cout << "#############酒店入住情况介绍#############" << std::endl;
        std::cout << std::setw(10) << std::setiosflags(std::ios::left) << "已经入住房间为:" << std::endl;
        std::cout << "房间编号 星级 价格 床位数 顾客姓名" << std::endl;
        for (int i = 0; i < rooms.size(); i++)
        {
            if (rooms[i].ifFull())
            {
                rooms[i].showInfo();
            }
        }
        std::cout << std::setw(10) << std::setiosflags(std::ios::left) << "已经预定房间为:" << std::endl;
        std::cout << "房间编号 星级 价格 床位数 顾客姓名" << std::endl;
        for (int i = 0; i < rooms.size(); i++)
        {
            if (rooms[i].ifSchedual())
            {
                rooms[i].showInfo();
            }
        }
        this->showFree();
    }

    // 一次性初始化所用的函数
    void setAdmin(Admin &a)
    {
        admin.emplace_back(a);
    }
    void setWaiter(Waiter &w)
    {
        waiter.emplace_back(w);
    }

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
