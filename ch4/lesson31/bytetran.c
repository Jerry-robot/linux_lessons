#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    // 先判断本机的字节序
    // unsigned short int num = 0x0506;
    // char num1 = num;
    // printf("%d\n", (int)num1);

    union
    {
        unsigned int data;
        char bytes[sizeof(int)];
    } test;
    // test.data = 0x0102;
    // if(test.bytes[0] == 1 && test.bytes[1] == 0){
    //     printf("大端\n");
    // }else printf(("小端\n"));



    // char buf[4] = {12,10, 168, 192};
    test.data = 192 * 256*256*256+168*256*256+10*256+12;
    printf("%d %d %d %d\n", test.bytes[0], test.bytes[1], test.bytes[2], test.bytes[3]);
    char buf[4] = {192, 168, 10, 12};
    unsigned int num = *(unsigned int *)buf;
    unsigned int r1_num = htonl(num);
    unsigned int r2_num = ntohl(num);
    unsigned char *p3 = (char*)&num;
    unsigned char *p1 = (char*)&r1_num;
    unsigned char *p2 = (char*)&r2_num;
    printf("%d %d %d %d\n", *p1, *(p1+1), *(p1+2), *(p1+3));
    printf("%d %d %d %d\n", *p2, *(p2+1), *(p2+2), *(p2+3));
    printf("%d %d %d %d\n", *p3, *(p3+1), *(p3+2), *(p3+3));





    return 0;
}


