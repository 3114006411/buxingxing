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

class GObject 							// ������ : ����͵��˵ĸ���
{
public:
	GObject(int Row, int Array)
	{
		m_nFrame = 1; 					// ֡��
		pStage = NULL; 					// ��ǰ�ؿ�
		this->m_nRow = Row;				// ��
		this->m_nArray = Array; 			// ����
		// ����λ��
		this->m_ptCenter.y = m_nRow * pStage->LD + pStage->LD / 2;
		this->m_ptCenter.x = m_nArray * pStage->LD + pStage->LD / 2;

		this->m_nX = m_ptCenter.x;
		this->m_nY = m_ptCenter.y;
	}

	void SetPosition(int Row, int Array);	// ����λ��
	void DrawBlank(HDC &hdc); 			// ���հ�
	void virtual Draw(HDC &hdc) = 0;		// ���ƶ���
	void virtual action() = 0;			// ���ݱ���ı���

	int GetRow();
	int GetArray();

	static GMap *pStage; //ָ���ͼ���ָ��,����Ϊ��̬,ʹ������������ܹ�ʹ����ͬ�ĵ�ͼ

protected:
	int m_nX;
	int m_nY;
	TWARDS m_cmd; 						// ָ���
	POINT m_ptCenter; 					// ��������
	int m_nRow; 							// �߼�������
	int m_nArray; 						// �߼�������
	int m_nSpeed; 						// �ٶ�
	TWARDS m_dir; 						// ����
	int m_nFrame; 						// ����
	bool Achive();						// �ж������Ƿ񵽴��߼�����λ��
	bool Collision();						// �߼���ײ���,������ڷŵ������λ��
	int PtTransform(int k); 				// ��ʵ������ת��Ϊ�߼�����
	virtual void AchiveCtrl();			// �����߼�����������
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
	void SetTwCommand(TWARDS command);//���������һ��ָ��
	PacMan(int x, int y) :GObject(x, y)
	{
		this->m_nSpeed = PLAYERSPEED;//��������ٶ�
		m_cmd = m_dir = LEFT;//���ó���

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

