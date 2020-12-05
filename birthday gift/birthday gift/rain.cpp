#include "number_rain.h"

//#include<Windows.h>
//#include<Mmsystem.h>

//#include<iostream>
//#include<locale>
//#include<cstdlib>
//#include<wchar.h>

//using namespace std;
//setlocale(LC_ALL, "chs");
//wchar_t word[6] = L"我佛了呀";
//wcout << word;

int num = 100;
char strrain[number_rain][lelong_rain] = { 0 };
POINT rainpos[number_rain] = { 0 };
char w[11] = "~生日快乐~";
int b_rain = 0;
int h_rain = 0;
bool putword = false;
bool snow = false;


void rain();
void initrain();
void update();
void down(DWORD&);
void randomstr();

void rain() {
	initgraph(1200, 800);
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
	DWORD t1 = timeGetTime();
	DWORD st1 = timeGetTime();
	while (1) {
		DWORD t2 = timeGetTime();
		if (t2 - t1 > 10000)
		{
			while (1) {
				h_rain = rand() % 3;
				if (h_rain != b_rain) {
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
		for (int j = 0; j < lelong_rain; j++) {
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

	for (int i = 0; i < num; i++) {
		rainpos[i].x = rand() % width_rain;
		rainpos[i].y = rand() % height_rain;
	}
	settextstyle(18, 0, "楷体");
}

void update() {
	for (int i = 0; i < num; i++) {
		rainpos[i].y += 15;
		if (rainpos[i].y >= height_rain) {
			rainpos[i].y = 0;
			rainpos[i].x = rand() % width_rain;
		}
	}
}

void down(DWORD& st1) {
	DWORD st2 = timeGetTime();
	BeginBatchDraw();
	cleardevice();
	b_rain = h_rain;
	for (int i = 0; i < num; i++) {
		//settextcolor(RGB(255, 255, 255));
		if (st2 - st1 > 20000&& st2 - st1 < 40000) {
			snow = true;
			putword = false;
			settextstyle(20, 0, "楷体");
		}
		else if (st2 - st1 > 40000&&st2 - st1 < 60000) {
			putword = true;
			snow = false;
			settextstyle(30, 0, "楷体");
		}
		else if (st2 - st1 > 60000)
		{
			settextstyle(18, 0, "楷体");
			putword = false;
			snow = false;
			st1 = st2;
		}
		if (putword == false && snow == false) {
			num = 80;
			for (int j = 0; j < lelong_rain; j++) {
				switch (b_rain) {
				case 0:
					settextcolor(RGB(0 + 28 * j, 255, 255));//天蓝色
					break;
				case 1:
					settextcolor(RGB(0 + j * 28, 255, 0 + j * 28));//绿色
					break;
				case 2:
					settextcolor(RGB(255, 255, 0 + j * 28));//黄色
					break;
				}
				outtextxy(rainpos[i].x, rainpos[i].y - j * 15, strrain[i][j]);
				//randomstr();卡卡的，注释掉
			}
		}
		else if (putword == true) {
			num = 40;
			switch (b_rain) {
			case 0:
				settextcolor(RGB(0, 255, 255));//天蓝色
				break;
			case 1:
				settextcolor(RGB(0, 255, 0));//绿色
				break;
			case 2:
				settextcolor(RGB(255, 255, 0));//黄色
				break;
			}
			outtextxy(rainpos[i].x, rainpos[i].y, w);
		}
		else if (snow == true && putword == false) {
			switch (b_rain) {
			case 0:
				settextcolor(RGB(0, 255, 255));//天蓝色
				break;
			case 1:
				settextcolor(RGB(0, 255, 0));//绿色
				break;
			case 2:
				settextcolor(RGB(255, 255, 0));//黄色
				break;
			}
			outtextxy(rainpos[i].x, rainpos[i].y, "*");
		}
	}
	EndBatchDraw();
	Sleep(80);
}

//字母随机生成
void randomstr()
{
	char a = 0;
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < lelong_rain; j++) {
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
}
