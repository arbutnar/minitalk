Project aiming to create a simple form of inter-process communication using signals. In Unix-like and POSIX-compliant os, signals can be utilized to make a process asynchronously react to a sort of event.
Here this principle is utilized by mapping 0 to SIGUSR1 signal and 1 to SIGUSR2; effectively creating a way for a client to communicate the passed string to the server.
