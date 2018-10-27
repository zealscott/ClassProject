#include <iostream>
#include <fstream>
#include "Trie.h"



void readTxt(string file,Trie* root)
{
    ifstream infile;
    infile.open(file.data());   //将文件流对象与文件连接起来
    assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行

    string s;
    while(getline(infile,s))
    {
        root->insert(s.substr(0,s.length()-1));
    }
    infile.close();             //关闭文件输入流
}




void parseChinese(Trie* root,string str){
    unsigned long len = str.length(),pos,size = CHARSIZE;


    for (pos = 0; pos<len;)
    {
        if (pos + size > len){
            cout << str.substr(pos,size - CHARSIZE)<< endl;
            break;
        }
        string subStr = str.substr(pos,size);
//        cout<<subStr << endl;
        bool result = root->isExist(subStr);
        if (result){
            size+=CHARSIZE;
        }
        else{
//            cout <<"not found "<<endl;
            cout << str.substr(pos,size-CHARSIZE)<<" ";
            pos += (size-CHARSIZE);
            size = CHARSIZE;
        }
    }
}

int main(int argc,char* argv[]) {
    Trie* root = new Trie();
    readTxt("/Users/scott/Desktop/30wChinsesSeqDic.csv",root);

    for (int i = 1; i <argc;i++)
    {
        string tmp = argv[i];
        parseChinese(root,tmp);

    }



    return 0;
}