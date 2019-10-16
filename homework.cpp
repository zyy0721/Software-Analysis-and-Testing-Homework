#include<iostream>
using namespace std;

int main()
{
    int x,y,z;
    int* ptr;
    x = 10;
    y = x;
    ptr = &x;
    if( y < 1)
    {
        z = 1;
    }
    else
    {
        z = 2;
        ptr = &y;
        *ptr = 16;
        y = *ptr;
    }
    while (x != 1)
    {
       y += x;
       x--;
    }
    
    return 0;
}