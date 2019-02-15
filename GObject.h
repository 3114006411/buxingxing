#pragma once

#include"stdafx.h"
#include"GMap.h"
#include<time.h>

#define PLAYERSPEED 6
#define ENERMYSPEED 4
#define LEGCOUNTS 5
#define DISTANCE 10
#define BLUE_ALERT 8
#define D_OFFSET 2
#define RD (DISTANCE+D_OFFSET)

enum TWARDS
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	OVER,
};

class GObject 							// 物体类 : 大嘴和敌人的父类
{
public:
	GObject(int Row, int Array)
	{
		m_nFrame = 1; 					// 帧数
		pStage = NULL; 					// 当前关卡
		this->m_nRow = Row;				// 行
		this->m_nArray = Array; 			// 数组
		// 中心位置
		this->m_ptCenter.y = m_nRow * pStage->LD + pStage->LD / 2;
		this->m_ptCenter.x = m_nArray * pStage->LD + pStage->LD / 2;

		this->m_nX = m_ptCenter.x;
		this->m_nY = m_ptCenter.y;
	}

	void SetPosition(int Row, int Array);	// 设置位置
	void DrawBlank(HDC &hdc); 			// 画空白
	void virtual Draw(HDC &hdc) = 0;		// 绘制对象
	void virtual action() = 0;			// 数据变更的表现

	int GetRow();
	int GetArray();

	static GMap *pStage; //指向地图类的指针,设置为静态,使所有自类对象都能够使用相同的地图

protected:
	int m_nX;
	int m_nY;
	TWARDS m_cmd; 						// 指令缓存
	POINT m_ptCenter; 					// 中心坐标
	int m_nRow; 							// 逻辑横坐标
	int m_nArray; 						// 逻辑纵坐标
	int m_nSpeed; 						// 速度
	TWARDS m_dir; 						// 朝向
	int m_nFrame; 						// 祯数
	bool Achive();						// 判断物体是否到达逻辑坐标位置
	bool Collision();						// 逻辑碰撞检测,将物体摆放到合理的位置
	int PtTransform(int k); 				// 将实际坐标转换为逻辑坐标
	virtual void AchiveCtrl();			// 到达逻辑点后更新数据
};


class PacMan :public GObject
{
protected:
	virtual void AchiveCtrl();

public:
	POINT GetPos();
	bool IsOver();
	bool IsWin();
	void Draw(HDC &hdc);
	void SetTwCommand(TWARDS command);//设置玩家下一步指令
	PacMan(int x, int y) :GObject(x, y)
	{
		this->m_nSpeed = PLAYERSPEED;//设置玩家速度
		m_cmd = m_dir = LEFT;//设置朝向

	}
	void action();
	void SetOver();
	
};

class Enermy :public GObject
{



public:
	static std::shared_ptr<PacMan> player;
	void virtual Draw(HDC &hdc);
	Enermy(int x, int y) :GObject(x, y)
	{
		this->m_nSpeed = ENERMYSPEED;
		m_dir = LEFT;
		m_cmd = UP;
	}
	void virtual action();


protected:
	void Catch();
	void virtual MakeDecision(bool b)=0;
	COLORREF color = RGB(0, 0, 0);


};


class RedOne :public Enermy
{
protected:
	void virtual MakeDecision(bool b);

public:
	void Draw(HDC &hdc);
	RedOne(int x, int y) :Enermy(x, y)
	{
		color = RGB(255, 0, 0);
	}
};


class BlueOne :public RedOne
{
protected:
	void virtual MakeDecision(bool b);

public:
	void Draw(HDC &hdc);
	BlueOne(int x, int y) :RedOne(x, y)
	{
		color = RGB(0, 0, 255);
	}
};

class YellowOne :public RedOne
{
protected:
	void virtual MakeDecision(bool b);

public:
	void Draw(HDC &hdc);
	YellowOne(int x, int y) :RedOne(x, y)
	{
		color = RGB(0, 255, 0);
	}
};

