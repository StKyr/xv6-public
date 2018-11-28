



#include "types.h"
#include "stat.h"
#include "user.h"

#include "fcntl.h" /* for O_RDONLY */

#define N 10
#define K 100



#define SMALLEST_PRINTABLE 0x20
#define LARGEST_PRINTABLE 0x7e


void exncryptCeasar(char *text, int size, int key){

    int i;
    for (i=0; i< size; i++){

        text[i] = (text[i] + key) % (LARGEST_PRINTABLE - SMALLEST_PRINTABLE) + LARGEST_PRINTABLE;
    }

}

void doStuffInBuff(char *buff, int size){

    int i;
    for (i=0; i<K; i++){
        exncryptCeasar( buff, size, +3 );
    }

}

int main(void){

    char * inputFile = "myfile.txt";



    char buf[1];
    int n;

    int i;
    for (i=0; i<N; i++){

        int fdesc = open(inputFile, O_RDONLY);

        int j = 0;
        while( (n=read(fdesc, buf, sizeof buf)) != 0 ){

            if(n < 0){
                printf(2, "read error\n");
                exit();
            }

            j++;

            doStuffInBuff(buf, n);

            if (j < 20) {
                if (write(1, buf, n) != n) {
                    printf(2, "write error\n");
                    exit();
                }

            }

        }
        close(fdesc);
    }
    exit();

}