#include <stdio.h>

int main(int argc, char *argv[]) {
    int i;
    for (i = 0; i < argc; i++) {
        char *arg = argv[i];
        if (arg[0] == '-') {
            printf("%s\n", arg);
        }
    }
    return 0;
}
