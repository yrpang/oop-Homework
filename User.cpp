#include "User.hpp"

User::User(std::string userName, std::string passwd, std::string name, std::string phoneNum)
{
    this->No = ++maxNo;
    this->userName = userName;
    this->passwd = passwd;
    this->name = name;
    this->phoneNum = phoneNum;
    lastLogin = time(0);
}
std::string User::getUserName()
{
    return userName;
}
int User::getNo()
{
    return No;
}
bool User::login(std::string passwd)
{
    if (this->passwd == passwd)
    {
        lastLogin = time(0);
    }
    return passwd == this->passwd;
}

Admin::Admin(std::string userName, std::string passwd, std::string name, std::string phoneNum, Hotel &h) : User(userName, passwd, name, phoneNum)
{
    this->h = &h;
}
void Admin::showstatus()
{
    h->showStatus();
}
void Admin::setPrice(int roomNo, float newprice)
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
void Admin::setUserDiscount()
{
    h->setUserDiscount();
}
void Admin::setDaysDiscount()
{
    h->setDaysDiscount();
}
void Admin::addRoom()
{
    h->addRoom();
}
void Admin::delRoom()
{
    h->delRoom();
}

Waiter::Waiter(std::string userName, std::string passwd, std::string name, std::string phoneNum, Hotel &h) : User(userName, passwd, name, phoneNum)
{
    this->h = &h;
}
void Waiter::showstatus()
{
    h->showStatus();
}
void Waiter::checkIn()
{
    h->checkin();
}
void Waiter::changeRoom()
{
    h->changeRoom();
}
void Waiter::checkOut()
{
    h->checkOut();
}

Customer::Customer(std::string userName, std::string passwd, std::string name, std::string phoneNum, Hotel &h) : User(userName, passwd, name, phoneNum)
{
    starLevel = 0;
    status = 0;
    roomNo = -1;
    this->h = &h;
}
void Customer::checkIn(int roomNo)
{
    status = 2;
    startTime = time(0);
    this->roomNo = roomNo;
}

int Customer::getRoomNo()
{
    return roomNo;
}

void Customer::changeRoomNo(int roomNo)
{
    this->roomNo = roomNo;
}

int Customer::checkOut()
{
    status = 3;
    endTime = time(0);
    roomNo = -1;

    long remainSec = difftime(endTime, startTime);
    int days = (remainSec / (60 * 60 * 24));
    return days;
    // return 3;
}

bool Customer::ifIn()
{
    return status == 2;
}

int Customer::getStar()
{
    return starLevel;
}

void Customer::book()
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

void Customer::cancel()
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

void Customer::comment()
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

int User::maxNo = -1;