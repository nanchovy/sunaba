#include <stdio.h>
#include <sys/mman.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MMAP_SIZE ((size_t)1024*1024*1024)

int main(int argc, char *argv[])
{
        int fd;
        void *p;

        if (argc != 2) {
                printf("dax_read_loop <file or device name>\n");
                return -1;
        }

        fd = open(argv[1], O_RDWR);
        if (fd == -1) {
                perror("open");
                return -1;
        }

        p = mmap(NULL, MMAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
        if (p == MAP_FAILED) {
                perror("mmap");
                return -1;
        }

        printf("mmapped p=%p\n", p);

        memcpy(p, "hogehoge\n", sizeof("hogehoge\n"));
        msync(p, 4096, MS_SYNC);
        for(;;);

}