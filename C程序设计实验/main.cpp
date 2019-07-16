#include <stdio.h>
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define PI 3.1415926535

struct Node
{
	int x, y, num;
};
Node nodes[10000];                                                          //�м�����ÿգ���Ҫ����
int getScore = 0, nodeNum = 0;

struct Color
{
	int r, g, b;                                                            //���弰������ɫ���
};
Color bColor[] = { {255, 231, 186}, {238, 216, 174}, {205, 186, 150},       //����ɫ��16��
				   {139, 126, 102}, {255, 165, 79}, {238, 154, 73},
				   {205, 133, 63}, {139, 90, 43}, {255, 127, 36},
				   {238, 118, 32}, {205, 102, 29}, {139, 69, 19},
				   {255, 211, 155}, {238, 197, 145}, {205, 170, 125},
				   {139, 115, 85} };
Color cColor[] = { {131, 111, 255}, {122, 103, 238}, {105, 89, 205},        //����ɫ�����2^11�����11��
				   {71, 60, 139}, {72, 118, 255}, {67, 110, 238},
				   {58, 95, 205}, {39, 64, 139}, {0, 0, 255},
				   {0, 0, 238}, {0, 0, 205} };

void initAll();                                                             //��ʼ���ͼ�ν����ʼ��
void firstGraph();				     							 	  		//��ӭ�����ʼ��
void reStart();                                                             //���¿�ʼ��ť
int moveNode();                                                             //�ƶ�
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
	initgraph(600, 400);                                                     //��ʼ������ͼ
	firstGraph();                                                            //������ҳ����ӭ���棩
	while (1)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 140 && m.x <= 240 && m.y >= 200 && m.y <= 300)    //�������������ʼ��Ϸ��
		{
			initAll();                                                        //��ʼ����Ϸ���棬����������
			start();                                                          //��ʼ��Ϸ
		}
		else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 350 && m.x <= 450 && m.y >= 200 && m.y <= 300)         //���������Ϣ������˳���Ϸ��
			return 0;                                                                                      //��������
	}
	_getch();
	closegraph();
	return 0;
}
//��ʼ����Ϸ����
void initAll()
{
	int i, j, firstLocX, firstLocY, firstNum;                                                                //firstLocX��Y��ʾ�������0-3�����ӣ��Դ�ȷ��λ��
	TCHAR s[6] = _T("����:");                                                                                //�����������ַ���ֻ����TCHAR������ʾ�ڻ�����
	nodeNum = 0, getScore = 0;																				 //��ʼ���ڵ��������÷���
	memset(nodes, 0, sizeof(Node) * 10000);                                                                  //��սڵ���Ϣ
	srand(time(NULL));                                                                                       //ʱ�����ӣ��������
	//��ʼ������
	loadimage(NULL, _T("F:\\VS_test\\C�������ʵ��\\xk.jpg"));

	//��ʼ���Ҳ��ֵ÷����
	LOGFONT f;
	settextcolor(RGB(255, 106, 106));                                              //����������ɫΪRGB��ɫ
	gettextstyle(&f);
	f.lfHeight = 48;
	_tcscpy_s(f.lfFaceName, _T("����"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);                                                 //���ÿ����Ч��
	setbkmode(TRANSPARENT);                                           //���ñ���͸��
	outtextxy(450, 20, s);                                            //��(500, 20)�����������
	outtextxy(490, 80, _T("0"));                                      //��ʼ����Ϊ0

	//��ʼ�����飬���ڷ�����ɫ�ڽṹ����0-15��i��j�ֱ��0��ʼ�����ԣ�i+1��*��j+1����ʾ
	//�ַ�1-16�����ȥ1
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

	//��ʼ��2��2��4�ķ��飬Ϊ����������ʾ�ڷ����м䣬firstLocX��firstLocYҪ*100+40��30
	while(nodeNum != 2)
	{
		firstNum = rand() % 2;
		firstLocX = rand() % 4;
		firstLocX = firstLocX * 100 + 40;
		firstLocY = rand() % 4;
		firstLocY = firstLocY * 100 + 30;

		if (nodeNum == 1)
		{
			if (firstLocX == nodes[0].x * 100 + 40 && firstLocY == nodes[0].y * 100 + 30)               //�����ڶ����������ʱ��Ҫ�жϲ��ܳ����ڵ�һ�������λ��
				continue;
		}
		nodes[nodeNum].x = (firstLocX - 40) / 100;
		nodes[nodeNum].y = (firstLocY - 30) / 100;
		setbkmode(TRANSPARENT);
		_tcscpy_s(f.lfFaceName, _T("����"));
		if (firstNum == 0)                                                                              //firstNum��0��ʾ����2����1��ʾ����4�����β���ֻ��2��4
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

	//���¿�ʼ��ť
	settextstyle(36, 0, _T("����"));
	outtextxy(450, 350, _T("���¿�ʼ"));
}
//��ʼ����
void firstGraph()
{
	//�ȴ���ʼ
	LOGFONT f;
	RECT r = { 150, 300, 600, 400 };                                                                        //����drawtext��ָ������
	loadimage(NULL, _T("F:\\VS_test\\C�������ʵ��\\mt.jpg"));
	setbkmode(TRANSPARENT);                                                                                 //����������͸��ɫ��ʾ
	settextcolor(BLACK);                                                                                    //��������Ϊ��ɫ
	gettextstyle(&f);
	f.lfHeight = 60;
	_tcscpy_s(f.lfFaceName, _T("����"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);                                                                                       //���ÿ����Ч��
	outtextxy(235, 120, _T("2048"));                                                                        //��Ϸ��
	outtextxy(140, 200, _T("��ʼ  �˳�"));                                                                  //��ʼ��Ϸ���˳���Ϸ��ť
	settextstyle(20, 0, _T("����"));                                                                        //������Ϸ���������С
	drawtext(_T("�����ü��̽����������Ҳ���\n���Ʒ�����������ֱ���ϳ�2048"), &r, DT_WORDBREAK);           //��ָ���������Զ����и�ʽ��ʾ����
	memset(nodes, 0, sizeof(Node) * 10000);                                                                 //��ʼǰ��սڵ���Ϣ

	setbkmode(TRANSPARENT);                                                                                 //��������Ϣ
	outtextxy(450, 380, _T("�����ߣ��ؿ���"));
}
//���¿�ʼ
void reStart()
{
	MOUSEMSG m;
	TCHAR sscore[10];                                                                           //������intת��ΪTCHAR
	setfillcolor(RGB(238, 130, 238)); 
	fillrectangle(190, 200, 410, 250);                                                          //���¿�ʼ��ť�ľ���
	
	setbkmode(TRANSPARENT);
	settextcolor(RGB(72, 118, 255));
	settextstyle(48, 0, _T("����"));
	outtextxy(200, 200, _T("���¿�ʼ"));                                                        //�������¿�ʼ��ť

	setbkmode(TRANSPARENT);
	settextcolor(RGB(72, 118, 255));
	outtextxy(210, 260, _T("������"));
	_stprintf_s(sscore, _T("%d"), getScore);                                                     //������ת��ΪTCHAR��
	outtextxy(330, 260, sscore);                                                                 //��ʾ�÷�

	while (1)
	{
		m = GetMouseMsg();                                                                       //��ȡ�����Ϣ
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 190 && m.x <= 430 && m.y >= 200 && m.y <= 250)    //���������ϢΪ�����������¿�ʼ����ť
		{
			firstGraph();                                                                        //�ٴλص��׽���
			break;                                                                               //����ѭ�����ص���һ��
		}
	}
}
//�ƶ��㣬ʤ����ʧ�ܷ���0�����򷵻�1
int moveNode()
{
	char ch;
	while (_kbhit())                                                                       //�����õ�������Ϣ
	{
		ch = _getch();
		if (checkWin())                                                                     //����ʤ������
		{
			reStart();                                                                      //�������¿�ʼ��ť����ѡ�񷵻��׽���
			return 0;                                                                       //����
		}
		if (ch == 'a')                                                                      //�������ͷ
		{
			if (!moveToLeft())                                                              //����ʧ�ܣ���Ϸ����
			{
				reStart();                                                                  //�������¿�ʼ��ť����ѡ�񷵻��׽���
				return 0;																	//����
			}
		}
		if (ch == 'd')                                                                      //�����Ҽ�ͷ
		{
			if (!moveToRight())
			{
				reStart();
				return 0;
			}
		}
		if (ch == 'w')                                                                       //�����ϼ�ͷ
		{
			if (!moveToUp())
			{
				reStart();
				return 0;
			}
		}
		if (ch == 's')                                                                       //�����¼�ͷ
		{
			if (!moveToDown())
			{
				reStart();
				return 0;
			}
		}
	}
	return 1;																				 //δʧ�ܻ�ɹ�������1
}
//ÿ���ƶ����»�ͼ
void redraw()
{
	int i, j;
	TCHAR squareNum[10];                                                    //������������ת�ַ�
	//char str[10];
	TCHAR s[6] = _T("����:");
	//����
	loadimage(NULL, _T("F:\\VS_test\\C�������ʵ��\\xk.jpg"));

	//���µ÷�
	LOGFONT f;
	settextcolor(RGB(255, 106, 106));                                       //����������ɫΪRGB��ɫ
	gettextstyle(&f);
	f.lfHeight = 48;
	_tcscpy_s(f.lfFaceName, _T("����"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);                                                       //���ÿ����Ч��
	setbkmode(TRANSPARENT);  
	outtextxy(450, 20, s);                                                  //��(500, 20)�����������
	_stprintf_s(squareNum, _T("%d"), getScore);                             //������תΪTCHAR
	outtextxy(490, 80, squareNum);                                          //����ѵ÷���

	//��ʼ�����飬ͬ��
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
	//���¿�ʼ��ť
	settextstyle(36, 0, _T("����"));
	outtextxy(450, 350, _T("���¿�ʼ"));
	//����ÿ���ڵ㣬�ж���num��Ϊ0ʱ���������ֵȷ����ɫ��2-2048��log����1-11����˶�Ӧ11����ɫ
	for (i = 0; i < nodeNum; i++)
	{
		if (nodes[i].num == 0)
			continue;
		_stprintf_s(squareNum, _T("%d"), nodes[i].num);
		settextcolor(RGB(cColor[(int)log( nodes[i].num )].r, cColor[(int)log(nodes[i].num)].g, cColor[(int)log(nodes[i].num)].b));
		outtextxy(nodes[i].x * 100 + 40, nodes[i].y * 100 + 30, squareNum);
	}
}
//�����µĵ�
void proNode()                                                                         //�����µķ���
{
	int newX, newY, newNum, i, isExist = 0;                                            //newX newY��ʾ������ɵ�����꣬newNum��ʾ�µ�����0��1��2��ʾ2��4��8
	while (1)                                                                          //isExistΪ�жϴ�λ���Ƿ��Ѵ��ڵ�
	{
		isExist = 0;
		newX = rand() % 4;
		newY = rand() % 4;
		newNum = rand() % 3;
		for (i = 0; i < nodeNum; i++)                                                   //�жϲ����ķ���λ���Ƿ���ڷ���
		{
			if (newX == nodes[i].x && newY == nodes[i].y && nodes[i].num != 0)          //Ҫ��nodes[i].num��=0��ʾ��λ���е�
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
	int exsitSquare = 0;                                                                      //�Ƿ���ڷ���
	for (i = 0; i < nodeNum; i++)                                                             //����֪ÿ������м��
	{	
		if (nodes[i].num != 0)                                                                //���д���x=y=num=0���������
			exsitSquare++;
		if (exsitSquare == 16)                                                                //�Ѿ���16������
		{
			initgraph(600, 400);                                                              //��ʼ������
			setbkcolor(WHITE);                                                                //��ɫΪ��ɫ
			cleardevice();                                                    
			loadimage(NULL, _T("F:\\VS_test\\C�������ʵ��\\animal1.jpg"));                   //����ʧ��ͼƬ����
			
			setbkmode(TRANSPARENT);                                        
			settextcolor(RGB(25, 25, 112));
			settextstyle(48, 0, _T("����"));
			outtextxy(250, 120, _T("ʧ�ܣ�"));                                                 //ʧ��������ʾ
			return 0;                                                                          //ʧ�ܷ���0��δʧ�ܷ���1
		}
	}
	return 1;
}
//����
int moveToLeft()
{
	int i, j, hNode;                                                                                                   //hNode��ʾhaveNode��Ҫ�ƶ�λ���Ƿ��з���
	if (!check())                                                                                                      //check����Ϊ0��16����������
		return 0;
	for (i = 0; i < nodeNum; i++)                                                                                      //δ��ʱ�����ÿ�������Ƿ����ƶ�
	{
		hNode = 0;                                                                                                     //��ʼ��
		if (nodes[i].x > 0)                                                                                            //λ����߽�x=0���ĸ��Ӳ��ƶ���Ҫ��x>0
		{
			for (j = 0; j < nodeNum; j++)                                                                         
			{
				if (j == i)
					continue;
				if (nodes[i].x == nodes[j].x + 1 && nodes[i].y == nodes[j].y && nodes[i].num == nodes[j].num)           //�ƶ�Ŀ��λ������ͬ����
				{
					nodes[j].num *= 2;                                                                                  //Ŀ��λ�÷���ֵ*2
					nodes[i].x = nodes[i].y = nodes[i].num = 0;                                                         //���ƶ��ڵ����
					hNode = 1;                                                                                          //�������з�����
					getScore++;                                                                                         //�÷�
					break;
				}
				else if (nodes[i].x == nodes[j].x + 1 && nodes[i].y == nodes[j].y && nodes[i].num != nodes[j].num)      //�ƶ�Ŀ��λ���в�ֵͬ����
					hNode = 1;                                                                                          //���ǵ���������
			}
			if (!hNode)
				nodes[i].x--;                                                                                           //Ŀ��λ�ò����ڷ��飬�ƶ�
		}
	}
	proNode();                                                                                                          //ÿ���ƶ�������µķ���
	redraw();                                                                                                           //���»�ͼ
	return 1;                                                                                                           //�ɹ��ƶ�
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
	IMAGE img1, img2;                                                                               //Ϊ��תʤ��ͼƬ����
	for (i = 0; i < nodeNum; i++)
	{
		if (nodes[i].num == 2048)                                                                   //����һ�����ֵ�ﵽ2048
		{
			initgraph(600, 400);                                                                    //��ʾʤ��ͼƬ����
			setbkcolor(WHITE);
			cleardevice();
			loadimage(&img1, _T("F:\\VS_test\\C�������ʵ��\\ʤ��.jpg"));
			rotateimage(&img2, &img1, PI / 4);
			putimage(0, 0, &img2);


			setbkmode(TRANSPARENT);                                                                  //��ʾʤ������
			settextcolor(RGB(255, 69, 0));
			settextstyle(48, 0, _T("����"));
			outtextxy(250, 150, _T("�ɹ���"));
			return 1;                                                                                //�����ѳɹ�
		}
	}
	return 0;                                                                                        //������δ�ɹ�
}
//��ʼ������ʵ����������¿�ʼ�ͼ��̲����ƶ�
void start()
{
	MOUSEMSG m;
	int flag;                                                                                                                   //Ϊ1��ʾ�ɹ��ƶ���Ϊ0��ʾʤ������ʧ��
	while (1)
	{
		FlushMouseMsgBuffer();
		m = GetMouseMsg();

		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 450 && m.x <= 600 && m.y >= 350 && m.y <= 400)                                   //������¿�ʼ�����������沢����������
		{
			firstGraph();
			return;
		}
		flag = moveNode();
		if (!flag)                                                                                                              //�����»��ƣ�����������
			return;
	}
}