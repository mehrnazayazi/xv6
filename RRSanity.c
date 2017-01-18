#include "types.h"
#include "stat.h"
#include "user.h"

void print() {
    int k;
    //int sizeOfArray = 100;
    printf(1, "\n");
    for (k=0;k<1000;k++){
        printf(2, "Child %d baraye bare %d chap mishavad.\n",getpid(),k);
    }
}

void test(void) {
    //int numberOfForks = 10;
    int wTime[10];
    int rTime[10];
    int Id;

    int i;
    for (i=0;i<10;i++){
        Id = fork();
        if(Id == 0) {
            print();
            exit();
        }
    }
    while(wait() > 0){
            if(i<10)
        printf(2, "PID : %d -run time : %d - turn around time : %d - waiting time : %d.\n",getyear(&wTime[i],&rTime[i]),rTime[i],rTime[i]+wTime[i],wTime[i]);
    }
}

int main(void) {
    test();
    exit();
}
