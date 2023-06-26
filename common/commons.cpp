#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include "commons.h"

void isNull(char *s) {
    if(s == NULL) {
        custom_exit("NULL token");
    }
}

void custom_exit(char const *message, int error_code) {

    perror(message);
    exit(error_code);
}

int my_read(int fd, void *buf, int size) {

    int total, curr;
    total = 0;
    while (total < size) {
        curr = read(fd, (char*) buf + total, size - total);
        if (curr < 0) {
            custom_exit("Failure in my_read");
        }
        total += curr;
    }
    return total;
}


/* Grafei size bytes apo to fd */
int my_write(int fd, void *buf, int size) {

    int total, curr;
    total = 0;
    while(total < size) {
        curr = write(fd, (char*)buf + total, size - total);
        if (curr < 0){
            custom_exit("Failure in my_write");
        }
        total += curr;
    }
    return total;
}