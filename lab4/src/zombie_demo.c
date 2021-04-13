#include <unistd.h>
#include <stdio.h>
 
int main(void) {
 
        int i;
        if (!fork()) {
                /* Потомок немедленно завершается*/
                _exit(0);
        }
    /* Родитель ждет около минуты*/
        sleep(60);
}