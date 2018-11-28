#include "types.h"
#include "stat.h"
#include "user.h"

#include "fcntl.h" /* for O_RDONLY */

#define N 1000

int main(void){
    
    char * inputFile = "myfile.txt";

    

    char buf[10000];
    int n;

    int i;

    int timesWrote = 0;

    for (i=0; i<N; i++){
    
        int fdesc = open(inputFile, O_RDONLY);

        while( (n=read(fdesc, buf, sizeof buf)) != 0 ){
            
            if(n < 0){
                printf(2, "read error\n");
                exit();
            }

            if (timesWrote < 3) {
                if (write(1, buf, n) != n) {
                    printf(2, "write error\n");
                    exit();
                }
                timesWrote++;
            }

        }
        close(fdesc);
    }
    exit();

}