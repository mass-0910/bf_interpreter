#include <stdio.h>
#include <stdlib.h>

struct src_char{
    char chara;
    struct src_char *next;
};

int readBrainfuckSource(char *fileName, struct src_char *head){
    
}

int main(int argc, char *argv[]){
    struct src_char *head, *pc;

    if(argc == 1){
        if(readBrainfuckSource(argv[1], head) == -1){
            return -1;
        }
    }else{
        printf("Usage: %s source_file_name\n", argv[0]);
        return -1;
    }

    while(pc->next != NULL){

    }
}

