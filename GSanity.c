#include "types.h"
#include "stat.h"
#include "user.h"

void print() {
    int k;
    printf(1, "\n");
    for (k=0;k<50;k++){
        printf(2, "farayand %d baraye bare  %d chap mishavad.\n", getpid(), k);
    }
}

void mygsanity(void) {
    printf(2, "Father pid is %d.\n", getpid());
    sleep(200);
    int Id;
    Id = fork();
    if(Id == 0) {
        print();
        exit();
    }
    print();
}

int main(void) {
    mygsanity();
    exit();
}
