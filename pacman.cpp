﻿// pacman.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "pacman.h"
#include"GMap.h"
#include"GObject.h"

using namespace std;
#define WLENTH 700
#define WHIGHT 740
#define STAGE_COUNT 3
static HWND g_hwnd;

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


void Draw0()//玩家绘图
{
	int x = 300, y = 300;
	int r = 100;
	HDC hdc = ::GetDC(g_hwnd);

	Ellipse(hdc, x - r, y - r, x + r, y + r);
	MoveToEx(hdc, x, y, NULL);
	LineTo(hdc, x - r, y);

	ReleaseDC(g_hwnd, hdc);
}

#define PI 3.1415926f

void Draw1()//玩家绘图
{
	int x = 300, y = 300;
	int r = 100;
	HDC hdc = ::GetDC(g_hwnd);

	int x0, y0, x1, y1;
	x0 = x - r*sin(PI * 0.75f);
	y0 = y - r*sin(PI * 0.75f);

	x1 = x + r*sin(PI * 1.25f);
	y1 = y - r*sin(PI * 1.25f);

	

	Arc(hdc, x - r, y - r, x + r, y + r, x1, y1, x0, y0);
	MoveToEx(hdc, x, y, NULL);
	LineTo(hdc, x0, y0);
	MoveToEx(hdc, x, y, NULL);
	LineTo(hdc, x1, y1);

	/*Ellipse(hdc, x - r, y - r, x + r, y + r);
	MoveToEx(hdc, x, y, NULL);
	LineTo(hdc, x - r, y);*/

	ReleaseDC(g_hwnd, hdc);
}

void Draw2()//玩家绘图
{
	int x = 300, y = 300;
	int r = 100;
	HDC hdc = ::GetDC(g_hwnd);
	int x0, y0, x1, y1;
	x0 = x, y0 = y - r;
	x1 = x, y1 = y + r;

	Arc(hdc, x - r, y - r, x + r, y + r, x1, y1, x0, y0);

	MoveToEx(hdc, x0, y0, NULL);
	LineTo(hdc, x1, y1);

	ReleaseDC(g_hwnd, hdc);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// 参数不再使用了
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PACMAN, szWindowClass, MAX_LOADSTRING);
	// 注册窗口类
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PACMAN));


	// 当前的关卡
	int s_n = 0; // [0, 1, 2]
	// 地图
	GMap *MapArray[STAGE_COUNT] = { new Stage_1(), new Stage_2(), new Stage_3() };

	// 玩家对象
	// 自己
	auto g_me = std::make_shared<PacMan>(P_ROW, P_ARRAY);
	// 设定四个敌人对象
	auto e1 = std::make_shared<RedOne>(E_ROW, E_ARRAY);			// 红色敌军对象
	auto e2 = std::make_shared<RedOne>(E_ROW, E_ARRAY);			// 红色敌军对象
	auto e3 = std::make_shared<BlueOne>(E_ROW, E_ARRAY);			// 蓝色敌军对象
	auto e4 = std::make_shared<YellowOne>(E_ROW, E_ARRAY);			// 黄色敌军对象


	// 关卡
	GObject::pStage = MapArray[s_n]; 								// 初始化为第一关地图

	// 设定玩家
	Enermy::player = g_me;										// 用一个指针指向玩家对象

	MSG msg;

	DWORD dwLastTime = 0;


	// 主消息循环:
// 玩家没有被抓,并且关卡<3
	while (!g_me->IsOver() && s_n < STAGE_COUNT) {
		// 判断是否赢得比赛
		if (g_me->IsWin()) {
			s_n++;												// 移动到下一关
			// 重设自己和敌人位置
			g_me->SetPosition(P_ROW, P_ARRAY);
			e1->SetPosition(E_ROW, E_ARRAY);					// 设置敌军一的位置
			e2->SetPosition(E_ROW, E_ARRAY); 					// 设置敌军二的位置
			e3->SetPosition(E_ROW, E_ARRAY); 					// 设置敌军三的位置
			e4->SetPosition(E_ROW, E_ARRAY); 					// 设置敌军四的位置
			// 判断是否完成了3关,如果完成,退出游戏,否则进入下一关
			if (s_n < 3) {
				MessageBox(g_hwnd, _T("恭喜过关"), _T("吃豆子提示"), MB_OK);
				GObject::pStage = MapArray[s_n];				// 
				RECT screenRect;
				screenRect.top = 0;
				screenRect.left = 0;
				screenRect.right = WLENTH;
				screenRect.bottom = WHIGHT;

				HDC hdc = GetDC(g_hwnd);						// 获取设备
				std::shared_ptr<HDC__> dc(hdc, [](HDC hdc) {	// 智能指针，自动管理资源
					::ReleaseDC(g_hwnd, hdc);
				});
				::FillRect(dc.get(), &screenRect, CreateSolidBrush(RGB(255, 255, 255)));
				GObject::pStage->DrawMap(hdc);					// 画地图
				continue; 									// 继续进行循环
			}
			else {
				// 跳出循环
				break;
			}
		}



		// 获取消息
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);							// 翻译消息
			DispatchMessage(&msg);							// 分发消息
		}



		// 判断时间,否则画得太快
		if (GetTickCount() - dwLastTime >= 40) {
			dwLastTime = GetTickCount();						// 记住本次的时间
		}
		else {
			continue; 										// 时间不到,本次不进行绘画
		}


		{
			HDC hdc = ::GetDC(g_hwnd); 					// 获得设备
			std::shared_ptr<HDC__> dc(hdc, [](auto hdc) {	// 不使用时自动释放
				::ReleaseDC(g_hwnd, hdc);					// 释放设备
			});
			MapArray[s_n]->DrawPeas(hdc);					// 画豆子
			MapArray[s_n]->DrawMap(hdc);					// 画地图

			// 画敌人及自动运动
			{
				e1->action();								// 敌军一的行为函数
				e1->DrawBlank(hdc);						// 画敌军一的空白
				e1->Draw(hdc);							// 画敌军一的主体部分

				e2->action();								// 敌军一的行为函数
				e2->DrawBlank(hdc); 						// 画敌军一的空白
				e2->Draw(hdc); 							// 画敌军一的主体部分

				e3->action();								// 敌军一的行为函数
				e3->DrawBlank(hdc); 						// 画敌军一的空白
				e3->Draw(hdc); 							// 画敌军一的主体部分

				e4->action();								// 敌军一的行为函数
				e4->DrawBlank(hdc); 						// 画敌军一的空白
				e4->Draw(hdc); 							// 画敌军一的主体部分
			}

			{
				// 画自己
				g_me->DrawBlank(hdc);
				g_me->Draw(hdc);
				// 自己向前移动
				g_me->action();

				// 获取按键 : 控制自己的方向
				if (GetAsyncKeyState(VK_DOWN) & 0x8000) {	// 检测到下方向键按下
					g_me->SetTwCommand(DOWN);				// 设置下一步的移动方向为向下
				}
				if (GetAsyncKeyState(VK_LEFT) & 0x8000) {	// 检测到左方向键按下
					g_me->SetTwCommand(LEFT); 				// 设置下一步的移动方向为向左
				}
				if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {	// 检测到右方向键按下
					g_me->SetTwCommand(RIGHT); 			// 设置下一步的移动方向为向右
				}
				if (GetAsyncKeyState(VK_UP) & 0x8000) {		// 检测到上方向键按下
					g_me->SetTwCommand(UP); 				// 设置下一步的移动方向为向上
				}
			}
		}
	}


	// 如果游戏结束
	if (g_me->IsOver()) {
		MessageBoxA(NULL, "出师未捷", "吃豆子提示", MB_OK);
	}
	// 否则,提示赢得游戏
	else {
		MessageBoxA(NULL, "恭喜您赢得了胜利\r\n确定后游戏退出", "吃豆子提示", MB_OK);
	}

	return (int)msg.wParam;

}



/*{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PACMAN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PACMAN));
	
	int s_n = 0;
	GMap *MapArray[STAGE_COUNT] = { new Stage_1(),new Stage_2(),new Stage_3() };

	MSG msg;

	bool bRunning = true;
	while (bRunning && s_n < STAGE_COUNT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		HDC hdc = ::GetDC(g_hwnd);
		{
			MapArray[s_n]->DrawMap(hdc);
			MapArray[s_n]->DrawPeas(hdc);
		}
		::ReleaseDC(g_hwnd, hdc);
	}
	return (int)msg.wParam;
}*/

/*
    MSG msg;

    // 主消息循环:
	bool bRunning = true;
	while (bRunning)
	{
		//获取消息
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		////画点测试
		//{
		//	HDC hdc = ::GetDC(g_hwnd);
		//	SetPixel(hdc, rand() % WLENTH, rand() % WHIGHT,
		//		RGB(rand() % 256, rand() % 256, rand() % 256));
		//	::ReleaseDC(g_hwnd, hdc);
		//}
		////画线测试
		//{
		//	HDC hdc = ::GetDC(g_hwnd);
		//创建画笔
		//	HPEN Pen = CreatePen(PS_SOLID, 2, RGB(rand() % 256, rand() % 256, rand() % 256));
		//	//选择画笔
		//	HPEN oldPen = (HPEN)SelectObject(hdc, Pen);
		//	MoveToEx(hdc, rand() % WLENTH, rand() % WHIGHT, NULL);
		//	LineTo(hdc, rand() % WLENTH, rand() % WHIGHT);
		//	//恢复画笔
		//	SelectObject(hdc, oldPen);
		//	::ReleaseDC(g_hwnd, hdc);
		//	//暂停1毫秒，不然会看不清
		//	Sleep(1);
		//}

		////画矩形测试
		//{
		//	HDC hdc = ::GetDC(g_hwnd);
		//	{
		//		HPEN Pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		//		HPEN oldPen = (HPEN)SelectObject(hdc, Pen);
		//		//画矩形（空心）
		//		Rectangle(hdc, 100, 200, 300, 500);
		//		//恢复画笔
		//		SelectObject(hdc, oldPen);
		//		DeleteObject(Pen);
		//	}

		//	{
		//		HBRUSH bBrush = CreateSolidBrush(RGB(50, 0, 0));
		//		RECT rect;

		//		rect.left = 50;
		//		rect.top = 270;
		//		rect.right = 150;
		//		rect.bottom = 370;
		//		//画矩形（实心）
		//		FillRect(hdc, &rect, bBrush);

		//		//恢复画笔
		//		DeleteObject(bBrush);
		//	}

		//	ReleaseDC(g_hwnd, hdc);
		//	Sleep(1);
		//}

		////画圆测试
		//{
		//	HDC hdc = ::GetDC(g_hwnd);
		//	Ellipse(hdc, 200, 150, 300, 250);
		//	Ellipse(hdc, 200, 270, 340, 370);
		//	Ellipse(hdc, 100, 100, 200, 150);
		//	::ReleaseDC(g_hwnd, hdc);
		//}

		////画弧线
		//{
		//	HDC hdc = ::GetDC(g_hwnd);
		//	Arc(hdc, 100, 100, 200, 300
		//		, 150, 200
		//		, 100, 200);
		//	ReleaseDC(g_hwnd, hdc);
		//}

		//画玩家
		{

			static DWORD dwTime = GetTickCount();
			if (GetTickCount() - dwTime >= 100)
			{
				dwTime = GetTickCount();
			}
			else {
				continue;
			}
			HDC hdc =GetDC(g_hwnd);
			RECT rc;
			GetClientRect(g_hwnd, &rc);
			HBRUSH hbr = CreateSolidBrush(RGB(255, 255, 255));
			FillRect(hdc, &rc, hbr);

			DeleteObject(hbr);
			ReleaseDC(g_hwnd, hdc);
			//闭嘴
			//半张
			//全张
			//半张
			//闭嘴

			static int iFrame = 0;
			++iFrame;
			if (iFrame >= 5)
			{
				iFrame = 0;
			}
			switch (iFrame)
			{
			case 0:Draw0(); break;
			case 1:Draw1(); break;
			case 2:Draw2(); break;
			case 3:Draw1(); break;
			case 4:Draw0(); break;
			default:
				break;
			}
		}
		
	}

    return (int) msg.wParam;
}

*/

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PACMAN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PACMAN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0,
	  0,
	  WLENTH,
	  WHIGHT, 
	  nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   g_hwnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


