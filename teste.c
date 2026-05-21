#include "keyboard.h"
#include "screen.h"
#include <stdio.h>

int main() {
    screenInit(1);
    keyboardInit();
    int ch = 0;
    while (ch != 10) {
        if (keyhit()) {
            ch = readch();
            screenGotoxy(0, 0);
            printf("tecla: %d   ", ch);
            screenUpdate();
        }
    }
    keyboardDestroy();
    screenDestroy();
    return 0;
}
