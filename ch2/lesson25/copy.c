#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    // 1、对原始文件进行内存映射
    int fd = open("english.txt", O_RDWR);
    if (fd == -1)
    {
        perror("open");
        exit(0);
    }
    int size = lseek(fd, 0, SEEK_END);
    // 2、创建文件，文件拓展
    int fd1 = open("cpy.txt", O_RDWR | O_CREAT, 0664);
    if (fd == -1)
    {
        perror("open");
        exit(0);
    }
    // 文件拓展
    truncate("cpy.txt", size);
    write(fd1, " ", 1);
    // 3、把新文件数据映射到内存中
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    void *ptr1 = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);
    if (ptr == MAP_FAILED)
    {
        perror("map");
        exit(0);
    }
    if (ptr1 == MAP_FAILED)
    {
        perror("map1");
        exit(0);
    }

    // 4、内存拷贝原始文件数据到新文件中
    memcpy(ptr1, ptr, size);
    // 5、释放资源
    munmap(ptr1, size);
    munmap(ptr, size);
    close (fd);
    close (fd1);
    return 0;
}
