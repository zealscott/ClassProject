#include <string>
#include <vector>
#include <iostream>
#include <set>
#include "puzzle.h"

using namespace std;

matrix::matrix()
{
    string temp;
    string temp2;
    cur=0;
    //-------------------//
    node start=new Node;
    node finish=new Node;
    //-------------------//
    cin >> temp;
    //-------------------//
    start->pre=finish->pre=-1;
    start->status=temp;
    start->step=finish->step=0;
    temp2="123456780";
    finish->status=temp2;
    //-------------------//
    select1.push_back(start);
    select2.push_back(finish);
    //-------------------//
    SetString1.insert(temp);
    SetString2.insert(temp2);
    //-------------------//
    if (!judge(temp))
    {
        cout<<"cannot finish!"<<endl;
        return;
    }
    process();
}

void matrix::process()
{
    string target="123456780";
    int ans=(target==select1[0]->status?1:0);
    while (!ans)
    {
        if (BFS(select2,2,SetString2))
        {
            ans=2;
            break;
        }
        if (BFS(select1,1,SetString1))
        {
            ans=1;
            break;
        }
        cur++;
    }
    printResult(ans);
}

int matrix::judge(string start)
{
    int arr[8];
    int j=0;
    int cunt=0;
    for (int i=0; i<9; i++)
    {
        int temp=start[i]-'0';
        if (temp==0) continue;
        else
            arr[j++]=temp;
    }
    //计算逆序对，是否能够有出路
    for (int i=0; i<7; i++)
    {
        for (int j=i+1; j<8; j++)
        {
            if (arr[i]>arr[j])
            {
                cunt+=1;
            }
        }
    }
    if (cunt%2==0) return 1;
    else return 0;
}

int matrix::BFS(vector<node> &select,int N,set<string>& st)
{
    string now=select[cur]->status;
    string temp;
    node next;
    int pos=findZreo(now);  //以0位表示第一个位置
    //-------------------//
    if (pos-3>=0)             //up
    {
        temp=swep(now,pos,pos-3);
        if (ExistIn(N,temp,select)==1)
        {
            return N;
        }
    }
    //-------------------//
    if (pos+3<=8)              //down
    {
        temp=swep(now,pos,pos+3);
        if (ExistIn(N,temp,select)==1)
        {
            return N;
        }
    }
    //-------------------//
    if (pos%3!=0)              //left
    {
        temp=swep(now,pos,pos-1);
        if (ExistIn(N,temp,select)==1)
        {
            return N;
        }
    }
    //-------------------//
    if (pos%3!=2)              //right
    {
        temp=swep(now,pos,pos+1);
        if (ExistIn(N,temp,select)==1)
        {
            return N;
        }
    }
    return 0;
}

int matrix::ExistIn(int n,string now,vector<node> &select)
{
    int flag=0;
    node next;
    set<string>::iterator it;
    if (n==1)
    {
        it=SetString1.find(now);
        flag=(it==SetString1.end());
        if (!flag)
        {
            return -1;
        }
        else
        {
            next=new Node;
            next->status=now;
            next->step=select[cur]->step+1;
            next->pre=cur;
            select.push_back(next);
            SetString1.insert(now);

            it=SetString2.find(now);
            flag=(it==SetString2.end());
            if (flag)
            {
                return 0;          //为新点
            }
            else
            {
                return 1;          //相交点
            }
        }
    }
    else
    {
        it=SetString2.find(now);
        flag=(it==SetString2.end());
        if (!flag)
        {
            return -1;
        }
        else
        {
            next=new Node;
            next->status=now;
            next->step=select[cur]->step+1;
            next->pre=cur;
            select.push_back(next);
            SetString2.insert(now);

            it=SetString1.find(now);
            flag=(it==SetString1.end());
            if (flag)
            {
                return 0;          //为新点
            }
            else
            {
                return 1;          //相交点
            }
        }
    }
}

void matrix::printString(string now)
{
    for (int j=0; j<9; j++)
    {
        cout<<now[j]<<" ";
        if (j%3==2)
        {
            cout<<endl;
        }
    }
    cout<<endl;
}

int matrix::findZreo(string now)
{
    for (size_t i=0; i!=now.size(); i++)
    {
        if (now[i]=='0')
        {
            return i;
        }
    }
    return -1;
}

string matrix::swep(string now,int i,int j)
{
    string temp=now;
    char a=now[i];
    char b=now[j];
    temp.replace(i,1,1,b);
    temp.replace(j,1,1,a);
    return temp;
}

void matrix::printResult(int ans)
{
    vector<node>result;     //store select1
    node temp;
    string occur;
    int index;

    if (ans==1)
    {
        occur=select1[select1.size()-1]->status;
        //-------------------//
        vector<node>::iterator iter=select1.end()-1;
        index=(*iter)->pre;
        result.push_back((*iter));
        while (index!=-1)
        {
            temp=select1[index];
            result.push_back(temp);
            index=select1[index]->pre;
        }
        //-------------------//
        for (int i=result.size()-1; i>=0; i--)
        {
            printString(result[i]->status);
        }
        //-------------------//
        for (size_t i=0; i<select2.size(); i++)
        {
            if (select2[i]->status==occur)
            {
                index=select2[i]->pre;
                if (index!=-1)
                {
                    printString(select2[index]->status);
                    index=select2[index]->pre;
                }
                break;
            }
        }
    }
    else
    {
        occur=select2[select2.size()-1]->status;
        //-------------------//
        for (size_t i=0; i<select1.size(); i++)
        {
            if (select1[i]->status==occur)
            {
                index=i;
                while (index!=-1)
                {
                    result.push_back(select1[index]);
                    index=select1[index]->pre;
                }
                break;
            }
        }
        //-------------------//
        for (int i=result.size()-1; i>=0; i--)
        {
            printString(result[i]->status);
        }
        //-------------------//
        index=select2[select2.size()-1]->pre;
        while (index!=-1)
        {
            printString(select2[index]->status);
            index=select2[index]->pre;
        }
    }
}

