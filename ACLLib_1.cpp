#include "acllib.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "framework.h"
#include "ACLLib_1.h"

const int edg = 600;//window edge
const int sid = 10;//row, col
const int wid = edg / sid;
const int timer = 300;
const int defaultlen = 5;//length

int wd[sid][sid];//window
int po[2] = { 0, 0,};//position
int len = defaultlen;//length
int dir = 0;//direction 0:Right,1:Down,2:Left,3:Up
const int bre = edg / sid - 6;//breadth
int rpo[2] = { 0, 0,};//rand point
bool sgn = true;//end sign

void reloadwd()
{
//	初始化矩阵信息
	for (int i = 0; i < sid; ++i)
	{
		for (int j = 0; j < sid; ++j)
		{
			wd[i][j] = 0;
		}
	}
	wd[0][0] = defaultlen;
	len = defaultlen;
	po[0] = 0;
	po[1] = 0;
	dir = 0;
}

void randpoint()
{
//	生成初始随机点
	rpo[0] = 0;
	rpo[1] = 0;
	srand(time(NULL));
	while (rpo[0] == 0 || rpo[0] == 1 || rpo[1] == 0)
	{
		rpo[0] = rand() % sid;
		rpo[1] = rand() % sid;
	}
	wd[rpo[0]][rpo[1]] = -1;
}

void paint()
{
//运算
	bool k = false;
	switch (dir)
	{
	case 0:
		if (po[0] < sid - 1 && wd[po[0]+1][po[1]] <= 0)
		{
			++po[0];
			k = true;
		}
		break;
	case 1:
		if (po[1] < sid - 1 && wd[po[0]][po[1]+1] <= 0)
		{
			++po[1];
			k = true;
		}
		break;
	case 2:
		if (po[0] > 0 && wd[po[0]-1][po[1]] <= 0)
		{
			--po[0];
			k = true;
		}
		break;
	case 3:
		if (po[1] > 0 && wd[po[0]][po[1]-1] <= 0)
		{
			--po[1];
			k = true;
		}
		break;
	}
	//判定这个移动方向是否合法 && 是否已通关
	if (k && sgn)
	{
		wd[po[0]][po[1]] = len;

		//绘制	
		beginPaint();
		{
			clearDevice();
			setPenColor(BLACK);
			setPenWidth(1);
			//绘制网格
			/*for (int i = 0; i < 600; i += wid)
			{
				line(i, 0, i, 600);
				line(0, i, 600, i);
			}*/
			//方格绘制&控制台调试
		//	rectangle(po[0] * bre, po[1] * bre, po[0] * bre + wid, po[1] * bre + wid);
			system("cls");//
			printf("===============================================================================\n");//
			for (int i = 0; i < sid; ++i)
			{
				for (int j = 0; j < sid; ++j)
				{
					printf("|%3.0f|", (double)wd[j][i]);//
					if (wd[i][j] > 0)//绘制snake
					{
						rectangle(i * wid + (wid - bre) / 2, j * wid + (wid - bre) / 2, i * wid - (wid - bre) / 2 + wid, j * wid - (wid - bre) / 2 + wid);
						if (abs(wd[i + 1][j] - wd[i][j]) <= 2 && wd[i + 1][j] != 0)
						{
							rectangle(i * wid + (wid - bre) / 2 + bre, j * wid + (wid - bre) / 2, i * wid + (wid - bre) / 2 + wid, j * wid - (wid - bre) / 2 + wid);
						}
						if (abs(wd[i][j + 1] - wd[i][j]) <= 2 && wd[i][j + 1] != 0)
						{
							rectangle(i * wid + (wid - bre) / 2, j * wid + (wid - bre) / 2 + bre, i * wid - (wid - bre) / 2 + wid, j * wid + (wid - bre) / 2 + wid);
						}
						--wd[i][j];
					}
					if (wd[i][j] < 0)//绘制红点
					{
						setPenColor(RED);
						setBrushColor(RED);
						ellipse(i * wid + (wid - bre) / 2, j * wid + (wid - bre) / 2, i * wid - (wid - bre) / 2 + wid, j * wid - (wid - bre) / 2 + wid);
						setPenColor(BLACK);
						setBrushColor(BLACK);
					}
				}
				printf("\n");//
			}
			printf("===============================================================================\n");//
		}
		endPaint();
		if (po[0] == rpo[0] && po[1] == rpo[1])
		{
			++len;
			if (len == sid * sid)
			{
				sgn = false;
				beginPaint();
				{
					setTextColor(RED);
					setTextSize(30);
					paintText(200, 300, "恭喜你！已通关");
					paintText(200, 350, "单击鼠标以重置");
				}
				endPaint();
			}
			else
			{
				do
				{
					rpo[0] = rand() % sid;
					rpo[1] = rand() % sid;
				} while (wd[rpo[0]][rpo[1]] != 0);
				wd[rpo[0]][rpo[1]] = -1;

				for (int i = 0; i < sid; ++i)
				{
					for (int j = 0; j < sid; ++j)
					{
						if (wd[i][j] > 0)
						{
							++wd[i][j];
						}
					}
				}
			}
		}
	}
	else if (!k)
	{
		beginPaint();
		{
			setTextColor(RED);
			setTextSize(30);
			paintText(200, 300, "移动方向非法！");
			paintText(200, 350, "单击鼠标以重置");
		}
		endPaint();
	}
	else
	{
		beginPaint();
		{
			setTextColor(RED);
			setTextSize(30);
			paintText(200, 300, "恭喜你！已通关");
			paintText(200, 350, "单击鼠标以重置");
		}
		endPaint();
	}
}

void mouseevent(int x, int y, int bt, int event)
{
	if (event != BUTTON_DOWN)
	{
		return;
	}
//重置
	len = 1;
	reloadwd();
	randpoint();
	sgn = true;

	paint();
}

void keyboardevent(int key, int event)
{
//操作
	if (event != KEY_DOWN)
	{
		return;
	}
	switch (key)
	{
	case VK_UP:
		dir = 3;
		break;
	case VK_DOWN:
		dir = 1;
		break;
	case VK_LEFT:
		dir = 2;
		break;
	case VK_RIGHT:
		dir = 0;
		break;
	}
	cancelTimer(0);
	startTimer(0, timer);
	paint();
}

void timerevent(int id)
{
	if (id == 0)
	{
		paint();
	}
}

int Setup()
{
	initConsole();
	
	initWindow("Snake", DEFAULT, DEFAULT, 600, 600);
	registerMouseEvent(mouseevent);
	registerKeyboardEvent(keyboardevent);
	registerTimerEvent(timerevent);
	reloadwd();

	startTimer(0, timer);

//生成初始随机点
	randpoint();

	paint();

	return 0;
}