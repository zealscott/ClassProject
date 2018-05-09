#include<iostream>
#include<vector>
using namespace std;

void print_permutation(int n, int *p, vector<int> &seq, int cur)
{
    if(cur==n)
    {
        for(int z=0; z<n; z++)
            cout<<seq[z]<<" ";
        cout<<endl;
    }
    else
    {
        for(int z=0; z<n; z++)
        {
            if(!z||p[z]!=p[z-1])
            { //保证只有不相同元素才执行
                int c1=0, c2=0;
                for(int zz=0; zz<cur; zz++) if (seq[zz]==p[z]) c1++;
                for(int zz=0; zz<n; zz++) if(p[zz]==p[z]) c2++;
                if(c1<c2)
                {
                    cout<<"z = "<<z<<endl;
                    cout<<"c1 = "<<c1<<endl;
                    cout<<"c2 = "<<c2<<endl;
                    cout<<"push "<<p[z]<<endl;
                    seq.push_back(p[z]);
                    print_permutation(n,p,seq,cur+1);
                    seq.pop_back();
                }
            }
        }
    }

}
int main()
{
    int p[]= {1,2,3,3,5};
    int n=sizeof(p)/sizeof(int);
    vector<int> seq;
    print_permutation(n,p,seq,0);
    return 0;
}
