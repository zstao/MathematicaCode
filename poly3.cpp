#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

//删除字符串s中的d
void delStr(string& s, string d) {
    while(s.find(d) != string::npos) {
        s.erase(s.find(d), d.length());
    }
}

// 删除字符串中连续的等号
void delContinousEqual(string& s) {
    delStr(s, " "); // 删除空格
    int i = 0;
    int f = 0;
    while(i < s.length()) {
        if(s[i] == '=') {
            if(f == 1) {
                s[i] = ' ';
            }
            f = 1;
        }else {
            f = 0;
        }
        i++;
    }
    delStr(s, " "); // 删除空格
}

void delVar(string& poly, string& var) {
    int i = 0, left = 0, j = 0;
    while(i < poly.length()) {
        if(poly[i] == '=') {
            for(j = left; j <= i; j++) {
                // 如果匹配到var，则删除var所在的项
                if(poly[j] == var[0]) {
                    int f = 1;
                    for(int t = 1; t < var.length(); t++) {
                        if(poly[j + t] != var[t]) {
                            f = 0;
                            break;
                        }
                    }

                    if(f == 1) {
                        int k = j;
                        while(poly[k] != '=' && poly[k] != '+' && poly[k] != '-') {
                            poly[k] = ' ';
                            k++;
                        }
                        k = j;
                        while(k != left - 1) {
                            if(poly[k] == '+' || poly[k] == '-') {
                                poly[k] =  ' ';
                                break;
                            }
                            poly[k] = ' ';
                            k--;
                        }
                    }
                }
            }
            left = j;
        }
        i++;
    }
    delStr(poly, " ");  // 删除含有var的项后，再删除空格
    delContinousEqual(poly); // 删除可能含有的连续等号
}

// 找到形如 x^n = 0 的项
int findVar(string& poly, string& var) {
    cout<<poly<<endl<<endl;
    int i = 0, left = 0, j = 0;
    int numOfBracket = 0;
    while(i < poly.length()) {
        if(poly[i] == '[') {
            numOfBracket++;
            left = i;   // 记录左中括号的位置
        }
        if(poly[i] == '=') {
            if(numOfBracket == 1) {
                int commaNum = 0;
                for(j = 0; poly[left + j] != ']'; j++) {
                    if(poly[left + j] == ',') {
                        commaNum++;
                    }
                    if(commaNum == 3) {
                        break;
                    }
                    var += poly[left + j];
                }
                return 1;
            }
            numOfBracket = 0;
        }
        i++;
    }
    cout<<var<<endl;
    // 不存在，返回0
    return 0;
}

// 找到形如 xy = 0 的项
int findVar2(string& poly, string& item) {
    string var;  // 变量数组
    // 删除x^n = 0 的项
    while(findVar(poly, var)) {
        delVar(poly, var);
        cout<<"单项： %s]=0"<<var<<endl;
    }

    int i = 0, j = 0, left = 0;  // xy=0项的左侧
    int numOfPlusSub = 0;  // +,-的个数，去掉第一个后个数为零则为xy=0的项
    int value = 0;  // 函数返回值

    while(i < poly.length()) {
        if((poly[i] == '+' || poly[i] == '-') && i != 0 && poly[i - 1] != '=') {
            numOfPlusSub++;
        }

        if(poly[i] == '=' && poly[i - 1] != '=') { // 去掉==的情况
            if(numOfPlusSub == 0) {
                for(int k = 0; left + k <= i; k++) {
                    item += poly[left + k];
                    j++;
                }
                value = 1;
            }else {
                numOfPlusSub = 0;
            }
            left = i + 1;
        }
        i++;
    }

    // 去重
    for(int k = 0; k < item.length(); k++) {
        if(item[k] == '[') {
            int commaNum = 0, len = 0;
            for(; item[k + len] != ']'; len++) {
                if(item[k + len] == ',') {
                    commaNum++;
                }
                if(commaNum == 3) {
                    break;
                }
            }
            for(int t = k + 6; t < item.length(); t++) {
                if(item[t] == '['){
                    int f = 1;
                    for(int n = 1; n < len; n++) {
                        if(item[k + n] != item[t + n]) {
                            f = 0;
                            break;
                        }
                    }
                    if(f == 1) {
                        int m;
                        for(m = t; item[m] != ']'; m++) {
                            item[m] = ' ';
                        }
                        item[m] = ' ';
                    }
                }
            }
        }
    }
    delStr(item, " ");  // 删除空格
    return value;

}

int main()
{
    char polyFileName[] = "poly2.txt";  // 文件路径

    ifstream inputFile(polyFileName);

    string strLine, temp;

    if(!inputFile.is_open()) {
        printf("%s\n", "未打开文件");
    }

    while(getline(inputFile, temp)){
        strLine += temp + "\n";
    }

    // 预处理
    delStr(strLine, " ");
    delStr(strLine, "\n");
    delStr(strLine, "{");
    delStr(strLine, "=0,}");
    delStr(strLine, "=0,,");
    delStr(strLine, "Subscript");
    delStr(strLine, "Subsuperscript");

    cout<<strLine<<endl<<strLine.length()<<endl;

    string var;  // 变量字符串
    string item;
    if(findVar2(strLine, item)){
        cout<<"项: "<<item<<endl;
    }
    return 0;
}
