#include <iostream>
#include <map>

using namespace std;
int main2()
{
    map<const char,int> m;
    m['a']=1;
    m['b']=6;
//    m.insert(pair<const char,int>('c',9));
    m.insert(pair<const char,int>('d',8));
    pair<int,int> p=make_pair(1,1);
//    m['c']=9;
    map<const char,int>::iterator it;
    it=m.begin();
    const char c =it->first;
    cout<<"first element is :"<<c<<endl;
    int i = m['c'];
    while(it!=m.end()){
        cout << it->first<<";"<<it->second<<endl;
        ++it;
    }
    cout<<m.lower_bound('c')->second;
//    cout <<"m[\"c\"]="<<i<<endl;
//    cout <<"sizeof m:"<<m.size()<<endl;
//    cout <<"erase m[\"c\"](1:succ 0:failed):"<<m.erase('c')<<endl;
//    cout <<"erase m[\"c\"]:"<<m.erase('c')<<endl;
//    cout <<"sizeof m:"<<m.size()<<endl;
//    cout<<"m[c]="<<m['c']<<endl;
//    cout<<"sizeof m :"<<m.size()<<endl;

    return 0;

}
