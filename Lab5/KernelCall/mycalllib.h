#include <lib.h>
#include <unistd.h>

//int mycall(int i){
//    message m;
//    m.m1_i1 = i;
//    return (_syscall(PM_PROC_NR, MYCALL, &m));
//}

int mycall(int deadline){
    message m;
    m.m1_i2 = deadline;
    printf("mycalllib.h %d\n", deadline);
    return (_syscall(PM_PROC_NR, MYCALL, &m));
}