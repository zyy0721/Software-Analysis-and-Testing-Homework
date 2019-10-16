#include<iostream>
using namespace std;

int main()
{
    int x,y,z;
    int* ptr;
    int** pptr;
    x = 10;
    y = x;
    ptr = &x;
    if( y < 1)
    {
        z = y + x;
        pptr = &ptr;
        *ptr = z;
    }
    else
    {
        y = *ptr;
        *pptr = ptr;
    }
    while (x != 1)
    {
       y += x;
       x--;
    }
    
    return 0;
}