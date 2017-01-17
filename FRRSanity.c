#include "types.h"
#include "stat.h"
#include "user.h"

void print() {
    int k;
    for (k=0;k<1000;k++){
        printf(2, "Child %d baraye bare %d chap mishavad.\n", getpid(), k);
    }
}

void test(void) {

    int Id;

    int i;
    for (i=0;i<10;i++){
        Id = fork();
        if(Id == 0) {
            print();
            exit();
        }
    }
    wait();
}

int main(void) {
    test();
    exit();
}
