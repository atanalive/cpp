#include<stdio.h>
#include<string.h>
#include <windows.h>

void yanshi(int a) {
	int b = 0;
	while (b < a) {
		for (int i = 0; i < 100; i++);
		b++;
	}
}
int main(void){
	int h = 1;
	int time = 2000000;
	char a[50];
	printf("帅气的召唤师,欢迎来到神圣的祭坛，请问你的祭品是：\n");
	gets(a);
	yanshi(time);
	printf("叮~~~~~~~~~~~~~~~~~~~~~~~~~~\n恭喜玩家献祭成功\n");
	yanshi(time);
	printf("请问你要召唤的神兽是(请选择以下神兽中的一种):\n1 小怪兽    2 小仙女   3 小骚猪   4 小苹果\n");
	gets(a);
	yanshi(time);
		printf("天空一声巨响，咻咻咻，小怪兽闪亮登场\n");
		yanshi(time);
		printf("小怪兽：你好？召唤我出来干嘛，我不要睡觉的吗\n");
		gets(a);
		yanshi(time);
		printf("小怪兽：对不起，等级太低，打不过\n");
		gets(a);
		gets(a);
		yanshi(time);
		printf("系统：你好，骚年，请问你的WiFi密码是：");
		gets(a);
		yanshi(time);
		printf("系统：恭喜玩家威威泽作弊成功！！！\n");
		yanshi(time);
		printf("叮~~~~~~~~~~~~,你已作弊成功，恭喜你的小怪兽升至满级，品级为sss\n");
		yanshi(time);
		yanshi(time);
		printf("现发布全服通缉令，威威泽涉嫌开挂，特此悬赏，首个击杀威威泽的凹凸曼将会赢得高数练习册加厚版一本\n");
		yanshi(time);
		yanshi(time);
		printf("警报！！！！！！！\n");
		yanshi(time);
		printf("前方发现一个凹凸曼\n");
		yanshi(time);
		printf("请问是要捕捉还是攻击\n");
		fflush(stdin);
		gets(a);
		if (strcmp(a, "攻击")==0)
		{
			printf("叮~~~~~~~~~~~~~恭喜玩家威威泽攻击迪迦凹凸曼成功\n");
			yanshi(time);
			yanshi(time);
			printf("叮~~~~~~~~~~~~本次攻击获得物品如下：五年高考三年模拟*1，衡水卷*10000000，高数作业*100,\n工图作业永久版(珍稀度：五星)，小怪兽一只，英语四级听力buff，线性代数课程一套\n");
			yanshi(time);
			yanshi(time);
			printf("叮~~~~~~~~~~~~~~~~~温馨提示：本次攻击共消耗颜值1000000000，颜值余额：-100000000，请及时充值\n");
		}
		    gets(a);
		while(h==1) {
			yanshi(time);
            printf("对不起，系统检测到你的颜值不足，已自动删除账号，如有疑问，请拨打10086\n");
			gets(a);
			if (strcmp(a,"我草泥马")==0) {
				yanshi(time);
				printf("由于你的不文明用语，系统心态炸了，现已崩溃\n");
				while (h<12) {
					printf("系统崩溃中，，，，，，屏幕君正在疯狂抢救，，，，，，，，，，，，，，，，，，\n");
					yanshi(time);
					h++;
				}
				printf("抢救失败,电脑将于10秒后爆炸，请做好准备,倒计时开始\n");
				for (int i = 0,e=10; i < 10; i++,e--) {
					Sleep(1000);
					printf("               %d\n",e);
				}
				Sleep(1000);
				printf("               0");
			}
		}
		yanshi(time);
		printf("轰轰轰轰轰轰轰轰轰轰轰轰轰轰轰轰轰轰轰轰轰轰轰,爆炸开始\n");
		while (1) {
			printf("                 林方泽好帅好帅林方泽好帅好帅\n");
			yanshi(time);
		}
			return 0;

}