
/**
 * #include <sys/mman.h>

       void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
            -参数：
                addr: NULL
                length:建议写文件长度
                    获取文件长度: stat lseek
                - int prot：对申请的内存映射区的操作权限
                    PROT_EXEC  Pages may be executed.
                    PROT_READ  Pages may be read.
                    PROT_WRITE Pages may be written.
                    PROT_NONE  Pages may not be accessed.
                    要操作映射内存，必须有读的权限

                - int flags：
                    MAP_SHARED：内存与磁盘文件同步
                    MAP_PRIVATE：不同步，内存区改变，不会对原来文件直接更改，会新建一个文件
                - int fd：需要映射文件对应的文件描述符
                - off_t offset：偏移量，一般不用。必须指定为4K的整数倍
            -返回值：成功：返回内存首地址
                    失败：返回-1指针

    int munmap(void *addr, size_t length);
        - 功能： 释放内存映射
*/

/**

        使用内存映射实现进程间通信:
        1.有关系的进程（父子进程)
            -还没有子进程的时候
            -通过唯一的父进程，先创建内存映射区-有了内存映射区以后，创建子进程
            -父子进程共享创建的内存映射区


        2.没有关系的进程间通信
            -准备一个大小不是0的磁盘文件
            -进程1通过磁盘文件创建内存映射区
                -得到一个操作这块内存的指针-
            -进程2通过磁盘文件创建内存映射区
                -得到一个操作这块内存的指针-使用内存映射区通信Ⅰ
            注意:内存映射区通信，是非阻塞。

*/

#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>
int main(int argc, char *argv[])
{
    // 1、打开文件
    int fd = open("test.txt", O_RDWR);
    if (fd == -1)
    {
        perror("open");
        exit(0);
    }
    int size = lseek(fd, 0, SEEK_END);
    // 2、创建内存映射区
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED){
        perror("MMP");
        exit(0);
    }

    // 3、创建子进程
    pid_t pid = fork();
    if(pid>0){
        wait(NULL);
        char buf[64];
        strcpy(buf, (char*)ptr);
        printf("read data : %s \n", buf);
    }else if(pid == 0){
        strcpy((char*)ptr, "hello, i am son!!!");        
    }
    munmap(ptr, size);
    close(fd);
    return 0;
}
