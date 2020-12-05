#include <graphics.h>
#include <conio.h>
//#include <iostream.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <Mmsystem.h> // ʹ�øü�ʱ������������ļ�
#pragma comment ( lib, "Winmm.lib" )

/***** �궨���� ******/

#define NUM 13  // �̻����������궨��

/***** �ṹ������ **********/

// �̻��ṹ
struct FIRE
{
	int r;   // ��ǰ��ը�뾶
	int max_r;  // ��ը���ľ����Ե���뾶
	int x, y;  // ��ը�����ڴ��ڵ�����
	int cen_x, cen_y; // ��ը�������ͼƬ���Ͻǵ�����
	int width, height; // ͼƬ�Ŀ��
	int xy[581][612]; // ����ͼƬ���ص�

	bool show=false;  // �Ƿ�����
	bool draw;  // ��ʼ������ص�
	bool style = false;//�����̻����Ϳ���
	DWORD t1, t2, dt; // �����ٶ�
}Fire[NUM];

// �̻����ṹ
struct JET
{
	int x, y;  // ���������
	int hx, hy;  // ��ߵ�����------����ֵ�� FIRE ����� x, y
	int height;  // �̻��߶�
	bool shoot=false;  // �Ƿ���Է���

	DWORD t1, t2, dt; // �����ٶ�
	IMAGE img;  // ���滨��һ��һ��ͼƬ
	//byte n : 1;  // ͼƬ�±�
}Jet[NUM];

// happy birthday �����ֽṹ
struct HAPPY  // ���� Happy Birthday ������ͼƬ
{
	int x, y;  // ÿ���ֵ�ͼƬ�����Ͻ�����
	IMAGE img;  // ���浥����ͼƬ
	static int num;  // ��ĸ���
}Happy[NUM];
int HAPPY::num = 0;

// For.2012��زŮ �ṹ
struct WISH   // ����ף��ͼƬ�ṹ
{
	int x, y;  // ͼƬ���Ͻ�����
	DWORD t1, t2, dt; // ͼƬ�˶�ʱ����
	IMAGE img;  // ����ͼƬ
	int dxy;  // ÿ�ε��ƶ���
	byte dir : 1;  // ����������������
}Wish;

struct CONTROL
{
	int styletime = 0;//���������ͷŴ���
	bool show13 = false;//����13���̻������л�
	bool show14 = false;//����14���̻������л�
	int  locate13 = 0;//13���̻�λ�ñ仯������
	int  locate14 = 0;//14���̻�λ�ñ仯������
	bool initcon = true;//���������̻�����
	int firenum=13;//�̻���Ŀ����
	int wordtime = 0;//��������ʱ�������
	int num13 = 3;//13���̻���Ŀ����
	int num14 = 4;//14���̻���Ŀ����
	bool exit = false;//�����̻�ѭ���Ľ���
	int hword = 0;//���Ķ����ִ�ӡ�Ŀ�����
}control;


/**** ���������� ****/

void Init(int); // ��ʼ���̻�
void Inito13(int);
void Inito14(int);
void Load(); // �����̻�ͼƬ
void Shoot(); // �����̻�
void Chose(DWORD&); // ɸѡ�̻�
void Wishing(); // ����ף��
void Style(DWORD&, DWORD*pMem); // ������ʽ
void Show(DWORD& st1, DWORD*pMem); // �����̻�
void text(int x);//ͼƬ�ı�
void disapear(DWORD* pMem);//����ͼƬ
void paintword(int i, DWORD* pMem);//ͼƬ�ı���ӡ������
void timeword(DWORD&,const char a[],int time,int delay,int x,int y,int i);//��ӡ�ı�
void fireword(DWORD st4);
void heartword();

// ������
void main()
{
	initgraph(1200, 800);
	srand(time(0));

	// ���ű�������
	mciSendString("open ying.mp3 alias ly", 0, 0, 0);
	mciSendString("play ly", 0, 0, 0);

	DWORD* pMem = GetImageBuffer(); // ��ȡ�����Դ�ָ��

	Sleep(1000);
	paintword(0, pMem);
	paintword(1, pMem);
	paintword(2, pMem);
	paintword(3, pMem);

	DWORD t1 = timeGetTime(); // ɸѡ�̻���ʱ
	DWORD st1 = timeGetTime(); // ���Ż�����ʱ
	DWORD st4 = timeGetTime();//���ֲ��ż�ʱ

	for (int i = 0; i < NUM+2; i++) // ��ʼ���̻�
	{
		Init(i);
	}
	Load();    // ���̻�ͼƬ��Ϣ���ؽ���Ӧ�ṹ��
	BeginBatchDraw();   // ��ʼ������ͼ

	//settextstyle(40, 0, "����");
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
		    // ���ѡ�� 4000 �����ص����
			for (int clr = 0; clr < 2500; clr++)
			{
				for (int j = 0; j < 2; j++)
				{
					int px1 = rand() % 1200;
					int py1 = rand() % 800;

					if (py1 < 799)  // ��ֹԽ��
						pMem[py1 * 1200 + px1] = pMem[py1 * 1200 + px1 + 1] = BLACK; // ���Դ渳ֵ�������ص�
				}
			}
		}
		if (control.exit == true) {
			break;
		}
		Chose(t1); // ɸѡ�̻�
		Style(st1,pMem); // ��������
		Shoot(); // �����̻�
		Show(st1,pMem); // �����̻�
		//Wishing(); // �����ַ�
		fireword(st4);//���̻�ʱ���ı�����
		heartword();
		FlushBatchDraw(); // ��ʾǰ������л�ͼ����
	}
	EndBatchDraw();
	paintword(4, pMem);
	while (1);
}


// ��ʼ���̻�����
void Init(int i)
{
	// �ֱ�Ϊ���̻����ĵ�ͼƬ��Ե����Զ���롢�̻����ĵ�ͼƬ���Ͻǵľ��� (x��y) ��������
	int r[15] = { 97,97,97,97,97,90,97,97,95,97,97,97,97,126,125};
	int x[15] = { 97,97,103,97,97,90,106,104,92,97,97,97,106,124,122};
	int y[15] = {102,116,98,102,104,102,101,98,100,102,104,105,101,124,122};

	/**** ��ʼ���̻� *****/

	Fire[i].x = 0;  // �̻���������
	Fire[i].y = 0;
	if (i < 13) {
		Fire[i].width = 194;  // ͼƬ��
		Fire[i].height = 204;  // ͼƬ��
	}
	else if(i==13){
		Fire[i].width = 249;  // ͼƬ��
		Fire[i].height = 249;  // ͼƬ��
	}
	else if (i == 14) {
		Fire[i].width = 245;  // ͼƬ��
		Fire[i].height = 245;  // ͼƬ��
	}
	Fire[i].max_r = r[i];  // ���뾶
	Fire[i].cen_x = x[i];  // ���ľ����ϽǾ���
	Fire[i].cen_y = y[i];
	Fire[i].show = false;  // �Ƿ�����
	Fire[i].dt = 7;  // ����ʱ����
	Fire[i].t1 = timeGetTime();
	Fire[i].r = 0;  // �� 0 ��ʼ����
	Fire[i].style = false;

	/**** ��ʼ���̻��� *****/

	Jet[i].x = -133;  // �̻������Ͻ�����
	Jet[i].y = -131;
	Jet[i].hx = -133;  // �̻���������ߵ�����
	Jet[i].hy = -131;
	Jet[i].height = 0;  // ����߶�
	Jet[i].t1 = timeGetTime();
	Jet[i].dt = rand() % 10; // �����ٶ�ʱ����
	//Jet[i].n = 0;  // �̻�����˸ͼƬ�±�
	Jet[i].shoot = false;  // �Ƿ���
}


// ����ͼƬ
void Load()
{
	/**** �����̻������ص���ɫ ****/
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

	/**** �����̻��� ************/
	IMAGE sm;
	loadimage(&sm, "1.jpg", 30, 30);

	for (int i = 0; i < NUM; i++)
	{
		SetWorkingImage(&sm);
		//int n = rand() % 5;

		getimage(&Jet[i].img, 0, 0, 79, 78);
		//getimage(&Jet[i].img[1], (n + 5) * 20, 0, 20, 50); // ��
	}

	/***** ���� Happy Birthday ͼƬ ********/
	IMAGE hm;
	loadimage(&hm, ".jpg", 1920,1200);
	SetWorkingImage(&hm);

	for (int i = 0; i < 13; i++)
	{
		Happy[i].x = i * 90;
		Happy[i].y = rand() % 100 + 500;
		getimage(&Happy[i].img, i * 53, 0, 53, 115);
	}

	/***** ���� For.2012��زŮ ͼƬ *********/
	Wish.x = 0;
	Wish.y = 100;
	Wish.t1 = timeGetTime();
	Wish.dt = 46;
	Wish.dir = 0;
	Wish.dxy = rand() % 8 + 1;
	loadimage(&Wish.img, ".jpg", 1920, 1200);
	putimage(Wish.x, Wish.y, &Wish.img, SRCINVERT);

	SetWorkingImage(); // ���ûػ�ͼ����
}


// ��һ����Χ��ɸѡ�ɷ�����̻�������ʼ���������������̻�������Ļ����������
void Chose(DWORD& t1)
{
	DWORD t2 = timeGetTime();

	if (t2 - t1 > 100)
	{
		int n = rand() % 50;
		if (n < control.firenum&& Jet[n].shoot == false && Fire[n].show == false&&control.show13==false&&control.show14==false)
		{
			/**** �����̻�����Ԥ������ *****/
			Jet[n].x = rand() % 1200;
			Jet[n].y = rand() % 100 + 600;
			Jet[n].hx = Jet[n].x;
			Jet[n].hy = rand() % 400;
			Jet[n].height = Jet[n].y - Jet[n].hy;
			Jet[n].shoot = true;
			putimage(Jet[n].x, Jet[n].y, &Jet[n].img, SRCINVERT);

			/**** ����ÿ���̻��������� *****/
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
			/**** ���ɷֲ��̻��� ***/
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
					putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT); // ��ʾ�̻���

					/**** �����̻����� ***/
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
					putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT); // ��ʾ�̻���

					/**** �����̻����� ***/
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
			/**** ���ɷֲ��̻��� ***/
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
					putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT); // ��ʾ�̻���

					/**** �����̻����� ***/
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


// ɨ���̻���������
void Shoot()
{
	for (int i = 0; i < control.firenum; i++)
	{
		Jet[i].t2 = timeGetTime();

		if (Jet[i].t2 - Jet[i].t1 > Jet[i].dt && Jet[i].shoot == true)
		{
			/**** �̻��������� *****/
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT);

			if (Jet[i].y > Jet[i].hy)
			{
				//Jet[i].n++;
				Jet[i].y -= 5;
			}

			putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT);

			/**** �������߶ȵ� 3 / 4������ *****/
			if ((Jet[i].y - Jet[i].hy) * 4 < Jet[i].height)
				Jet[i].dt = rand() % 4 + 10;

			/**** ���������߶� *****/
			if (Jet[i].y <= Jet[i].hy)
			{
				// ���ű�ը��
				char c1[50], c2[30], c3[30];
				printf(c1, "open ./fire/bomb.wav alias n%d", i);
				printf(c2, "play n%d", i);
				printf(c3, "close s%d", i);

				//mciSendString(c3, 0, 0, 0);
				//mciSendString(c1, 0, 0, 0);
				//mciSendString(c2, 0, 0, 0);

				if (Fire[i].style == false) {
					putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT); // �����̻���
				}
				Fire[i].x = Jet[i].hx +10;      // ���̻����м䱬ը
				Fire[i].y = Jet[i].hy;      // ����ߵ�����
				Fire[i].show = true;   // ��ʼ����
				Jet[i].shoot = false;   // ֹͣ����

				// ��ʾ��Ӧ��ĸ
				putimage(Happy[HAPPY::num].x, Happy[HAPPY::num].y, &Happy[HAPPY::num].img, SRCINVERT);
				HAPPY::num++;

				if (HAPPY::num > 12)
					HAPPY::num = 0;
			}
			Jet[i].t1 = Jet[i].t2;
		}
	}
}


// ����ˮƽ����ֱ�ƶ���ͼƬ
void Wishing()
{
	Wish.t2 = timeGetTime();

	if (Wish.t2 - Wish.t1 > Wish.dt)
	{
		/**** ͼƬ���ƶ� *****/
		putimage(Wish.x, Wish.y, &Wish.img, SRCINVERT);

		if (Wish.dir == 0)
			Wish.x += Wish.dxy;
		else
			Wish.y -= Wish.dxy;

		putimage(Wish.x, Wish.y, &Wish.img, SRCINVERT);

		/**** ͼƬ����� *****/
		if (Wish.x > 1200 || Wish.y < 0)
		{
			Wish.dir = rand() % 2;  // ��ȡ�������

			if (Wish.dir == 0)  // ��������˶�
			{
				Wish.y = rand() % 700; // ��ʼλ��
				Wish.x = -490;
				Wish.dxy = rand() % 7 + 1; // ����˶�����
			}
			else    // �����˶�
			{
				Wish.dxy = rand() % 7 + 1;
				Wish.x = rand() % 700;
				Wish.y = 800;
			}
		}
		Wish.t1 = Wish.t2;
	}
}


// ��ʾ����
void Style(DWORD& st1,DWORD*pMem)
{
	DWORD st2 = timeGetTime();

	if (st2 - st1 > 77000&&control.styletime==0) // һ�׸��ʱ��
	{
		// ��������
		int x[13] = { 60, 75, 91, 100, 95, 75, 60, 45, 25, 15, 25, 41, 60 };
		int y[13] = { 65, 53, 40, 22, 5, 4, 20, 4, 5, 22, 40, 53, 65 };
		for (int i = 0; i < 13; i++)
		{
			cleardevice();
			/**** ���ɷֲ��̻��� ***/
			Jet[i].x = x[i] * 10;
			Jet[i].y = (y[i] + 75) * 10;
			Jet[i].hx = Jet[i].x;
			Jet[i].hy = y[i] * 10;
			Jet[i].height = Jet[i].y - Jet[i].hy;
			Jet[i].shoot = true;
			Jet[i].dt = 7;
			Fire[i].style = true;
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT); // ��ʾ�̻���

			/**** �����̻����� ***/
			Fire[i].x = Jet[i].x + 10;
			Fire[i].y = Jet[i].hy;
			Fire[i].show = false;
			Fire[i].r = 0;

			/**** ���ŷ������� ***/
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


// �����̻�
void Show(DWORD& st1, DWORD* pMem)
{
	// �̻����׶�����ʱ������������������Ч��
	int drt[16] = { 5, 5, 5, 5, 5, 6, 25, 25, 25, 25, 55, 55, 55, 55, 55 };
	DWORD st3 = timeGetTime();
	if (control.initcon == false &&control.styletime==1) {
		control.show13 = true;
		control.styletime = 2;
	}
	for (int i = 0; i < control.firenum; i++)
	{
		Fire[i].t2 = timeGetTime();

		// ���ӱ�ը�뾶�������̻�������ʱ����������Ч��
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

		// ����ú��ڻ��ɱ�ը�����ݵ�ǰ��ը�뾶���̻�����ɫֵ�ӽ���ɫ�Ĳ������
		if (Fire[i].draw)
		{

			for (double a = 0; a <= 6.28; a += 0.01)
			{
				int x1 = (int)(Fire[i].cen_x + Fire[i].r * cos(a));  // �����ͼƬ���Ͻǵ�����
				int y1 = (int)(Fire[i].cen_y - Fire[i].r * sin(a));

				if (x1 > 0 && x1 < Fire[i].width && y1 > 0 && y1 < Fire[i].height) // ֻ���ͼƬ�ڵ����ص�
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

					// �̻����ص��ڴ����ϵ�����
					int xx = (int)(Fire[i].x + Fire[i].r * cos(a));
					int yy = (int)(Fire[i].y - Fire[i].r * sin(a));

					// �ϰ������ص㲻�������ֹԽ��
					if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && xx < 1200 && yy > 0 && yy < 800 && !Fire[i].style&&control.show14 == false&&control.show13==false) {
						pMem[yy * 1200 + xx] = BGR(Fire[i].xy[x1][y1]); // �Դ���������̻�
					}
					else if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && xx < 1200 && yy > 0 && yy < 800 && Fire[i].style) {
						//pMem[yy * 1200 + xx] = BGR(Fire[14].xy[x1][y1]); // �Դ���������̻�
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



//�����ı�
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
		settextstyle(30, 0, "����");
		setcolor(LIGHTMAGENTA);
		setbkmode(TRANSPARENT);
		Sleep(time);
		outtextxy(a, 150, "С���ް�");
		Sleep(time);
		outtextxy(a, 200, "�Ҳ�֪��");
		Sleep(time);
		outtextxy(a, 250, "���㿴���ⲿ��Ʒ��ʱ��");
		Sleep(time);
		outtextxy(a, 300, "�����˽ڣ���Ϧ�ڣ�������");
		Sleep(time+500);
		outtextxy(a, 350, "�ֻ�ֻ��һ������ͨͨ������");
		Sleep(time+500);
		outtextxy(a, 400, "��Ҳ��֪��");
		Sleep(time);
		outtextxy(a, 450, "�������");
		Sleep(time);
		outtextxy(a, 500, "�ǿ��ģ����������Σ�����");
		Sleep(time+500);
		outtextxy(a, 550, "�ֻ�ֻ��ƽ�����治��һ�㲨��");
		Sleep(time+500);
		outtextxy(b, 200, "���������");
		Sleep(time);
		outtextxy(b, 250, "��������ϲ������");
		Sleep(time);
		outtextxy(b, 300, "�����ǹ�ȥ");
		Sleep(time);
		outtextxy(b, 350, "��������");
		Sleep(time);
		outtextxy(b, 400, "��Ϊ��");
		Sleep(time);
		outtextxy(b, 450, "����һ�������Ĳ���");
		Sleep(time);
		outtextxy(b, 500, "�������");
		Sleep(time+1000);
		break;
	case 1:
		a = 80;
		b = 850;
		loadimage(&bk, "bk1.jpg", 1200, 800);
		putimage(0, 0, &bk);
		//setfillstyle(0);
		settextstyle(30, 0, "����");
		setcolor(LIGHTRED);
		setbkmode(TRANSPARENT);
		Sleep(time);
		outtextxy(a, 150, "Сɵ�ϰ�");
		Sleep(time);
		outtextxy(a, 200, "��ϲ����");
		Sleep(time);
		outtextxy(a, 250, "����ϲ���������");
		Sleep(time);
		outtextxy(a, 300, "Ҳϲ��������ǰʱ");
		Sleep(time);
		outtextxy(a, 350, "�ҵ�����");
		Sleep(time);
		outtextxy(a, 400, "��ϲ����");
		Sleep(time);
		outtextxy(a, 450, "����ϲ����Ϊ��������");
		Sleep(time);
		outtextxy(a, 500, "Ҳϲ��Ϊ����");
		Sleep(time);
		outtextxy(a, 550, "�������ɵ���");
		Sleep(time);
		outtextxy(b, 150, "���ԣ���֪����");
		Sleep(time);
		outtextxy(b, 200, "��˵�㲻�ż�̸����");
		Sleep(time);
		outtextxy(b, 250, "�Ҳ���");
		Sleep(time);
		outtextxy(b, 300, "�ҵ���");
		Sleep(time);
		outtextxy(b, 350, "��ֻ��");
		Sleep(time);
		outtextxy(b, 400, "�㲻��Ҫ");
		Sleep(time);
		outtextxy(b, 450, "�����ҵ���");
		Sleep(time);
		outtextxy(b, 500, "�����Ҿͻ�������");
		Sleep(time);
		outtextxy(b, 550, "�Ҳ���������");
		Sleep(time+1000);
		break;
	case 2:
		a = 80;
		b = 80;
		loadimage(&bk, "bk2.jpg", 1200, 800);
		putimage(0, 0, &bk);
		//setfillstyle(0);
		settextstyle(30, 0, "����");
		setcolor(BROWN);
		setbkmode(TRANSPARENT);
		Sleep(time);
		outtextxy(a, 50, "С���Ѱ�");
		Sleep(time);
		outtextxy(a, 100, "���룬����㲻ϲ����");
		Sleep(time);
		outtextxy(a, 150, "��ô���ɸ�����");
		Sleep(time);
		outtextxy(a, 200, "��Ҫ�����ݵ�����");
		Sleep(time);
		outtextxy(a, 250, "�����ӱ����˺���");
		Sleep(time);
		outtextxy(a, 300, "��ʵ����֪��");
		Sleep(time);
		outtextxy(a, 350, "������ʶ��ô��");
		Sleep(time);
		outtextxy(b, 400, "���û�����ҹ���");
		Sleep(time);
		outtextxy(b, 450, "�������ڲ����������");
		Sleep(time+500);
		outtextxy(b, 500, "�����㲻������ҵ�����");
		Sleep(time+500);
		outtextxy(b, 550, "���һ���");
		Sleep(time);
		outtextxy(b, 600, "ɵɵ��ϣ����Ҳ��ϲ���Ұ�");
		Sleep(time+500);
		outtextxy(b, 650, "��Ϊ�����");
		Sleep(time);
		outtextxy(b, 700, "������ȥϲ��������ѽ");
		Sleep(time+1000);
		break;
	case 3:
		loadimage(&bk, "bk3.jpg", 1200, 800);
		putimage(0, 0, &bk);
		//setfillstyle(0);
		settextstyle(35, 0, "����");
		setcolor(LIGHTGREEN);
		setbkmode(TRANSPARENT);
		Sleep(time);
		outtextxy(470, 100, "С���Ӱ�");
		Sleep(time);
		outtextxy(300, 150, "���������ܶ����Լ��������Լ�����");
		Sleep(time+500);
		outtextxy(400, 200, "�й��ܶ��ڵľ���");
		Sleep(time);
		outtextxy(400, 250, "������ܶ��������");
		Sleep(time);
		outtextxy(420, 300, "Ҳ˵����ܶ�Ļ�");
		Sleep(time);
		outtextxy(470, 350, "�����ڰ�");
		Sleep(time);
		outtextxy(440, 400, "��ϲ���������");
		Sleep(time);
		outtextxy(450, 450, "����ѡ���˰�");
		Sleep(time);
		outtextxy(470, 500, "ϲ��ɼ�");
		Sleep(time);
		outtextxy(470, 550, "ҩʯ��ҽ");
		Sleep(time+1000);
		break;
	case 4:
		loadimage(&bk, "bk4.jpg", 1200, 800);
		putimage(0, 0, &bk);
		//setfillstyle(0);
		settextstyle(30, 0, "����");
		setcolor(YELLOW);
		setbkmode(TRANSPARENT);
		Sleep(time);
		outtextxy(a, 150, "С����");
		Sleep(time);
		outtextxy(a, 200, "�Ҽǵ�ÿһ�������йص�˲��");
		Sleep(time+500);
		outtextxy(a, 250, "��������");
		Sleep(time);
		outtextxy(a, 300, "Ҳû����");
		Sleep(time);
		outtextxy(a, 350, "��ϲ����������ʱ���Ʋ�ס�Ŀ���");
		Sleep(time+500);
		outtextxy(a, 400, "�ҿ��Ʋ���");
		Sleep(time);
		outtextxy(a, 450, "Ҳû�����");
		Sleep(time);
		outtextxy(b, 150, "��ϲ������");
		Sleep(time);
		outtextxy(b, 200, "�����ҵ�����");
		Sleep(time);
		outtextxy(b, 250, "�ҵ�����");
		Sleep(time);
		outtextxy(b, 300, "�ҵ�����");
		Sleep(time);
		outtextxy(b, 350, "����Ҳʮ����");
		Sleep(time);
		outtextxy(b, 400, "�˽��������");
		Sleep(time);
		outtextxy(b, 450, "�������");
		Sleep(time);
		outtextxy(b, 500, "���һ��");
		Sleep(time);
		putimage(0, 0, &bk);
		settextstyle(35, 0, "����");
		Sleep(time);
		outtextxy(500, 100, "����");
		Sleep(time);
		outtextxy(500, 150, "����");
		Sleep(time);
		outtextxy(410, 200, "������һ�����");
		Sleep(time);
		outtextxy(430, 250, "һ���ӵ�����");
		Sleep(time);
		outtextxy(480, 300, "�����");
		Sleep(time);
		outtextxy(410, 350, "������������");
		Sleep(time);
		outtextxy(340, 400, "������������һ�������");
		Sleep(time+500);
		outtextxy(270, 450, "���к�����Ŀ��ǹ�Ҫ��Ҫ�����");
		Sleep(time+500);
		outtextxy(250, 500, "������Ҳϣ����Ҳ������Ŀ���һ��");
		Sleep(time+500);
		outtextxy(430, 550, "Ҫ��Ҫ�����");
		Sleep(time);
		outtextxy(900,700,"(ȫ����)");
		Sleep(time + 1000);
		break;
	}
}

//�����ı�
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

				if (py1 < 799)  // ��ֹԽ��
					pMem[py1 * 1200 + px1] = pMem[py1 * 1200 + px1 + 1] = BLACK; // ���Դ渳ֵ�������ص�
			}
		}
		FlushBatchDraw();
		Sleep(100);
	}
}

//��ӡ�ı�
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

//���ų�ʼ��
void Inito13(int i) {
	int r[13] = {126,126,126,126,126,126,126,126,126,126,126,126,126 };
	int x[13] = {124,124,124,124,124,124,124,124,124,124,124,124,124 };
	int y[13] = {124,124,124,124,124,124,124,124,124,124,124,124,124 };

	int jx[13] = { 50,60,70,45,60,75,30,60,90,20,35,85,100 };
	int jy[13] = { 30,40,30,30,30,30,30,40,50,30,30,30,30 };

	/**** ��ʼ���̻� *****/
	//Fire[i].x = 0;  // �̻���������
	//Fire[i].y = 0;
	Fire[i].width = 249;  // ͼƬ��
	Fire[i].height = 249;  // ͼƬ��
	Fire[i].max_r = r[i];  // ���뾶
	Fire[i].cen_x = x[i];  // ���ľ����ϽǾ���
	Fire[i].cen_y = y[i];
	Fire[i].show = false;  // �Ƿ�����
	//Fire[i].dt = 7;  // ����ʱ����
	Fire[i].t1 = timeGetTime();
	//Fire[i].r = 0;  // �� 0 ��ʼ����
	//Fire[i].style = false;

				/**** ���ɷֲ��̻��� ***/
	Jet[i].x = jx[i] * 10;
	Jet[i].y = (jy[i] + 75) * 10;
	Jet[i].hx = Jet[i].x;
	Jet[i].hy = jy[i] * 10;
	Jet[i].height = Jet[i].y - Jet[i].hy;
	Jet[i].shoot = false;
	Jet[i].dt = 7;
	//Fire[i].style = true;
	putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT); // ��ʾ�̻���

	/**** �����̻����� ***/
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

	/**** ��ʼ���̻� *****/
	//Fire[i].x = 0;  // �̻���������
	//Fire[i].y = 0;
	Fire[i].width = 245;  // ͼƬ��
	Fire[i].height = 245;  // ͼƬ��
	Fire[i].max_r = r[i];  // ���뾶
	Fire[i].cen_x = x[i];  // ���ľ����ϽǾ���
	Fire[i].cen_y = y[i];
	Fire[i].show = false;  // �Ƿ�����
	//Fire[i].dt = 7;  // ����ʱ����
	Fire[i].t1 = timeGetTime();
	//Fire[i].r = 0;  // �� 0 ��ʼ����
	//Fire[i].style = false;

				/**** ���ɷֲ��̻��� ***/
	Jet[i].x = jx[i] * 10;
	Jet[i].y = (jy[i] + 75) * 10;
	Jet[i].hx = Jet[i].x;
	Jet[i].hy = jy[i] * 10;
	Jet[i].height = Jet[i].y - Jet[i].hy;
	Jet[i].shoot = false;
	Jet[i].dt = 7;
	//Fire[i].style = true;
	putimage(Jet[i].x, Jet[i].y, &Jet[i].img, SRCINVERT); // ��ʾ�̻���

	/**** �����̻����� ***/
	Fire[i].x = Jet[i].x + 10;
	Fire[i].y = Jet[i].hy;
	Fire[i].show = false;
	Fire[i].r = 0;
}

//����ʱ�䲢�������
void timeword(DWORD& st4,const char a[],int time,int delay,int x,int y,int i) {
	DWORD st5 = timeGetTime();
	if (st5 - st4 > time&&control.wordtime == i) {
		if (st5 - st4 - time < 2000) {
			settextstyle(30, 0, "����");
			setcolor(YELLOW);
		}
		else {
			settextstyle(30, 0, "����");
			setcolor(LIGHTCYAN);
		}
		//setbkmode(TRANSPARENT);
		outtextxy(x, y, a);
		if (st5 -st4- time > delay) {
			control.wordtime++;
		}
	}
}

//�̻�������
void fireword(DWORD st4) {
	timeword(st4, "�ޣ����ǴӸ�һ��ʶ�������Ѿ�������˰�", 3000, 20000, 300, 100, 0);
	timeword(st4, "ʱ����׾Թ�϶", 5000, 100000, 300, 150, 0);
	timeword(st4, "һգ�����Ǿ͸��Գ�����", 7000, 100000, 300, 200, 0);
	timeword(st4, "��Ҳ��֪���Լ�", 9000, 100000, 300, 250, 0);
	timeword(st4, "Ϊʲô", 11000, 100000, 300, 300, 0);
	timeword(st4, "��һ�����㷢�Ǹ���Ϣ˵Ҫ����΢��", 13000, 100000, 300, 350, 0);
	timeword(st4, "������ܶ����鶼�ǲ���Ҫ���ɵĶ�ô", 15000, 100000, 300, 400, 0);
	timeword(st4, "���ǽ�������", 17000, 100000, 300, 450, 0);
	timeword(st4, "��ÿһ������������˲���Ҷ��ǵú������", 19000, 100000, 300, 500, 0);

	timeword(st4, "��һ�˶������˵���", 27000, 22000, 300, 100, 1);
	timeword(st4, "�����������������", 29000, 100000, 300, 150, 1);
	timeword(st4, "������·�����ɵ��������������", 31000, 100000, 300, 200, 1);
	timeword(st4, "�߶��˶���Ҫ�Ҷ�����������յ���", 33000, 100000, 300, 250, 1);
	timeword(st4, "�����г��ϰ���ֱ�ӱ��׼۸��ҵ���", 35000, 100000, 300, 300, 1);
	timeword(st4, "������Ҫ��ٵ���", 37000, 100000, 300, 350, 1);
	timeword(st4, "Ӣ���������ϵ����������", 39000, 100000, 300, 400, 1);
	timeword(st4, "������ͷɢ�����߿�ǰȴͻȻ������β����", 41000, 100000, 300, 450, 1);
	timeword(st4, "�Ҳŷ���", 43000, 100000, 300, 500, 1);
	timeword(st4, "ԭ���Ҷ����ÿһ����������ô������", 45000, 100000, 300, 550, 1);

	timeword(st4, "���㲻��֪��", 55000, 24000, 300, 100, 2);
	timeword(st4, "��һ�˶�����͵�������", 57000, 100000, 300, 150, 2);
	timeword(st4, "��һ��ٶ�ݸ��ͬѧ��ϲ������Ҳ�ڶ�ݸ����", 59000, 100000, 300, 200, 2);
	timeword(st4, "ͻȻ������������ȥ�������", 61000, 100000, 300, 250, 2);
	timeword(st4, "�߶����ӻ�������", 63000, 100000, 300, 300, 2);
	timeword(st4, "����Ӣ����������ȥ���ǰ�����͵͵�������", 65000, 100000, 300, 350, 2);
	timeword(st4, "�ڼ���͵������ҵ�����ֵ���", 67000, 100000, 300, 400, 2);
	timeword(st4, "��·�ϵ�����������������к�����", 69000, 100000, 300,450, 2);
	timeword(st4, "�ܸ�����˵���ж�õ���", 71000, 100000, 300, 500, 2);
	timeword(st4, "���������Ϣ��Ц��������", 73000, 100000, 300, 550, 2);
	timeword(st4, "�����Լ�ϲ�������", 75000, 100000, 300, 600, 2);
}

//���Ķ�����
void heartword() {
	settextstyle(40, 0, "����");
	switch (control.locate14)
	{
	case 1:
		settextcolor(LIGHTCYAN);
		outtextxy(130,220,"ϲ��ɼ�");
		outtextxy(930, 220, "ҩʯ��ҽ");
		break;
	case 2:
		settextcolor(LIGHTGRAY);
		outtextxy(500, 200, "�㱣������");
		outtextxy(520, 300, "�ұ�����");
		break;
	case 3:
		settextcolor(WHITE);
		outtextxy(200, 600, "˭��������ĳ��");
		outtextxy(550, 700, "�Ҷ�����ta��������");
		control.locate14++;
		break;
	}
	switch (control.locate13)
	{
	case 1:
		settextcolor(LIGHTRED);
		outtextxy(150, 300, "������������");
		outtextxy(150, 400, "�����ǧ�ֺη�");
		outtextxy(850, 300, "������������");
		outtextxy(800, 400, "�������������");
		break;
	case 2:
		settextcolor(LIGHTMAGENTA);
		outtextxy(320, 500, "������һʱ�ĳ嶯");
		outtextxy(570, 600, "Ҳ�������õ�ϲ��");
		break;
	case 3:
		settextcolor(LIGHTGRAY);
		outtextxy(200, 500, "������");
		outtextxy(700, 200, "��Ͳ��ó���");
		break;
	case 4:
		settextcolor(WHITE);
		outtextxy(100, 500, "���Ƿǳ��ɰ�����");
		outtextxy(100, 600, "��Ӧ��������õ���");
		outtextxy(800, 500, "��Ҳ��ϣ��");
		outtextxy(800, 600, "�Ҿ���");
	}
}