#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int fds[2];
    char tab[BUFSIZ];
    int fd, n;

    char *myfifo = "/tmp/server";
    char *myfifo2 = "/tmp/client";

    pipe(fds);
    mkfifo(myfifo,0666);

    while(1)
    {
        fds[0]=open(myfifo2,O_RDONLY);
        fds[1]=open(myfifo,O_WRONLY);

        read(fds[0],tab,BUFSIZ);

        if (strcmp("klient",tab)==0) {
            printf("Od klienta: %s\n",tab);
            fd=open(tab,O_WRONLY);

            if(fork()==0)
            {
                dup2(fds[1],1);
                close(fds[1]);
                execlp("ls","ls","-l",NULL);
                close(fds[0]);
                close(fds[1]);
            }
            else
            {
                dup2(fds[0],0);
                n = read(fds[0],tab,BUFSIZ);
                write(fd,tab,n);
                close(fds[0]);
                close(fds[1]);
            }
        }
        memset(tab, 0, sizeof(tab));
        close(fd);
        close(fds[0]);
        close(fds[1]);
    }

    unlink(myfifo);
    return 0;
}