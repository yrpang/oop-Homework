#include "Hotel.hpp"

void Hotel::addRoom()
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

void Hotel::delRoom()
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

void Hotel::setUserDiscount()
{
    std::cout << "请输入顾客星级数目：";
    int n;
    std::cin >> n;
    std::cout << "请依次输入顾客星级和对应优惠(star discount格式):";
    int star;
    float dis;
    for (int i = 0; i < n; i++)
    {
        std::cin >> star >> dis;
        UserToDiscount[star] = dis;
    }
}

void Hotel::setDaysDiscount()
{
    std::cout << "请输入日期优惠种类：";
    int n;
    std::cin >> n;
    std::cout << "请依次起止天数和对应优惠[start, end) e.g. 2 4 0.8 表示住2、3天的为8折:";
    int start, end;
    float dis;
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
int Hotel::getRoomIndex(int roomNo)
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

Hotel::Hotel()
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
void Hotel::showFree()
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

void Hotel::showStatus()
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
void Hotel::setAdmin(Admin &a)
{
    admin.emplace_back(a);
}
void Hotel::setWaiter(Waiter &w)
{
    waiter.emplace_back(w);
}

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

void Hotel::showUser(bool availableOnly)
{
    std::cout << std::setw(10) << std::setiosflags(std::ios::left) << "现有顾客为:" << std::endl;
    std::cout << "顾客编号 姓名" << std::endl;
    for (int i = 0; i < customer.size(); i++)
    {
        if (!availableOnly || !customer[i].ifIn())
        {
            std::cout << customer[i].getNo() << " " << customer[i].getUserName() << std::endl;
        }
    }
}

// 办理入住
void Hotel::checkin()
{
    std::cout << "-------可用顾客信息-------------" << std::endl;
    this->showUser(true);

    std::cout << "请输入顾客编号:";
    int no, roomNo;
    std::cin >> no;

    int j = this->getCustomIndex(no);
    // cout<<"checkIn cusNo:"<<j<<endl;
    if (j == -1)
    {
        std::cout << "用户不存在" << std::endl;
        return;
    }
    if (customer[j].ifIn())
    {
        std::cout << "顾客已经入住了不可以重复入住！" << std::endl;
    }
    else
    {
        if (customer[j].ifBook())
        {
            roomNo = customer[j].getRoomNo();
        }
        else
        {
            std::cout << "-------可用房间信息-------------" << std::endl;
            this->showFree();
            std::cout << "用户没有预定，请输入房间号:";
            std::cin >> roomNo;
        }

        customer[j].checkIn(roomNo);
    }

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
        std::cout << "用户不存在" << std::endl;
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
    std::cout << "--------现有入住客人信息----------" << std::endl;
    std::cout << "编号 姓名 房间号" << std::endl;
    for (int i = 0; i < customer.size(); i++)
    {
        if (customer[i].ifIn())
        {
            std::cout << customer[i].getNo() << " " << customer[i].getUserName() << " " << customer[i].getRoomNo() << std::endl;
        }
    }

    std::cout << "请输入顾客编号:";
    int no;
    std::cin >> no;

    int i = this->getCustomIndex(no);

    if (i == -1)
    {
        std::cout << "用户不存在" << std::endl;
        return;
    }
    if (!customer[i].ifIn())
    {
        std::cout << "错误！客人没入住！" << std::endl;
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
    std::cout << "收入:" << moneyIncome << std::endl;
    std::cout << "目前总收入:" << totalIncome << std::endl;
}

// 顾客注册
void Hotel::reg(Hotel &h)
{
    std::string userName;
    std::string passwd;
    std::string name;
    std::string phoneNum;

    std::cout << "请输入用户名 密码 姓名 手机号";
    std::cin >> userName >> passwd >> name >> phoneNum;

    Customer c(userName, passwd, name, phoneNum, h);
    customer.emplace_back(c);
}

// 顾客预定
int Hotel::book(int cusNo) // 此处处理房间部分
{
    int no;
    this->showFree();
    std::cout << "请输入房间号";
    std::cin >> no;

    // 拿到 用户的index
    int j = this->getCustomIndex(cusNo);

    // 拿到房间index
    int i = this->getRoomIndex(no);
    if (i == -1)
    {
        std::cout << "房间不存在" << std::endl;
        return -1;
    }
    if (rooms[i].book(customer[j].getUserName()) == -1)
    {
        std::cout << "错误！房间已满" << std::endl;
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
