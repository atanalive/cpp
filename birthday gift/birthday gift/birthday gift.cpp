#include "number_rain.h"
//#include <windows.h>
//#include <iostream.h>
#include <math.h>
#include <Mmsystem.h> // 使用该计时器必须包含的文件

/***** 宏定义区 ******/

#define NUM 13  // 烟花种类数量宏定义

/***** 结构定义区 **********/

// 烟花结构
struct FIRE
{
	int r;   // 当前爆炸半径
	int max_r;  // 爆炸中心距离边缘最大半径
	int x, y;  // 爆炸中心在窗口的坐标
	int cen_x, cen_y; // 爆炸中心相对图片左上角的坐标
	int width, height; // 图片的宽高
	int xy[581][612]; // 储存图片像素点

	bool show = false;  // 是否绽放
	bool draw=false;  // 开始输出像素点
	bool style = false;//心型烟花类型控制
	DWORD t1, t2, dt; // 绽放速度
}Fire[NUM+2];

// 烟花弹结构
struct JET
{
	int x, y;  // 喷射点坐标
	int hx, hy;  // 最高点坐标------将赋值给 FIRE 里面的 x, y
	int height;  // 烟花高度
	bool shoot = false;  // 是否可以发射

	DWORD t1, t2, dt; // 发射速度
	IMAGE img;  // 储存花弹一亮一暗图片
	//byte n : 1;  // 图片下标
}Jet[NUM];


struct CONTROL
{
	int styletime = 0;//控制心型释放次数
	bool show13 = false;//控制13号烟花类型切换
	bool show14 = false;//控制14号烟花类型切换
	int  locate13 = 0;//13号烟花位置变化控制量
	int  locate14 = 0;//14号烟花位置变化控制量
	bool initcon = true;//控制心型烟花定义
	int firenum = 13;//烟花数目控制
	int wordtime = 0;//播放字体时序控制量
	int num13 = 3;//13号烟花数目控制
	int num14 = 4;//14号烟花数目控制
	bool exit = false;//控制烟花循环的结束
	int hword = 0;//爱心段文字打印的控制量
}control;


/**** 函数申明区 ****/

void Init(int); // 初始化烟花
void Inito13(int);
void Inito14(int);
void Load(); // 加载烟花图片
void Shoot(); // 发射烟花
void Chose(DWORD&); // 筛选烟花
void Style(DWORD&, DWORD*pMem); // 发射样式
void Show(DWORD& st1, DWORD*pMem); // 绽放烟花
void text(int x);//图片文本
void disapear(DWORD* pMem);//消除图片
void paintword(int i, DWORD* pMem);//图片文本打印和消除
void timeword(DWORD&, const char a[], unsigned int time, unsigned int delay, int x, int y, int i);//打印文本
//void fireword(DWORD st4);
void heartword();

// 主函数
int main()
{
	initgraph(1200, 800);
	srand((unsigned int)time(0));

	// 播放背景音乐
	mciSendString("open 云烟成雨.mp3 alias ly", 0, 0, 0);
	mciSendString("play ly", 0, 0, 0);

	DWORD* pMem = GetImageBuffer(); // 获取窗口显存指针

	Sleep(1000);

	paintword(0, pMem);
    paintword(1, pMem);
	paintword(2, pMem);
    paintword(3, pMem);
    paintword(4, pMem);

	DWORD t1 = timeGetTime(); // 筛选烟花计时
	DWORD st1 = timeGetTime(); // 播放花样计时
	DWORD st4 = timeGetTime();//文字播放计时

	for (int i = 0; i < NUM + 2; i++) // 初始化烟花
	{
		Init(i);
	}
	Load();    // 将烟花图片信息加载进相应结构中
	BeginBatchDraw();   // 开始批量绘图

	//settextstyle(40, 0, "楷体");
	//setcolor(YELLOW);
	setbkmode(TRANSPARENT);
	while (!_kbhit())
	{
		Sleep(10);
		if (control.show13 || control.show14 || control.styletime == 1) {
			if (Fire->show == false && Jet->shoot == false && control.styletime != 1) {
				Sleep(1000);
				disapear(pMem);
			}
		}
		else {
			// 随机选择 4000 个像素点擦除
			for (int clr = 0; clr < 2500; clr++)
			{
				for (int j = 0; j < 2; j++)
				{
					int px1 = rand() % 1200;
					int py1 = rand() % 800;

					if (py1 < 799)  // 防止越界
						pMem[py1 * 1200 + px1] = pMem[py1 * 1200 + px1 + 1] = BLACK; // 对显存赋值擦出像素点
				}
			}
		}
		if (control.exit == true) {
			break;
		}
		Chose(t1); // 筛选烟花
		Style(st1, pMem); // 花样发射
		Shoot(); // 发射烟花
		Show(st1, pMem); // 绽放烟花
		//fireword(st4);//放烟花时的文本绘制
		heartword();
		FlushBatchDraw(); // 显示前面的所有绘图操作
	}
	EndBatchDraw();
	paintword(5, pMem);

	MOUSEMSG m;
	while (1)
	{
	    m = GetMouseMsg();
	    if (m.uMsg == WM_LBUTTONDOWN)break;
	}
	disapear(pMem);
	closegraph();

	rain();
	return 0;
}


// 初始化烟花参数
void Init(int i)
{
	// 分别为：烟花中心到图片边缘的最远距离、烟花中心到图片左上角的距离 (x、y) 两个分量
	int r[15] = { 97,97,97,97,97,90,97,97,95,97,97,97,97,126,125 };
	int x[15] = { 97,97,103,97,97,90,106,104,92,97,97,97,106,124,122 };
	int y[15] = { 102,116,98,102,104,102,101,98,100,102,104,105,101,124,122 };

	/**** 初始化烟花 *****/

	Fire[i].x = 0;  // 烟花中心坐标
	Fire[i].y = 0;
	if (i < 13) {
		Fire[i].width = 194;  // 图片宽
		Fire[i].height = 204;  // 图片高
	}
	else if (i == 13) {
		Fire[i].width = 249;  // 图片宽
		Fire[i].height = 249;  // 图片高
	}
	else if (i == 14) {
		Fire[i].width = 245;  // 图片宽
		Fire[i].height = 245;  // 图片高
	}
	Fire[i].max_r = r[i];  // 最大半径
	Fire[i].cen_x = x[i];  // 中心距左上角距离
	Fire[i].cen_y = y[i];
	Fire[i].show = false;  // 是否绽放
	Fire[i].dt = 7;  // 绽放时间间隔
	Fire[i].t1 = timeGetTime();
	Fire[i].r = 0;  // 从 0 开始绽放
	Fire[i].style = false;

	/**** 初始化烟花弹 *****/

	Jet[i].x = -133;  // 烟花弹左上角坐标
	Jet[i].y = -131;
	Jet[i].hx = -133;  // 烟花弹发射最高点坐标
	Jet[i].hy = -131;
	Jet[i].height = 0;  // 发射高度
	Jet[i].t1 = timeGetTime();
	Jet[i].dt = rand() % 10; // 发射速度时间间隔
	//Jet[i].n = 0;  // 烟花弹闪烁图片下标
	Jet[i].shoot = false;  // 是否发射
}


// 加载图片
void Load()
{
	/**** 储存烟花的像素点颜色 ****/
	IMAGE fm, gm;


	for (int i = 0; i < 15; i++)
	{
		if (i < 9) {
			loadimage(&fm, "0.jpg", 581, 612);
			SetWorkingImage(&fm);
			switch (i) {
			case 0:
			case 1:
				getimage(&gm, i * 194, 0, 194, 204);
				SetWorkingImage(&gm);
				break;
			case 2:
				getimage(&gm, (i - 1) * 194, 408, 194, 204);
				SetWorkingImage(&gm);
				break;
			case 3:
			case 4:
			case 5:
				getimage(&gm, (i - 3) * 194, 204, 194, 204);
				SetWorkingImage(&gm);
				break;
			case 6:
			case 7:
			case 8:
				getimage(&gm, (i - 6) * 194, 408, 194, 204);
				SetWorkingImage(&gm);
				break;
			}
			for (int a = 0; a < 194; a++)
				for (int b = 0; b < 204; b++)
					Fire[i].xy[a][b] = getpixel(a, b);
		}
		else if (i >= 9 && i <= 12) {
			loadimage(&fm, "0.jpg", 581, 612);
			SetWorkingImage(&fm);
			switch (i) {
			case 9:
			case 10:
				getimage(&gm, (i - 9) * 194, 204, 194, 204);
				SetWorkingImage(&gm);
				break;
			case 11:
				getimage(&gm, (i - 11) * 194, 0, 194, 204);
				SetWorkingImage(&gm);
				break;
			case 12:
				getimage(&gm, (i - 12) * 194, 408, 194, 204);
				SetWorkingImage(&gm);
				break;
			}
			for (int a = 0; a < 194; a++)
				for (int b = 0; b < 204; b++)
					Fire[i].xy[a][b] = getpixel(a, b);
		}
		else {
			switch (i) {
			case 13:
				loadimage(&fm, "02.jpg", 249, 249);
				SetWorkingImage(&fm);
				getimage(&gm, 0, 0, 249, 249);
				SetWorkingImage(&gm);
				for (int a = 0; a < 249; a++)
					for (int b = 0; b < 249; b++)
						Fire[i].xy[a][b] = getpixel(a, b);
				break;
			case 14:
				loadimage(&fm, "03.jpg", 245, 245);
				SetWorkingImage(&fm);
				getimage(&gm, 0, 0, 245, 245);
				SetWorkingImage(&gm);
				for (int a = 0; a < 245; a++)
					for (int b = 0; b < 245; b++)
						Fire[i].xy[a][b] = getpixel(a, b);
				break;
			}
		}
	}

	/**** 加载烟花弹 ************/
	IMAGE sm;
	loadimage(&sm, "1.jpg", 30, 30);

	for (int i = 0; i < NUM; i++)
	{
		SetWorkingImage(&sm);
		//int n = rand() % 5;

		getimage(&Jet[i].img, 0, 0, 79, 78);
		//getimage(&Jet[i].img[1], (n + 5) * 20, 0, 20, 50); // 亮
	}

	SetWorkingImage(); // 设置回绘图窗口
}


// 在一定范围内筛选可发射的烟花，并初始化发射参数，输出烟花弹到屏幕，播放声音
void Chose(DWORD& t1)
{
	DWORD t2 = timeGetTime();

	if (t2 - t1 > 100)
	{
		int n = rand() % 50;
		if (n < control.firenum&& Jet[n].shoot == false && Fire[n].show == false && control.show13 == false && control.show14 == false)
		{
			/**** 重置烟花弹，预备发射 *****/
			Jet[n].x = rand() % 1200;
			Jet[n].y = rand() % 100 + 600;
			Jet[n].hx = Jet[n].x;
			Jet[n].hy = rand() % 400;
			Jet[n].height = Jet[n].y - Jet[n].hy;
			Jet[n].shoot = true;
			putimage(Jet[n].x, Jet[n].y, &Jet[n].img, SRCINVERT);

		}
		else if (Jet[0].shoot == false && Jet[1].shoot == false && Jet[2].shoot == false && Fire->show == false && control.show13 == true && control.show14 == false)
		{
			/**** 规律分布烟花弹 ***/
			for (int i = 0; i < control.num13; i++) {
				if (control.num13 == 3) {
					Jet[i].x = Jet[i + control.locate13 * control.num13].x;
					Jet[i].y = Jet[i + control.locate13 * control.num13].y;
					Jet[i].hx = Jet[i + control.locate13 * control.num13].hx;
					Jet[i].hy = Jet[i + control.locate13 * control.num13].hy;
					Jet[i].height = Jet[i + control.locate13 * control.num13].height;
					Jet[i].shoot = true;
					Jet[i].dt = 7;
					//Fire[i].style = true;
					putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT); // 显示烟花弹

					/**** 设置烟花参数 ***/
					Fire[i].x = Fire[i + control.locate13 * 3].x;
					Fire[i].y = Fire[i + control.locate13 * 3].y;
					Fire[i].show = false;
					Fire[i].r = 0;
				}
				else if (control.num13 == 4) {
					Jet[i].x = Jet[i + control.locate13 * control.num13 - 3].x;
					Jet[i].y = Jet[i + control.locate13 * control.num13 - 3].y;
					Jet[i].hx = Jet[i + control.locate13 * control.num13 - 3].hx;
					Jet[i].hy = Jet[i + control.locate13 * control.num13 - 3].hy;
					Jet[i].height = Jet[i + control.locate13 * control.num13 - 3].height;
					Jet[i].shoot = true;
					Jet[i].dt = 7;
					//Fire[i].style = true;
					putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT); // 显示烟花弹

					/**** 设置烟花参数 ***/
					Fire[i].x = Fire[i + control.locate13 * 3 - 3].x;
					Fire[i].y = Fire[i + control.locate13 * 3 - 3].y;
					Fire[i].show = false;
					Fire[i].r = 0;
				}
			}
			control.locate13++;
			if (control.locate13 == 5 && control.show13 == true) {
				control.exit = true;
			}
		}
		else if (Jet[0].shoot == false && Jet[1].shoot == false && Jet[2].shoot == false && Fire->show == false && control.show13 == false && control.show14 == true)
		{
			/**** 规律分布烟花弹 ***/
			if (control.locate14 < 3) {
				for (int i = 0; i < control.num14; i++) {
					Jet[i].x = Jet[i + control.locate14 * 4].x;
					Jet[i].y = Jet[i + control.locate14 * 4].y;
					Jet[i].hx = Jet[i + control.locate14 * 4].hx;
					Jet[i].hy = Jet[i + control.locate14 * 4].hy;
					Jet[i].height = Jet[i + control.locate14 * 4].height;
					Jet[i].shoot = true;
					Jet[i].dt = 7;
					//Fire[i].style = true;
					putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT); // 显示烟花弹

					/**** 设置烟花参数 ***/
					Fire[i].x = Fire[i + control.locate14 * 4].x;
					Fire[i].y = Fire[i + control.locate14 * 4].y;
					Fire[i].show = false;
					Fire[i].r = 0;
				}
			}
			control.locate14++;
		}
		t1 = t2;
	}
}


// 扫描烟花弹并发射
void Shoot()
{
	for (int i = 0; i < control.firenum; i++)
	{
		Jet[i].t2 = timeGetTime();

		if (Jet[i].t2 - Jet[i].t1 > Jet[i].dt && Jet[i].shoot == true)
		{
			/**** 烟花弹的上升 *****/
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT);

			if (Jet[i].y > Jet[i].hy)
			{
				//Jet[i].n++;
				Jet[i].y -= 5;
			}

			putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT);

			/**** 上升到高度的 3 / 4，减速 *****/
			if ((Jet[i].y - Jet[i].hy) * 4 < Jet[i].height)
				Jet[i].dt = rand() % 4 + 10;

			/**** 上升到最大高度 *****/
			if (Jet[i].y <= Jet[i].hy)
			{
				// 播放爆炸声
				char c1[50], c2[30], c3[30];
				printf(c1, "open ./fire/bomb.wav alias n%d", i);
				printf(c2, "play n%d", i);
				printf(c3, "close s%d", i);

				if (Fire[i].style == false) {
					putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT); // 擦掉烟花弹
				}
				Fire[i].x = Jet[i].hx + 10;      // 在烟花弹中间爆炸
				Fire[i].y = Jet[i].hy;      // 在最高点绽放
				Fire[i].show = true;   // 开始绽放
				Jet[i].shoot = false;   // 停止发射

			}
			Jet[i].t1 = Jet[i].t2;
		}
	}
}


// 显示花样
void Style(DWORD& st1, DWORD*pMem)
{
	DWORD st2 = timeGetTime();

	if (st2 - st1 > 65000 && control.styletime == 0) // 一首歌的时间
	{
		// 心形坐标
		int x[13] = { 60, 75, 91, 100, 95, 75, 60, 45, 25, 15, 25, 41, 60 };
		int y[13] = { 65, 53, 40, 22, 5, 4, 20, 4, 5, 22, 40, 53, 65 };
		for (int i = 0; i < 13; i++)
		{
			cleardevice();
			/**** 规律分布烟花弹 ***/
			Jet[i].x = x[i] * 10;
			Jet[i].y = (y[i] + 75) * 10;
			Jet[i].hx = Jet[i].x;
			Jet[i].hy = y[i] * 10;
			Jet[i].height = Jet[i].y - Jet[i].hy;
			Jet[i].shoot = true;
			Jet[i].dt = 7;
			Fire[i].style = true;
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT); // 显示烟花弹

			/**** 设置烟花参数 ***/
			Fire[i].x = Jet[i].x + 10;
			Fire[i].y = Jet[i].hy;
			Fire[i].show = false;
			Fire[i].r = 0;

		}
		st1 = st2;
		control.styletime = 1;
	}
}


// 绽放烟花
void Show(DWORD& st1, DWORD* pMem)
{
	// 烟花个阶段绽放时间间隔，制作变速绽放效果
	int drt[16] = { 5, 5, 5, 5, 5, 6, 25, 25, 25, 25, 55, 55, 55, 55, 55 };
	DWORD st3 = timeGetTime();
	if (control.initcon == false && control.styletime == 1) {
		control.show13 = true;
		control.styletime = 2;
	}
	for (int i = 0; i < control.firenum; i++)
	{
		Fire[i].t2 = timeGetTime();

		// 增加爆炸半径，绽放烟花，增加时间间隔做变速效果
		if (Fire[i].t2 - Fire[i].t1 > Fire[i].dt && Fire[i].show == true)
		{
			if (Fire[i].r < Fire[i].max_r)
			{
				Fire[i].r++;
				Fire[i].dt = drt[Fire[i].r / 10];

				Fire[i].draw = true;
			}

			if (Fire[i].r >= Fire[i].max_r - 1 && control.show14 == false && control.show13 == false)
			{
				Fire[i].draw = false;
				Init(i);
			}
			else if (Fire[i].r >= Fire[i].max_r - 1 && control.show14 == true && control.show13 == false)
			{
				Fire[i].draw = false;
				Inito14(i);
			}
			else if (Fire[i].r >= Fire[i].max_r - 1 && control.show14 == false && control.show13 == true)
			{
				Fire[i].draw = false;
				Inito13(i);
			}
			Fire[i].t1 = Fire[i].t2;
		}

		// 如果该号炮花可爆炸，根据当前爆炸半径画烟花，颜色值接近黑色的不输出。
		if (Fire[i].draw)
		{

			for (double a = 0; a <= 6.28; a += 0.01)
			{
				int x1 = (int)(Fire[i].cen_x + Fire[i].r * cos(a));  // 相对于图片左上角的坐标
				int y1 = (int)(Fire[i].cen_y - Fire[i].r * sin(a));

				if (x1 > 0 && x1 < Fire[i].width && y1 > 0 && y1 < Fire[i].height) // 只输出图片内的像素点
				{
					int b;
					int g;
					int r;
					if (control.show14) {
						b = Fire[14].xy[x1][y1] & 0xff;
						g = (Fire[14].xy[x1][y1] >> 8) & 0xff;
						r = (Fire[14].xy[x1][y1] >> 16);
					}
					else if (control.show13) {
						b = Fire[13].xy[x1][y1] & 0xff;
						g = (Fire[13].xy[x1][y1] >> 8) & 0xff;
						r = (Fire[13].xy[x1][y1] >> 16);
					}
					else {
						b = Fire[i].xy[x1][y1] & 0xff;
						g = (Fire[i].xy[x1][y1] >> 8) & 0xff;
						r = (Fire[i].xy[x1][y1] >> 16);
					}

					// 烟花像素点在窗口上的坐标
					int xx = (int)(Fire[i].x + Fire[i].r * cos(a));
					int yy = (int)(Fire[i].y - Fire[i].r * sin(a));

					// 较暗的像素点不输出、防止越界
					if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && xx < 1200 && yy > 0 && yy < 800 && !Fire[i].style&&control.show14 == false && control.show13 == false) {
						pMem[yy * 1200 + xx] = BGR(Fire[i].xy[x1][y1]); // 显存操作绘制烟花
					}
					else if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && xx < 1200 && yy > 0 && yy < 800 && Fire[i].style) {
						//pMem[yy * 1200 + xx] = BGR(Fire[14].xy[x1][y1]); // 显存操作绘制烟花
						Sleep(1500);
						disapear(pMem);
						//cleardevice();
						control.styletime = 2;
						control.show14 = true;
						control.firenum = control.num14;
						for (int i = 0; i < 12; i++) {
							Inito14(i);
							Fire[i].style = false;
						}
						mciSendString("close ly", 0, 0, 0);
						mciSendString("open 麋鹿森林.mp3 alias fz", 0, 0, 0);
						mciSendString("play fz", 0, 0, 0);
					}
					else if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && xx < 1200 && yy > 0 && yy < 800 && Fire[i].style == false && control.show14 == true) {
						pMem[yy * 1200 + xx] = BGR(Fire[14].xy[x1][y1]);
						if (st3 - st1 > 32000) {
							control.show13 = true;
							control.show14 = false;
							control.firenum = control.num13;
							for (int i = 0; i < 13; i++) {
								Inito13(i);
							}
							control.initcon = false;
						}
					}
					else if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && xx < 1200 && yy > 0 && yy < 800 && Fire[i].style == false && control.show13 == true) {
						pMem[yy * 1200 + xx] = BGR(Fire[13].xy[x1][y1]);
					}
				}
			}
		}
		Fire[i].draw = false;
		if (i + control.locate13 * control.num13 == 9 && control.show13 == true) {
			control.firenum = 4;
			control.num13 = 4;
		}
	}
}



//设置文本
void text(int i) {
	IMAGE bk;
	int a = 130;
	int b = 650;
	int time = 2000;
	switch (i) {
	case 0:
		a = 130;
		b = 650;
		loadimage(&bk, "bk0.jpg", 1200, 800);
		putimage(0, 0, &bk);
		//setfillstyle(0);
		settextstyle(30, 0, "楷体");
		setcolor(LIGHTMAGENTA);
		setbkmode(TRANSPARENT);
		Sleep(time);
		outtextxy(a, 150, "哈喽？你好\\(￣︶￣*\\))");
		Sleep(time);
		outtextxy(a, 200, "当你看到这部作品的时候");
		Sleep(time + 500);
		outtextxy(a, 250, "表明你已经收到礼物了");
		Sleep(time + 500);
		outtextxy(a, 300, "给你发这个没有其他什么想法");
		Sleep(time + 500);
		outtextxy(a, 350, "我只是记得自己曾答应了你");
		Sleep(time + 500);
		outtextxy(a, 400, "要给你生日礼物");
		Sleep(time);
		outtextxy(a, 450, "无它（小声bb：只是想省个红包2333）");
		Sleep(time);
		outtextxy(a, 500, "只是觉得");
		Sleep(time);
		outtextxy(a, 550, "有些东西答应了应该要做到ψ(._. )>");
		Sleep(time + 500);
		outtextxy(b, 200, "算一算的话(⊙-⊙)");
		Sleep(time);
		outtextxy(b, 250, "都半年多了诶");
		Sleep(time);
		outtextxy(b, 300, "感觉日子过得飞快");
		Sleep(time);
		outtextxy(b, 350, "好像昨天还聊着天一样2333");
		Sleep(time);
		outtextxy(b, 400, "不过我也没空想这些〒▽〒");
		Sleep(time);
		outtextxy(b, 450, "这段时间你过的怎么样");
		Sleep(time);
		outtextxy(b, 500, "应该还活着吧你哈哈哈");
		Sleep(time + 1500);
		break;
	case 1:
		a = 80;
		b = 850;
		loadimage(&bk, "bk1.jpg", 1200, 800);
		putimage(0, 0, &bk);
		//setfillstyle(0);
		settextstyle(30, 0, "楷体");
		setcolor(LIGHTRED);
		setbkmode(TRANSPARENT);
		Sleep(time);
		outtextxy(a, 150, "感受到了吗┗|｀O′|┛ 嗷~~");
		Sleep(time);
		outtextxy(a, 200, "这个我制作的五毛钱大场面");
		Sleep(time + 500);
		outtextxy(a, 250, "java和c#没学到家，只能用c将就了哈哈哈");
		Sleep(time);
		outtextxy(a, 300, "反正中二是不可能不中二的");
		Sleep(time + 500);
		outtextxy(a, 350, "说话又好听= _ =");
		Sleep(time);
		outtextxy(a, 400, "一起开拖拉机去摆地摊吧什么的傻话");
		Sleep(time + 500);
		outtextxy(a, 450, "又不可能不说←_←");
		Sleep(time);
		outtextxy(a, 500, "所以有时间找个风口");
		Sleep(time);
		outtextxy(a, 550, "一起去做猪怎么样");
		Sleep(time+ 1500);
		break;
	case 2:
		a = 70;
		b = 70;
		loadimage(&bk, "bk2.jpg", 1200, 800);
		putimage(0, 0, &bk);
		//setfillstyle(0);
		settextstyle(30, 0, "楷体");
		setcolor(BROWN);
		setbkmode(TRANSPARENT);
		Sleep(time);
		outtextxy(a, 50, "我说会儿人话(￣、￣)");
		Sleep(time);
		outtextxy(a, 100, "这年头连冒充儿童过儿童节都犯罪了");
		Sleep(time);
		outtextxy(a, 150, "不好好说话可能会被拉去毙了(→_→)");
		Sleep(time);
		outtextxy(a, 200, "之前那么尬的陪我聊天真是辛苦你了");
		Sleep(time + 500);
		outtextxy(a, 250, "感觉自己在感情方面还是很幼稚哈哈");
		Sleep(time + 500);
		outtextxy(a, 300, "不过管不了，管不了自己哈哈(ˉ﹃ˉ)");
		Sleep(time);
		outtextxy(a, 350, "那时候确实是闲的2333←_←");
		Sleep(time);
		outtextxy(b, 400, "不过以后估计都没时间了哈哈");
		Sleep(time);
		outtextxy(b, 450, "只想努力学习去搞钱的我");
		Sleep(time + 500);
		outtextxy(b, 500, "对感情真的毫无办法哈哈");
		Sleep(time);
		outtextxy(b, 550, "不过至少我还记得你生日ㄟ( ▔, ▔ )ㄏ");
		//Sleep(time);
		//outtextxy(b, 600, "");
		Sleep(time + 1500);
		break;
	case 3:
		loadimage(&bk, "bk3.jpg", 1200, 800);
		putimage(0, 0, &bk);
		//setfillstyle(0);
		settextstyle(35, 0, "楷体");
		setcolor(LIGHTGREEN);
		setbkmode(TRANSPARENT);
		Sleep(time);
		outtextxy(470, 100, "但是我希望你");
		Sleep(time);
		outtextxy(400, 150, "总是能开开心心的（￣︶￣）↗　");
		Sleep(time);
		outtextxy(380, 200, "虽然你不是我第一个喜欢的人");
		Sleep(time);
		outtextxy(320, 250, "但你是我表白失败后还能说话的第一人");
		Sleep(time);
		outtextxy(460, 300, "我知道ψ(._. )>");
		Sleep(time);
		outtextxy(480, 350, "很多时候");
		Sleep(time);
		outtextxy(440, 400, "你仅是出于礼貌→_→");
		Sleep(time);
		outtextxy(480, 450, "但是确实=.=");
		Sleep(time);
		outtextxy(300, 500, "你没有删我微信,也没有用语言伤害我");
		Sleep(time);
		outtextxy(360, 550, "所以我确实还是很想谢谢你的");
		Sleep(time + 1500);
		break;
	case 4:
		a = 400;
		loadimage(&bk, "bk4.jpg", 1200, 800);
		putimage(0, 0, &bk);
		//setfillstyle(0);
		settextstyle(35, 0, "楷体");
		setcolor(LIGHTCYAN);
		setbkmode(TRANSPARENT);
		Sleep(time);
		outtextxy(a, 150, "老实说(￣_ ￣ )");
		Sleep(time);
		outtextxy(a-80, 200, "你这人真的挺有趣的");
		Sleep(time + 500);
		outtextxy(a-120, 250, "也希望你一直都这么开心(￣▽￣)\"");
		Sleep(time);
		outtextxy(a, 300, "不用长大啊");
		Sleep(time);
		outtextxy(a-80, 350, "早日找到宠自己的那个人");
		Sleep(time + 500);
		outtextxy(a, 400, "年年十八 > <");
		Sleep(time);
		outtextxy(a, 450, "越长越漂亮");
		Sleep(time);
		outtextxy(a-150, 500, "早日一米八哈哈（想想就害怕(￣_￣|||)）");
		Sleep(time+2000);
		putimage(0, 0, &bk);
		settextstyle(50, 0, "楷体");
		Sleep(time);
		outtextxy(400, 200, "梁滢（￣︶￣）↗　！！！");
		Sleep(time);
		outtextxy(350, 350, "祝你生日快乐ψ(._. )>!!!");
		Sleep(time+1500);
		break;
	case 5:
		loadimage(&bk, "bk5.jpg", 1200, 800);
		putimage(0, 0, &bk);
		settextstyle(35, 0, "楷体");
		setcolor(LIGHTCYAN);
		setbkmode(TRANSPARENT);

		Sleep(time);
		outtextxy(300, 100, "讲道理(￣、￣)");
		Sleep(time);
		outtextxy(250, 150, "这东西花了我不少功夫");
		Sleep(time);
		outtextxy(250, 200, "所以生日要快乐知道嘛");
		Sleep(time);
		outtextxy(200, 250, "不快乐我就锤你(╯‵□′)╯︵┻━┻");
		Sleep(time);
		outtextxy(300, 300, "最后(￣、￣)");
		Sleep(time);
		outtextxy(300, 350, "还有一场彩蛋");
		Sleep(time+1000);
		putimage(0, 0, &bk);
		settextstyle(40, 0, "楷体");
		outtextxy(420, 350, "(o゜▽゜)o☆");
		outtextxy(350, 400, "(鼠标左键点击打开彩蛋)");
		break;
	}
}

//消除文本
void disapear(DWORD* pMem) {
	Sleep(1000);
	for (int i = 0; i < 10; i++)
	{
		for (int clr = 0; clr < 250000; clr++)
		{
			for (int j = 0; j < 2; j++)
			{
				int px1 = rand() % 1200;
				int py1 = rand() % 800;

				if (py1 < 799)  // 防止越界
					pMem[py1 * 1200 + px1] = pMem[py1 * 1200 + px1 + 1] = BLACK; // 对显存赋值擦出像素点
			}
		}
		FlushBatchDraw();
		Sleep(100);
	}
}

//打印文本
void paintword(int i, DWORD* pMem) {
	if (i <= 4) {
		text(i);
		Sleep(2000);
		disapear(pMem);
	}
	else if (i == 5) {
		text(i);
	}
}

//绽放初始化
void Inito13(int i) {
	int r[13] = { 126,126,126,126,126,126,126,126,126,126,126,126,126 };
	int x[13] = { 124,124,124,124,124,124,124,124,124,124,124,124,124 };
	int y[13] = { 124,124,124,124,124,124,124,124,124,124,124,124,124 };

	int jx[13] = { 50,60,70,45,60,75,30,60,90,20,35,85,100 };
	int jy[13] = { 30,40,30,30,30,30,30,40,50,30,30,30,30 };

	/**** 初始化烟花 *****/
	//Fire[i].x = 0;  // 烟花中心坐标
	//Fire[i].y = 0;
	Fire[i].width = 249;  // 图片宽
	Fire[i].height = 249;  // 图片高
	Fire[i].max_r = r[i];  // 最大半径
	Fire[i].cen_x = x[i];  // 中心距左上角距离
	Fire[i].cen_y = y[i];
	Fire[i].show = false;  // 是否绽放
	//Fire[i].dt = 7;  // 绽放时间间隔
	Fire[i].t1 = timeGetTime();
	//Fire[i].r = 0;  // 从 0 开始绽放
	//Fire[i].style = false;

				/**** 规律分布烟花弹 ***/
	Jet[i].x = jx[i] * 10;
	Jet[i].y = (jy[i] + 75) * 10;
	Jet[i].hx = Jet[i].x;
	Jet[i].hy = jy[i] * 10;
	Jet[i].height = Jet[i].y - Jet[i].hy;
	Jet[i].shoot = false;
	Jet[i].dt = 7;
	//Fire[i].style = true;
	putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT); // 显示烟花弹

	/**** 设置烟花参数 ***/
	Fire[i].x = Jet[i].x + 10;
	Fire[i].y = Jet[i].hy;
	Fire[i].show = false;
	Fire[i].r = 0;
}

void Inito14(int i) {
	int r[12] = { 125,125,125,125,125,125,125,125,125,125,125,125 };
	int x[12] = { 122,122,122 ,122 ,122 ,122 ,122 ,122 ,122 ,122 ,122 ,122 };
	int y[12] = { 122,122,122,122,122,122,122,122,122,122,122,122 };

	int jx[12] = { 20,50,65,100,20,35,85,100,30,50,70,90 };
	int jy[12] = { 25,35,35,25,35,35,35,35,30,50,30,50 };

	/**** 初始化烟花 *****/
	//Fire[i].x = 0;  // 烟花中心坐标
	//Fire[i].y = 0;
	Fire[i].width = 245;  // 图片宽
	Fire[i].height = 245;  // 图片高
	Fire[i].max_r = r[i];  // 最大半径
	Fire[i].cen_x = x[i];  // 中心距左上角距离
	Fire[i].cen_y = y[i];
	Fire[i].show = false;  // 是否绽放
	//Fire[i].dt = 7;  // 绽放时间间隔
	Fire[i].t1 = timeGetTime();
	//Fire[i].r = 0;  // 从 0 开始绽放
	//Fire[i].style = false;

				/**** 规律分布烟花弹 ***/
	Jet[i].x = jx[i] * 10;
	Jet[i].y = (jy[i] + 75) * 10;
	Jet[i].hx = Jet[i].x;
	Jet[i].hy = jy[i] * 10;
	Jet[i].height = Jet[i].y - Jet[i].hy;
	Jet[i].shoot = false;
	Jet[i].dt = 7;
	//Fire[i].style = true;
	putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT); // 显示烟花弹

	/**** 设置烟花参数 ***/
	Fire[i].x = Jet[i].x + 10;
	Fire[i].y = Jet[i].hy;
	Fire[i].show = false;
	Fire[i].r = 0;
}

//控制时间并输出文字
void timeword(DWORD& st4, const char a[], unsigned int time, unsigned int delay, int x, int y, int i) {
	DWORD st5 = timeGetTime();
	if (st5 - st4 > time&&control.wordtime == i) {
		if (st5 - st4 - time < 2000) {
			settextstyle(50, 0, "楷体");
			setcolor(YELLOW);
		}
		else {
			settextstyle(50, 0, "楷体");
			setcolor(LIGHTCYAN);
		}
		//setbkmode(TRANSPARENT);
		outtextxy(x, y, a);
		if (st5 - st4 - time > delay) {
			control.wordtime++;
		}
	}
}

//烟花段文字
//蒜了蒜了
/*
void fireword(DWORD st4) {

	timeword(st4, "滢，我们从高一认识到现在已经三年半了吧", 3000, 20000, 300, 100, 0);
	timeword(st4, "时间如白驹过隙", 5000, 100000, 300, 150, 0);
	timeword(st4, "一眨眼我们就各自长大了", 7000, 100000, 300, 200, 0);
	timeword(st4, "我也不知道自己", 9000, 100000, 300, 250, 0);
	timeword(st4, "为什么", 11000, 100000, 300, 300, 0);
	timeword(st4, "那一天会给你发那个信息说要加你微信", 13000, 100000, 300, 350, 0);
	timeword(st4, "但好像很多事情都是不需要理由的对么", 15000, 100000, 300, 400, 0);
	timeword(st4, "我们交集不多", 17000, 100000, 300, 450, 0);
	timeword(st4, "但每一个与你相遇的瞬间我都记得很清楚啊", 19000, 100000, 300, 500, 0);

	timeword(st4, "高一运动会受伤的你", 27000, 22000, 300, 100, 1);
	timeword(st4, "向我问物理问题的你", 29000, 100000, 300, 150, 1);
	timeword(st4, "回宿舍路上像个傻逼嘻嘻哈哈的你", 31000, 100000, 300, 200, 1);
	timeword(st4, "高二运动会要我蹲下来跟你合照的你", 33000, 100000, 300, 250, 1);
	timeword(st4, "跳蚤市场上把书直接报底价给我的你", 35000, 100000, 300, 300, 1);
	timeword(st4, "高三总要请假的你", 37000, 100000, 300, 350, 1);
	timeword(st4, "英语听力课上电脑室里的你", 39000, 100000, 300, 400, 1);
	timeword(st4, "总是披头散发，高考前却突然扎起马尾的你", 41000, 100000, 300, 450, 1);
	timeword(st4, "我才发现", 43000, 100000, 300, 500, 1);
	timeword(st4, "原来我对你的每一次相遇都那么的在意", 45000, 100000, 300, 550, 1);

	timeword(st4, "但你不会知道", 55000, 24000, 300, 100, 2);
	timeword(st4, "高一运动会上偷拍你的我", 57000, 100000, 300, 150, 2);
	timeword(st4, "高一暑假东莞看同学惊喜发现你也在东莞的我", 59000, 100000, 300, 200, 2);
	timeword(st4, "突然鼓起所有勇气去找你的我", 61000, 100000, 300, 250, 2);
	timeword(st4, "高二脑子坏掉的我", 63000, 100000, 300, 300, 2);
	timeword(st4, "高三英语听力课跑去你们班蹭电脑偷偷看你的我", 65000, 100000, 300, 350, 2);
	timeword(st4, "在级室偷看你作业还留字的我", 67000, 100000, 300, 400, 2);
	timeword(st4, "走路上等你擦肩而过才想起打招呼的我", 69000, 100000, 300, 450, 2);
	timeword(st4, "总跟舍友说你有多好的我", 71000, 100000, 300, 500, 2);
	timeword(st4, "看了你的信息而笑出声的我", 73000, 100000, 300, 550, 2);
	timeword(st4, "发现自己喜欢你的我", 75000, 100000, 300, 600, 2);

}
*/


//爱心段文字
void heartword() {
	settextstyle(40, 0, "楷体");
	switch (control.locate14)
	{
	case 1:
		settextcolor(LIGHTCYAN);
		outtextxy(130, 220, "一路走来");
		outtextxy(930, 220, "一路花开");
		break;
	case 2:
		settextcolor(LIGHTGRAY);
		outtextxy(450, 200, "你一定要过得好");
		outtextxy(450, 500, "别辜负我的不打扰");
		break;
	case 3:
		settextcolor(WHITE);
		outtextxy(200, 600, "谁敢折了你的翅膀");
		outtextxy(550, 700, "我定废了ta整个天堂");
		control.locate14++;
		break;
	}
	switch (control.locate13)
	{
	case 1:
		settextcolor(LIGHTRED);
		outtextxy(150, 300, "时光若水");
		outtextxy(150, 400, "总是无言");
		outtextxy(850, 300, "你若安好");
		outtextxy(800, 400, "便是晴天");
		break;
	case 2:
		settextcolor(LIGHTMAGENTA);
		outtextxy(320, 500, "总之岁月漫长");
		outtextxy(570, 600, "然而值得等待");
		break;
	case 3:
		settextcolor(LIGHTGRAY);
		outtextxy(200, 500, "有我在");
		outtextxy(700, 200, "你就不用长大");
		break;
	case 4:
		settextcolor(WHITE);
		outtextxy(100, 500, "别为不属于你的观众");
		outtextxy(700, 600, "演你不擅长的人生");
		break;
	}
}