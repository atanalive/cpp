#include<stdio.h>
#include<graphics.h>
#include<windows.h>
#include<conio.h>
//#include<iostream>
//#include<stdlib.h>
#pragma comment(lib,"winmm.lib")
//#include<mmsystem.h>
#include<time.h>

void gameinit();
void snakemove(int a);
void changech();
void createfood();
int eatfood(int score);
void menu();
void instruction();
void gameover(int score);
void surprise();

struct Coor
{
	int x, y;
};
typedef enum ch
{
	up = 101, 
	down = 102, 
	right = 104, 
	left = 103
}CH;
struct Snake
{
	struct Coor a[1000];
	int n;
	CH ch;
}snake;
struct Food {
	struct Coor fcr;
	int flag;
}food;

int main()//����
{   
	menu();
	gameinit();
	int score = 0;
	while (1) 
	{
		while (!_kbhit()) {
			snakemove(score);
			score= eatfood(score);
		}
		changech();
		snakemove(score);
		score=eatfood(score);
	}
	getchar();
	return 0;

}
void gameinit()//��ʼ����Ϸ
{
	initgraph(600, 600);
	//setfillcolor(LIGHTGREEN);
	HRGN rec = CreateRectRgn(0,0,600,600);
	setcliprgn(rec);
	DeleteObject(rec);
	//solidrectangle(600,0,800,600);
	//setcolor(BLACK);
	//settextstyle(30, 0, "�п�");
	//setbkmode(TRANSPARENT);
	//outtextxy(650, 50, "̰����");
	//һ�ֺܶ����ֵ����
	//RECT rect = { 625,100,775,500 };
	//settextstyle(15, 0, "����");
	//drawtext(_T("����˹̹-������-��\n\n\n\n\n����Ϸ˵����ͨ������������ƶ�\n\n\n����ܰ��ʾ����������ٳԵ����ʳ��\n"), &rect, DT_WORDBREAK);
	//RECT scorerec = { 660,370,750,400 };
	//settextstyle(30, 0, "����");
	//drawtext(_T("�÷�:\n"),&scorerec,DT_WORDBREAK);
	//settextstyle(30, 20, "����");
	//outtextxy(675, 425, printf("%d",score));
	//�����Ϸ��ĳ�ʼ��
	//HRGN rec1 = CreateRectRgn(0,0,600,600);
	//setcliprgn(rec1);
	//DeleteObject(rec1);
	mciSendString("open  liang.mp3 alias ly",0,0,0);
	mciSendString("play ly repeat",0,0,0);
	//PlaySound("1.mp3", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	//cleardevice();
	setbkcolor(RGB(100, 250, 450));
    //solidrectangle(0, 0, 600, 600);
	cleardevice();
	//�ߵĳ�ʼ��
	setcolor(BROWN);
	setfillcolor(WHITE);
	fillrectangle(300, 300, 310, 310);
	snake.n = 5;
	for (int h=0; h < snake.n; h++)
	{
		snake.a[h].x = 300;
		snake.a[h].y = 300;
	}
	snake.ch = right;
	food.flag = 0;
}
void snakemove(int a)//�ߵ��ƶ�
{
		for (int k = snake.n; k > 0; k--) 
		{
			snake.a[k].x = snake.a[k-1].x;
			snake.a[k].y = snake.a[k-1].y;
		}
		switch (snake.ch)
		{
		case up:
			if (snake.a[0].y == 0) {
				snake.a[0].y = 590;
			}
			else {
				snake.a[0].y -= 10;
			}
			break;
		case down:
			if (snake.a[0].y == 590) {
				snake.a[0].y = 0;
			}
			else {
				snake.a[0].y += 10;
			}
			break;
		case left:
			if (snake.a[0].x == 0) {
				snake.a[0].x = 590;
			}
			else {
				snake.a[0].x -= 10;
			}
			break;
		case right:
			if (snake.a[0].x == 590) {
				snake.a[0].x = 0;
			}
			else {
				snake.a[0].x += 10;
			}
			break;
		}
		cleardevice();
		createfood();
		food.flag = 1;
		for (int i =snake.n; i >0; i--) {
			fillrectangle(snake.a[i].x, snake.a[i].y, snake.a[i].x + 10, snake.a[i].y + 10);
		}
		Sleep(80);
		gameover(a);
}
void changech()//�ü��̸ı��ߵķ���
{
	int input;
	input=_getch();
	switch(input) {    //�����뷽���Ӧ
	    case *"W":
			input = 101;
			break;
		case *"w":
			input = 101;
			break;
		case *"S":
			input = 102;
			break;
		case *"s":
			input = 102;
			break;
		case *"A":
			input = 103;
			break;
		case *"a":
			input = 103;
			break;
		case *"D":
			input = 104;
			break;
		case *"d":
			input = 104;
			break;

	}
	switch (input)//�ı䷽��
	{
	case up:
		if (snake.ch != down) 
		{
			snake.ch = up;
		};
		break;
	case down:
		if (snake.ch != up)
		{
			snake.ch = down;
		}
		break;
	case left:
		if(snake.ch!=right)
		{
			snake.ch = left;
		}
		break;
	case right:
		if(snake.ch!=left)
		{
			snake.ch = right;
		}
		break;
	}
}
void createfood() //����ʳ��
{
int f = 1;
if (!food.flag) {
	while (f) {
		srand(unsigned int(time(NULL)));
		food.fcr.x = rand() % (590 / 10) * 10;
		food.fcr.y = rand() % (590 / 10) * 10;
		f = 0;
		for (int g=0; g < snake.n; g++) {
			if (food.fcr.x ==snake.a[g].x && food.fcr.y == snake.a[g].y) {
				f = 1;
			}
		}
	}
}
	fillroundrect(food.fcr.x, food.fcr.y, food.fcr.x + 10, food.fcr.y + 10, 10, 10);
}
int eatfood(int score) //��ʳ��
{
	int a;
	if (snake.a[0].x == food.fcr.x&&snake.a[0].y == food.fcr.y) {
		food.flag = 0;
		snake.n+=1;
		a=score+100;
	}
	else {
		a = score;
	}
		return a;
}
void menu()//�˵�
{
	initgraph(500, 500);
	setbkcolor(WHITE);
	cleardevice();
	setfillcolor(LIGHTCYAN);
	fillrectangle(150, 150, 350, 200);
	fillrectangle(150, 220, 350, 270);
	fillrectangle(150, 290, 350, 340);
	settextcolor(BLUE);
	settextstyle(30, 0, "����");
	setbkmode(TRANSPARENT);
	outtextxy(177, 90, "̰������Ϸ");
	settextcolor(GREEN);
	outtextxy(190, 160, "��ʼ��Ϸ");
	outtextxy(190, 230, "��Ϸ˵��");
	outtextxy(190, 300, "�˳���Ϸ");
	settextstyle(20, 0, "����");
	settextcolor(BLACK);
	outtextxy(110, 400, "����˹̹-������-��  v2.0�汾");
	MOUSEMSG m;
	while (1) {
		m = GetMouseMsg();
		if (m.x >= 150 && m.x <= 350 && m.y >= 150 && m.y <= 200)
		{
			setcolor(YELLOW);
			rectangle(150 - 5, 150 - 5, 350 + 5, 200 + 5);
			if (m.uMsg == WM_LBUTTONDOWN) {
				break;
			}
		}
		else if (m.x >= 150 && m.x <= 350 && m.y >= 220 && m.y <= 270) {
			setcolor(YELLOW);
			rectangle(150 - 5, 220 - 5, 350 + 5, 270 + 5);
			if (m.uMsg == WM_LBUTTONDOWN) {
				instruction();
			}
		}
		else if (m.x >= 150 && m.x <= 350 && m.y >= 290 && m.y <= 340) {
			setcolor(YELLOW);
			rectangle(150 - 5, 290 - 5, 350 + 5, 340 + 5);
			if (m.uMsg == WM_LBUTTONDOWN) {
				exit(0);
			}
		}
		else {
			setcolor(WHITE);
			rectangle(150 - 5, 150 - 5, 350 + 5, 200 + 5);
			rectangle(150 - 5, 220 - 5, 350 + 5, 270 + 5);
			rectangle(150 - 5, 290 - 5, 350 + 5, 340 + 5);
		}
	}
}
void instruction() //��Ϸ˵��
{
	initgraph(500, 500);
	setbkcolor(WHITE);
	cleardevice();
	setfillcolor(LIGHTCYAN);
	fillrectangle(150, 290, 350, 340);
	settextcolor(BLACK);
	settextstyle(20, 0, "����");
	RECT A = { 50,50,450,300 };
	drawtext(_T("����Ϸ˵����\n              w,s,a,d���Ʒ��� \n\n����ܰ��ʾ��\n              1.�����ٳԵ�ʮ��ʳ��.\n              2.����߳Ե���������Ϸ����\n\n�����ߡ�\n              ����˹̹-������-��\n"),&A,DT_WORDBREAK);
	settextcolor(GREEN);
	settextstyle(30,0,"����");
	setbkmode(TRANSPARENT);
	outtextxy(210,300,"����");
	MOUSEMSG M;
	while (1) {
	    M = GetMouseMsg();
		if (M.x >= 150 && M.x <= 350 && M.y >= 290 && M.y <= 340) {
			setcolor(YELLOW);
			rectangle(150 - 5, 290 - 5, 350 + 5, 340 + 5);
			if (M.uMsg == WM_LBUTTONDOWN) {
				break;
			}
		}
		else {
			setcolor(WHITE);
			rectangle(150 - 5, 290 - 5, 350 + 5, 340 + 5);
		}
	}
	initgraph(500, 500);
	setbkcolor(WHITE);
	cleardevice();
	setfillcolor(LIGHTCYAN);
	fillrectangle(150, 150, 350, 200);
	fillrectangle(150, 220, 350, 270);
	fillrectangle(150, 290, 350, 340);
	settextcolor(BLUE);
	settextstyle(30, 0, "����");
	setbkmode(TRANSPARENT);
	outtextxy(177, 90, "̰������Ϸ");
	settextcolor(GREEN);
	outtextxy(190, 160, "��ʼ��Ϸ");
	outtextxy(190, 230, "��Ϸ˵��");
	outtextxy(190, 300, "�˳���Ϸ");
	settextstyle(20, 0, "����");
	settextcolor(BLACK);
	outtextxy(110, 400, "����˹̹-������-��  v2.0�汾");
}
void gameover(int score) {
	for (int f = 1; f < snake.n; f++) {
		if (snake.a[0].x == snake.a[f].x&&snake.a[0].y == snake.a[f].y) {
			initgraph(600, 600);
			setbkcolor(BROWN);
			cleardevice();
			settextcolor(WHITE);
			settextstyle(30, 0, "����");
			setbkmode(TRANSPARENT);
			RECT A = { 150,100,450,200 };
			if (score >= 0 && score <= 500) {
				drawtext(_T("��Ϸ����\n��Ķ�λΪ:��ͭ"), &A, DT_WORDBREAK);
			}
			else if (score > 500 && score <= 1000) {
				drawtext(_T("��Ϸ����\n��Ķ�λΪ:�ƽ�"), &A, DT_WORDBREAK);
			}
			else if (score > 1000 && score <= 1500) {
				drawtext(_T("��Ϸ����\n��Ķ�λΪ:����"), &A, DT_WORDBREAK);
			}
			else if (score > 1500 && score <= 2000) {
				drawtext(_T("��Ϸ����\n��Ķ�λΪ:��ʯ"), &A, DT_WORDBREAK);
			}
			else if (score > 2000 && score <= 2500) {
				drawtext(_T("��Ϸ����\n��Ķ�λΪ:��ҫ"), &A, DT_WORDBREAK);
			}
			else if (score > 2500 && score <= 3000) {
				drawtext(_T("��Ϸ����\n��Ķ�λΪ:����"), &A, DT_WORDBREAK);
			}
			else if (score > 3000) {
				drawtext(_T("��Ϸ����\n��ϲ�㣬���Ѿ��޵�"), &A, DT_WORDBREAK);
			}
			setfillcolor(LIGHTCYAN);
			settextstyle(35, 0, "����");
			fillrectangle(220, 320, 380, 355);
			outtextxy(230, 320, "����һ��");
			mciSendString("close ly", 0, 0, 0);
			MOUSEMSG m;
			while(1) {
				m = GetMouseMsg();
				if (m.x >= 220 && m.x <= 380 && m.y >= 320 && m.y <= 355) {
					setcolor(WHITE);
					rectangle(220 - 5, 320 - 5, 380 + 5, 355 + 5);
					if (m.uMsg == WM_LBUTTONDOWN) {
						gameinit();
						int score = 0;
						break;
					}
				}
				else {
					setcolor(BROWN);
					rectangle(220 - 5, 320 - 5, 380 + 5, 355 + 5);
				}
			}
		}
	 }
}
void surprise() {

}