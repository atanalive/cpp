#include<graphics.h>
#include<time.h>
#include<stdio.h>
//#include<Windows.h>
//#include<Mmsystem.h>
#include<conio.h>
#pragma comment(lib,"winmm.lib")
//#include<iostream>
//#include<locale>
//#include<cstdlib>
//#include<wchar.h>

//using namespace std;
//setlocale(LC_ALL, "chs");
//wchar_t word[6] = L"我佛了呀";
//wcout << word;

#define number 100
#define lelong 8
#define height 1080
#define width 1920

int num = 100;
char strrain[number][lelong] = { 0 };
POINT rainpos[number] = { 0 };
char w[11]="生日快乐";
int b = 0;
int h = 0;
bool putword = false;
bool snow = false;

void rain();
void initrain();
void update();
void down(DWORD&);

void main() {
	rain();
}

void rain() {
	initgraph(1200, 800);
	getchar();
	//创建窗口
	HWND hwnd = initgraph(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	//设置窗口风格
	SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) - WS_CAPTION);
	//设置窗口位置
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CXSCREEN), SWP_SHOWWINDOW);
	//设置为透明
	LONG windowlong = GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED;
	//设置窗口风格
	SetWindowLong(hwnd, GWL_EXSTYLE, windowlong);
	//将背景改为黑色
	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

	srand((unsigned)time(NULL));
	initrain();
	DWORD t1=timeGetTime();
	DWORD st1 = timeGetTime();
	while (1) {
		DWORD t2 = timeGetTime();
		if(t2-t1>5000)
		{
			while (1) {
				h = rand() % 4;
				if (h != b) {
					break;
				}
			}
			t1 = t2;
		}
		update();
		down(st1);
	}

}

void initrain() {
	char a = 0;
	for (int i = 0; i < num; i++) {
		for (int j=0; j < lelong; j++) {
			while (1) {
				//字母雨
				a = rand() % 123;
				if (a >= 65 && a <= 90 || a >= 97 && a <= 122) {
					break;
				}
			}
			strrain[i][j] = a;
		}
	}

	for (int i=0; i < num; i++) {
		rainpos[i].x = rand() % width;
		rainpos[i].y = rand() % height;
	}

}

void update() {
	for (int i=0; i < num; i++) {
		rainpos[i].y += 15;
		if (rainpos[i].y >= height) {
			rainpos[i].y = 0;
			rainpos[i].x = rand() % width;
		}
	}
}

void down(DWORD& st1) {
	DWORD st2 = timeGetTime();
	BeginBatchDraw();
	cleardevice();
	b = h;
	for (int i = 0; i < num; i++) {
		//settextcolor(RGB(255, 255, 255));
		if (st2 - st1 > 50000&&snow==false) {
			snow = true;
		}
		else if (st2 - st1 > 80000 && putword == false&&snow==true) {
			putword = true;
			settextstyle(25, 0, "楷体");
		}
		if(putword==false&&snow==false){
			num = 80;
			for (int j = 0; j < lelong; j++) {
				switch (b) {
				case 0:
					settextcolor(RGB(0 + 28 * j, 255, 255));//天蓝色
					break;
				case 1:
					settextcolor(RGB(0 + j * 28, 255, 0 + j * 28));//绿色
					break;
				case 2:
					settextcolor(RGB(255, 130 + j * 13, 0 + j * 28));//橙色
					break;
				case 3:
					settextcolor(RGB(255, 255, 0 + j * 28));//黄色
					break;
				}
				outtextxy(rainpos[i].x, rainpos[i].y - j * 15, strrain[i][j]);
			}
		}
		else if (putword == true) {
			num = 50;
			switch (b) {
			case 0:
				settextcolor(RGB(0, 255, 255));//天蓝色
				break;
			case 1:
				settextcolor(RGB(0, 255, 0));//绿色
				break;
			case 2:
				settextcolor(RGB(255, 130, 0));//橙色
				break;
			case 3:
				settextcolor(RGB(255, 255, 0));//黄色
				break;
			}
			outtextxy(rainpos[i].x, rainpos[i].y, w);
		}
		else if (snow == true&&putword==false) {
			switch (b) {
			case 0:
				settextcolor(RGB(0, 255, 255));//天蓝色
				break;
			case 1:
				settextcolor(RGB(0, 255, 0));//绿色
				break;
			case 2:
				settextcolor(RGB(255, 130, 0));//橙色
				break;
			case 3:
				settextcolor(RGB(255, 255, 0));//黄色
				break;
			}
			outtextxy(rainpos[i].x, rainpos[i].y, "*");
		}
	}
	EndBatchDraw();
	Sleep(80);
}