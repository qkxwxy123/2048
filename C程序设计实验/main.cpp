/*----------------------------------------------------------------------------------------------------------
  -----------------------------------Copyright © 2019 kxxy. All rights reserved.----------------------------
  ----------------------------------------------------------------------------------------------------------*/

/*-------------------------------说明-----------------------------------------*/
/*用户登录机制，下次登陆可以打开上次记录，也可以重新开始*/
/*记录以一行3个数字，分别存储x,y,num,为标准存储nodes，直到存完读取下一行*/
/*记录ID下第一行为nodeNum 和 getScore*/
#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#define PI 3.1415926535

struct Node
{
	int x, y, num;
};
Node nodes[10000];                                                          //中间存在置空，需要开大
int getScore = 0, nodeNum = 0;
TCHAR Identity1[20];                                          //用户输入的ID
char Identity[20];                                            //转换成字符串的ID


struct Color
{
	int r, g, b;                                                            //字体及背景颜色组成
};
Color bColor[] = { {255, 231, 186}, {238, 216, 174}, {205, 186, 150},       //背景色共16种
				   {139, 126, 102}, {255, 165, 79}, {238, 154, 73},
				   {205, 133, 63}, {139, 90, 43}, {255, 127, 36},
				   {238, 118, 32}, {205, 102, 29}, {139, 69, 19},
				   {255, 211, 155}, {238, 197, 145}, {205, 170, 125},
				   {139, 115, 85} };
Color cColor[] = { {131, 111, 255}, {122, 103, 238}, {105, 89, 205},        //字体色，最大2^11因此由11种
				   {71, 60, 139}, {72, 118, 255}, {67, 110, 238},
				   {58, 95, 205}, {39, 64, 139}, {0, 0, 255},
				   {0, 0, 238}, {0, 0, 205} };

void initAll();                                                             //开始后的图形界面初始化
void firstGraph();				     							 	  		//欢迎界面初始化
void reStart();                                                             //重新开始按钮
int moveNode();                                                             //移动
void proNode();                                                             //产生一个随即方块
void redraw();                                                              //更新画面
int check();                                                                //检查格子是否已经满
int moveToUp();																//向上
int moveToDown();															//向下
int moveToLeft();															//向左
int moveToRight();															//向右
int checkWin();                                                             //检查是否达到2048
void start();                                                               //开始
void TcharToChar(const TCHAR* tchar, char* _char);                          //TCHAR转char
int Login();                                                                //登陆选项
void deleteN(char*);                                                        //删除换行符
void dataFind();                                                            //文件中数据找回
int preserve();                                                             //储存档案

int main(void)
{
	MOUSEMSG m;
	initgraph(600, 400);                                                     //初始化创建图
	firstGraph();                                                            //创建首页（欢迎界面）
	while (1)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 140 && m.x <= 240 && m.y >= 200 && m.y <= 300)    //读到鼠标点击“开始游戏”
		{
			initAll();                                                        //初始化游戏界面，生成两个块
			start();                                                          //开始游戏
		}
		else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 350 && m.x <= 450 && m.y >= 200 && m.y <= 300)         //读到鼠标信息点击“退出游戏”
			return 0;                                                                                      //结束程序
		else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 500 && m.x <= 600 && m.y >= 0 && m.y <= 50)
		{
			if (!Login())
				continue;
			dataFind();
			redraw();
			start();
		}
	}
	_getch();
	closegraph();
	return 0;
}
//初始化游戏界面
void initAll()
{
	int i, j, firstLocX, firstLocY, firstNum;                                                                //firstLocX及Y表示随机出第0-3个格子，以此确定位置
	TCHAR s[6] = _T("分数:");                                                                                //“分数：”字符串只能是TCHAR才能显示在画面上
	nodeNum = 0, getScore = 0;																				 //初始化节点数，所得分数
	memset(nodes, 0, sizeof(Node) * 10000);                                                                  //清空节点信息
	srand(time(NULL));                                                                                       //时间种子，随机数用
	//初始化背景
	loadimage(NULL, _T("F:\\VS_test\\C程序设计实验\\xk.jpg"));

	//初始化右部分得分情况
	LOGFONT f;
	settextcolor(RGB(255, 106, 106));                                              //设置字体颜色为RGB颜色
	gettextstyle(&f);
	f.lfHeight = 48;
	_tcscpy_s(f.lfFaceName, _T("宋体"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);                                                 //设置抗锯齿效果
	setbkmode(TRANSPARENT);                                           //设置背景透明
	outtextxy(450, 20, s);                                            //在(500, 20)输出“分数”
	outtextxy(490, 80, _T("0"));                                      //初始分数为0

	//初始化方块，由于方块颜色在结构体中0-15，i，j分别从0开始，则以（i+1）*（j+1）表示
	//又非1-16，则减去1
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1))
			{
				setfillcolor(RGB(bColor[(i + 1) * (j + 1) - 1].r, bColor[(i + 1) * (j + 1) - 1].g, bColor[(i + 1) * (j + 1) - 1].b));
				fillrectangle(i * 100 + 10, j * 100 + 10, (i + 1) * 100 - 10, (j + 1) * 100 - 10);
			}
			else
			{
				setfillcolor(RGB(bColor[(i + 1) * (j + 1) - 1].r, bColor[(i + 1) * (j + 1) - 1].g, bColor[(i + 1) * (j + 1) - 1].b));
				fillrectangle(i * 100 + 10, j * 100 + 10, (i + 1) * 100 - 10, (j + 1) * 100 - 10);
			}
		}
	}

	//初始化2个2或4的方块，为了让数字显示在方块中间，firstLocX，firstLocY要*100+40或30
	while(nodeNum != 2)
	{
		firstNum = rand() % 2;
		firstLocX = rand() % 4;
		firstLocX = firstLocX * 100 + 40;
		firstLocY = rand() % 4;
		firstLocY = firstLocY * 100 + 30;

		if (nodeNum == 1)
		{
			if (firstLocX == nodes[0].x * 100 + 40 && firstLocY == nodes[0].y * 100 + 30)               //产生第二个随机方块时，要判断不能出现在第一个方块的位置
				continue;
		}
		nodes[nodeNum].x = (firstLocX - 40) / 100;
		nodes[nodeNum].y = (firstLocY - 30) / 100;
		setbkmode(TRANSPARENT);
		_tcscpy_s(f.lfFaceName, _T("宋体"));
		if (firstNum == 0)                                                                              //firstNum是0表示数字2，是1表示数字4，初次产生只有2和4
		{
			settextcolor(RGB(cColor[0].r, cColor[0].g, cColor[0].b));
			outtextxy(firstLocX, firstLocY, _T("2"));
			nodes[nodeNum++].num = 2;
		}
		else
		{
			settextcolor(RGB(cColor[1].r, cColor[1].g, cColor[1].b));
			nodes[nodeNum++].num = 4;
			outtextxy(firstLocX, firstLocY, _T("4"));
		}
	}

	//重新开始按钮
	settextstyle(36, 0, _T("隶书"));
	outtextxy(450, 350, _T("重新开始"));

	//存档按钮
	settextstyle(36, 0, _T("隶书"));
	outtextxy(480, 300, _T("存档"));
}
//开始界面
void firstGraph()
{
	//等待开始
	LOGFONT f;
	RECT r = { 150, 300, 600, 400 };                                                                        //定义drawtext的指定区域
	loadimage(NULL, _T("F:\\VS_test\\C程序设计实验\\mt.jpg"));
	setbkmode(TRANSPARENT);                                                                                 //设置文字以透明色显示
	settextcolor(BLACK);                                                                                    //设置字体为黑色
	gettextstyle(&f);
	f.lfHeight = 60;
	_tcscpy_s(f.lfFaceName, _T("隶书"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);                                                                                       //设置抗锯齿效果
	outtextxy(235, 120, _T("2048"));                                                                        //游戏名
	outtextxy(140, 200, _T("开始  退出"));                                                                  //开始游戏和退出游戏按钮
	settextstyle(20, 0, _T("隶书"));                                                                        //更改游戏规则字体大小
	drawtext(_T("规则：用键盘进行上下左右操作\n控制方块左移右移直到合成2048"), &r, DT_WORDBREAK);           //在指定区域以自动换行格式显示规则
	memset(nodes, 0, sizeof(Node) * 10000);                                                                 //开始前清空节点信息

	setbkmode(TRANSPARENT);                                                                                 //开发者信息
	outtextxy(450, 380, _T("开发者：秦凯鑫"));
	
	settextstyle(40, 0, _T("隶书"));                                                                        //设置读档按钮
	outtextxy(500, 0, _T("读档"));
}
//重新开始
void reStart()
{
	MOUSEMSG m;
	TCHAR sscore[10];                                                                           //将分数int转换为TCHAR
	setfillcolor(RGB(238, 130, 238)); 
	fillrectangle(190, 200, 410, 250);                                                          //重新开始按钮的矩形
	
	setbkmode(TRANSPARENT);
	settextcolor(RGB(72, 118, 255));
	settextstyle(48, 0, _T("隶书"));
	outtextxy(200, 200, _T("重新开始"));                                                        //绘制重新开始按钮

	setbkmode(TRANSPARENT);
	settextcolor(RGB(72, 118, 255));
	outtextxy(210, 260, _T("分数："));
	_stprintf_s(sscore, _T("%d"), getScore);                                                     //将分数转换为TCHAR型
	outtextxy(330, 260, sscore);                                                                 //显示得分

	while (1)
	{
		m = GetMouseMsg();                                                                       //读取鼠标信息
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 190 && m.x <= 430 && m.y >= 200 && m.y <= 250)    //读到鼠标信息为左键点击“重新开始”按钮
		{
			firstGraph();                                                                        //再次回到首界面
			break;                                                                               //结束循环，回到上一层
		}
	}
}
//移动点，胜利或失败返回0，否则返回1
int moveNode()
{
	char ch;
	while (_kbhit())                                                                       //持续得到键盘信息
	{
		ch = _getch();
		if (checkWin())                                                                     //特判胜利条件
		{
			reStart();                                                                      //出现重新开始按钮并可选择返回首界面
			return 0;                                                                       //返回
		}
		if (ch == 'a')                                                                      //键盘左箭头
		{
			if (!moveToLeft())                                                              //左移失败，游戏结束
			{
				reStart();                                                                  //出现重新开始按钮并可选择返回首界面
				return 0;																	//返回
			}
		}
		if (ch == 'd')                                                                      //键盘右箭头
		{
			if (!moveToRight())
			{
				reStart();
				return 0;
			}
		}
		if (ch == 'w')                                                                       //键盘上箭头
		{
			if (!moveToUp())
			{
				reStart();
				return 0;
			}
		}
		if (ch == 's')                                                                       //键盘下箭头
		{
			if (!moveToDown())
			{
				reStart();
				return 0;
			}
		}
	}
	return 1;																				 //未失败或成功，返回1
}
//每次移动重新画图
void redraw()
{
	int i, j;
	TCHAR squareNum[10];                                                    //将方块中数字转字符
	//char str[10];
	TCHAR s[6] = _T("分数:");
	//绘制
	loadimage(NULL, _T("F:\\VS_test\\C程序设计实验\\xk.jpg"));

	//更新得分
	LOGFONT f;
	settextcolor(RGB(255, 106, 106));                                       //设置字体颜色为RGB颜色
	gettextstyle(&f);
	f.lfHeight = 48;
	_tcscpy_s(f.lfFaceName, _T("宋体"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);                                                       //设置抗锯齿效果
	setbkmode(TRANSPARENT);  
	outtextxy(450, 20, s);                                                  //在(500, 20)输出“分数”
	_stprintf_s(squareNum, _T("%d"), getScore);                             //将分数转为TCHAR
	outtextxy(490, 80, squareNum);                                          //输出已得分数

	//初始化方块，同上
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1))
			{
				setfillcolor(RGB(bColor[(i + 1) * (j + 1) - 1].r, bColor[(i + 1) * (j + 1) - 1].g, bColor[(i + 1) * (j + 1) - 1].b));
				fillrectangle(i * 100 + 10, j * 100 + 10, (i + 1) * 100 - 10, (j + 1) * 100 - 10);
			}
			else
			{
				setfillcolor(RGB(bColor[(i + 1) * (j + 1) - 1].r, bColor[(i + 1) * (j + 1) - 1].g, bColor[(i + 1) * (j + 1) - 1].b));
				fillrectangle(i * 100 + 10, j * 100 + 10, (i + 1) * 100 - 10, (j + 1) * 100 - 10);
			}
		}
	}
	//重新开始按钮
	settextstyle(36, 0, _T("隶书"));
	outtextxy(450, 350, _T("重新开始"));

	//存档按钮
	settextstyle(36, 0, _T("隶书"));
	outtextxy(480, 300, _T("存档"));

	//对于每个节点，判断其num不为0时，输出，由值确定颜色，2-2048求log后是1-11，因此对应11种颜色
	for (i = 0; i < nodeNum; i++)
	{
		if (nodes[i].num == 0)
			continue;
		_stprintf_s(squareNum, _T("%d"), nodes[i].num);
		settextcolor(RGB(cColor[(int)log( nodes[i].num )].r, cColor[(int)log(nodes[i].num)].g, cColor[(int)log(nodes[i].num)].b));
		outtextxy(nodes[i].x * 100 + 40, nodes[i].y * 100 + 30, squareNum);
	}
}
//产生新的点
void proNode()                                                                         //产生新的方块
{
	int newX, newY, newNum, i, isExist = 0;                                            //newX newY表示随机生成点的坐标，newNum表示新的数字0，1，2表示2，4，8
	while (1)                                                                          //isExist为判断此位置是否已存在点
	{
		isExist = 0;
		newX = rand() % 4;
		newY = rand() % 4;
		newNum = rand() % 3;
		for (i = 0; i < nodeNum; i++)                                                   //判断产生的方块位置是否存在方块
		{
			if (newX == nodes[i].x && newY == nodes[i].y && nodes[i].num != 0)          //要加nodes[i].num！=0表示此位置有点
			{
				isExist = 1;
				break;
			}
		}
		if (isExist)
			continue;
		nodes[nodeNum].x = newX;
		nodes[nodeNum].y = newY;
		switch (newNum)
		{
		case 0:
			nodes[nodeNum].num = 2, nodeNum++;
			break;
		case 1:
			nodes[nodeNum].num = 4, nodeNum++;
			break;
		case 2:
			nodes[nodeNum].num = 8, nodeNum++;
			break;
		}
		return;
	}
}
//检查是否满16个且未达到2048
int check()
{
	int i;
	int exsitSquare = 0;                                                                      //是否存在方块
	for (i = 0; i < nodeNum; i++)                                                             //对已知每个点进行检查
	{	
		if (nodes[i].num != 0)                                                                //其中存在x=y=num=0的无意义点
			exsitSquare++;
		if (exsitSquare == 16)                                                                //已经有16个方块
		{
			initgraph(600, 400);                                                              //初始化背景
			setbkcolor(WHITE);                                                                //底色为白色
			cleardevice();                                                    
			loadimage(NULL, _T("F:\\VS_test\\C程序设计实验\\animal1.jpg"));                   //加载失败图片表情
			
			setbkmode(TRANSPARENT);                                        
			settextcolor(RGB(25, 25, 112));
			settextstyle(48, 0, _T("隶书"));
			outtextxy(250, 120, _T("失败！"));                                                 //失败字体显示
			return 0;                                                                          //失败返回0，未失败返回1
		}
	}
	return 1;
}
//向左
int moveToLeft()
{
	int i, j, hNode;                                                                                                   //hNode表示haveNode，要移动位置是否有方块
	if (!check())                                                                                                      //check（）为0，16个格子已满
		return 0;
	for (i = 0; i < nodeNum; i++)                                                                                      //未满时，检查每个方块是否能移动
	{
		hNode = 0;                                                                                                     //初始化
		if (nodes[i].x > 0)                                                                                            //位于左边界x=0处的格子不移动，要求x>0
		{
			for (j = 0; j < nodeNum; j++)                                                                         
			{
				if (j == i)
					continue;
				if (nodes[i].x == nodes[j].x + 1 && nodes[i].y == nodes[j].y && nodes[i].num == nodes[j].num)           //移动目标位置有相同方块
				{
					nodes[j].num *= 2;                                                                                  //目标位置方块值*2
					nodes[i].x = nodes[i].y = nodes[i].num = 0;                                                         //被移动节点清空
					hNode = 1;                                                                                          //打上已有方块标记
					getScore++;                                                                                         //得分
					break;
				}
				else if (nodes[i].x == nodes[j].x + 1 && nodes[i].y == nodes[j].y && nodes[i].num != nodes[j].num)      //移动目标位置有不同值方块
					hNode = 1;                                                                                          //打标记但不动方块
			}
			if (!hNode)
				nodes[i].x--;                                                                                           //目标位置不存在方块，移动
		}
	}
	proNode();                                                                                                          //每次移动后产生新的方块
	redraw();                                                                                                           //重新画图
	return 1;                                                                                                           //成功移动
}
//向右
int moveToRight()
{
	int i, j, hNode;
	if (!check())
		return 0;
	for (i = 0; i < nodeNum; i++)
	{
		hNode = 0;
		if (nodes[i].x >= 0 && nodes[i].x < 3)
		{
			for (j = 0; j < nodeNum; j++)
			{
				if (j == i)
					continue;
				if (nodes[i].x == nodes[j].x - 1 && nodes[i].y == nodes[j].y && nodes[i].num == nodes[j].num)
				{
					nodes[j].num *= 2;
					nodes[i].x = nodes[i].y = nodes[i].num = 0;
					hNode = 1;
					getScore++;
					break;
				}
				else if (nodes[i].x == nodes[j].x - 1 && nodes[i].y == nodes[j].y && nodes[i].num != nodes[j].num)
					hNode = 1;
			}
			if (!hNode)
				nodes[i].x++;
		}
	}
	proNode();
	redraw();
	return 1;
}
//向上
int moveToUp()
{
	int i, j, hNode;
	if (!check())
		return 0;
	for (i = 0; i < nodeNum; i++)
	{
		hNode = 0;
		if (nodes[i].y > 0)
		{
			for (j = 0; j < nodeNum; j++)
			{
				if (j == i)
					continue;
				if (nodes[i].y == nodes[j].y + 1 && nodes[i].x == nodes[j].x && nodes[i].num == nodes[j].num)
				{
					nodes[j].num *= 2;
					nodes[i].x = nodes[i].y = nodes[i].num = 0;
					hNode = 1;
					getScore++;
					break;
				}
				else if (nodes[i].y == nodes[j].y + 1 && nodes[i].x == nodes[j].x && nodes[i].num != nodes[j].num)
					hNode = 1;
			}
			if (!hNode)
				nodes[i].y--;
		}
	}
	proNode();
	redraw();
	return 1;
}
//向下
int moveToDown()
{
	int i, j, hNode;
	if (!check())
		return 0;
	for (i = 0; i < nodeNum; i++)
	{
		hNode = 0;
		if (nodes[i].y >= 0 && nodes[i].y < 3)
		{
			for (j = 0; j < nodeNum; j++)
			{
				if (j == i)
					continue;
				if (nodes[i].y == nodes[j].y - 1 && nodes[i].x == nodes[j].x && nodes[i].num == nodes[j].num)
				{
					nodes[j].num *= 2;
					nodes[i].x = nodes[i].y = nodes[i].num = 0;
					hNode = 1;
					getScore++;
					break;
				}
				else if (nodes[i].y == nodes[j].y - 1 && nodes[i].x == nodes[j].x && nodes[i].num != nodes[j].num)
					hNode = 1;
			}
			if (!hNode)
				nodes[i].y++;
		}
	}
	proNode();
	redraw();
	return 1;
}
//检查是否达到2048
int checkWin()
{
	int i;
	IMAGE img1, img2;                                                                               //为旋转胜利图片而设
	for (i = 0; i < nodeNum; i++)
	{
		if (nodes[i].num == 2048)                                                                   //存在一个点的值达到2048
		{
			initgraph(600, 400);                                                                    //显示胜利图片背景
			setbkcolor(WHITE);
			cleardevice();
			loadimage(&img1, _T("F:\\VS_test\\C程序设计实验\\胜利.jpg"));
			rotateimage(&img2, &img1, PI / 4);
			putimage(0, 0, &img2);


			setbkmode(TRANSPARENT);                                                                  //显示胜利字样
			settextcolor(RGB(255, 69, 0));
			settextstyle(48, 0, _T("黑体"));
			outtextxy(250, 150, _T("成功！"));
			return 1;                                                                                //返回已成功
		}
	}
	return 0;                                                                                        //返回仍未成功
}
//开始，控制实现鼠标点击重新开始和键盘操作移动
void start()
{
	MOUSEMSG m;
	int flag;                                                                                                                   //为1表示成功移动，为0表示胜利或者失败
	while (1)
	{
		FlushMouseMsgBuffer();
		m = GetMouseMsg();

		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 450 && m.x <= 600 && m.y >= 350 && m.y <= 400)        //如果重新开始，绘制主界面并返回主函数
		{
			firstGraph();
			return;
		}
		else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 480 && m.x <= 550 && m.y >= 300 && m.y <= 350)   //如果存档，存储数据并绘制主界面返回
		{
			if (!preserve())
				continue;
			firstGraph();
			return ;
		}
		flag = moveNode();
		if (!flag)                                                                                   //有重新绘制，返回主函数
			return;
	}
}
//TCHAR转char
void TcharToChar(const TCHAR* tchar, char* _char)
{
	int iLength;
	//获取字节长度   
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	//将tchar值赋给_char    
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}
//登陆
int Login()
{
	memset(Identity1, 0, sizeof(Identity1));
	memset(Identity, 0, sizeof(Identity));
	if (!InputBox(Identity1, 16, _T("请输入ID(ID为找回\n数据的凭证,最大15位数)"), _T("登陆"), _T("请输入ID"), 0, 0, false))
		return 0;                                                                                                         //弹窗输入账号
	TcharToChar(Identity1, Identity);                                                                                     //TCHAR转char类型       
	return 1;
}
//删除换行符
void deleteN(char * s)
{
	int len;
	len = strlen(s);
	if (s[len - 1] == '\n')
		s[len - 1] = '\0';
}
//从文档中将数据找回
void dataFind()
{
	FILE* f;
	char datas[20];
	char nums[10];
	char getIdentity[20];
	int len = 0, i, j, counts = 0, thisNode = 0;                                            //thisNode标记当前为第几个node
	nodeNum = 0, getScore = 0;
	memset(nodes, 0, sizeof(nodes));                                                        //清空节点集以备用
	memset(nums, 0, sizeof(nums));
	f = fopen("data.txt", "r");                                                             //打开文件data.txt
	while (1)
	{
		fgets(getIdentity, sizeof(getIdentity), f);                                         //尝试读取一个ID
		deleteN(getIdentity);
		if (!strcmp(getIdentity, Identity))                                                  //读到匹配的ID，开始抓取数据
		{
			fgets(datas, sizeof(datas), f);                                                 //ID下一行为nodeNum和getScore，单独处理
			deleteN(datas);                                                                 //删除datas换行符
			len = strlen(datas), j = 0;
			for (i = 0; i < len; i++)                                                       //对每一个字符串单独处理
			{
				if (datas[i] == ' ')                                                        //碰到两个数字之间的间断
				{
					nums[j++] = '\0';
					nodeNum = atoi(nums);
					memset(nums, 0, sizeof(nums));
					j = 0;
					continue;
				}
				nums[j++] = datas[i];                                                        //复制字符串
			}
			nums[j++] = '\0';
			getScore = atoi(nums);                                                           //将第二个数字赋值给getScore
			memset(nums, 0, sizeof(nums)), len = 0, j = 0;
			while (strchr(datas, ' ') != NULL && fgets(datas, sizeof(datas), f) != NULL)     //读入nodes
			{
				len = strlen(datas); 
				memset(nums, 0, sizeof(nums)), j = 0, counts = 0;
				for (i = 0; i < len; i++)                                                    //对于读入的一行
				{
					if (datas[i] == ' ')                                                     //碰到了空格
					{
						nums[j++] = '\0';                                                    //加终止符以形成字符串
						switch (counts)                                                      //counts为0表示第一个数字，1表示第二个数字
						{
						case 0:
							nodes[thisNode].x = atoi(nums);
							break;
						case 1:
							nodes[thisNode].y = atoi(nums);
							break;
						}
						counts++;                                                            //每记过一个+1
						memset(nums, 0, sizeof(nums));
						j = 0;
						continue;
					}
					nums[j++] = datas[i];
				}
				nums[j++] = '\0';                                                            //终止后nums少了个\0，加上后形成字符串
				nodes[thisNode].num = atoi(nums);
				thisNode++;                                                                  //node数加一
			}
			break;                                                                           //得到了存档，循环结束
		}
	}
}
//存档
int preserve()
{
	char data[100], exNum[10];
	char readName[20];
	int len, i, flag = 0;
	int p = 0;
	FILE* f;
	f = fopen("data.txt", "a+");                                                             //以更新方式打开文本

	while (1)                                                                                //判重
	{
		if (p == 0)
		{
			if (!InputBox(Identity1, 16, _T("请输入提取时的ID(ID为唯一凭证，\n请妥善保管)"), _T("存档"), _T("请输入ID"), 0, 0, false))
				return 0;
		}
		else if (p > 0)
		{
			if (!InputBox(Identity1, 16, _T("请输入提取时的ID(ID为唯一凭证，\n请妥善保管) ID已存在"), _T("存档"), _T("请输入ID"), 0, 0, false))
				return 0;
		}
		TcharToChar(Identity1, Identity);

		flag = 0;
		len = strlen(Identity);
		Identity[len] = '\n';                                                                //在行末加换行
		while (fgets(readName, sizeof(readName), f) != NULL)                                 //对文件进行按行读取
			if (!strcmp(Identity, readName))                                                 //存在相同的ID就打上标记
			{
				flag = 1;
				break;
			}
		if (flag == 1)                                                                       //存在相同ID再次弹窗输入
		{
			p++;
			continue;
		}
		else
		{
			fputs(Identity, f);                                                              //存储ID
			break;
		}
	}
	_itoa(nodeNum, data, 10);                                                                //nodeNum转字符后写入
	fputs(data, f);
	fputs(" ", f);
	memset(data, 0, sizeof(data));
	_itoa(getScore, data, 10);                                                               //getScore转字符后写入
	fputs(data, f);
	fputs("\n", f);
	memset(data, 0, sizeof(data));
	for (i = 0; i < nodeNum; i++)
	{
		memset(data, 0, sizeof(data));

		_itoa(nodes[i].x, exNum, 10);                                                        //存节点x坐标在exNum中
		strcat(data, exNum);
		memset(exNum, 0, sizeof(exNum));
		strcat(data, " ");

		_itoa(nodes[i].y, exNum, 10);                                                        //存节点y坐标在exNum中
		strcat(data, exNum);
		memset(exNum, 0, sizeof(exNum));
		strcat(data, " ");

		_itoa(nodes[i].num, exNum, 10);                                                      //存节点num在exNum中
		strcat(data, exNum);
		memset(exNum, 0, sizeof(exNum));
		strcat(data, "\n");

		fputs(data, f);
	}
	fclose(f);
	return 1;
}