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
	printf("˧�����ٻ�ʦ,��ӭ������ʥ�ļ�̳��������ļ�Ʒ�ǣ�\n");
	gets(a);
	yanshi(time);
	printf("��~~~~~~~~~~~~~~~~~~~~~~~~~~\n��ϲ����׼��ɹ�\n");
	yanshi(time);
	printf("������Ҫ�ٻ���������(��ѡ�����������е�һ��):\n1 С����    2 С��Ů   3 Сɧ��   4 Сƻ��\n");
	gets(a);
	yanshi(time);
		printf("���һ�����죬�����ݣ�С���������ǳ�\n");
		yanshi(time);
		printf("С���ޣ���ã��ٻ��ҳ�������Ҳ�Ҫ˯������\n");
		gets(a);
		yanshi(time);
		printf("С���ޣ��Բ��𣬵ȼ�̫�ͣ��򲻹�\n");
		gets(a);
		gets(a);
		yanshi(time);
		printf("ϵͳ����ã�ɧ�꣬�������WiFi�����ǣ�");
		gets(a);
		yanshi(time);
		printf("ϵͳ����ϲ������������׳ɹ�������\n");
		yanshi(time);
		printf("��~~~~~~~~~~~~,�������׳ɹ�����ϲ���С��������������Ʒ��Ϊsss\n");
		yanshi(time);
		yanshi(time);
		printf("�ַ���ȫ��ͨ������������ӿ��ң��ش����ͣ��׸���ɱ������İ�͹������Ӯ�ø�����ϰ��Ӻ��һ��\n");
		yanshi(time);
		yanshi(time);
		printf("������������������\n");
		yanshi(time);
		printf("ǰ������һ����͹��\n");
		yanshi(time);
		printf("������Ҫ��׽���ǹ���\n");
		fflush(stdin);
		gets(a);
		if (strcmp(a, "����")==0)
		{
			printf("��~~~~~~~~~~~~~��ϲ��������󹥻����Ȱ�͹���ɹ�\n");
			yanshi(time);
			yanshi(time);
			printf("��~~~~~~~~~~~~���ι��������Ʒ���£�����߿�����ģ��*1����ˮ��*10000000��������ҵ*100,\n��ͼ��ҵ���ð�(��ϡ�ȣ�����)��С����һֻ��Ӣ���ļ�����buff�����Դ����γ�һ��\n");
			yanshi(time);
			yanshi(time);
			printf("��~~~~~~~~~~~~~~~~~��ܰ��ʾ�����ι�����������ֵ1000000000����ֵ��-100000000���뼰ʱ��ֵ\n");
		}
		    gets(a);
		while(h==1) {
			yanshi(time);
            printf("�Բ���ϵͳ��⵽�����ֵ���㣬���Զ�ɾ���˺ţ��������ʣ��벦��10086\n");
			gets(a);
			if (strcmp(a,"�Ҳ�����")==0) {
				yanshi(time);
				printf("������Ĳ��������ϵͳ��̬ը�ˣ����ѱ���\n");
				while (h<12) {
					printf("ϵͳ�����У�������������Ļ�����ڷ�����ȣ�����������������������������������\n");
					yanshi(time);
					h++;
				}
				printf("����ʧ��,���Խ���10���ը��������׼��,����ʱ��ʼ\n");
				for (int i = 0,e=10; i < 10; i++,e--) {
					Sleep(1000);
					printf("               %d\n",e);
				}
				Sleep(1000);
				printf("               0");
			}
		}
		yanshi(time);
		printf("������������������������,��ը��ʼ\n");
		while (1) {
			printf("                 �ַ����˧��˧�ַ����˧��˧\n");
			yanshi(time);
		}
			return 0;

}