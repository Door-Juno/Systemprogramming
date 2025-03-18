/*
utmplib.c 파일과 합쳤습니다. 
*/

#include <stdio.h>
#include <sys/types.h>
#include <utmp.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

#define SHOWHOST
#define NRECS 16
#define NULLUT ((struct utmp *)NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE];
static int  num_recs;
static int  cur_rec;
static int  fd_utmp = -1;

void show_info(struct utmp * );
void showtime(time_t);
utmp_open(char *filename) ;
struct utmp *utmp_next();
int utmp_reload();
utmp_reload();

int main() {
    struct utmp *utbufp,
                *utmp_next() ;
	/* 

	utmp_open을 통해 파일을 열어준다.

	*/
    if( utmp_open ( UTMP_FILE ) == -1) {
        perror(UTMP_FILE) ;
        exit(1);
    }
	/*
	utmp_next() 를 이용해 한 단위씩 읽는 모습
	*/
    while ( (utbufp = utmp_next() ) != ( (struct utmp*) NULL))
        show_info(utbufp);

	/*
	utmp_close() 로 닫는 모습
	*/
    utmp_close();
    return 0;
}

/*
* show info()
*		display the contents of the utmp struct in human readale form
*		*displays nothing if record has no user name
*/
void show_info( struct utmp *utbufp )
{
	if( utbufp->ut_type != USER_PROCESS)		/* users only ! */
		return;

	printf("%-8.8s", utbufp->ut_name);	/* the logname */
	printf(" ");											/* a space		*/
	printf("%-8.8s", utbufp->ut_line);		/* the tty */
	printf(" ");											/* a space		*/
	showtime( utbufp->ut_time);
#ifdef SHOWHOST
	if( utbufp->ut_host[0] != '\0' )
		printf(" (%s) ", utbufp->ut_host);		/* the host */
#endif
	printf("\n");	
}

void showtime( long timeval )
/*
 * displays time in a format fit for human consumption
 * uses ctime to build a string then pics parts out of it
 * Note: %12.12s prints a string 12 chars wide and LIMITS it to 12 chars.
 */
{
	char *cp;								/* to hold address of time */
	cp = ctime(&timeval);			/* convert timeto stirng */
												/* string looks like Mon Feb 4 00:46:40 EST 1991 */
	printf("%12.12s", cp+4);	/* pick 12 chars from pos 4 */
}

utmp_open (char *filename) {
    fd_utmp = open(filename, O_RDONLY); //OPEN
    cur_rec = num_recs = 0; // 최신화
    return fd_utmp;
}  

struct utmp *utmp_next() {
    struct utmp *recp;
    if(fd_utmp == -1 ) //ERROR ?
        return NULLUT;
    if (cur_rec == num_recs && utmp_reload() == 0)
        return NULLUT;
    recp = ( struct utmp * ) &utmpbuf[cur_rec * UTSIZE];
    cur_rec ++;
    return recp;
}

int utmp_reload() {
    int amt_read;
    amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE) ;
    num_recs = amt_read/UTSIZE;
    cur_rec = 0;
    return num_recs;
}

utmp_close() {
    if( fd_utmp != -1 )
        close(fd_utmp);
}