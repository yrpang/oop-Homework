#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>
#include "Room.h"
#include "User.h"
#include "Hotel.h"

using namespace std;

// 登陆控制部分
Admin *Hotel::adminLogin(std::string user, std::string passwd)
{
    int i;
    for (i = 0; i < admin.size(); i++)
    {
        if (admin[i].getUserName() == user)
            break;
    }
    if (admin[i].login(passwd))
    {
        return &admin[i];
    }
    else
    {
        return nullptr;
    }
}

Waiter *Hotel::waiterLogin(std::string user, std::string passwd)
{
    int i;
    for (i = 0; i < waiter.size(); i++)
    {
        if (waiter[i].getUserName() == user)
            break;
    }
    if (waiter[i].login(passwd))
    {
        return &waiter[i];
    }
    else
    {
        return nullptr;
    }
}

Customer *Hotel::customerLogin(std::string user, std::string passwd)
{
    int i;
    for (i = 0; i < customer.size(); i++)
    {
        if (customer[i].getUserName() == user)
            break;
    }
    if (customer[i].login(passwd))
    {
        return &customer[i];
    }
    else
    {
        return nullptr;
    }
}

// 工具函数
int Hotel::getCustomIndex(int userNo)
{
    if (customer.size() == 0)
    {
        return -1;
    }
    int j = 0;
    while (customer[j].getNo() != userNo && j < customer.size())
    {
        j++;
    };
    if (j >= customer.size())
        return -1;
    else
        return j;
}

void Hotel::showUser()
{
    std::cout << std::setw(10) << std::setiosflags(std::ios::left) << "现有顾客为:" << std::endl;
    std::cout << "顾客编号 姓名" << std::endl;
    for (int i = 0; i < customer.size(); i++)
    {
        std::cout << customer[i].getNo() << " " << customer[i].getUserName() << std::endl;
    }
}

// 办理入住
void Hotel::checkin()
{
    std::cout << "-------可用房间信息-------------" << std::endl;
    this->showFree();
    std::cout << "-------可用顾客信息-------------" << std::endl;
    this->showUser();

    std::cout << "请输入顾客编号和房间号:";
    int no, roomNo;
    std::cin >> no >> roomNo;

    int j = this->getCustomIndex(no);
    // cout<<"checkIn cusNo:"<<j<<endl;
    if (j == -1)
    {
        std::cout << "用户不存在" << endl;
        return;
    }
    // cout<<"checkIn roomNo:"<<roomNo<<endl;
    customer[j].checkIn(roomNo);

    int i = this->getRoomIndex(roomNo);
    if (i == -1)
    {
        std::cout << "房间不存在";
        return;
    }
    rooms[i].checkIn(customer[j].getUserName());
    freeRoom--;
}

// 换房
void Hotel::changeRoom()
{
    std::cout << "请输入顾客编号";
    int no;
    std::cin >> no;
    this->showFree();
    std::cout << "请输入目标房间编号";
    int roomNo;
    std::cin >> roomNo;

    int j = this->getCustomIndex(no);
    if (j == -1)
    {
        std::cout << "用户不存在" << endl;
        return;
    }

    int newroom = this->getRoomIndex(roomNo);
    if (newroom == -1)
    {
        std::cout << "房间不存在";
        return;
    }
    rooms[newroom].checkIn(customer[j].getUserName());
    int oldroom = this->getRoomIndex(customer[j].getRoomNo());
    rooms[oldroom].checkOut();

    customer[j].changeRoomNo(roomNo);
}

// 退房收款
void Hotel::checkOut()
{
    std::cout << "请输入顾客编号:";
    int no;
    std::cin >> no;

    int i = this->getCustomIndex(no);

    if (i == -1)
    {
        std::cout << "用户不存在" << endl;
        return;
    }
    if (!customer[i].ifIn())
    {
        cout << "错误！客人没入住！" << endl;
        return;
    }
    //先把roomNo存起来
    int j = this->getRoomIndex(customer[i].getRoomNo());
    // 再执行checkOut()
    int days = customer[i].checkOut();

    rooms[j].checkOut();
    freeRoom++;

    float moneyIncome = rooms[j].getPrice() * days * DaysToDiscount[days] * UserToDiscount[customer[i].getStar()];
    totalIncome += moneyIncome;
    cout << "收入:" << moneyIncome << endl;
    cout << "目前总收入:" << totalIncome << endl;
}

// 顾客注册
void Hotel::reg(Hotel &h)
{
    string userName;
    string passwd;
    string name;
    string phoneNum;

    cout << "请输入用户名 密码 姓名 手机号";
    cin >> userName >> passwd >> name >> phoneNum;

    Customer c(userName, passwd, name, phoneNum, h);
    customer.emplace_back(c);
}

// 顾客预定
int Hotel::book(int cusNo) // 此处处理房间部分
{
    int no;
    this->showFree();
    cout << "请输入房间号";
    cin >> no;

    // 拿到 用户的index
    int j = this->getCustomIndex(cusNo);

    // 拿到房间index
    int i = this->getRoomIndex(no);
    if (i == -1)
    {
        cout << "房间不存在" << endl;
        return -1;
    }
    if (rooms[i].book(customer[j].getUserName()) == -1)
    {
        cout << "错误！房间已满" << endl;
        return -1;
    }

    return no;
    //用户状态由用户部分进行操作
}

// 顾客取消
void Hotel::cancel(int roomNo) // 处理房间部分
{
    int i = this->getRoomIndex(roomNo);
    rooms[i].checkOut();
}

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

int main()
{
    Hotel h;
    Admin admin("admin", "123", "yrpang", "1333", h);
    h.setAdmin(admin);

    Waiter waiter("waiter0", "123", "lll", "1333", h);
    h.setWaiter(waiter);
    while (true)
    {
        int choice;
        cout << "#############酒店管理系统#############" << std::endl;
        cout << "[1] Admin登陆" << std::endl;
        cout << "[2] 前台登陆" << std::endl;
        cout << "[3] 顾客模式" << std::endl;
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
        }
    }

    return 0;
}