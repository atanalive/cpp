#ifndef NUMBERRAIN
#define NUMBERRAIN

#include<graphics.h>
#include<time.h>
#include<stdio.h>
#include<conio.h>
#pragma comment(lib,"winmm.lib")

#define number_rain 100
#define lelong_rain 8
#define height_rain 1080
#define width_rain 1920




void rain();
void initrain();
void update();
void down(DWORD&);


#endif


