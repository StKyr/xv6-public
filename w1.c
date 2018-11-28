
#include "types.h"
#include "stat.h"
#include "user.h"

#define N 20000000000


void conditionalExit(int x){
    if (x > 100){
        exit();
    }else{
        conditionalExit(x+1);
    }
}

int main(int argc, char *argv[]){

    unsigned long long i=0;
    int s=0;

    //long long N = atoi(argv[1]);

    int k;

    for (i=0; i<N; i++){
        s += i;
    }

    k = s-1;

    conditionalExit(k);
}