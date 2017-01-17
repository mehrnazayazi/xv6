#include <stdio.h>
#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include <sys/types.h>
#include <unistd.h>
#include "types.h"
#include "stat.h"
#include "user.h"


void printer() {

    printf(1, "\n");
    for (int j=0;j<1000;j++){
        printf(2, "Child %d prints for the %d time.\n",getpid(),j);
    }

main(){

    pid_t pid;

    for(int i=0;i<10;i++){
        pid = fork();
        if(pid == 0){
            printer();
            exit();
        }

    }
    while(wait() > 0){
        printf(2, "PID : %d - Wait time : %d - Running time : %d - Turn Around time : %d.\n", getyear(&wTime[i], &rTime[i]), wTime[i], rTime[i], rTime[i] + wTime[i]);
    }


}
