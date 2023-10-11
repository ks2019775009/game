#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

void move_arrow_key(char chr, int *x, int *y, int x_b, int y_b);
void gotoxy(int x, int y);
void draw_check01(int c, int r);
void game_control(void);
void display_stone(int matrix[][20][20]);

int main(void)
{
	game_control();
	return 0;
}

void gotoxy(int x, int y)
{
   COORD Pos = {x - 1, y - 1};
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void draw_check01(int c, int r)
{
    int i, j;
    unsigned char a=0xa6;
    unsigned char b[12]; 
    for(i=1;i<12;i++)
	b[i]=0xa0+i;
    printf("%c%c",a, b[3]);
    for(i=0;i<c-1;i++)
	 printf("%c%c", a, b[8]);
    printf("%c%c", a, b[4]);
    printf("\n");
    for(i=0;i<r-1;i++)
    {
	printf("%c%c", a, b[7]);
	for(j=0;j<c-1;j++)
	    printf("%c%c", a, b[11]);
	printf("%c%c",a, b[9]);
	printf("\n");
    }
    printf("%c%c", a, b[6]);
    for(i=0;i<c-1;i++)
	printf("%c%c", a, b[10]);
    printf("%c%c", a, b[5]);
    printf("\n");
}

void game_control(void)
{
   int x=1, y=1, other=0; // 순서 추가 
   int matrix[2][20][20]={0}; // plyaer,x,y 
   char key;
   char *stone[2]={"○","●"}; // 바둑돌 색깔  
   clock_t start_time = clock();  
   
   do
   {
	gotoxy(1,1);
	draw_check01(18, 18);
	gotoxy(x, y);
	printf("%s", stone[other]);
	display_stone(matrix);
	gotoxy(1, 20);
	printf("방향키로 움직이고 ");
	printf("스페이스 키를 누르시오.");
	gotoxy(1,21);
        printf("돌을 놓았으면 상대방 차례입니다. ");
        
    // 시간제한 추가    
    while (!_kbhit()){ // 제한시간동안 입력버퍼가 없는동안 반복  
	    int remaining_time = (clock() - start_time) / CLOCKS_PER_SEC; // 현재시간-시작시간 / 초  
	    gotoxy(1, 23);
	    printf("남은 시간: %d 초     ", 30 - remaining_time); // 뒤의 공백들은 이전에 출력된 남은 시간을 지우는 역할을 합니다.
		    if (remaining_time >= 30){
		    	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			    	if(other == 0){
			    		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
						        printf("\n백돌 승리"); 
					}else if(other == 1){
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
						printf("\n흑돌 승리"); 
					}
					 
			        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // 원래 색상으로 복구
			        exit(0);
		    	}
	}
			    	
	key=getch();
	if (key==27)
		exit(0);
	else if(key==32)
		{
			// x,y좌표값에 돌이 놓여있는지 여부 체크 
			if (matrix[0][(x+1)/2][y] == 0 && matrix[1][(x+1)/2][y] == 0) {
				Beep(1000, 200); //  ( 주파수 ,  시간 ) 
				matrix[other][(x+1)/2][y]=1;
				start_time = clock(); // 시간 재설정				
			// 승리조건 추가  
            int winner = check_win(matrix, other);
			if(winner != -1) 
			{  
			    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	    
			    if(winner == 0) {
			        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			        printf("\n흑돌 승리");      
			    } else if (winner == 1){
			        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			        printf("\n백돌 승리");      
			    }  
			    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // 원래 색상으로 복구
			   exit(0); 
			}
			//차례 넘김 
            other=1-other;
				} 
				else {
				printf("\n이미 돌이 있는 위치입니다.\n");
				Sleep(1000); // 1초 대기후 
       		    system("cls"); // 화면 지움 
			}
		}
	else if (key>=72)
		move_arrow_key(key, &x, &y, 19, 19);
   }while(1);
}

void display_stone(int matrix[][20][20])
{
	int i, x, y;
	char  *stone[2]={"○", "●"};

	for(i=0;i<2;i++)
		for(x=1;x<20;x++)
			for(y=1;y<20;y++)
			{
				if (matrix[i][x][y]==1)
				{
					gotoxy(x*2-1, y);
					printf("%s", stone[i]);
				}
			}
}

void move_arrow_key(char key, int *x1, int *y1, int x_b, int y_b)
{
	switch(key)
	{
	case 72:  //위쪽(상) 방향의 화살표 키 입력
		*y1=*y1-1;
		if (*y1<1)	*y1=1; //y좌표의 최소값
		break;
	case 75:  //왼쪽(좌) 방향의 화살표 키 입력
		*x1=*x1-2;             //좌측방향으로 2씩 감소
		if (*x1<1)	*x1=1; //x좌표의 최소값
		break;
	case 77:  //오른쪽(우) 방향의 화살표 키 입력
		*x1=*x1+2;               //우측방향으로 2씩 증가
		if (*x1>x_b)  *x1=x_b; //x좌표의 최대값(경계)
		break;
	case 80:  //아래쪽(하) 방향의 화살표 키 입력
		*y1=*y1+1;
		if (*y1>y_b)  *y1=y_b; //y좌표의 최대값(경계)
		break;
	default:
		return;
	}
}

int check_win(int matrix[][20][20], int player) {
    int i, j;
    
    // 세로 체크 
    for (i = 1; i < 20; i++) {
        for (j = 1; j <= 16; j++) {
            if (matrix[player][i][j] == 1 &&
                matrix[player][i][j+1] == 1 &&
                matrix[player][i][j+2] == 1 &&
                matrix[player][i][j+3] == 1 &&
                matrix[player][i][j+4] == 1 &&
                (j==1 || matrix[player][i][(j-1)]==0) && 
                (j==16 || matrix[player][i][(j+5)]==0)) {
                    return player;
            }
        }
    }
    // 가로 체크  
	for (i = 1; i <= 16; i++) {
	    for (j = 1; j <= 20; j++) {
	        if (matrix[player][i][j] == 1 &&
	            matrix[player][i+1][j] == 1 &&
	            matrix[player][i+2][j] == 1 &&
	            matrix[player][i+3][j] == 1 &&
	            matrix[player][i+4][j] == 1 &&
	            (i==1 || matrix[(player)][(i-1)][j]==0) && 
	            (i==16 || matrix[(player)][(i+5)][j]==0)) {
	                return player;
	        }
	    }
	}
	// 좌측하단에서 우측상단으로 체크 
	for (i = 1; i <= 16; i++) {
	    for (j = 1; j <= 16; j++) {
	        if (matrix[player][i][j] == 1 &&
	            matrix[player][i+1][j+1] == 1 &&
	            matrix[player][i+2][j+2] == 1 &&
	            matrix[player][i+3][j+3] == 1 &&
	            matrix[player][i+4][j+4] == 1 && 
	            ((i==16 || j==16) || matrix[(player)][(i+5)][(j+5)]==0) && 
		        ((i==1 || j==1) || matrix[(player)][(i-1)%20][(j-1)]==0)) {
	                return player;
	        }
	    }
	}


	// 우측상단에서 좌측하단으로 체크 
	for (i = 20; i >= 5; i--) {
	    for (j = 1; j <= 16; j++) {
	        if(matrix[player][i][j] == 1 && 
	           matrix[player][i-1][j+1] == 1 && 
	           matrix[player][i-2][j+2] == 1 && 
	           matrix[player][i-3][j+3] == 1 &&
	           matrix[player][i-4][j+4] == 1 &&
	           ((i==20 || j==1) || matrix[(player)][(i+1)%20][(j-1)]==0) && 
	           ((i<=5 || j>=16) || matrix[(player)][(i-5)][(j+5)]==0)) {
	               return player;
	       }
	   }
	}

   return -1;
}






