#include<iostream>
using namespace std;

int* test()
{
    int x,y,z;
    int* ptr;
    x = 10;
    ptr = &x;
    if( z < 1)
    {
        x = 11;
        y = 11;
    }
    else
    {
        x = 22;
        y = 22;
    }
    x = x + 33;
    y = y + 33;
    
    return ptr;
}