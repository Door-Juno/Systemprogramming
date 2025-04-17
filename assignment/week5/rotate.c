/* rotate.c : map a->b, b->c, .. z->a
 *  urpose: useful for showing tty modes
 */

#include <stdio.h>
#include <ctype.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

void set_mode(void);
void reset_mode(void);
struct termios original_state;

int main(void)
{
	int c;
	set_mode();

	while( ( c=getchar())!= EOF ) {
		if( c == 'z' )
			c = 'a';
		else if ( islower(c) )
			c++;
		else if ( c == 'Q'){
			printf("\n");
			reset_mode();
			break;
		}
		putchar(c);
	}
	reset_mode()
	return 0;	
}

void set_mode(void){
	struct termios ttystate;

	tcgetattr(STDIN_FILENO, &original_state);
	ttystate = original_state;

	ttystate.c_lflag	&= ~ICANON;
	ttystate.c_lflag	&= ~ECHO;
	ttystate.c_cc[VMIN]	=1;
	tcsetattr(STDIN_FILENO,TCSANOW,&ttystate);
}

void reset_mode(void){
	tcsetattr(STDIN_FILENO, TCSANOW,&original_state);
}