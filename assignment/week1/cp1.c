#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void oops(char *, char *);

int main(int ac, char *av[]) {
    int in_fd, out_fd, n_chars;
    char buf[BUFFERSIZE];
    char *source, *destination;
	int mode = 0;
    /* check args */
    if (ac < 3 || ac > 4) { 
        fprintf(stderr, "usage: %s source destination\n", *av);
        exit(1);
    }

    // -i 옵션 확인
    // av = { ac , -i , soure , destination }
    if (ac == 4 && strcmp(av[1], "-i") == 0) {
        mode = 1;
        source = av[2];
        destination = av[3];
    } else { // av = { ac , source , destination }
        source = av[1];
        destination = av[2];
    }

    // 같은 파일인지 확인
    if (strcmp(source, destination) == 0) {
        fprintf(stderr, "%s: '%s' 와 '%s' 는 같은 파일\n", av[0], source, destination);
        exit(1);
    }

    /* open source file */
    if ((in_fd = open(source, O_RDONLY)) == -1)
        oops("Cannot open", source);

    //-i 옵션이 있는 경우 
    if (mode == 1 && access(destination, F_OK) == 0 ) {  
        char type;
        printf("%s: '%s'를 덮어쓸까요 (y/n)? ", av[0], destination);
        scanf(" %c", &type);
        if (type != 'y') {
            close(in_fd);
            return 0;  // 사용자 취소 시 종료
        }
    }

    /* create destination file */
    if ((out_fd = creat(destination, COPYMODE)) == -1)
        oops("Cannot create", destination);

    /* copy files */
    while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
        if (write(out_fd, buf, n_chars) != n_chars)
            oops("Write error to", destination);

    if (n_chars == -1)
        oops("Read error from", source);

    /* close files */
    if (close(in_fd) == -1 || close(out_fd) == -1)
        oops("Error closing files", "");

    return 0;
}

void oops(char *s1, char *s2) {
    fprintf(stderr, "Error: %s ", s1);
    perror(s2);
    exit(1);
}
