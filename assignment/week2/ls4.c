/** ls1.c
 ** purpose - list contents of directory or directories
 ** action - if no args, use . else list files in args
 **/
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 1024 

void do_ls(char[],int reverse);
int compare_filenames_reverse (const void *a, const void *b) ;
int compare_filenames(const void *a, const void *b) ;
int main (int ac, char *av[])
{
    int reverse = 0; // reverse == 1일때 역방향
    // -r모드일때
    if ( ac > 1 && strcmp(av[1],"-r") == 0) {
        reverse = 1;
        ac -- ;
        av ++ ;
    }
    // 일반 실행
    if( ac == 1)
        do_ls(".", reverse);
    else
		while( --ac ){
			printf("%s:\n", *++av );
			do_ls( *av, reverse );
		}
}

void do_ls( char dirname[], int reverse )
/*
 * list files in directory called dirname
 */
{
    DIR			*dir_ptr;	/* the directory */
    struct dirent	*direntp;	/* each entry	 */
    struct dirent *file_list[MAX_FILES];
    int count = 0;

    if( ( dir_ptr = opendir( dirname ) ) == NULL )
		fprintf(stderr, "ls1: cannot open %s\n", dirname);
    else
    {
		while( ( direntp = readdir( dir_ptr ) ) != NULL )
            // 일단 파일의 포인터를 배열에 저장
			file_list[count ++ ] = direntp;
		closedir(dir_ptr);
    }
    // -r 모드일때 
    if (reverse) 
        qsort(file_list, count, sizeof(struct dirent *), compare_filenames_reverse);
    // 일반 모드일때
    else 
        qsort(file_list, count, sizeof(struct dirent *), compare_filenames);
    
    //출력
    for (int i = 0 ; i < count ; i ++){
        printf("%s\n", file_list[i]->d_name);
    }
}

// 역방향 정렬
int compare_filenames_reverse (const void *a, const void *b) {
    struct dirent *file1 = *(struct dirent **)a;
    struct dirent *file2 = *(struct dirent **)b;
    return strcmp(file2 -> d_name , file1 -> d_name);
}

// 일반 정렬
int compare_filenames(const void *a, const void *b) {
    struct dirent *file1 = *(struct dirent **)a;
    struct dirent *file2 = *(struct dirent **)b;
    return strcmp(file1 -> d_name , file2 -> d_name);
}
