#include <map>
#include <string>
#include <iostream>
using namespace std;
struct student
{
    string name;
    int age;
    string city;
    string phone;
    bool operator <(const student &stu)const
    {
        if (age!=stu.age)
            return age<stu.age;
        else
        {
            return name.compare(stu.name)<0;
        }
    }
};

int main()
{
    student s[]=
    {
        {"童进",23,"武汉","XXX"},
        {"老大",24,"武汉","XXX"},
        {"饺子",25,"武汉","XXX"},
        {"王老虎",26,"武汉","XXX"},
        {"周润发",23,"武汉","XXX"},
        {"周星星",23,"武汉","XXX"}
    };
    pair<student,int> p1(s[0],4);
    pair<student,int> p2(s[1],2);
    pair<student,int> p3(s[2],1);
    pair<student,int> p4(s[3],5);  //键值key与p1相同
    pair<student,int> p5(s[4],7);
    pair<student,int> p6(s[5],8);
    multimap<student,int> a;
    a.insert(p1);
    a.insert(p2);
    a.insert(p3);
    a.insert(p4);
    a.insert(p5);
    a.insert(p6);
//    typedef multimap<student,int>::iterator int_multimap;
//    pair<int_multimap,int_multimap> p = a.equal_range(4);
//    int_multimap i = a.find(4);
//    cout<<"班上key值为"<< i->first<<"的学生有："<<a.count(4)<<"名,"<<"   他们是:"<<endl;
//    for(int_multimap k = p.first; k != p.second; k++)
//    {
//        cout<<k->second.name<<endl;
//    }
//    cout<<"删除重复键值的同学"<<endl;
//    a.erase(4);
    cout<<"现在班上总人数为："<<a.size()<<".   人员如下："<<endl;
    for(multimap<student,int>::iterator j=a.begin(); j != a.end(); j++)
    {
        cout<<"The name: "<<j->first.name<<"      "<<"age: "<<j->first.age<<"   "
            <<"city: "<<j->first.city<<"      "<<"phone: "<<j->first.phone<<endl;
    }

    return 0;
}
