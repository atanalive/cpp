#include <graphics.h>
#include <conio.h>
//#include <iostream.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <Mmsystem.h> // 使用该计时器必须包含的文件
#pragma comment ( lib, "Winmm.lib" )

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

	bool show=false;  // 是否绽放
	bool draw;  // 开始输出像素点
	bool style = false;//心型烟花类型控制
	DWORD t1, t2, dt; // 绽放速度
}Fire[NUM];

// 烟花弹结构
struct JET
{
	int x, y;  // 喷射点坐标
	int hx, hy;  // 最高点坐标------将赋值给 FIRE 里面的 x, y
	int height;  // 烟花高度
	bool shoot=false;  // 是否可以发射

	DWORD t1, t2, dt; // 发射速度
	IMAGE img;  // 储存花弹一亮一暗图片
	//byte n : 1;  // 图片下标
}Jet[NUM];

// happy birthday 艺术字结构
struct HAPPY  // 储存 Happy Birthday 艺术字图片
{
	int x, y;  // 每个字的图片的左上角坐标
	IMAGE img;  // 储存单个字图片
	static int num;  // 字母序号
}Happy[NUM];
int HAPPY::num = 0;

// For.2012の夭女 结构
struct WISH   // 滚动祝福图片结构
{
	int x, y;  // 图片左上角坐标
	DWORD t1, t2, dt; // 图片运动时间间隔
	IMAGE img;  // 储存图片
	int dxy;  // 每次的移动量
	byte dir : 1;  // 限制两个滚动方向
}Wish;

struct CONTROL
{
	int styletime = 0;//控制心型释放次数
	bool show13 = false;//控制13号烟花类型切换
	bool show14 = false;//控制14号烟花类型切换
	int  locate13 = 0;//13号烟花位置变化控制量
	int  locate14 = 0;//14号烟花位置变化控制量
	bool initcon = true;//控制心型烟花定义
	int firenum=13;//烟花数目控制
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
void Wishing(); // 滚动祝福
void Style(DWORD&, DWORD*pMem); // 发射样式
void Show(DWORD& st1, DWORD*pMem); // 绽放烟花
void text(int x);//图片文本
void disapear(DWORD* pMem);//消除图片
void paintword(int i, DWORD* pMem);//图片文本打印和消除
void timeword(DWORD&,const char a[],int time,int delay,int x,int y,int i);//打印文本
void fireword(DWORD st4);
void heartword();

// 主函数
void main()
{
	initgraph(1200, 800);
	srand(time(0));

	// 播放背景音乐
	mciSendString("open ying.mp3 alias ly", 0, 0, 0);
	mciSendString("play ly", 0, 0, 0);

	DWORD* pMem = GetImageBuffer(); // 获取窗口显存指针

	Sleep(1000);
	paintword(0, pMem);
	paintword(1, pMem);
	paintword(2, pMem);
	paintword(3, pMem);

	DWORD t1 = timeGetTime(); // 筛选烟花计时
	DWORD st1 = timeGetTime(); // 播放花样计时
	DWORD st4 = timeGetTime();//文字播放计时

	for (int i = 0; i < NUM+2; i++) // 初始化烟花
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
		if (control.show13 || control.show14||control.styletime==1) {
			if (Fire->show == false&&Jet->shoot==false&&control.styletime!=1) {
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
		Style(st1,pMem); // 花样发射
		Shoot(); // 发射烟花
		Show(st1,pMem); // 绽放烟花
		//Wishing(); // 滚动字符
		fireword(st4);//放烟花时的文本绘制
		heartword();
		FlushBatchDraw(); // 显示前面的所有绘图操作
	}
	EndBatchDraw();
	paintword(4, pMem);
	while (1);
}


// 初始化烟花参数
void Init(int i)
{
	// 分别为：烟花中心到图片边缘的最远距离、烟花中心到图片左上角的距离 (x、y) 两个分量
	int r[15] = { 97,97,97,97,97,90,97,97,95,97,97,97,97,126,125};
	int x[15] = { 97,97,103,97,97,90,106,104,92,97,97,97,106,124,122};
	int y[15] = {102,116,98,102,104,102,101,98,100,102,104,105,101,124,122};

	/**** 初始化烟花 *****/

	Fire[i].x = 0;  // 烟花中心坐标
	Fire[i].y = 0;
	if (i < 13) {
		Fire[i].width = 194;  // 图片宽
		Fire[i].height = 204;  // 图片高
	}
	else if(i==13){
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
		if(i<9){
		loadimage(&fm, "0.jpg", 581, 612);
		SetWorkingImage(&fm);
		switch (i) {
		case 0:
		case 1:
			getimage(&gm, i * 194, 0, 194, 204);
			SetWorkingImage(&gm);
			break;
		case 2:
			getimage(&gm, (i-1)* 194, 408, 194, 204);
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
		else if (i >= 9&&i <= 12) {
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
			switch(i) {
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

	/***** 加载 Happy Birthday 图片 ********/
	IMAGE hm;
	loadimage(&hm, ".jpg", 1920,1200);
	SetWorkingImage(&hm);

	for (int i = 0; i < 13; i++)
	{
		Happy[i].x = i * 90;
		Happy[i].y = rand() % 100 + 500;
		getimage(&Happy[i].img, i * 53, 0, 53, 115);
	}

	/***** 加载 For.2012の夭女 图片 *********/
	Wish.x = 0;
	Wish.y = 100;
	Wish.t1 = timeGetTime();
	Wish.dt = 46;
	Wish.dir = 0;
	Wish.dxy = rand() % 8 + 1;
	loadimage(&Wish.img, ".jpg", 1920, 1200);
	putimage(Wish.x, Wish.y, &Wish.img, SRCINVERT);

	SetWorkingImage(); // 设置回绘图窗口
}


// 在一定范围内筛选可发射的烟花，并初始化发射参数，输出烟花弹到屏幕，播放声音
void Chose(DWORD& t1)
{
	DWORD t2 = timeGetTime();

	if (t2 - t1 > 100)
	{
		int n = rand() % 50;
		if (n < control.firenum&& Jet[n].shoot == false && Fire[n].show == false&&control.show13==false&&control.show14==false)
		{
			/**** 重置烟花弹，预备发射 *****/
			Jet[n].x = rand() % 1200;
			Jet[n].y = rand() % 100 + 600;
			Jet[n].hx = Jet[n].x;
			Jet[n].hy = rand() % 400;
			Jet[n].height = Jet[n].y - Jet[n].hy;
			Jet[n].shoot = true;
			putimage(Jet[n].x, Jet[n].y, &Jet[n].img, SRCINVERT);

			/**** 播放每个烟花弹的声音 *****/
			char c1[50], c2[30], c3[30];
			printf(c1, "open ./fire/shoot.mp3 alias s%d", n);
			printf(c2, "play s%d", n);
			printf(c3, "close n%d", n);

			//mciSendString(c3, 0, 0, 0);
			//mciSendString(c1, 0, 0, 0);
			//mciSendString(c2, 0, 0, 0);
		}
		else if (Jet[0].shoot == false && Jet[1].shoot == false && Jet[2].shoot == false && Fire->show == false && control.show13==true && control.show14==false )
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
					Jet[i].x = Jet[i + control.locate13 * control.num13-3].x;
					Jet[i].y = Jet[i + control.locate13 * control.num13-3].y;
					Jet[i].hx = Jet[i + control.locate13 * control.num13-3].hx;
					Jet[i].hy = Jet[i + control.locate13 * control.num13-3].hy;
					Jet[i].height = Jet[i + control.locate13 * control.num13-3].height;
					Jet[i].shoot = true;
					Jet[i].dt = 7;
					//Fire[i].style = true;
					putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT); // 显示烟花弹

					/**** 设置烟花参数 ***/
					Fire[i].x = Fire[i + control.locate13 * 3-3].x;
					Fire[i].y = Fire[i + control.locate13 * 3-3].y;
					Fire[i].show = false;
					Fire[i].r = 0;
				}
			}
				control.locate13++;
				if (control.locate13 == 5 && control.show13 == true) {
					control.exit = true;
					//control.show13 = false;
					//control.firenum = 13;
					//for (int i = 0; i < 13; i++) {
					//	Init(i);
					//}
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

				//mciSendString(c3, 0, 0, 0);
				//mciSendString(c1, 0, 0, 0);
				//mciSendString(c2, 0, 0, 0);

				if (Fire[i].style == false) {
					putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT); // 擦掉烟花弹
				}
				Fire[i].x = Jet[i].hx +10;      // 在烟花弹中间爆炸
				Fire[i].y = Jet[i].hy;      // 在最高点绽放
				Fire[i].show = true;   // 开始绽放
				Jet[i].shoot = false;   // 停止发射

				// 显示对应字母
				putimage(Happy[HAPPY::num].x, Happy[HAPPY::num].y, &Happy[HAPPY::num].img, SRCINVERT);
				HAPPY::num++;

				if (HAPPY::num > 12)
					HAPPY::num = 0;
			}
			Jet[i].t1 = Jet[i].t2;
		}
	}
}


// 制作水平、竖直移动的图片
void Wishing()
{
	Wish.t2 = timeGetTime();

	if (Wish.t2 - Wish.t1 > Wish.dt)
	{
		/**** 图片的移动 *****/
		putimage(Wish.x, Wish.y, &Wish.img, SRCINVERT);

		if (Wish.dir == 0)
			Wish.x += Wish.dxy;
		else
			Wish.y -= Wish.dxy;

		putimage(Wish.x, Wish.y, &Wish.img, SRCINVERT);

		/**** 图片出界后 *****/
		if (Wish.x > 1200 || Wish.y < 0)
		{
			Wish.dir = rand() % 2;  // 获取随机方向

			if (Wish.dir == 0)  // 如果左右运动
			{
				Wish.y = rand() % 700; // 初始位置
				Wish.x = -490;
				Wish.dxy = rand() % 7 + 1; // 随机运动分量
			}
			else    // 上下运动
			{
				Wish.dxy = rand() % 7 + 1;
				Wish.x = rand() % 700;
				Wish.y = 800;
			}
		}
		Wish.t1 = Wish.t2;
	}
}


// 显示花样
void Style(DWORD& st1,DWORD*pMem)
{
	DWORD st2 = timeGetTime();

	if (st2 - st1 > 77000&&control.styletime==0) // 一首歌的时间
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

			/**** 播放发射声音 ***/
			char c1[50], c2[30], c3[30];
			printf(c1, "open ./fire/shoot.mp3 alias s%d", i);
			printf(c2, "play s%d", i);
			printf(c3, "close n%d", i);

			//mciSendString(c3, 0, 0, 0);
			//mciSendString(c1, 0, 0, 0);
			//mciSendString(c2, 0, 0, 0);
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
	if (control.initcon == false &&control.styletime==1) {
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

			if (Fire[i].r >= Fire[i].max_r - 1&& control.show14 == false&&control.show13==false)
			{
				Fire[i].draw = false;
				Init(i);
			}
			else if (Fire[i].r >= Fire[i].max_r - 1&& control.show14 == true&& control.show13 == false)
			{
				Fire[i].draw = false;
				Inito14(i);
			}
			else if (Fire[i].r >= Fire[i].max_r - 1 && control.show14 ==false&& control.show13 ==true)
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
					if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && xx < 1200 && yy > 0 && yy < 800 && !Fire[i].style&&control.show14 == false&&control.show13==false) {
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
						mciSendString("open liang.mp3 alias fz", 0, 0, 0);
						mciSendString("play fz", 0, 0, 0);
					}
					else if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && xx < 1200 && yy > 0 && yy < 800 && Fire[i].style==false&&control.show14 == true) {
						pMem[yy * 1200 + xx] = BGR(Fire[14].xy[x1][y1]);
						if (st3 - st1 >34500) {
							control.show13 = true;
							control.show14 = false;
							control.firenum = control.num13;
							for (int i = 0; i < 13; i++) {
								Inito13(i);
							}
							control.initcon = false;
						}
					}
					else if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && xx < 1200 && yy > 0 && yy < 800 && Fire[i].style==false&&control.show13 == true) {
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
	switch(i) {
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
		outtextxy(a, 150, "小怪兽啊");
		Sleep(time);
		outtextxy(a, 200, "我不知道");
		Sleep(time);
		outtextxy(a, 250, "当你看到这部作品的时候");
		Sleep(time);
		outtextxy(a, 300, "是情人节，七夕节，你生日");
		Sleep(time+500);
		outtextxy(a, 350, "抑或只是一个普普通通的日子");
		Sleep(time+500);
		outtextxy(a, 400, "我也不知道");
		Sleep(time);
		outtextxy(a, 450, "你的心情");
		Sleep(time);
		outtextxy(a, 500, "是开心，激动，愧疚，尴尬");
		Sleep(time+500);
		outtextxy(a, 550, "抑或只是平静湖面不起一点波澜");
		Sleep(time+500);
		outtextxy(b, 200, "但无论如何");
		Sleep(time);
		outtextxy(b, 250, "它代表我喜欢过你");
		Sleep(time);
		outtextxy(b, 300, "无论是过去");
		Sleep(time);
		outtextxy(b, 350, "还是现在");
		Sleep(time);
		outtextxy(b, 400, "因为你");
		Sleep(time);
		outtextxy(b, 450, "有我一听到就心颤的");
		Sleep(time);
		outtextxy(b, 500, "你的名字");
		Sleep(time+1000);
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
		outtextxy(a, 150, "小傻瓜啊");
		Sleep(time);
		outtextxy(a, 200, "我喜欢你");
		Sleep(time);
		outtextxy(a, 250, "不仅喜欢你的样子");
		Sleep(time);
		outtextxy(a, 300, "也喜欢在你面前时");
		Sleep(time);
		outtextxy(a, 350, "我的样子");
		Sleep(time);
		outtextxy(a, 400, "我喜欢你");
		Sleep(time);
		outtextxy(a, 450, "不仅喜欢你为我做的事");
		Sleep(time);
		outtextxy(a, 500, "也喜欢为了你");
		Sleep(time);
		outtextxy(a, 550, "我能做成的事");
		Sleep(time);
		outtextxy(b, 150, "所以，你知道吗");
		Sleep(time);
		outtextxy(b, 200, "你说你不着急谈恋爱");
		Sleep(time);
		outtextxy(b, 250, "我不怕");
		Sleep(time);
		outtextxy(b, 300, "我等你");
		Sleep(time);
		outtextxy(b, 350, "我只怕");
		Sleep(time);
		outtextxy(b, 400, "你不需要");
		Sleep(time);
		outtextxy(b, 450, "还让我等你");
		Sleep(time);
		outtextxy(b, 500, "那样我就会讨厌你");
		Sleep(time);
		outtextxy(b, 550, "我不想讨厌你");
		Sleep(time+1000);
		break;
	case 2:
		a = 80;
		b = 80;
		loadimage(&bk, "bk2.jpg", 1200, 800);
		putimage(0, 0, &bk);
		//setfillstyle(0);
		settextstyle(30, 0, "楷体");
		setcolor(BROWN);
		setbkmode(TRANSPARENT);
		Sleep(time);
		outtextxy(a, 50, "小盆友啊");
		Sleep(time);
		outtextxy(a, 100, "我想，如果你不喜欢我");
		Sleep(time);
		outtextxy(a, 150, "那么你大可告诉我");
		Sleep(time);
		outtextxy(a, 200, "不要等我陷得深了");
		Sleep(time);
		outtextxy(a, 250, "再来加倍的伤害我");
		Sleep(time);
		outtextxy(a, 300, "其实我有知道");
		Sleep(time);
		outtextxy(a, 350, "我们认识这么久");
		Sleep(time);
		outtextxy(b, 400, "你从没主动找过我");
		Sleep(time);
		outtextxy(b, 450, "或许我融不进你的世界");
		Sleep(time+500);
		outtextxy(b, 500, "或许你不想进入我的生活");
		Sleep(time+500);
		outtextxy(b, 550, "但我还是");
		Sleep(time);
		outtextxy(b, 600, "傻傻的希望你也能喜欢我啊");
		Sleep(time+500);
		outtextxy(b, 650, "因为我真的");
		Sleep(time);
		outtextxy(b, 700, "懒得再去喜欢别人了呀");
		Sleep(time+1000);
		break;
	case 3:
		loadimage(&bk, "bk3.jpg", 1200, 800);
		putimage(0, 0, &bk);
		//setfillstyle(0);
		settextstyle(35, 0, "楷体");
		setcolor(LIGHTGREEN);
		setbkmode(TRANSPARENT);
		Sleep(time);
		outtextxy(470, 100, "小胖子啊");
		Sleep(time);
		outtextxy(300, 150, "我有做过很多让自己很讨厌自己的事");
		Sleep(time+500);
		outtextxy(400, 200, "有过很多后悔的经历");
		Sleep(time);
		outtextxy(400, 250, "做错过很多的物理题");
		Sleep(time);
		outtextxy(420, 300, "也说错过很多的话");
		Sleep(time);
		outtextxy(470, 350, "但好在啊");
		Sleep(time);
		outtextxy(440, 400, "在喜欢这件事上");
		Sleep(time);
		outtextxy(450, 450, "我有选对人啊");
		Sleep(time);
		outtextxy(470, 500, "喜你成疾");
		Sleep(time);
		outtextxy(470, 550, "药石无医");
		Sleep(time+1000);
		break;
	case 4:
		loadimage(&bk, "bk4.jpg", 1200, 800);
		putimage(0, 0, &bk);
		//setfillstyle(0);
		settextstyle(30, 0, "楷体");
		setcolor(YELLOW);
		setbkmode(TRANSPARENT);
		Sleep(time);
		outtextxy(a, 150, "小松鼠啊");
		Sleep(time);
		outtextxy(a, 200, "我记得每一个与你有关的瞬间");
		Sleep(time+500);
		outtextxy(a, 250, "我忘不了");
		Sleep(time);
		outtextxy(a, 300, "也没想忘");
		Sleep(time);
		outtextxy(a, 350, "我喜欢和你聊天时控制不住的开心");
		Sleep(time+500);
		outtextxy(a, 400, "我控制不了");
		Sleep(time);
		outtextxy(a, 450, "也没想控制");
		Sleep(time);
		outtextxy(b, 150, "我喜欢和你");
		Sleep(time);
		outtextxy(b, 200, "分享我的生活");
		Sleep(time);
		outtextxy(b, 250, "我的心情");
		Sleep(time);
		outtextxy(b, 300, "我的世界");
		Sleep(time);
		outtextxy(b, 350, "但我也十分想");
		Sleep(time);
		outtextxy(b, 400, "了解你的生活");
		Sleep(time);
		outtextxy(b, 450, "你的心情");
		Sleep(time);
		outtextxy(b, 500, "你的一切");
		Sleep(time);
		putimage(0, 0, &bk);
		settextstyle(35, 0, "楷体");
		Sleep(time);
		outtextxy(500, 100, "所以");
		Sleep(time);
		outtextxy(500, 150, "梁滢");
		Sleep(time);
		outtextxy(410, 200, "和我在一起好吗");
		Sleep(time);
		outtextxy(430, 250, "一辈子的那种");
		Sleep(time);
		outtextxy(480, 300, "我真的");
		Sleep(time);
		outtextxy(410, 350, "很认真的想过了");
		Sleep(time);
		outtextxy(340, 400, "关于想与你在一起这件事");
		Sleep(time+500);
		outtextxy(270, 450, "我有很认真的考虑过要不要错过你");
		Sleep(time+500);
		outtextxy(250, 500, "所以我也希望你也能认真的考虑一下");
		Sleep(time+500);
		outtextxy(430, 550, "要不要错过我");
		Sleep(time);
		outtextxy(900,700,"(全集完)");
		Sleep(time + 1000);
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
void paintword(int i,DWORD* pMem) {
	if (i <= 3) {
		text(i);
		Sleep(2000);
		disapear(pMem);
	}
	else if (i == 4) {
		text(i);
	}
}

//绽放初始化
void Inito13(int i) {
	int r[13] = {126,126,126,126,126,126,126,126,126,126,126,126,126 };
	int x[13] = {124,124,124,124,124,124,124,124,124,124,124,124,124 };
	int y[13] = {124,124,124,124,124,124,124,124,124,124,124,124,124 };

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
	int x[12] = { 122,122,122 ,122 ,122 ,122 ,122 ,122 ,122 ,122 ,122 ,122  };
	int y[12] = { 122,122,122,122,122,122,122,122,122,122,122,122 };

	int jx[12] = { 20,50,65,100,20,35,85,100,30,50,70,90};
	int jy[12] = { 25,35,35,25,35,35,35,35,30,50,30,50};

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
void timeword(DWORD& st4,const char a[],int time,int delay,int x,int y,int i) {
	DWORD st5 = timeGetTime();
	if (st5 - st4 > time&&control.wordtime == i) {
		if (st5 - st4 - time < 2000) {
			settextstyle(30, 0, "楷体");
			setcolor(YELLOW);
		}
		else {
			settextstyle(30, 0, "楷体");
			setcolor(LIGHTCYAN);
		}
		//setbkmode(TRANSPARENT);
		outtextxy(x, y, a);
		if (st5 -st4- time > delay) {
			control.wordtime++;
		}
	}
}

//烟花段文字
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
	timeword(st4, "走路上等你擦肩而过才想起打招呼的我", 69000, 100000, 300,450, 2);
	timeword(st4, "总跟舍友说你有多好的我", 71000, 100000, 300, 500, 2);
	timeword(st4, "看了你的信息而笑出声的我", 73000, 100000, 300, 550, 2);
	timeword(st4, "发现自己喜欢你的我", 75000, 100000, 300, 600, 2);
}

//爱心段文字
void heartword() {
	settextstyle(40, 0, "楷体");
	switch (control.locate14)
	{
	case 1:
		settextcolor(LIGHTCYAN);
		outtextxy(130,220,"喜你成疾");
		outtextxy(930, 220, "药石无医");
		break;
	case 2:
		settextcolor(LIGHTGRAY);
		outtextxy(500, 200, "你保护世界");
		outtextxy(520, 300, "我保护你");
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
		outtextxy(150, 300, "我若在你心上");
		outtextxy(150, 400, "情敌三千又何妨");
		outtextxy(850, 300, "你若在我心上");
		outtextxy(800, 400, "负了世界又如何");
		break;
	case 2:
		settextcolor(LIGHTMAGENTA);
		outtextxy(320, 500, "你是我一时的冲动");
		outtextxy(570, 600, "也是我良久的喜欢");
		break;
	case 3:
		settextcolor(LIGHTGRAY);
		outtextxy(200, 500, "有我在");
		outtextxy(700, 200, "你就不用长大");
		break;
	case 4:
		settextcolor(WHITE);
		outtextxy(100, 500, "你是非常可爱的人");
		outtextxy(100, 600, "真应该遇到最好的人");
		outtextxy(800, 500, "我也真希望");
		outtextxy(800, 600, "我就是");
	}
}