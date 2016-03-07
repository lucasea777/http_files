#include <string>
#include <stdio.h>
#include "httpget.h"
using std::string;

int main(int argc, char * argv[]) {
    if(argc!=4 && argc != 2) {
        printf("\nInvalid usage\n");        
        return 1;
    }
    printf("%s %d",argv[1], argc);    
    char test[5] = "test";
    if (argc == 2 && strncmp(argv[1],test,5) == 0) {
        //char host[20] = "www.w3schools.com";
        char host[40] = "www.lukspage.hostzi.com";
        //char path[20] = "/html/horse.mp3";
        char path[40] = "/notasssss/index.php";
        //char filename[40] = "horse.mp3";
        char filename[40] = "index.php";

        return httpget(host, path, filename);
    }
    return httpget(argv[1],argv[2],argv[3]);
}
