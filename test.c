#include <stdio.h>

struct a
{
   int data;
   int data1;
};

struct a func()
{
    struct a A = {.data = 10,.data1 = 100};
    return A;
}

int main(void)
{
   printf("data : %d, data1 : %d", func().data,func().data1);
}
