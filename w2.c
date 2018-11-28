#include "types.h"
#include "stat.h"
#include "user.h"

#include "fcntl.h" /* for O_RDONLY */

#define N 4

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

            if (j % 20 == 0 && j < 10000) {

                if (write(1, buf, n) != n) {
                    printf(2, "write error\n");
                }
            }

        }
        close(fdesc);

        printf(1, "\n------------------------------\n");
    }
    exit();

}