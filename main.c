#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    struct winsize ws;
    int res = ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);

    printf("res %d\n", res);

    printf("lines %d\n", ws.ws_row);
    printf("columns %d\n", ws.ws_col);

    return 0;
}
