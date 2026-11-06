//第一部分：C++对C的改进和扩展完整源码
#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
using namespace std;

//============= 任务1：学生成绩管理 =============
void task1()
{
    struct Student {
        string name;
        double chinese, math, english;
        double avg;
    };
    int n;
    cin >> n;
    Student stu[100];
    for (int i = 0; i < n; ++i)
    {
        cin >> stu[i].name >> stu[i].chinese >> stu[i].math >> stu[i].english;
        stu[i].avg = (stu[i].chinese + stu[i].math + stu[i].english) / 3.0;
    }
    cout << fixed << setprecision(2);
    for (int i = 0; i < n; ++i)
    {
        cout << stu[i].name
            << " " << stu[i].chinese
            << " " << stu[i].math
            << " " << stu[i].english
            << " " << stu[i].avg;
        if (stu[i].avg >= 90)
            cout << " Excellent";
        cout << endl;
    }
}

//============= 任务2：const指针统计元音 =============
int countVowels(const char* p)
{
    int cnt = 0;
    while (*p != '\0')
    {
        char c = tolower(*p);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
            cnt++;
        p++;
    }
    return cnt;
}
void task2()
{
    char str[1000];
    cin.ignore();
    cin.getline(str, 1000);
    cout << "Number of vowels: " << countVowels(str) << endl;
}

//============= 任务3：内联、默认参数、函数重载计算器 =============
inline double square(double x)
{
    return x * x;
}

double power(double base, int exponent = 2)
{
    double res = 1.0;
    for (int i = 0; i < exponent; i++)
        res *= base;
    return res;
}

double calc(double a, double b, char op)
{
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    if (op == '/')
    {
        if (b == 0) return -1;
        return a / b;
    }
    return 0;
}

double calc(int a, int b, char op)
{
    return calc((double)a, (double)b, op);
}

void task3()
{
    cout << fixed << setprecision(2);
    double a, b;
    char op;
    cin >> a >> b >> op;

    cout << "square(" << a << ") = " << square(a) << endl;
    cout << "power(" << a << ",3) = " << power(a, 3) << endl;

    if (op == '/' && b == 0)
        cout << "Error: divide by zero" << endl;
    else
        cout << a << " " << op << " " << b << " = " << calc(a, b, op) << endl;
}

//============= 任务4：引用swap重载 =============
void swap(int& x, int& y)
{
    int t = x;
    x = y;
    y = t;
}
void swap(int* x, int* y)
{
    int t = *x;
    *x = *y;
    *y = t;
}

void task4()
{
    int x, y;
    cin >> x >> y;
    cout << "Before swap: x=" << x << ", y=" << y << endl;
    swap(x, y);
    cout << "After  swap: x=" << x << ", y=" << y << endl;
}

//============= 主菜单 =============
int main()
{
    int op;
    cout << "=====第二章实验=====\n1学生成绩 2元音统计 3计算器 4引用交换\n请选择：";
    cin >> op;
    if (op == 1) task1();
    if (op == 2) task2();
    if (op == 3) task3();
    if (op == 4) task4();
    return 0;
}
//第二部分：校园一卡通消费管理系统
#include <iostream>
#include <string>
#include <iomanip>
#include <stdexcept>

//系统常量
const double FEE_RATE = 0.02;
const double WARNING_BALANCE = 10.0;

//一卡通账户结构体
struct Card
{
    int id;                 //学号
    std::string name;       //姓名
    double balance;         //余额（元）
    int transactions;       //交易次数
};

//命名空间card封装输出工具
namespace card
{
    void printLine(char c = '-', int len = 60)
    {
        for (int i = 0; i < len; i++)
        {
            std::cout << c;
        }
        std::cout << std::endl;
    }

    void title(const std::string& s)
    {
        printLine('=');
        std::cout << std::setw((60 + (int)s.size()) / 2) << s << std::endl;
        printLine('=');
    }
}

//动态分配账户数组
Card* createCards(int n)
{
    if (n <= 0)
    {
        throw std::invalid_argument("账户数量不能小于等于0！");
    }
    Card* p = new Card[n];
    return p;
}

//释放动态数组
void destroyCards(Card* p)
{
    if (p != nullptr)
    {
        delete[] p;
        p = nullptr;
    }
}

//录入账户信息，加入异常校验：余额不能为负
void inputCards(Card* arr, int n)
{
    for (int i = 0; i < n; ++i)
    {
        std::cout << "请输入第" << i + 1 << "位学生：学号 姓名 初始余额：" << std::endl;
        std::cin >> arr[i].id >> arr[i].name >> arr[i].balance;
        arr[i].transactions = 0;

        if (arr[i].balance < 0)
        {
            throw std::runtime_error("异常：初始余额不能为负数！");
        }
    }
}

//充值函数，带默认手续费参数
void deposit(Card& c, double amount, double fee = FEE_RATE)
{
    if (amount < 0)
    {
        throw std::runtime_error("充值金额不能为负数！");
    }
    double realAdd = amount * (1 - fee);
    c.balance += realAdd;
    c.transactions++;
}

//消费基础版本
bool spend(Card& c, double cost)
{
    if (cost < 0)
    {
        throw std::runtime_error("消费金额不能为负数！");
    }
    if (c.balance < cost)
    {
        return false;
    }
    c.balance -= cost;
    c.transactions++;
    return true;
}

//重载：带消费项目名称
bool spend(Card& c, double cost, const std::string& item)
{
    std::cout << "消费项目：" << item << "，消费金额：" << std::fixed << std::setprecision(2) << cost << "元" << std::endl;
    return spend(c, cost);
}

//常引用查询余额，只读
void showBalance(const Card& c)
{
    std::cout << "【账户查询】姓名：" << c.name << " 当前余额：" << std::fixed << std::setprecision(2) << c.balance << " 元" << std::endl;
}

//const指针，只读查看学号
void viewStudentID(const Card* p)
{
    std::cout << "【ID查询】学生学号：" << p->id << std::endl;
}

//元转换为分，static_cast强制转换
int toCent(double amount)
{
    int cent = static_cast<int>(amount * 100);
    return cent;
}

//输出报表，格式化对齐
void printReport(const Card* arr, int n)
{
    using namespace std;
    card::printLine();
    cout << left
        << setw(10) << "ID"
        << setw(15) << "Name"
        << setw(12) << "Balance(Yuan)"
        << setw(15) << "Transactions" << endl;
    card::printLine();
    cout << fixed << setprecision(2);
    for (int i = 0; i < n; i++)
    {
        cout << left
            << setw(10) << arr[i].id
            << setw(15) << arr[i].name
            << setw(12) << arr[i].balance
            << setw(15) << arr[i].transactions << endl;
    }
    card::printLine();
}

//主程序入口
int main()
{
    using namespace std;
    int stuNum;
    Card* cardArr = nullptr;

    try
    {
        card::title("校园一卡通消费管理系统");
        cout << "请输入学生账户数量：";
        cin >> stuNum;

        //动态创建账户数组
        cardArr = createCards(stuNum);

        //录入账户信息
        inputCards(cardArr, stuNum);

        //对第一个账户充值100元
        cout << "\n>>>执行充值操作：给第一个账户充值100元" << endl;
        deposit(cardArr[0], 100.0);

        //消费：食堂午餐，8.5元
        cout << "\n>>>执行消费操作" << endl;
        bool ok = spend(cardArr[0], 8.5, "食堂午餐");
        if (!ok)
        {
            cout << "消费失败！余额不足！" << endl;
        }
        else
        {
            cout << "消费成功！" << endl;
        }

        //常引用查询余额
        cout << "\n>>>常引用showBalance查询：" << endl;
        showBalance(cardArr[0]);

        //const指针查询学号
        cout << "\n>>>const指针viewStudentID查询：" << endl;
        viewStudentID(&cardArr[0]);

        //元转为分
        int centVal = toCent(cardArr[0].balance);
        cout << "\n>>>余额换算成分：" << centVal << " 分" << endl;

        //打印全部报表
        cout << "\n====全部账户报表输出====" << endl;
        printReport(cardArr, stuNum);
    }
    catch (const bad_alloc& e)
    {
        cerr << "内存分配异常：" << e.what() << endl;
    }
    catch (const invalid_argument& e)
    {
        cerr << "参数异常：" << e.what() << endl;
    }
    catch (const runtime_error& e)
    {
        cerr << "业务异常：" << e.what() << endl;
    }
    catch (...)
    {
        cerr << "发生未知异常！" << endl;
    }

    //释放动态内存
    if (cardArr != nullptr)
    {
        destroyCards(cardArr);
        cout << "\n动态内存已释放，系统退出。" << endl;
    }
    return 0;
}
​