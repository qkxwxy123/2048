#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <time.h>

struct Node
{
	int x, y, num;
};
Node nodes[10000];                                                          //�м�����ÿգ���Ҫ����
int getScore = 0, nodeNum = 0;

void initAll();                                                             //��ʼ���ͼ�ν����ʼ��
void firstGraph();				     							 	  		//��ӭ�����ʼ��
void reStart();                                                             //���¿�ʼ��ť
void moveNode();                                                            //�ƶ�
void proNode();                                                             //����һ���漴����
void redraw();                                                              //���»���
int check();                                                                //�������Ƿ��Ѿ���
int moveToUp();																//����
int moveToDown();															//����
int moveToLeft();															//����
int moveToRight();															//����
int checkWin();                                                             //����Ƿ�ﵽ2048
void start();                                                               //��ʼ


int main(void)
{
	MOUSEMSG m;
	initgraph(600, 400);
	firstGraph();
	while (1)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 140 && m.x <= 240 && m.y >= 200 && m.y <= 300)
		{
			initAll();
			start();
		}
		else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 350 && m.x <= 450 && m.y >= 200 && m.y <= 300)
			return 0;
	}
	_getch();
	closegraph();
	return 0;
}
//��ʼ����Ϸ����
void initAll()
{
	int i, j, r, g, b, firstLocX, firstLocY, firstNum;
	TCHAR s[6] = _T("����:");
	MOUSEMSG m;
	nodeNum = 0, getScore = 0;
	memset(nodes, 0, sizeof(Node) * 10000);
	srand(time(NULL));
	//��ʼ������
	loadimage(NULL, _T("F:\\VS_test\\C�������ʵ��\\xk.jpg"));

	//��ʼ���Ҳ��ֵ÷����
	LOGFONT f;
	r = rand() % 255;
	g = rand() % 255;
	b = rand() % 255;
	settextcolor(RGB(r, g, b));                                              //����������ɫΪRGB��ɫ
	gettextstyle(&f);
	f.lfHeight = 48;
	_tcscpy_s(f.lfFaceName, _T("����"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);                                                 //���ÿ����Ч��
	setbkmode(TRANSPARENT);
	outtextxy(450, 20, s);                                            //��(500, 20)�����������
	outtextxy(490, 80, _T("0"));

	//��ʼ������
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

	//��ʼ��2��2��4�ķ���
	while(nodeNum != 2)
	{
		firstNum = rand() % 2;
		firstLocX = rand() % 4;
		firstLocX = firstLocX * 100 + 40;
		firstLocY = rand() % 4;
		firstLocY = firstLocY * 100 + 30;

		if (nodeNum == 1)
		{
			if (firstLocX == (nodes[0].x - 40) / 100 && firstLocY == (nodes[0].y - 30) / 100)
				continue;
		}
		nodes[nodeNum].x = (firstLocX - 40) / 100;
		nodes[nodeNum].y = (firstLocY - 30) / 100;
		setbkmode(TRANSPARENT);
		_tcscpy_s(f.lfFaceName, _T("����"));
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

	//���¿�ʼ��ť
	settextstyle(36, 0, _T("����"));
	outtextxy(450, 350, _T("���¿�ʼ"));
}
//��ʼ����
void firstGraph()
{
	//�ȴ���ʼ
	LOGFONT f;
	RECT r = { 150, 300, 600, 400 };
	loadimage(NULL, _T("F:\\VS_test\\C�������ʵ��\\mt.jpg"));
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	gettextstyle(&f);
	f.lfHeight = 60;
	_tcscpy_s(f.lfFaceName, _T("����"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);                                                 //���ÿ����Ч��
	outtextxy(235, 120, _T("2048"));
	outtextxy(140, 200, _T("��ʼ  �˳�"));
	settextstyle(20, 0, _T("����"));
	drawtext(_T("�����ü��̽����������Ҳ���\n���Ʒ�����������ֱ���ϳ�2048"), &r, DT_WORDBREAK);
	memset(nodes, 0, sizeof(Node) * 10000);
}
//���¿�ʼ
void reStart()
{
	MOUSEMSG m;
	setfillcolor(RGB(238, 130, 238));
	fillrectangle(190, 200, 430, 250);
	setbkmode(TRANSPARENT);
	settextcolor(RGB(25, 25, 112));
	settextstyle(48, 0, _T("����"));
	outtextxy(200, 200, _T("���¿�ʼ"));
	while (1)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 240 && m.x <= 480 && m.y >= 200 && m.y <= 300)
		{
			firstGraph();
			break;
		}
	}
}
//�ƶ���
void moveNode()
{
	char ch;
	MOUSEMSG m;
	while (_kbhit())
	{
		ch = _getch();
		if (checkWin())
		{
			reStart();
			return;
		}
		if (ch == 'a')                                                                      //�������ͷ
		{
			if (!moveToLeft())
			{
				reStart();
				return;
			}
		}
		if (ch == 'd')                                                                      //�����Ҽ�ͷ
		{
			if (!moveToRight())
			{
				reStart();
				return;
			}
		}
		if (ch == 'w')                                                                       //�����ϼ�ͷ
		{
			if (!moveToUp())
			{
				reStart();
				return;
			}
		}
		if (ch == 's')                                                                       //�����¼�ͷ
		{
			if (!moveToDown())
			{
				reStart();
				return;
			}
		}
	}
}
//ÿ���ƶ����»�ͼ
void redraw()
{
	int i, j, r, g, b;
	TCHAR squareNum[10];                                                 //������������ת�ַ�
	//char str[10];
	TCHAR s[6] = _T("����:");
	//����
	loadimage(NULL, _T("F:\\VS_test\\C�������ʵ��\\xk.jpg"));

	//���µ÷�
	LOGFONT f;
	r = rand() % 255;
	g = rand() % 255;
	b = rand() % 255;
	settextcolor(RGB(r, g, b));                                        //����������ɫΪRGB��ɫ
	gettextstyle(&f);
	f.lfHeight = 48;
	_tcscpy_s(f.lfFaceName, _T("����"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);                                                 //���ÿ����Ч��
	setbkmode(TRANSPARENT);
	outtextxy(450, 20, s);                                            //��(500, 20)�����������
	_stprintf_s(squareNum, _T("%d"), getScore);
	outtextxy(490, 80, squareNum);

	//��ʼ������
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
	//���¿�ʼ��ť
	settextstyle(36, 0, _T("����"));
	outtextxy(450, 350, _T("���¿�ʼ"));

	for (i = 0; i < nodeNum; i++)
	{
		if (nodes[i].num == 0)
			continue;
		_stprintf_s(squareNum, _T("%d"), nodes[i].num);
		outtextxy(nodes[i].x * 100 + 40, nodes[i].y * 100 + 30, squareNum);
	}
}
//�����µĵ�
void proNode()                                                                         //�����µķ���
{
	int newX, newY, newNum, i, isExist = 0;
	while (1)
	{
		isExist = 0;
		newX = rand() % 4;
		newY = rand() % 4;
		newNum = rand() % 3;
		for (i = 0; i < nodeNum; i++)                                                   //�жϲ����ķ���λ���Ƿ���ڷ���
		{
			if (newX == nodes[i].x && newY == nodes[i].y && nodes[i].num != 0)
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
//����Ƿ���16����δ�ﵽ2048
int check()
{
	int i;
	int exsitSquare = 0;
	for (i = 0; i < nodeNum; i++)
	{	
		if (nodes[i].num != 0)
			exsitSquare++;
		if (exsitSquare == 16)
		{
			setbkmode(TRANSPARENT);
			settextcolor(RGB(25, 25, 112));
			settextstyle(48, 0, _T("����"));
			outtextxy(250, 120, _T("ʧ�ܣ�"));
			return 0;
		}
	}
	return 1;
}
//����
int moveToLeft()
{
	int i, j, hNode;
	if (!check())
		return 0;
	for (i = 0; i < nodeNum; i++)
	{
		hNode = 0;
		if (nodes[i].x > 0)
		{
			for (j = 0; j < nodeNum; j++)
			{
				if (j == i)
					continue;
				if (nodes[i].x == nodes[j].x + 1 && nodes[i].y == nodes[j].y && nodes[i].num == nodes[j].num)
				{
					nodes[j].num *= 2;
					nodes[i].x = nodes[i].y = nodes[i].num = 0;
					hNode = 1;
					getScore++;
					break;
				}
				else if (nodes[i].x == nodes[j].x + 1 && nodes[i].y == nodes[j].y && nodes[i].num != nodes[j].num)
					hNode = 1;
			}
			if (!hNode)
				nodes[i].x--;
		}
	}
	proNode();
	redraw();
	return 1;
}
//����
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
//����
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
//����
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
//����Ƿ�ﵽ2048
int checkWin()
{
	int i;
	for (i = 0; i < nodeNum; i++)
	{
		if (nodes[i].num == 2048)
		{
			setbkmode(TRANSPARENT);
			settextcolor(RGB(255, 69, 0));
			settextstyle(48, 0, _T("����"));
			outtextxy(250, 150, _T("�ɹ���"));
			return 1;
		}
	}
	return 0;
}
//��ʼ������ʵ����������¿�ʼ�ͼ��̲����ƶ�
void start()
{
	MOUSEMSG m;
	int i, j, k;
	while (1)
	{
		FlushMouseMsgBuffer();
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 450 && m.x <= 600 && m.y >= 350 && m.y <= 400)                                   //������¿�ʼ�����������沢����������
		{
			firstGraph();
			return;
		}
		moveNode();
	}
}