
/*
    一秒计算机
*/

#include <unistd.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    alarm(1);
    int i = 1;
    while (1)
    {
        printf("%i\n", i++);
    }
    
    return 0;
}
