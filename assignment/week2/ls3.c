/* ls2.c
 *	purose 	lsit contents of directory or directories
 *	action	if no args, use . else list files in args
 * 	note	uses stat and pwd.h and grp.h
 *	BUG: try ls2 /tmp
 */
#include	<stdio.h>
#include	<sys/types.h>
#include	<dirent.h>
#include	<sys/stat.h>
#include	<string.h>

void do_ls(char[]);
void dostat(char *);
void show_file_info( char *, struct stat *);
void mode_to_letters( int, char [] );
char *uid_to_name( uid_t );
char *gid_to_name( gid_t );

// total size를 세는 함수
long total_size(char dirname[]) ;

int main( int ac, char *av[] )
{
	if( ac == 1 )
		do_ls( "." );
	else
		while( --ac ) {
			printf("%s:\n", *++av );
		    	do_ls( *av );
		}
}


void do_ls( char dirname[] )
/*
 * list files in director called dirname
 */
{
	DIR		*dir_ptr;	/* the directory */
	struct dirent	*direntp;	/* each entry    */
	if( ( dir_ptr = opendir( dirname ) ) == NULL )
		fprintf(stderr, "ls1: cannot open %s\n", dirname);
	else
	{
		//total을 최상단에 출력하기 위해 while문 밖에 설치
        printf("total %8ld\n", total_size(dirname));
		while ( ( direntp = readdir( dir_ptr ) ) != NULL )
			 dostat( direntp->d_name );
		closedir(dir_ptr);
	}
}

void dostat( char *filename )
{
	struct stat info;
	
	if( stat( filename, &info ) == -1 )	/* cannot stat	*/
		perror( filename );		/* say why	*/
	else					/* else show inof */
		show_file_info( filename, &info );
}

void show_file_info( char *filename, struct stat *info_p)
/*
 * display the info about 'filename'. The info is stored in struct at *info_p
 */
{
	char	*uid_to_name(), *ctime(), *gid_to_name(), *filemode();
	void 	mode_to_letters();
	char	modestr[11];

	mode_to_letters( info_p->st_mode, modestr );

	printf( "%s"	, modestr );
	printf( "%4d "	, (int) info_p->st_nlink);
	printf( "%-8s "	, uid_to_name( info_p->st_uid ));
	printf( "%-8s "	, gid_to_name( info_p->st_gid ));
	printf( "%8ld "	, (long) info_p->st_size );
	printf( "%.12s ", 4+ctime( &info_p->st_mtime )) ;
	printf( "%s\n"	, filename );
}

/* 
 * utility functions
 */
/*
 * This function takes a mode value and a char array 
 * and puts into the char array the file type and the 
 * nine letter5s that correspond to the bits in mode.
 * NOTE: It does not code setuid, setgid, and sticky codes.
 */
void mode_to_letters( int mode, char str[])
{
	strcpy( str, "----------" );		/* dfault = no perms 	*/

	if( S_ISDIR( mode ) ) str[0] = 'd';	/* directory?		*/
	if( S_ISCHR( mode ) ) str[0] = 'c';	/* char devices		*/
	if( S_ISBLK( mode ) ) str[0] = 'b';	/* block device		*/

	if( mode & S_IRUSR )  str[1] = 'r'; 	/* 3 bits for user	*/
	if( mode & S_IWUSR )  str[2] = 'w';
	if( mode & S_IXUSR )  str[3] = 'x'; 
  
	if( mode & S_IRGRP )  str[4] = 'r'; 	/* 3 bits for group	*/
	if( mode & S_IWGRP )  str[5] = 'w';
	if( mode & S_IXGRP )  str[6] = 'x';

	if( mode & S_IROTH )  str[7] = 'r'; 	/* 3 bits for otehr	*/
	if( mode & S_IWOTH )  str[8] = 'w';
	if( mode & S_IXOTH )  str[9] = 'x';
}

#include	<pwd.h>

char *uid_to_name( uid_t uid )
/*
 *	returns pointer to username associated with uid, uses getpw()
 */
{
	struct passwd *getpwuid(), *pw_ptr;
	static char numstr[10];

	if(( pw_ptr = getpwuid( uid ) ) == NULL ) {
		sprintf(numstr, "%d", uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name;
}

#include	<grp.h>
char *gid_to_name( gid_t gid )
/*
 *	returns pointer to group number gid. used getgrgid(3)
 */
{
	struct group *getgrgid(), *grp_ptr;
	static char numstr[10];

	if( ( grp_ptr = getgrgid(gid) ) == NULL ) {
		sprintf( numstr, "%d", gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}

// 토탈 사이즈를 세는 함수이다.
long total_size(char dirname[]) {
	DIR		*dir_ptr;	/* the directory */
	struct dirent	*direntp;	/* each entry    */
    struct stat file_stat;
    char path[1024] ;
    long total = 0;
	if( ( dir_ptr = opendir( dirname ) ) == NULL )
		fprintf(stderr, "ls1: cannot open %s\n", dirname);
	else
	{
		while ( ( direntp = readdir( dir_ptr ) ) != NULL ){
            //stat을 사용하기 위한 경로 설정
            snprintf(path, sizeof(path), "%s/%s", dirname, direntp->d_name);
			// path를 file_stat에 저장한 후 total에 누적한다.
			// while문에 의해 모든 file를 순회한후 종료
            stat(path, &file_stat);
            total += file_stat.st_blocks;
        }
		closedir(dir_ptr);
	}
    return total;
}

