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
   int x=1, y=1, other=0; // ���� �߰� 
   int matrix[2][20][20]={0}; // plyaer,x,y 
   char key;
   char *stone[2]={"��","��"}; // �ٵϵ� ����  
   clock_t start_time = clock();  
   
   do
   {
	gotoxy(1,1);
	draw_check01(18, 18);
	gotoxy(x, y);
	printf("%s", stone[other]);
	display_stone(matrix);
	gotoxy(1, 20);
	printf("����Ű�� �����̰� ");
	printf("�����̽� Ű�� �����ÿ�.");
	gotoxy(1,21);
        printf("���� �������� ���� �����Դϴ�. ");
        
    // �ð����� �߰�    
    while (!_kbhit()){ // ���ѽð����� �Է¹��۰� ���µ��� �ݺ�  
	    int remaining_time = (clock() - start_time) / CLOCKS_PER_SEC; // ����ð�-���۽ð� / ��  
	    gotoxy(1, 23);
	    printf("���� �ð�: %d ��     ", 30 - remaining_time); // ���� ������� ������ ��µ� ���� �ð��� ����� ������ �մϴ�.
		    if (remaining_time >= 30){
		    	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			    	if(other == 0){
			    		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
						        printf("\n�鵹 �¸�"); 
					}else if(other == 1){
						SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
						printf("\n�浹 �¸�"); 
					}
					 
			        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // ���� �������� ����
			        exit(0);
		    	}
	}
			    	
	key=getch();
	if (key==27)
		exit(0);
	else if(key==32)
		{
			// x,y��ǥ���� ���� �����ִ��� ���� üũ 
			if (matrix[0][(x+1)/2][y] == 0 && matrix[1][(x+1)/2][y] == 0) {
				Beep(1000, 200); //  ( ���ļ� ,  �ð� ) 
				matrix[other][(x+1)/2][y]=1;
				start_time = clock(); // �ð� �缳��				
			// �¸����� �߰�  
            int winner = check_win(matrix, other);
			if(winner != -1) 
			{  
			    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	    
			    if(winner == 0) {
			        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
			        printf("\n�浹 �¸�");      
			    } else if (winner == 1){
			        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			        printf("\n�鵹 �¸�");      
			    }  
			    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // ���� �������� ����
			   exit(0); 
			}
			//���� �ѱ� 
            other=1-other;
				} 
				else {
				printf("\n�̹� ���� �ִ� ��ġ�Դϴ�.\n");
				Sleep(1000); // 1�� ����� 
       		    system("cls"); // ȭ�� ���� 
			}
		}
	else if (key>=72)
		move_arrow_key(key, &x, &y, 19, 19);
   }while(1);
}

void display_stone(int matrix[][20][20])
{
	int i, x, y;
	char  *stone[2]={"��", "��"};

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
	case 72:  //����(��) ������ ȭ��ǥ Ű �Է�
		*y1=*y1-1;
		if (*y1<1)	*y1=1; //y��ǥ�� �ּҰ�
		break;
	case 75:  //����(��) ������ ȭ��ǥ Ű �Է�
		*x1=*x1-2;             //������������ 2�� ����
		if (*x1<1)	*x1=1; //x��ǥ�� �ּҰ�
		break;
	case 77:  //������(��) ������ ȭ��ǥ Ű �Է�
		*x1=*x1+2;               //������������ 2�� ����
		if (*x1>x_b)  *x1=x_b; //x��ǥ�� �ִ밪(���)
		break;
	case 80:  //�Ʒ���(��) ������ ȭ��ǥ Ű �Է�
		*y1=*y1+1;
		if (*y1>y_b)  *y1=y_b; //y��ǥ�� �ִ밪(���)
		break;
	default:
		return;
	}
}

int check_win(int matrix[][20][20], int player) {
    int i, j;
    
    // ���� üũ 
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
    // ���� üũ  
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
	// �����ϴܿ��� ����������� üũ 
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


	// ������ܿ��� �����ϴ����� üũ 
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






