#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

//     CLIENTE

int main(int argc, char *argv[])
{

    int fd;
    int fd2;
    char *args[10];
    int sizeArgs = 0;
    int i;
    int size;
    char *arguments[10];
    char str[100];
    int argum = argc - 2;

    if (argc == 1)
    { //     Show usage protocol
        write(1, "./aurras transform input-filename output-filename filter id-1 filter id-2 ...\n", 79);
        return 1;
    }

    if ((strcmp(argv[1], "status") == 0) && argc == 2)
    {
        int a = 1;
        int b = 7;
        char c[7] = "status";
        int arrR[5];
        int sz;
        int pid;

        fd = open("fifoC_S", O_WRONLY);
        write(fd, &a, sizeof(int));
        write(fd, &b, sizeof(int));
        write(fd, c, sizeof(c));
        fd2 = open("fifoS_C", O_RDONLY);
        char task[sz];
        read(fd2, task, 100);
        printf("Task: transform %s\n", task);
        for (size_t q = 0; q < 5; q++)
        {
            read(fd2, &arrR[q], sizeof(int));
            if (q == 0)
            {
                printf("Filtro Alto: %d disponiveis\n", arrR[q]);
            }
            if (q == 1)
            {
                printf("Filtro Baixo:%d disponiveis\n", arrR[q]);
            }
            if (q == 2)
            {
                printf("Filtro Eco:%d disponiveis\n", arrR[q]);
            }
            if (q == 3)
            {
                printf("Filtro Rapido:%d disponiveis\n", arrR[q]);
            }
            if (q == 4)
            {
                printf("Filtro Lento:%d disponiveis\n", arrR[q]);
            }
        }

        read(fd2, &pid, sizeof(int));
        printf("Pid:%d\n", pid);
    }

    if (strcmp(argv[1], "transform") == 0)
    {
        if (argc < 3)
        {
            perror("Command invalid");
            return 1;
        }
        fd = open("fifoC_S", O_WRONLY);
        for (i = 0; i < argum; i++)
        {
            arguments[i] = argv[i + 2];
            strcat(str, arguments[i]);
            strcat(str, " ");
            size = size + strlen(arguments[i]) + 1;
        }
        write(fd, &argum, sizeof(int));
        write(fd, &size, sizeof(int));
        write(fd, str, sizeof(str));
    }

    return 0;
}
