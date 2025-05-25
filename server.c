#define _POSIX_C_SOURCE 200809L

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

void    handle_sig(int signum, siginfo_t *info, void *context) {
    (void)context;
    static int  i = 0, c = 0;
    int         bit = (signum == SIGUSR1) ? 0 : 1;

    /* TEST */
    static struct timeval start, end;
    static int started = 0, c_cnt = 0;
    if (!started) {
        gettimeofday(&start, NULL);
        started = 1;
    }
    /* END TEST */

    c = (c << 1) | bit;
    i++;

    kill(info->si_pid, SIGUSR1);

    if (i == 8) {
        write(1, &c, 1);
        c_cnt++;
        if (c == '\0') {
            /* TEST */
            gettimeofday(&end, NULL);
            long seconds = end.tv_sec - start.tv_sec;
            long microseconds = end.tv_usec - start.tv_usec;
            double elapsed = seconds + microseconds*1e-6;
            printf("\n\nRead: %d chars, time taken: %.6f seconds\n", c_cnt, elapsed);
            started = c_cnt = 0;
            /* END TEST */
        }
        i = c = 0;
    }
}

int main() {
    struct sigaction    sa = { 0 };
    sigset_t            block_alarm;

    sigemptyset(&block_alarm);
    sigaddset(&block_alarm, SIGUSR1);
    sigaddset(&block_alarm, SIGUSR2);

    sa.sa_mask = block_alarm;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = &handle_sig;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    printf("PID: %d\n", getpid());

    while(1) {
        pause();
    }

    return (0);
}