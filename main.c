#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char memory[30000] = {0};
int ptr = 0;

struct src_char{
    char chara;
    struct src_char *loopto;
    struct src_char *next;
    int index;
};

int isBrainfuckCharacter(char c){
    switch(c){
        case '>':
        case '<':
        case '+':
        case '-':
        case '.':
        case ',':
        case '[':
        case ']':
        case '\n':
        case ' ':
        case '\t':
            return 0;
        default:
            return -1;
    }
}

int readBrainfuckSource(char *fileName, struct src_char **head){
    FILE *fp;
    char cbuf;
    struct src_char **p;
    int index = 0;

    if((fp = fopen(fileName, "r")) == NULL){
        printf("%s cannnot open\n", fileName);
        return -1;
    }

    p = head;

    while((cbuf = fgetc(fp)) != EOF){

        // character exception
        if(isBrainfuckCharacter(cbuf) == -1){
            printf("%c is not a Brainf**k character.\n", cbuf);
            return -1;
        }
        if(cbuf == ' ' || cbuf == '\n' || cbuf == '\t')continue;

        *p = malloc(sizeof(struct src_char));
        (*p)->chara = cbuf;
        (*p)->loopto = NULL;
        (*p)->index = index++;
        p = &((*p)->next);
    }

    (*p) = NULL;

    return 0;
}

int addLoopInfo(struct src_char *head){
    static struct src_char *p;
    struct src_char *loopStart = NULL;
    struct src_char *loopEnd = NULL;

    p = head;

    while(p != NULL){
        if(p->chara == '['){
            loopStart = p;
            if(addLoopInfo(p->next) == -1){
                return -1;
            }
        }
        if(p->chara == ']'){
            loopEnd = p;
            if(loopStart == NULL){
                return 0;
            }
            loopStart->loopto = loopEnd;
            loopEnd->loopto = loopStart;
            loopStart = (loopEnd = NULL);
        }
        p = p->next;
    }

    if(loopStart != NULL || loopEnd != NULL){
        return -1;
    }

    return 0;
}

void free_char(struct src_char *p){
    if(p == NULL){
        return;
    }else{
        free_char(p->next);
        free(p);
    }
}

int main(int argc, char *argv[]){
    struct src_char *head, *pc;
    int opt;
    char debug_mode = 0;

    opterr = 0;

    while((opt = getopt(argc, argv, "d")) != -1){
        switch(opt){
            case 'd':
                debug_mode = 1;
                break;
            default:
                break;
        }
    }

    if(argc == 1 + optind){
        if(readBrainfuckSource(argv[optind], &head) == -1){
            return -1;
        }
        if(head == NULL){
            printf("%s is empty file.\n", argv[0]);
        }
    }else{
        printf("Usage: %s source_file_name\n", argv[0]);
        return -1;
    }

    pc = head;

    addLoopInfo(head);

    while(pc != NULL){
        switch(pc->chara){
            case '+':
                memory[ptr]++;
                if(debug_mode)printf("mem[%d]++ -> %d\n", ptr, memory[ptr]);
                break;
            case '-':
                memory[ptr]--;
                if(debug_mode)printf("mem[%d]-- -> %d\n", ptr, memory[ptr]);
                break;
            case '>':
                ptr++;
                if(debug_mode)printf("ptr++ -> %d\n", ptr);
                break;
            case '<':
                ptr--;
                if(debug_mode)printf("ptr-- -> %d\n", ptr);
                break;
            case '.':
                if(debug_mode)printf("mem[%d] OUTPUT:\n", ptr);
                putchar(memory[ptr]);
                break;
            case ',':
                if(debug_mode)puts("INPUT:");
                memory[ptr] = (char)getchar();
                break;
            case '[':
                if(memory[ptr] == 0){
                    if(debug_mode){
                        if(pc->loopto != NULL)printf("pc(%d) goto %d\n", pc->index, pc->loopto->index);
                    }
                    pc = pc->loopto;
                    if(pc == NULL){
                        printf("loop error.\n");
                        return -1;
                    }
                }
                break;
            case ']':
                if(memory[ptr] != 0){
                    if(debug_mode){
                        if(pc->loopto != NULL)printf("pc(%d) backto %d\n", pc->index, pc->loopto->index);
                    }
                    pc = pc->loopto;
                    if(pc == NULL){
                        printf("loop error.\n");
                        return -1;
                    }
                }
                break;
            default:
                break;
        }

        pc = pc->next;
    }

    free_char(head);
    return 0;
}

