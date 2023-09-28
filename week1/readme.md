##수업에 전체적인 방향에 대한 설명 및 C언어 기초 문제 해석해보기

3_7_5 발표 1회 완료

#include <stdio.h> #include <stdlib.h> #include <time.h>

int main(void) { int i; srand(time(NULL)); for(i=1;i<=6;i++) printf("%2d:%d\n",rand()%45+1); return 0; }

time(NULL)을 통한 현재 시각에 따른 srand로 시드값을 매 번 초기화 후 1부터 45까지의 랜덤 난수를 총 6번 출력
