#ifndef CLASS_INT_H_INCLUDED
#define CLASS_INT_H_INCLUDED
#include <iostream>
#include <vector>
using namespace std;

class integers
{
public:
    vector<int> number;
    void init();
    integers& operator=(const integers& In);
    integers operator+(const integers& In);
    bool operator <(const integers& In)const;
    bool operator ==(const integers& In)const;
    bool operator >(const integers& In)const;
    friend ostream& operator<<(ostream & os,const integers &In);
};

void integers::init()
{
    string temp;
    cout<<"enter the number"<<endl;
    cin>>temp;
    int i=temp.length()-1;
    while (i>=0)
    {
        number.push_back(temp[i]-'0');
        i--;
    }
}

ostream& operator<<(ostream & os,const integers &In)
{
    for (int i=In.number.size()-1; i>=0; i--)
    {
        os<<In.number[i];
    }
    return os;
}

integers integers::operator + (const integers& In)
{
    integers temp;
    size_t i=0;
    size_t a=number.size();
    size_t b=In.number.size();
    while (i!=a && i!=b)
    {
        temp.number.push_back(number[i] + In.number[i]);
        i++;
    }
    if(i==a)
    {
        while (i!=b)
        {
            temp.number.push_back(In.number[i++]);
        }
    }
    else
    {
        while (i!=a)
        {
            temp.number.push_back(number[i++]);
        }
    }
    for (i=0; i<temp.number.size()-1; i++)
    {
        if (temp.number[i]>10)
        {
            temp.number[i+1]+=(temp.number[i]/10);
            temp.number[i]%=10;
        }
    }
    if (temp.number[i]>10)
    {
        temp.number.push_back(temp.number[i]/10);
        temp.number[i]%=10;
    }
    return temp;
}


integers& integers::operator =(const integers& In)
{
    number.clear();
    for (size_t i=0; i < In.number.size(); i++)
        number.push_back(In.number[i]);
    return *this;
}

bool integers::operator <(const integers& In)const
{
    int a=number.size();
    int b=In.number.size();
    if (a>b)
        return false;
    else if (a<b)
        return true;
    else
    {
        for (int i=a-1; i>=0; i--)
        {
            if (number[i]<In.number[i])
                return true;
        }
        return false;
    }
}

bool integers::operator ==(const integers& In)const
{
    int a=number.size();
    int b=In.number.size();
    if (a>b || a<b)
        return false;
    else
    {
        for (int i=a-1; i>=0; i--)
        {
            if (number[i]!=In.number[i])
                return false;
        }
        return true;
    }
}

bool integers::operator >(const integers& In)const
{
    int a=number.size();
    int b=In.number.size();
    if (a>b)
        return true;
    else if (a<b)
        return false;
    else
    {
        for (int i=a-1; i>=0; i--)
        {
            if (number[i]>In.number[i])
                return true;
        }
        return false;
    }
}


#endif // CLASS_INT_H_INCLUDED
