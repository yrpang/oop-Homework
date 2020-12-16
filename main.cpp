#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "Hotel.hpp"

using namespace std;

// 主函数各种模式
void adminMode(Hotel &h)
{
    string user, passwd;
    cout << "请输入用户名: ";
    cin >> user;
    cout << "密码: ";
    cin >> passwd;

    Admin *a = h.adminLogin(user, passwd);
    if (a)
    {
        cout << "-----------管理员模式-----------" << std::endl;
        cout << "[1] 显示现状" << std::endl;
        cout << "[2] 添加房间" << std::endl;
        cout << "[3] 删除房间" << std::endl;
        cout << "[4] 修改顾客星级优惠政策" << std::endl;
        cout << "[5] 修改天数优惠政策" << std::endl;
        cout << "[0] 返回上级" << std::endl;
        while (true)
        {
            int choice;
            cout << "请选择操作: ";
            cin >> choice;
            switch (choice)
            {
            case 0:
                return;
            case 1:
                a->showstatus();
                break;
            case 2:
                a->addRoom();
                break;
            case 3:
                a->delRoom();
                break;
            case 4:
                a->setUserDiscount();
                break;
            case 5:
                a->setDaysDiscount();
                break;
            }
        }
    }
    else
    {
        cout << "用户名或密码错误!" << endl;
    }
}

void waiterMode(Hotel &h)
{
    string user, passwd;
    cout << "请输入用户名: ";
    cin >> user;
    cout << "密码: ";
    cin >> passwd;

    Waiter *w = h.waiterLogin(user, passwd);
    if (w)
    {
        cout << "-----------前台模式-----------" << std::endl;
        cout << "[1] 显示现状" << std::endl;
        cout << "[2] 办理入住" << std::endl;
        cout << "[3] 办理换房" << std::endl;
        cout << "[4] 退房收款" << std::endl;
        cout << "[0] 返回上级" << std::endl;
        while (true)
        {
            int choice;
            cout << "请选择操作: ";
            cin >> choice;
            switch (choice)
            {
            case 0:
                return;
            case 1:
                w->showstatus();
                break;
            case 2:
                w->checkIn();
                break;
            case 3:
                w->changeRoom();
                break;
            case 4:
                w->checkOut();
                break;
            }
        }
    }
    else
    {
        cout << "用户名或密码错误!" << endl;
    }
}

// 顾客登陆后模式
void loginMode(Hotel &h)
{
    string user, passwd;
    cout << "请输入用户名: ";
    cin >> user;
    cout << "密码: ";
    cin >> passwd;

    Customer *c = h.customerLogin(user, passwd);
    if (c)
    {
        cout << "-----------前台模式-----------" << std::endl;
        cout << "[1] 预定" << std::endl;
        cout << "[2] 取消" << std::endl;
        cout << "[3] 评论" << std::endl;
        cout << "[0] 返回上级" << std::endl;
        while (true)
        {
            int choice;
            cout << "请选择操作: ";
            cin >> choice;
            switch (choice)
            {
            case 0:
                return;
            case 1:
                c->book();
                break;
            case 2:
                c->cancel();
                break;
            case 3:
                c->comment();
            }
        }
    }
    else
    {
        cout << "用户名或密码错误!" << endl;
    }
}

void customerMode(Hotel &h)
{
    while (true)
    {
        cout << "-----------用户模式-----------" << std::endl;
        cout << "[1] 注册" << std::endl;
        cout << "[2] 登陆" << std::endl;
        cout << "[0] 返回上级" << std::endl;
        int choice;
        cout << "请选择操作: ";
        cin >> choice;
        switch (choice)
        {
        case 0:
            return;
        case 1:
            h.reg(h);
            break;
        case 2:
            loginMode(h);
            break;
        }
    }
}

void save(Hotel &h)
{
    std::ofstream ofs("data.dat");
    {
        boost::archive::binary_oarchive oa(ofs);
        // write class instance to archive
        oa << h;
        // archive and stream closed when destructors are called
    }
}

int main()
{
    Hotel h;

    std::ifstream ifs("data.dat");
    if (ifs)
    {
        boost::archive::binary_iarchive ia(ifs);
        ia >> h;
    }
    else
    {
        Admin admin("admin", "123", "yrpang", "1333", h);
        h.setAdmin(admin);

        Waiter waiter("waiter0", "123", "lll", "1333", h);
        h.setWaiter(waiter);
        cout << "这是您第一次使用已经自动初始化，请记得保存。" << endl;
    }

    while (true)
    {
        int choice;
        cout << "#############酒店管理系统#############" << std::endl;
        cout << "[1] Admin登陆" << std::endl;
        cout << "[2] 前台登陆" << std::endl;
        cout << "[3] 顾客模式" << std::endl;
        cout << "[0] 保存退出" << std::endl;
        cout << "请选择登陆模式: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            adminMode(h);
            break;
        case 2:
            waiterMode(h);
            break;
        case 3:
            customerMode(h);
            break;
        case 0:
            save(h);
            return 0;
        }
    }

    return 0;
}
