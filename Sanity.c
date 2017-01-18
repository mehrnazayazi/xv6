#include "types.h"
#include "stat.h"
#include "user.h"

void printLine() {
    int k;
    printf(1, "\n");
    for (k=0;k<500;k++){
        printf(2, "PID %d baraye bare %d chap mishavad.\n",getpid(),k);
    }
}

void sanitytest(void) {
    int wTime;
    int rTime;
    int pid;
    int Id;

    int i;
    for (i=0;i<30;i++){
        Id = fork();
        if(Id == 0) {
            pid = getpid();
            if(pid %3 == 0){
                nice();
            }
            else if(pid %3 == 1){
                nice();
                nice();
            }
            else{
            }
            printLine();
            exit();
        }
    }
    while(wait() > 0){
        printf(2, "PID : %d - Wait time : %d - Running time : %d - Turn Around time : %d.\n", getyear(&wTime, &rTime), wTime, rTime, rTime + wTime);
    }
}

int main(void) {
    sanitytest();
    exit();
}
