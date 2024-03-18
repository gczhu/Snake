/*
* 作者：祝广程
* 日期：2021/1/22
* 该程序需导入easyX图形库
*/
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>				//sprintf()
#include<stdlib.h>				//srand()
#include<graphics.h>			//画图文件
#include<conio.h>				//_kbhit() _getch()
#include<time.h>				//time()
#include<Windows.h>				//MessageBox()窗口
#include<mmsystem.h>			//多媒体API声音输出
#pragma comment(lib,"winmm.lib")//引入mciSendString
//坐标属性
typedef struct pointXY {
	int x;
	int y;
}MYPOINT;
//蛇的属性
struct Snake {
	int num;			//蛇的节数
	MYPOINT xy[100];	//蛇最多100节
	char position;		//蛇的方向 
}snake;
//食物的属性
struct Food {
	MYPOINT fdxy;		//食物的坐标
	int eatgrade;		//吃了食物后的得分
	int flag;			//食物是否存在 
}food;
//初始化界面的按钮属性
struct button {
	int x, y;			//按钮的坐标
	int height, width;	//按钮的高和宽
	COLORREF color;		//按钮的颜色
	char* pText;		//指向文本的字符串
};
HWND hwnd = NULL;		//用句柄表示主窗口
//把方向枚举出来,对应小键盘的键码值,增强程序的可读性
enum movePosition { right = 77, left = 75, down = 80, up = 72 };
IMAGE foodimg;	//创建嵌入的图像
/************************************
*		初始化蛇：initSnake
*		画蛇：drawSnake
*		移动蛇：moveSnake
*		按键处理：keyDown
*		初始化食物：initFood
*		画食物：drawFood
*		吃食物：eatFood
*		显示分数：showGrade
*		游戏结束：snakeDie
************************************/
void initSnake();
void drawSnake();
void moveSnake();
void keyDown();
void initFood();
void drawFood();
void eatFood();
void showGrade();
int snakeDie1();
int snakeDie2();
void MODEL1();
void MODEL2();
//以下函数用于初始化菜单界面
struct button* creatButton(int x, int y, int width, int height, COLORREF color, const char* pText);
void drawButton(struct button* pB);
int mouseInButton(struct button* pB, MOUSEMSG m);
int clickButton(struct button* pB, MOUSEMSG m);
int main(void)
{
	hwnd = initgraph(640, 480);		//初始化一个窗口
	setbkcolor(BLACK);				//设置窗口颜色为黑色
	cleardevice();
	char topic[] = "贪吃蛇";
	//创建菜单各按钮的结构指针
	struct button* model1 = creatButton(220, 200, 200, 50, YELLOW, "普通模式");
	struct button* model2 = creatButton(220, 260, 200, 50, YELLOW, "穿墙模式");
	struct button* Exit = creatButton(220, 320, 200, 50, YELLOW, "退出游戏");
	while (1) {
		BeginBatchDraw();	//防止闪屏
		cleardevice();
		settextstyle(80, 0, "楷体");
		settextcolor(COLORREF RGB(134, 235, 246));
		outtextxy(190, 100, topic);
		//画出按钮
		drawButton(Exit);
		drawButton(model1);
		drawButton(model2);
		MOUSEMSG m = GetMouseMsg();
		if (clickButton(model1, m)) {
			MODEL1();
			break;
		}
		else if (clickButton(model2, m)) {
			MODEL2();
			break;
		}
		else if (clickButton(Exit, m)) {
			printf("游戏已退出");
			return 0;
		}
		EndBatchDraw();
	}
	closegraph();			//关闭窗口
	return 0;
}
/*   主框架：设置两种模式
主要区别是snakeDie的判定方式不同*/
void MODEL1() {
	mciSendString("open music.mp3 alias BGM", 0, 0, 0);
	mciSendString("play BGM repeat", 0, 0, 0);		//添加BGM
	srand((unsigned int)time(0));	//设定随机数的产生和系统时钟相关联
	setbkcolor(BLACK);		//设置背景为黑色
	cleardevice();			//清屏
	initSnake();			//初始化蛇
	initFood();				//初始化食物
	food.eatgrade = 0;		//初始化分数
	while (1) {
		BeginBatchDraw();	//防止闪屏
		cleardevice();		//清屏
		drawSnake();		//画蛇
		if (snakeDie1()) {
			break;
		}
		moveSnake();		//移动蛇
		/*有按键按下的时候接收按键
		  _kbhit()函数用来判断*/
		if (_kbhit()) {
			keyDown();
		}
		drawFood();			//画食物
		eatFood();			//吃食物
		if (food.flag == 0) {
			initFood();		//如果食物被吃掉，继续初始化食物
		}
		showGrade();		//显示分数
		Sleep(100);			//设置两次循环间的时间间隔
		EndBatchDraw();
	}
}
void MODEL2() {
	mciSendString("open music.mp3 alias BGM", 0, 0, 0);
	mciSendString("play BGM repeat", 0, 0, 0);		//添加BGM
	srand((unsigned int)time(0));	//设定随机数的产生和系统时钟相关联
	setbkcolor(BLACK);		//设置背景为黑色
	cleardevice();			//清除菜单界面
	initSnake();			//初始化蛇
	initFood();				//初始化食物
	food.eatgrade = 0;		//初始化分数
	while (1) {
		BeginBatchDraw();	//防止闪屏
		cleardevice();		//清屏
		drawSnake();		//画蛇
		if (snakeDie2()) {
			break;
		}
		moveSnake();		//移动蛇
		/*有按键按下的时候接收按键
		  _kbhit()函数用来判断*/
		if (_kbhit()) {
			keyDown();
		}
		drawFood();			//画食物
		eatFood();			//吃食物
		if (food.flag == 0) {
			initFood();		//如果食物被吃掉，继续初始化食物
		}
		showGrade();		//显示分数
		Sleep(100);			//设置两次循环间的时间间隔
		EndBatchDraw();
	}
}
//第一步：初始化蛇
void initSnake() {
	//结合蛇的属性
	//蛇的节数
	snake.num = 3;
	//蛇每一节的坐标（结合数组）
	snake.xy[2].x = 0;
	snake.xy[2].y = 0;
	snake.xy[1].x = 10;
	snake.xy[1].y = 0;
	snake.xy[0].x = 20;
	snake.xy[0].y = 0;
	//蛇的初始方向（向右）
	snake.position = right;
}
//第二步：画蛇
void drawSnake() {
	int i;
	for (i = 0; i < snake.num; i++) {
		setlinecolor(BLACK);		//矩形边框线的颜色为黑色
		setfillcolor(WHITE);
		//画矩形（需要左上角坐标和右下角坐标）
		fillrectangle(snake.xy[i].x, snake.xy[i].y, snake.xy[i].x + 10, snake.xy[i].y + 10);
	}
}
//第三步：移动蛇
void moveSnake() {
	//除了第一节，后面的每一节都是前面的坐标
	int i;
	for (i = snake.num - 1; i > 0; i--) {
		snake.xy[i].x = snake.xy[i - 1].x;
		snake.xy[i].y = snake.xy[i - 1].y;
	}
	//第一节的处理
	switch (snake.position) {
		case right:
			snake.xy[0].x = snake.xy[0].x + 10;
			break;
		case left:
			snake.xy[0].x = snake.xy[0].x - 10;
			break;
		case up:
			snake.xy[0].y = snake.xy[0].y - 10;
			break;
		case down:
			snake.xy[0].y = snake.xy[0].y + 10;
			break;
	}
}
//第四步：按键处理
void keyDown() {
	char userKey = 0;
	userKey = _getch();		//接收用户按键(不用按回车就返回）包含在conio.h头文件中
	switch (userKey) {
		case right:
			if (snake.position != left)
				snake.position = right;
			break;
		case left:
			if (snake.position != right)
				snake.position = left;
			break;
		case up:
			if (snake.position != down)
				snake.position = up;
			break;
		case down:
			if (snake.position != up)
				snake.position = down;
			break;
		default:
			break;
	}
}
//第五步：初始化食物
void initFood() {
	/*rand() % 64随机产生0~63
	  再乘以10确保随机数是10的倍数
	  防止蛇吃不到食物*/
	food.fdxy.x = rand() % 64 * 10;
	food.fdxy.y = rand() % 48 * 10;
	food.flag = 1;
	//如果食物出现在蛇身上，重新生成食物坐标
	int i;
	//食物不能生成在蛇身上
	for (i = 0; i < snake.num; i++) {
		while (food.fdxy.x == snake.xy[i].x && food.fdxy.y == snake.xy[i].y) {
			food.fdxy.x = rand() % 64 * 10;
			food.fdxy.x = rand() % 48 * 10;
		}
	}
	//食物不能生成在分数区域
	while(food.fdxy.x >= 570 && food.fdxy.y == 10) {
		food.fdxy.x = rand() % 64 * 10;
		food.fdxy.x = rand() % 48 * 10;
	}
}
//第六步：画食物
void drawFood() {
	loadimage(&foodimg, "food.png", 10, 10);
	setfillcolor(YELLOW);
	putimage(food.fdxy.x, food.fdxy.y, &foodimg);
	//fillrectangle(food.fdxy.x, food.fdxy.y, food.fdxy.x + 10, food.fdxy.y + 10);
}
//第七步：吃食物
void eatFood() {
	/*吃食物带来的变化
	  1.蛇变长了
	  2.食物要重新生成
	  3.分数要增加*/
	if (snake.xy[0].x == food.fdxy.x && snake.xy[0].y == food.fdxy.y) {
		snake.num++;
		food.eatgrade += 10;
		food.flag = 0;
	}
}
//第八步：显示分数
void showGrade() {
	char grade[6] = "";
	sprintf(grade, "%d", food.eatgrade);	
	/*调用sprintf()用于将格式化的数据写入字符串，包含在<stdio.h>中
	  不安全会报错，需要在预处理器定义加上_CRT_SECURE_NO_WARNINGS
	  或添加宏定义#define _CRT_SECURE_NO_WARNINGS*/
	setbkmode(TRANSPARENT);		//设置文字输出的背景为透明的
	settextcolor(WHITE);		//设置文字颜色为白色
	settextstyle(15,0,"宋体");
	outtextxy(570, 10, "分数：");
	outtextxy(570 + 40, 10, grade);
	/*outtextxy(int x, int y, string)用于在指定位置输出字符串
	  需要修改编码防止报错*/
}
//第九步：游戏结束
int snakeDie1() {
	int i;
	if (snake.xy[0].x >= 640 || snake.xy[0].x < 0 || snake.xy[0].y >= 480 || snake.xy[0].y < 0) {
		MessageBoxA(hwnd, TEXT("Game Over"), TEXT("你撞墙了"), MB_OK);
		//调用MessageBoxA()函数弹出窗口，包含在<windows.h>头文件中
		return 1;		//返回1表示游戏结束
	}
	for (i = 1; i < snake.num; i++) {
		if (snake.xy[0].x == snake.xy[i].x && snake.xy[0].y == snake.xy[i].y) {
			MessageBoxA(hwnd, TEXT("Game Over"), TEXT("你咬到自己了"), MB_OK);
			return 1;
		}
	}
	return 0;
}
int snakeDie2() {
	int i;
	//如果碰到墙，修改头坐标位置及蛇头方向
	if (snake.xy[0].x == 640) {
		snake.xy[0].x = 0;
		snake.position = right;
	}
	else if (snake.xy[0].x < 0) {
		snake.xy[0].x = 630;
		snake.position = left;
	}
	else if(snake.xy[0].y == 480) {
		snake.xy[0].y = 0;
		snake.position = down;
	}
	else if(snake.xy[0].y < 0) {
		snake.xy[0].y = 470;
		snake.position = up;
	}
	for (i = 1; i < snake.num; i++) {
		if (snake.xy[0].x == snake.xy[i].x && snake.xy[0].y == snake.xy[i].y) {
			MessageBoxA(hwnd, TEXT("Game Over"), TEXT("你咬到自己了"), MB_OK);
			return 1;
		}
	}
	return 0;
}
//第十步：创建菜单
//1.初始化按钮的属性
struct button* creatButton(int x, int y, int width, int height, COLORREF color, const char* pText) {
	struct button* pB = (struct button*)malloc(sizeof(struct button));
	if (pB != NULL) {
		pB->x = x;
		pB->y = y;
		pB->width = width;
		pB->height = height;
		pB->color = color;
		pB->pText = (char*)malloc(strlen(pText) + 1);
		if (pB->pText != NULL) {
			strcpy(pB->pText, pText);
		}
	}
	return pB;
}
//2.画出按钮
void drawButton(struct button* pB) {
	setfillcolor(pB->color);
	settextstyle(35, 0, "楷体");
	setlinecolor(BLACK);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	fillrectangle(pB->x, pB->y, pB->x + pB->width, pB->y + pB->height);
	outtextxy(pB->x + 25, pB->y + 10, pB->pText);
}
//3.判定鼠标的位置是否在按钮上
int mouseInButton(struct button* pB, MOUSEMSG m) {
	if (m.x >= pB->x && m.x <= pB->x + pB->width && m.y >= pB->y && m.y <= pB->y + pB->height) {
		pB->color = RED;
		return 1;
	}
	pB->color = YELLOW;
	return 0;
}
//4.判定是否点击了按钮
int clickButton(struct button* pB, MOUSEMSG m) {
	if (mouseInButton(pB, m) && m.uMsg == WM_LBUTTONDOWN) {
		return 1;
	}
	return 0;
}