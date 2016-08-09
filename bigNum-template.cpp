#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <string>
using namespace std;
#define MAX 9999
#define MAXSIZE 10
#define DLEN 4

class BigNum
{
public:
    int a[500];    
    int len;       

    BigNum(){ len = 1;memset(a,0,sizeof(a)); }
    BigNum(const int);
    BigNum(const char*);
    BigNum(const BigNum &);
    BigNum &operator=(const BigNum &);


    BigNum operator+(const BigNum &) const;   
    BigNum operator-(const BigNum &) const;   
    BigNum operator*(const BigNum &) const;   
    BigNum operator/(const int   &) const;    

    BigNum operator^(const int  &) const;    
    int    operator%(const int  &) const;    
    bool   operator>(const BigNum & T)const;   
    bool   operator>(const int & t)const;      

    void print();       
};
BigNum::BigNum(const int b)     
{
    int c,d = b;
    len = 0;
    memset(a,0,sizeof(a));
    while(d > MAX)
    {
        c = d - (d / (MAX + 1)) * (MAX + 1);
        d = d / (MAX + 1);
        a[len++] = c;
    }
    a[len++] = d;
}
BigNum::BigNum(const char*s)     
{
    int t,k,index,l,i;
    memset(a,0,sizeof(a));
    l=strlen(s);
    len=l/DLEN;
    if(l%DLEN)
        len++;
    index=0;
    for(i=l-1;i>=0;i-=DLEN)
    {
        t=0;
        k=i-DLEN+1;
        if(k<0)
            k=0;
        for(int j=k;j<=i;j++)
            t=t*10+s[j]-'0';
        a[index++]=t;
    }
}
BigNum::BigNum(const BigNum & T) : len(T.len)  
{
    int i;
    memset(a,0,sizeof(a));
    for(i = 0 ; i < len ; i++)
        a[i] = T.a[i];
}
BigNum & BigNum::operator=(const BigNum & n)   
{
    int i;
    len = n.len;
    memset(a,0,sizeof(a));
    for(i = 0 ; i < len ; i++)
        a[i] = n.a[i];
    return *this;
}

BigNum BigNum::operator+(const BigNum & T) const   
{
    BigNum t(*this);
    int i,big;      //位数
    big = T.len > len ? T.len : len;
    for(i = 0 ; i < big ; i++)
    {
        t.a[i] +=T.a[i];
        if(t.a[i] > MAX)
        {
            t.a[i + 1]++;
            t.a[i] -=MAX+1;
        }
    }
    if(t.a[big] != 0)
        t.len = big + 1;
    else
        t.len = big;
    return t;
}
BigNum BigNum::operator-(const BigNum & T) const   
{
    int i,j,big;
    bool flag;
    BigNum t1,t2;
    if(*this>T)
    {
        t1=*this;
        t2=T;
        flag=0;
    }
    else
    {
        t1=T;
        t2=*this;
        flag=1;
    }
    big=t1.len;
    for(i = 0 ; i < big ; i++)
    {
        if(t1.a[i] < t2.a[i])
        {
            j = i + 1;
            while(t1.a[j] == 0)
                j++;
            t1.a[j--]--;
            while(j > i)
                t1.a[j--] += MAX;
            t1.a[i] += MAX + 1 - t2.a[i];
        }
        else
            t1.a[i] -= t2.a[i];
    }
    t1.len = big;
    while(t1.a[t1.len - 1] == 0 && t1.len > 1)
    {
        t1.len--;
        big--;
    }
    if(flag)
        t1.a[big-1]=0-t1.a[big-1];
    return t1;
}

BigNum BigNum::operator*(const BigNum & T) const   
{
    BigNum ret;
    int i,j,up;
    int temp,temp1;
    for(i = 0 ; i < len ; i++)
    {
        up = 0;
        for(j = 0 ; j < T.len ; j++)
        {
            temp = a[i] * T.a[j] + ret.a[i + j] + up;
            if(temp > MAX)
            {
                temp1 = temp - temp / (MAX + 1) * (MAX + 1);
                up = temp / (MAX + 1);
                ret.a[i + j] = temp1;
            }
            else
            {
                up = 0;
                ret.a[i + j] = temp;
            }
        }
        if(up != 0)
            ret.a[i + j] = up;
    }
    ret.len = i + j;
    while(ret.a[ret.len - 1] == 0 && ret.len > 1)
        ret.len--;
    return ret;
}
BigNum BigNum::operator/(const int & b) const   
{
    BigNum ret;
    int i,down = 0;
    for(i = len - 1 ; i >= 0 ; i--)
    {
        ret.a[i] = (a[i] + down * (MAX + 1)) / b;
        down = a[i] + down * (MAX + 1) - ret.a[i] * b;
    }
    ret.len = len;
    while(ret.a[ret.len - 1] == 0 && ret.len > 1)
        ret.len--;
    return ret;
}
int BigNum::operator %(const int & b) const    
{
    int i,d=0;
    for (i = len-1; i>=0; i--)
    {
        d = ((d * (MAX+1))% b + a[i])% b;
    }
    return d;
}
BigNum BigNum::operator^(const int & n) const    
{
    BigNum t,ret(1);
    int i;
    if(n<0)
        exit(-1);
    if(n==0)
        return 1;
    if(n==1)
        return *this;
    int m=n;
    while(m>1)
    {
        t=*this;
        for( i=1;i<<1<=m;i<<=1)
        {
            t=t*t;
        }
        m-=i;
        ret=ret*t;
        if(m==1)
            ret=ret*(*this);
    }
    return ret;
}

bool BigNum::operator>(const BigNum & T) const   
{
    int ln;
    if(len > T.len)
        return true;
    else if(len == T.len)
    {
        ln = len - 1;
        while(a[ln] == T.a[ln] && ln >= 0)
            ln--;
        if(ln >= 0 && a[ln] > T.a[ln])
            return true;
        else
            return false;
    }
    else
        return false;
}
bool BigNum::operator >(const int & t) const    
{
    BigNum b(t);
    return *this>b;
}

void BigNum::print()    
{
    int i;
    cout << a[len - 1];
    for(i = len - 2 ; i >= 0 ; i--)
    {
        cout.width(DLEN);
        cout.fill('0');
        cout << a[i];
    }
    cout << endl;
}
 
BigNum ans[250];
char str[250];

int main()
{

    int n;
    while(scanf("%s", str) != EOF)
    {
        BigNum ret = BigNum(str);
        BigNum four = BigNum(4);
        BigNum one = BigNum(1);
        BigNum plus_one = BigNum(-1);
        BigNum two = BigNum(2);
                
        BigNum tmp = ret * ret;
        ret  = ret^4;
        tmp.print();
        ret.print();
    }
    return 0;
}
