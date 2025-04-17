#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#define SLEEPTIME 10000

int is_prime(int);
void signal_handler(int);

unsigned long current_num = 3 ;
unsigned long max_prime = 2;
unsigned long prime_conut = 0;
int signal_count = 0;

int main() {

    signal(SIGINT,signal_handler);

    while(1){
        usleep(SLEEPTIME);
        if(is_prime(current_num)){
            max_prime = current_num;
            prime_conut++;
        }
        printf("현재 숫자 :     %ld\n",current_num);
        current_num ++;
    }
}

int is_prime(int num) {
    if (num < 2){
        return 0;
    }
    if(num == 2) {
        return 1;
    }
    for (int i = 2 ; i*i <= num ; i++){
        if(num % i == 0) {
            return 0;
        }
    }
    return 1;
}

void signal_handler(int signum) {
    signal_count ++ ;
    printf("\n");
    //printf("현재 숫자 :     %ld\n",current_num);
    printf("최대 소수 :     %ld\n",max_prime);
    printf("소수 개수 :     %ld\n",prime_conut);
    if(signal_count <= 2){
        printf("Ctrl+C를 %d회 입력하였습니다.\n",signal_count);
    }
    else {
        printf("Ctrl+C를 %d회 입력하였습니다. - 프로그램을 종료합니다.\n", signal_count);
        exit(0);
    }
}