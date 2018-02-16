#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <malloc.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string>

#define maxsize 5
#define len 20
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define UP 4
char edg[len][len];
int length = 0;

using namespace std;

typedef pair<int,int> p;
pair<int, int> items[3];



typedef struct SNode
{
	int data;
	struct SNode *next;
}SNode, *Slink;

typedef struct mazenode
{
	char edg[len][len];
	mazenode *next;
}mazenode;

typedef struct BiTNode						//决策二叉树
{
	int data;
	struct BiTNode *lchild, *rchild;
}BiTNode, *BiTree;

BiTree BT;

mazenode *head;

SNode *top = NULL;

void push(int e);

int empty();								//判断栈是否为空

int pop();

void showmaze();

void createmaze();							//新建随机迷宫AA

void create();                              //新建有效迷宫

int searchexit();							//自动寻找迷宫出口

void savemaze1();							//存储当前迷宫

void savemaze2();

void readmaze1();							//读取指定迷宫 bug:输入字母

void readmaze2();

void createmaze100();

void movemaze();

int battle();

void CreateBiTree();

void savepos(int a,int b);

void readpos(int *a, int *b);

void op();

void autosearch(int a,int b);

void main()
{
	int i, j, a, b, x;
	head = (mazenode *)malloc(sizeof(mazenode));
	head->next = NULL;
	while (1)
	{
		system("cls");
		printf("\n\n");
		printf("\t\t1.随机生成一个有出口的迷宫\n\n");
		printf("\t\t2.打印显示迷宫\n\n");
		printf("\t\t3.寻找迷宫出口\n\n");
		printf("\t\t4.存储当前迷宫（即时存储）\n\n");
		printf("\t\t5.读取任意指定已存储迷宫（即时读取）\n\n");
		printf("\t\t6.存储当前迷宫（文件存储d:\\maze.dat）\n\n");
		printf("\t\t7.读取任意指定已存储迷宫（文件读取）\n\n");
		printf("\t\t8.添加100个地图于文件(d:\\maze.dat)\n\n");
		printf("\t\t9.移动角色\n\n");
		printf("\t\t10.退出\n\n");
		printf("\t\t请输入选择的序号(1-10): ");
		scanf_s("%d", &x);
		if (x == 10) break;
		switch (x)
		{
		case 1: create(); break;
		case 2: showmaze(); break;
		case 3: cout<<searchexit(); system("pause"); break;
		case 4: savemaze1(); break;
		case 5: readmaze1(); break;
		case 6: savemaze2();
			printf("\t\t\n存储完成\n\n");
			system("pause"); break;
		case 7: readmaze2(); break;
		case 8: createmaze100(); break;
		case 9: op(); movemaze(); break;
		default: printf("\n\t\t请输入正确的序号!"); getchar(); getchar();
		}
	}
}

void push(int e)
{
	SNode *p;
	p = (SNode *)malloc(sizeof(SNode));
	p->data = e;
	p->next = top;
	top = p;
}

int empty() //判断栈是否为空
{
	if (top == NULL)
		return 1;
	return 0;
}

int pop()
{
	int e;
	SNode *p;
	e = top->data;
	p = top;
	top = p->next;
	free(p);
	return e;
}

void showmaze()
{
	system("cls");
	printf("\t");
	for (int i = 0; i<len; i++)	//将迷宫打印显示
	{
		printf("\n");
		for (int j = 0; j<len; j++)
		{
			if (edg[i][j] == 004) {
				printf("■");
			}
			if (edg[i][j] == 000)
			{
				printf("  ");
			}
			if (edg[i][j] == 001)
				printf("%c ", edg[i][j]);
			if (edg[i][j] == '>')
				printf(">>");
		}
	}
	printf("\n");
	system("pause");
}

void createmaze() //新建随机迷宫
{
	int a, b;
	for (int i = 0; i<len; i++) //设置迷宫边界
	{
		for (int j = 0; j<len; j++)
		{
			if (!i || i == len - 1 || !j || j == len - 1)
			{
				edg[i][j] = 004;
			}
			else
				edg[i][j] = 000;
		}
	}
	srand((unsigned)time(NULL));
	for (int i = 0; i<len * 2 / 4; i++) //随机设置迷宫的墙
	{
		for (int j = 0; j<len * 3 / 4; j++)
		{
			a = rand() % len;
			b = rand() % len;
			edg[a][b] = 004;
		}
	}
	for (int i = 1; i < 2; i++) //设
	{                                         //(可以设置大小)
		edg[i][0] = 000;
		edg[len - 1 - i][len - 1] = 000;
	}
	edg[1][1] = 001;
	//设置起点符号
	edg[len - 2][len - 2] = NULL;							//为保证尽可

	edg[len - 2][len - 3] = NULL;						//设置成通路
	edg[len - 3][len - 2] = NULL;
}

void create() {
	createmaze();
	Sleep(1000);
	//cout << searchexit();
	while (searchexit()!=1) {
		//cout << searchexit();
		//system("pause");
		Sleep(1000);
		createmaze();
	}
}

void movemaze()
{
	int a = 1, b = 1, x, y;
	for (int i = 0; i < 3; i++)
		items[i] = make_pair(0, 0);
	showmaze();
	while (char c = _getch())//阻塞方式获取键盘按下的值
	{
		switch (c)//判断按下的是哪个键并分别作处理
		{
		case 'w':
			if (edg[a - 1][b] == 000|| edg[a - 1][b] == '>') //向上走一步
			{
				edg[a][b] = 000;
				a--;
				edg[a][b] = 001;
			}
			break;
		case 'a':
			if (edg[a][b - 1] == 000|| edg[a][b - 1] == '>') //向左走一步
			{
				edg[a][b] = 000;
				b--;
				edg[a][b] = 001;
			}
			break;
		case 's':
			if (edg[a + 1][b] == 000|| edg[a + 1][b] == '>') //向下走一步
			{
				edg[a][b] = 000;
				a++;
				edg[a][b] = 001;
			}
			break;
		case 'd':
			if (edg[a][b + 1] == 000|| edg[a][b + 1] == '>') //向右走一步
			{
				edg[a][b] = 000;
				b++;
				edg[a][b] = 001;
			}
			break;
		case 'm':
			savepos(a,b);
			
			break;
		case 'l':
			readpos(&a, &b);

			break;
		case 'k':
			autosearch(a,b);

			break;
		}
		system("cls");
		printf("\t");
		for (int i = 0; i<len; i++)	//将迷宫打印显示
		{
			printf("\n");
			for (int j = 0; j<len; j++)
			{
				if (edg[i][j] == 004) {
					printf("■");
				}
				if (edg[i][j] == 000)
				{
					printf("  ");
				}
				if (edg[i][j] == 001)
					printf("%c ", edg[i][j]);
				if (edg[i][j] == '>')
					printf(">>");
			}
		}
		printf("\n");
		cout << "W S A D 控制上、下、左、右,按 M 保存当前进度,按 L 读取进度,按 K 自动寻路";
		srand((unsigned)time(NULL));
		x = rand() % 20;
		if (x >= 19) {
			if (!battle()) {
				edg[a][b] = 000;
				a = 1, b = 1;
				edg[a][b] = 001;
			}
		}
			
		if (a == len - 2 && b == len - 1)
		{
			printf("\t\t\n您已走出该迷宫\n\n");
			system("pause");
			break;
		}
	}
}

int searchexit()							//判断迷宫是否有出口，有则返回1，否则返回0
{
	SNode s;
	char cpy[len+1][len+1];
	int a = 1, b = 1, path;
	for (int i = 0; i<len; i++)	            //将迷宫复制到cpy
	{
		for (int j = 0; j<len; j++)
		{
			cpy[i][j] = edg[i][j];
		}
	}
	while (a >= 0 && a <= len - 1 && b >= 0 && b <= len - 1) //开始进迷宫
	{

		if (a == len - 2 && b == len - 2)
		{
			return 1;
		}
		if (cpy[a][b + 1] == 000) //向右走一步
		{
			cpy[a][b] = '>';
			b++;
			cpy[a][b] = 001;
			push(RIGHT);
			continue;
		}
		if (cpy[a + 1][b] == 000) //向下走一步
		{
			cpy[a][b] = '>';
			a++;
			cpy[a][b] = 001;
			push(DOWN);
			continue;
		}
		if (cpy[a][b - 1] == 000) //向左走一步
		{
			cpy[a][b] = '>';
			b--;
			cpy[a][b] = 001;
			push(LEFT);
			continue;
		}
		if (cpy[a - 1][b] == 000) //向上走一步
		{
			cpy[a][b] = '>';
			a--;
			cpy[a][b] = 001;
			push(UP);
			continue;
		}
		path = pop();
		if (empty())
		{
			return 0;
		}
		switch (path)
		{
		case LEFT:
			cpy[a][b] = '>';
			b = b + 1;
			cpy[a][b] = 001;
			break;
		case UP:
			cpy[a][b] = '>';
			a = a + 1;
			cpy[a][b] = 001;
			break;
		case DOWN:
			cpy[a][b] = '>';
			a = a - 1;
			cpy[a][b] = 001;
			break;
		case RIGHT:
			cpy[a][b] = '>';
			b = b - 1;
			cpy[a][b] = 001;
			break;
		}
	}
}

void savemaze1()
{
	mazenode *p;
	p = (mazenode *)malloc(sizeof(mazenode));
	p->next = head->next;
	head->next = p;
	memcpy(p->edg, edg, sizeof(p->edg));
	length++;
	printf("\t\t\n存储完成\n\n");
	system("pause");
}

void savemaze2()
{
	FILE* fp;
	errno_t err;
	int i, j;
	if ((err = fopen_s(&fp, "d:\\maze.dat", "a+")) == NULL)
	{
		for (i = 0; i < len; i++)
		{
			if (fwrite(edg[i], len * sizeof(char), 1, fp) != 1);
		}

	}
	else
	{
		printf("can't open file. ");
		exit(0);
	}

	fclose(fp);
	fp = NULL;
}

void readmaze1()
{
	int k;
	printf("\t\t\n请输入所要读取地图的页数\n\n");
	scanf_s("%d", &k);
	if (k > length || k<1)
	{
		printf("\t\t\n您所输入的页数当前没有迷宫\n\n");
		system("pause");
	}
	else
	{
		mazenode *p = head;
		for (int i = 0; i < length - k + 1; i++)
		{
			p = p->next;
		}
		memcpy(edg, p->edg, sizeof(edg));

		showmaze();
	}
}

void readmaze2()
{
	FILE* fp;
	errno_t err;
	int i, j;
	int k;
	if ((err = fopen_s(&fp, "d:\\maze.dat", "rb")) == NULL)
	{
		int k;
		printf("\t\t\n请输入所要读取地图的页数\n\n");
		scanf_s("%d", &k);
		if (k<1)
		{
			printf("\t\t\n您所输入的页数当前没有迷宫\n\n");
		}
		else
		{
			system("cls");
			printf("\t");
			fseek(fp, (k - 1)*len*len * sizeof(char), SEEK_SET);
			for (int i = 0; i <len; i++)	//将迷宫打印显示
			{
				fread(edg[i], len * sizeof(char), 1, fp);
				printf("\n");
				for (int j = 0; j < len; j++)
				{
					if (edg[i][j] == 004) {
						printf("■");
					}
					if (edg[i][j] == 000)
					{
						printf("  ");
					}
					if (edg[i][j] == 001)
						printf("%c ", edg[i][j]);
				}
			}
			printf("\n");
		}

	}
	else
	{
		printf("file can not open!");
		system("pause");
		exit(0);
	}
	system("pause");
	fclose(fp);
	fp = NULL;
}

void createmaze100() {
	for (int i = 0; i < 20; i++)
	{
		Sleep(1000);
		create();
		savemaze2();
	}
}

int battle() {							//循环or决策二叉树战斗系统
	int x, y, h1 = 10,h2=10;
	system("cls");
	cout << "进入战斗画面" << endl;
	while (1)						//循环选择攻击
	{
		system("cls");
		printf("\n\n");
		printf("\t\t己方HP:%d\n\n", h2);
		printf("\t\t敌方HP:%d\n\n", h1);
		printf("\t\t1.攻击1(80%几率成功)\n\n");
		printf("\t\t2.攻击2(50%几率成功)\n\n");
		printf("\t\t3.攻击3(20%几率成功)\n\n");
		printf("\t\t4.攻击4(秒杀)\n\n");
		printf("\t\t请输入选择的序号(1-4): ");
		scanf_s("%d", &x);

		switch (x)
		{
		case 1: srand((unsigned)time(NULL));
				y = rand() % 100;
				if (y <=79 ) { cout << "\n攻击成功！" << endl; getchar(); getchar(); h1 -= 1; break; }
				else { cout << "\n攻击失败！" << endl; getchar(); getchar(); break; }
		case 2: srand((unsigned)time(NULL));
				y = rand() % 100;
				if (y <=49) { cout << "\n攻击成功！" << endl; getchar(); getchar(); h1 -= 2; break; }
				else { cout << "\n攻击失败！" << endl; getchar(); getchar(); break; }
				
		case 3:  srand((unsigned)time(NULL));
				y = rand() % 100;
				if (y <=19) { cout << "\n攻击成功！" << endl; getchar(); getchar(); h1 -= 3; break; }
				else { cout << "\n攻击失败！" << endl; getchar(); getchar(); break; };
		case 4:  srand((unsigned)time(NULL));
				y = rand() % 100;
				if (y <=9) { cout << "\n攻击成功！" << endl; getchar(); getchar(); h1 -= 10; break; }
				else { cout << "\n攻击失败！" << endl; getchar(); getchar(); break; }
		default: printf("\n\t\t请输入正确的序号!"); getchar(); getchar();
		}
		srand((unsigned)time(NULL));
		y = rand() % 100;
		if (y <= 59) {
			cout << "Miss,躲过敌人的攻击" << endl;
			getchar(); 
		}
		else {
			h2 -= 1;
			cout << "收到敌人攻击"<<endl;
			getchar(); 
		}
		if (h2 <= 0) { printf("你被击败!退回起点."); return 0; break; }
		if (h1 <= 0) { printf("敌人已被击败!继续前进."); return 1; break; }
	}
}
	/*
	CreateBiTree();
	cout << "输入你的选择1或2:" << endl;
	while (BT->data != 1 && BT->data != 0)
	{
		cin >> x;
		switch (x)
		{
		case 1:	BT = BT->lchild; break;
		case 2:	BT = BT->rchild; break;
		default:	break;
		}
		switch (BT->data)
		{
		case 1:		cout << "恭喜你，战斗胜利！继续前进。" << endl; return 1;
		case 0:		cout << "很遗憾，战斗失败！退回起点。" << endl; return 0;
		case 2:		cout << "你躲过了敌人的攻击" << endl; 	 break;
		default:	break;
		}
	}
	
}
/*
void CreateBiTree()
{
	BT = (BiTNode*)malloc(sizeof(BiTNode));								// 1
	BT->data = 2;
	BT->lchild = (BiTNode*)malloc(sizeof(BiTNode));
	BT->rchild = (BiTNode*)malloc(sizeof(BiTNode));
	BT->lchild->data = 2; BT->rchild->data = 0;
	BT->lchild->lchild = (BiTNode*)malloc(sizeof(BiTNode));
	BT->lchild->rchild = (BiTNode*)malloc(sizeof(BiTNode));
	BT->lchild->lchild->data = 0; BT->lchild->rchild->data = 1;
	BT->rchild->lchild = NULL; BT->rchild->rchild = NULL;
	BT->lchild->lchild->lchild = NULL; BT->lchild->lchild->rchild = NULL;
	BT->lchild->rchild->lchild = NULL; BT->lchild->rchild->rchild = NULL;
}
*/
void savepos(int a,int b)
{
	int x;
	system("cls");
	cout << "存储进度" << endl;
	while (1)
	{
		system("cls");
		printf("\n\n");
		printf("\t\t1.存档1\n\n");
		printf("\t\t2.存档2\n\n");
		printf("\t\t3.存档3\n\n");
		printf("\t\t4.返回\n\n");
		printf("\t\t请输入选择的序号(1-4): ");
		scanf_s("%d", &x);
		if (x == 4) break;
		switch (x)
		{
		case 1:
			if (items[0].first&&items[0].second)
			{
				cout << "是否覆盖当前进度" << endl;
				cout << "1.是" << endl;
				cout << "2.否" << endl;
				int y;
				cin >> y;
				switch (y)
				{
				case 1:
					items[0].first = a;
					items[0].second = b;
					cout << "存储完成" << endl;
					system("pause");
					break;
				case 2:
					break;
				}
			}
			if (!items[0].first && !items[0].second)
			{
				items[0].first = a;
				items[0].second = b;
				cout<<"存储完成"<<endl;
				system("pause");
			}
			break;
		case 2: 
			if (items[1].first&&items[1].second)
			{
				cout << "是否覆盖当前进度" << endl;
				cout << "1.是" << endl;
				cout << "2.否" << endl;
				int y;
				cin >> y;
				switch (y)
				{
				case 1:
					items[1].first = a;
					items[1].second = b;
					cout << "存储完成" << endl;
					system("pause");
					break;
				case 2:
					break;
				}
			}
			if (!items[1].first && !items[1].second)
			{
				items[1].first = a;
				items[1].second = b;
				cout << "存储完成" << endl;
				system("pause");
			}
			break;
		case 3:  
			if (items[2].first&&items[2].second)
			{
				cout << "是否覆盖当前进度" << endl;
				cout << "1.是" << endl;
				cout << "2.否" << endl;
				int y;
				cin >> y;
				switch (y)
				{
				case 1:
					items[2].first = a;
					items[2].second = b;
					cout << "存储完成" << endl;
					system("pause");
					break;
				case 2:
					break;
				}
			}
			if (!items[2].first && !items[2].second)
			{
				items[2].first = a;
				items[2].second = b;
				cout << "存储完成" << endl;
				system("pause");
			}
			break;
		default: printf("\n\t\t请输入正确的序号!"); getchar(); getchar();
		}
		
	}
}

void readpos(int *a, int *b)
{
	int x;
	system("cls");
	while (1)
	{
		system("cls");
		printf("\n\n");
		printf("\t\t1.存档1\n\n");
		printf("\t\t2.存档2\n\n");
		printf("\t\t3.存档3\n\n");
		printf("\t\t4.返回\n\n");
		printf("\t\t请输入选择的序号(1-4): ");
		scanf_s("%d", &x);
		if (x == 4) break;
		switch (x)
		{
		case 1:
			edg[*a][*b] = 000;
			edg[items[0].first][items[0].second] = 001;
			*a = items[0].first;
			*b = items[0].second;
			break;
		case 2: 
			edg[*a][*b] = 000;
			edg[items[1].first][items[1].second] = 001;
			*a = items[1].first;
			*b = items[1].second;
			break;
		case 3:  
			edg[*a][*b] = 000;
			edg[items[2].first][items[2].second] = 001;
			*a = items[2].first;
			*b = items[2].second;
			break;
		default: printf("\n\t\t请输入正确的序号!"); getchar(); getchar();
		}

	}
}

void op(){
	string opwords = "long long ago\n\nthere was a small country\n\npeople living there were happy and harmonious.\n\nhowever\n\none day \n\nhere came a lot of monsters,who robbed the daughter of the king.\n\nthus,countless warriors were sent to rescue the princess\n\nbut no one came back.\n\nnow you are one of the countless warriors.\n\nhere comes the story......";
	int i = 0;
	system("cls");
	cout << "\n\n\t\t按任意键继续\n" << endl;
	while (!_kbhit()&&opwords[i]!='\0') {
		cout <<opwords[i];
		i++;
		Sleep(150);
	}
	Sleep(500);
}  

void autosearch(int a,int b) {
	SNode s;
	int path;

	while (a >= 0 && a <= len - 1 && b >= 0 && b <= len - 1) //开始进迷宫
	{

		if (a == len - 2 && b == len - 2)
		{
			cout<<"自动寻路完成"<<endl;
			return;
		}
		if (edg[a][b + 1] == 000) //向右走一步
		{
			edg[a][b] = '>';
			b++;
			
			push(RIGHT);
			continue;
		}
		if (edg[a + 1][b] == 000) //向下走一步
		{
			edg[a][b] = '>';
			a++;
			
			push(DOWN);
			continue;
		}
		if (edg[a][b - 1] == 000) //向左走一步
		{
			edg[a][b] = '>';
			b--;
			
			push(LEFT);
			continue;
		}
		if (edg[a - 1][b] == 000) //向上走一步
		{
			edg[a][b] = '>';
			a--;
			
			push(UP);
			continue;
		}
		path = pop();
		if (empty())
		{
			cout << "自动寻路失败，没有找到出口" << endl;
			return;
		}
		switch (path)
		{
		case LEFT:
			edg[a][b] = '>';
			b = b + 1;
			
			break;
		case UP:
			edg[a][b] = '>';
			a = a + 1;
			
			break;
		case DOWN:
			edg[a][b] = '>';
			a = a - 1;
			
			break;
		case RIGHT:
			edg[a][b] = '>';
			b = b - 1;
			
			break;
		}
	}
}