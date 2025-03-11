#include <stdio.h>

int main(int ac, char *av[]){
    printf("%d\n",ac);
    for(int i = 1 ; i < ac ; i ++) {
        printf("%s\n",av[i]);
    }
}