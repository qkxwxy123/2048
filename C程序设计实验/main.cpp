#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <time.h>

struct Node
{
	int x, y, num;
};
Node nodes[17];
int getScore = 0, nodeNum = 0;

void initAll();                                                         //开始后的图形界面初始化
void firstGraph();														//欢迎界面初始化
void restartCount();                                                    //重新开始按钮
void moveNode();                                                        //移动
//void produceNode();                                                     //产生一个随即方块
void redraw();                                                          //更新画面

int main(void)
{
	MOUSEMSG m;
	initgraph(600, 400);
	firstGraph();
	while (1)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 140 && m.x <= 240 && m.y >= 200 && m.y <= 300)
			initAll();
		else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 350 && m.x <= 450 && m.y >= 200 && m.y <= 300)
			return 0;
		moveNode();
	}
	_getch();
	closegraph();
	return 0;
}
//初始化游戏界面
void initAll()
{
	int i, j, r, g, b, firstLocX, firstLocY, firstNum;
	srand(time(NULL));
	TCHAR s[6] = _T("分数:");
	//初始化背景
	loadimage(NULL, _T("F:\\VS_test\\C程序设计实验\\xk.jpg"));

	//初始化右部分得分情况
	LOGFONT f;
	r = rand() % 255;
	g = rand() % 255;
	b = rand() % 255;
	settextcolor(RGB(r, g, b));                                              //设置字体颜色为RGB颜色
	gettextstyle(&f);
	f.lfHeight = 48;
	_tcscpy_s(f.lfFaceName, _T("宋体"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);                                                 //设置抗锯齿效果
	setbkmode(TRANSPARENT);
	outtextxy(450, 20, s);                                            //在(500, 20)输出“分数”
	outtextxy(490, 80, _T("0"));

	//初始化方块
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1))
			{
				r = rand() % 255;
				g = rand() % 255;
				b = rand() % 255;
				setfillcolor(RGB(r, g, b));
				fillrectangle(i * 100 + 10, j * 100 + 10, (i + 1) * 100 - 10, (j + 1) * 100 - 10);
			}
			else
			{
				r = rand() % 255;
				g = rand() % 255;
				b = rand() % 255;
				setfillcolor(RGB(r, g, b));
				fillrectangle(i * 100 + 10, j * 100 + 10, (i + 1) * 100 - 10, (j + 1) * 100 - 10);
			}
		}
	}

	//初始化2个2或4的方块
	while(nodeNum != 2)
	{
		firstNum = rand() % 2;
		firstLocX = rand() % 4;
		firstLocX = firstLocX * 100 + 40;
		firstLocY = rand() % 4;
		firstLocY = firstLocY * 100 + 30;

		if (nodeNum == 1)
		{
			if (firstLocX == nodes[0].x && firstLocY == nodes[0].y)
				continue;
		}
		nodes[nodeNum].x = (firstLocX - 40) / 100;
		nodes[nodeNum].y = (firstLocY - 30) / 100;
		setbkmode(TRANSPARENT);
		_tcscpy_s(f.lfFaceName, _T("宋体"));
		if (firstNum == 0)
		{
			outtextxy(firstLocX, firstLocY, _T("2"));
			nodes[nodeNum++].num = 2;
		}
		else
		{
			nodes[nodeNum++].num = 4;
			outtextxy(firstLocX, firstLocY, _T("4"));
		}
	}


	//重新开始按钮
	settextstyle(36, 0, _T("隶书"));
	outtextxy(450, 350, _T("重新开始"));
	//restartCount();
}
//开始界面
void firstGraph()
{
	//等待开始
	LOGFONT f;
	RECT r = { 150, 300, 600, 400 };
	loadimage(NULL, _T("F:\\VS_test\\C程序设计实验\\mt.jpg"));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	gettextstyle(&f);
	f.lfHeight = 60;
	_tcscpy_s(f.lfFaceName, _T("隶书"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);                                                 //设置抗锯齿效果
	outtextxy(235, 120, _T("2048"));
	outtextxy(140, 200, _T("开始  退出"));
	settextstyle(20, 0, _T("隶书"));
	drawtext(_T("规则：用键盘进行上下左右操作\n控制方块左移右移直到合成2048"), &r, DT_WORDBREAK);
}

void restartCount()
{
	MOUSEMSG m;
	while (1)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 450 && m.x <= 600 && m.y >= 350 && m.y <= 400)
		{
			firstGraph();
			return;
		}
	}
}

void moveNode()
{
	char ch;
	int i, j;
	int hNode;
	Node exNode;
	while (_kbhit())
	{
		ch = _getch();
		if (ch == 'a')                                                                      //键盘左箭头
		{
			//produceNode();
			for (i = 0; i < nodeNum; i++)
			{
				hNode = 0;
				if (nodes[i].x > 0)
				{
					for (j = 0; j < nodeNum && j != i; j++)
					{
						if (nodes[i].x == nodes[j].x + 1 && nodes[i].y == nodes[j].y && nodes[i].num == nodes[j].num)
						{
							nodes[j].num *= 2;
							nodes[i].x = nodes[i].y = nodes[i].num = 0;
							exNode = nodes[i];
							nodes[i] = nodes[nodeNum];
							nodes[nodeNum] = exNode;
							nodeNum--;
							hNode = 1;
							getScore++;
							break;
						}
					}
					if (!hNode)
						nodes[i].x--;
				}
			}
			redraw();
		}
	}
}

void redraw()
{
	int i, j, r, g, b;
	char squareNum[10];                                                 //将方块中数字转字符
	char str[10];
	TCHAR s[6] = _T("分数:");
	//绘制
	loadimage(NULL, _T("F:\\VS_test\\C程序设计实验\\xk.jpg"));

	//更新得分
	LOGFONT f;
	r = rand() % 255;
	g = rand() % 255;
	b = rand() % 255;
	settextcolor(RGB(r, g, b));                                        //设置字体颜色为RGB颜色
	gettextstyle(&f);
	f.lfHeight = 48;
	_tcscpy_s(f.lfFaceName, _T("宋体"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);                                                 //设置抗锯齿效果
	setbkmode(TRANSPARENT);
	outtextxy(450, 20, s);                                            //在(500, 20)输出“分数”
	_itoa_s(getScore, str, 10);
	outtextxy(490, 80, (TCHAR)str);

	//初始化方块
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1))
			{
				r = rand() % 255;
				g = rand() % 255;
				b = rand() % 255;
				setfillcolor(RGB(r, g, b));
				fillrectangle(i * 100 + 10, j * 100 + 10, (i + 1) * 100 - 10, (j + 1) * 100 - 10);
			}
			else
			{
				r = rand() % 255;
				g = rand() % 255;
				b = rand() % 255;
				setfillcolor(RGB(r, g, b));
				fillrectangle(i * 100 + 10, j * 100 + 10, (i + 1) * 100 - 10, (j + 1) * 100 - 10);
			}
		}
	}
	//重新开始按钮
	settextstyle(36, 0, _T("隶书"));
	outtextxy(450, 350, _T("重新开始"));

	for (i = 0; i < nodeNum; i++)
	{
		_itoa_s(nodes[i].num, squareNum, 10);
		outtextxy(nodes[i].x * 100 + 40, nodes[i].y * 100 + 30, (TCHAR)squareNum);
	}
}