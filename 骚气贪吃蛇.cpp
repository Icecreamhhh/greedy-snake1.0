#include<stdio.h>
#include<conio.h>//键盘
#include<graphics.h>//图形多媒体
#include<mmsystem.h>//音乐播放 
#include<stdlib.h>//随机数种子
#include"resource.h"
#pragma comment(lib,"winmm.lib")//音乐播放
#define maxnum 900

void BGM();
void start();
void end();
void gameinit();
void gamedraw();
void snakemove();
void command();
void eatfood();

//【添加音乐】
void BGM()
{
/*	if (0 == PlaySound(MAKEINTRESOURCE(IDR_WAV1), NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP))
	{
		printf("playsound false");
	}

	system("pause");*/
	PlaySound(MAKEINTRESOURCE(IDR_WAV1), NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);
}
//【添加开始窗口】
void start()
{
	HWND hnd = GetHWnd();
	SetWindowText(hnd, "《TGA2024年度最佳游戏》");
	int judge = MessageBox(hnd, "Enjoy Your Game and Life !\n\n作者：Icecream\n\nBGM: 花（feat.花谱）\n\n游戏规则：\n使用方向键↑←↓→等控制您的蛇运动，蛇的速度会逐渐加快\n进食会改变长度和大小，但需要注意如果咬到自己会发生断裂，这会减少得分\n\n撞到墙壁则游戏结束，控制台会显示您的分数，之后按下回车键关闭游戏\n操纵您的贪吃蛇成为泰坦巨物吧！", "前言", MB_OKCANCEL);
	if (judge == IDCANCEL)
	{
		printf("游戏结束\n");
	}
	else if (judge == IDOK)
	{
		printf("游戏开始\n");
	}
}
//【添加结束窗口】
void end()
{
	HWND hnd = GetHWnd();
	SetWindowText(hnd, "《TGA2024年度最佳游戏》");
	int judge = MessageBox(hnd, "您是否要退出游戏？", "尾声", MB_OK);
	if (judge == IDOK)
		printf("游戏结束\n");
}
//设置蛇的结构
struct snake
{
	int size;     //节长
	int range;    //粗度(半径）
	int dir;      //方向
	int turn;     //控制开关
	int king;     //蛇头
	//颜色
	int r[maxnum];
	int g[maxnum];
	int b[maxnum];
	POINT adress[maxnum];//坐标(point是一个定义好的包含x，y的结构体)
}snake;
//设置食物的结构
struct food
{
	int number;  //初始数量
	int color;   //颜色
	int flag;    //食用判定
	int range;   //大小
	int time;    //吃到食物次数
	int x;
	int y;
	POINT adress[maxnum];
}food;
//游戏初始化
void gameinit()
{
	initgraph(800, 800, SHOWCONSOLE);
	setbkcolor(WHITE);
	BGM();
	start();
	//设置文字样式
	settextcolor(RED);
	//初始化蛇
	snake.size = 6;
	snake.range = 10;
	snake.dir = 2;
	snake.adress[0].x = 100;
	snake.adress[0].y = 100;
	snake.turn = 1;
	snake.king = 4;
	for (int i = 0; i < snake.size; i++)
	{
		snake.adress[i + 1].x = snake.adress[i].x + snake.range;
		snake.adress[i + 1].y = snake.adress[i].y;
	}
	//初始化食物
	srand(GetTickCount());//获取系统开机到现在的时间
//	food.number = 10;
	food.time = 0;
	food.color = RGB(rand() % 150, rand() % 150, rand() % 150);
	food.range = rand() % 4 + 6;
	food.x = rand() % 700+50;
	food.y = rand() % 700+50;
	//rand()随机产生一个整数，若没有设计随机种子，每次产生都是固定整数
	//设置种子需要头文件stdlib，一般把时间作为种子
	food.flag = 0;
}
//游戏绘制
void gamedraw()
{
	//double soft-closing
	BeginBatchDraw();
	//clear device
	cleardevice();
	//snake draw
	//colorchange
	snake.r[0] = 15;
	snake.g[0] = 70;
	snake.b[0] = 170;
	for (int i = 1; i < snake.size; i++)
	{
		snake.r[i] = snake.r[i - 1] + 20;
		snake.g[i] = snake.g[i - 1] + 10;
		snake.b[i] = snake.b[i - 1] - 20;
	}
	for (int i = 0; i < snake.size; i++)
	{
		setfillcolor(RGB(snake.r[i], snake.g[i], snake.b[i]));
		solidcircle(snake.adress[i].x, snake.adress[i].y, snake.range);
	}
	setfillcolor(WHITE);
	solidcircle(snake.adress[0].x, snake.adress[0].y, snake.range - snake.king);
	//border draw
	setlinecolor(RGB(snake.r[snake.size-1], snake.g[snake.size-1], snake.b[snake.size-1]));
	setlinestyle(PS_SOLID, 16);
	rectangle(1, 1, 799, 799);
	//fooddraw
	if (food.flag == 0)
	{
		setfillcolor(food.color);
		solidcircle(food.x, food.y, food.range);
	}
	else if (food.flag)
	{
		food.color = RGB(rand() % 150, rand() % 150, rand() % 150);
		food.x = rand() % 760+20;
		food.y = rand() % 760+20;
		setfillcolor(food.color);
		solidcircle(food.x, food.y, food.range);
		food.flag = 0;
	}
	FlushBatchDraw();
}
void snakemove()
{
	command();
	for (int i = snake.size - 1; i > 0; i--)
	{
		snake.adress[i].x = snake.adress[i - 1].x;
		snake.adress[i].y = snake.adress[i - 1].y;
	}
	switch (snake.dir)
	{
	case 1:
		snake.adress[0].y -=snake.range;
		break;
	case 2:
		snake.adress[0].y +=snake.range;
		break;
	case 3:
		snake.adress[0].x -=snake.range;
		break;
	case 4:
		snake.adress[0].x +=snake.range;
		break;
	}
	if (snake.adress[0].x <= 790 && snake.adress[0].x >= 10 && snake.adress[0].y >= 10 && snake.adress[0].y <= 790)
	{
		snake.turn = 1;
		for (int i = 1; i < snake.size; i++)
		{
			/*模式1 咬到自己游戏结束
			if (snake.adress[i].x == snake.adress[0].x && snake.adress[i].y == snake.adress[0].y)
			{
				settextstyle(40, 0, "楷体");
				outtextxy(280, 300, "GAME OVER!");
				printf("gameover\n");
				snake.turn = 0;
			}*/
			//模式2 咬到自己则截断
			if (snake.adress[i].x == snake.adress[0].x && snake.adress[i].y == snake.adress[0].y)
			{
				snake.size = i;
			}
		}
	}
	else
	{
		settextstyle(40, 0, "黑体");
		outtextxy(280, 300, "GAME OVER!");
		printf("gameover\n");
		printf("你的分数是%d", snake.size*2);
		snake.turn = 0;
	}
}
void command()
{
//	printf("%d %c\n", snake.adress[0].x, snake.adress[0].y);
	if (GetAsyncKeyState(VK_UP))
	{
		snake.dir = 1;
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		snake.dir = 2;
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		snake.dir = 3;
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		snake.dir = 4;
	}
}
void eatfood()
{
	if ((food.x - snake.adress[0].x) * (food.x - snake.adress[0].x) + (food.y - snake.adress[0].y) * (food.y - snake.adress[0].y) <= (food.range + snake.range) * (snake.range + food.range))
	{
		food.flag = 1;
		snake.size++;
		food.time++;
	}
	if (food.time == 4)
	{
		snake.range += 1;
		snake.king += 1;
		food.time = 0;
	}
}
//游戏总运行
int main()
{
	gameinit();
	gamedraw();
	while (snake.turn)
	{
		gamedraw();
		snakemove();
		//设置延迟...毫秒
		Sleep(32);
		eatfood();
	}
	EndBatchDraw();
	getchar();
	end();
	closegraph();
}