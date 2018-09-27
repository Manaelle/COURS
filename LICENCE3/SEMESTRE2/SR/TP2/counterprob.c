#include "csapp.h"
int counter = 0;
void handler(int sig)
{
    counter++;
    sleep(1);
    return;
}

int main()
{
    int i, temps;
    Signal(SIGUSR2, handler);

    if (Fork() == 0) {  /* child */
        for (i = 0; i < 5; i++) {
            Kill(getppid(), SIGUSR2);
            temps = sleep(2);
            printf("sent SIGUSR2 to parent\n");
        }
        exit(0);
    }

    Wait(NULL);
    printf("counter=%d\n", counter);
    exit(0);
}
