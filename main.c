#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#define ERROR_MESS 24
#define HALF_SECOND 500000


void handler(){
    static int num = 0;
    if (num++%2){
        write(STDOUT_FILENO, "Tock\n", 5);
    } else{
        write(STDOUT_FILENO, "Tick...", 7);
    }
}
int main(int argc, char *argv[]) {
    // FILE *file = fopen("Make");
    struct sigaction sa;

    struct itimerval it;
    int num;
    char *end;
    if (argc == 2){
        num = strtol(argv[1], &end, 10) * 2;
        if (*end != '\0'){
            printf("%s", end);
            write(STDOUT_FILENO, "usage: timeit <seconds>\n", ERROR_MESS);
            exit(0);
        }
    } else{
        write(STDOUT_FILENO, "usage: timeit <seconds>\n", ERROR_MESS);
        exit(0);
    }
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, NULL);
    it.it_interval.tv_sec = 0;
    it.it_interval.tv_usec = HALF_SECOND;
    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = HALF_SECOND;
    setitimer(ITIMER_REAL, &it, NULL);

    while (num--){
        sigsuspend(&sa.sa_mask);
    }
    write(STDOUT_FILENO, "Time's up!\n", 11);

    return 0;
}
