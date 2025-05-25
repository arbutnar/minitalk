#define _POSIX_C_SOURCE 200809L

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sched.h>
#include <time.h>

volatile sig_atomic_t ack_received = 0;

void    handle_ack() {
    ack_received = 1;
}

void    microsleep(int microseconds) {
    struct timespec ts;
    ts.tv_sec = microseconds / 1000000;
    ts.tv_nsec = microseconds * 100;
    nanosleep(&ts, NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3)
        return (1);

    char                *str = argv[1];
    pid_t               server_pid = atoi(argv[2]);
    struct sigaction    sa = { 0 };

    sa.sa_handler = &handle_ack;
    sigaction(SIGUSR1, &sa, NULL);

    for (size_t i = 0; i < strlen(str); i++) {
        unsigned char byte = str[i];
        for (int p = 7; p >= 0; p--) {
            ack_received = 0;
            int bit = (byte >> p) & 1;
            kill(server_pid, (bit == 1) ? SIGUSR2 : SIGUSR1);
            microsleep(100);

            // Reliable wait
            while (!ack_received)
                pause();
        }
    }
    for (int i = 0; i < 8; i++) {
        ack_received = 0;
        kill(server_pid, SIGUSR1);
        while (!ack_received)
            pause();
    }

    return (0);
}
