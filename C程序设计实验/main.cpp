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

void initAll();                                                         //��ʼ���ͼ�ν����ʼ��
void firstGraph();														//��ӭ�����ʼ��
void restartCount();                                                    //���¿�ʼ��ť
void moveNode();                                                        //�ƶ�
//void produceNode();                                                     //����һ���漴����
void redraw();                                                          //���»���

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
//��ʼ����Ϸ����
void initAll()
{
	int i, j, r, g, b, firstLocX, firstLocY, firstNum;
	srand(time(NULL));
	TCHAR s[6] = _T("����:");
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
			if (firstLocX == nodes[0].x && firstLocY == nodes[0].y)
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
	//restartCount();
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
		if (ch == 'a')                                                                      //�������ͷ
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
	char squareNum[10];                                                 //������������ת�ַ�
	char str[10];
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
	_itoa_s(getScore, str, 10);
	outtextxy(490, 80, (TCHAR)str);

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
		_itoa_s(nodes[i].num, squareNum, 10);
		outtextxy(nodes[i].x * 100 + 40, nodes[i].y * 100 + 30, (TCHAR)squareNum);
	}
}