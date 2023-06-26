#pragma once

void isNull(char *);
void custom_exit(char const *message, int error_code = 1);
int my_read(int fd, void *buf, int size);
int my_write(int fd, void *buf, int size);