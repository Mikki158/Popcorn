#pragma once

#include <Windows.h>

#include "Ball.h"
// Бинго 3 = 26000 1 стр
// Диван Лайт, категория ткани 2 = 17000 8 стр
// диван нео 16000 9 стр
class AGraphics_Object
{
public:
	virtual ~AGraphics_Object();

	virtual void Clear(HDC hdc, RECT& paint_area) = 0;
	virtual void Draw(HDC hdc, RECT& paint_area) = 0;
	virtual void Act() = 0;
	virtual bool Is_Finished() = 0;
};

class AActive_Brick: public AGraphics_Object
{
public:
	void Get_Level_Pos(int& dest_brick_x, int& dest_brick_y);


protected:
	int Level_X, Level_Y;

	AActive_Brick(EBrick_Type brick_type, int level_x, int level_y);

	virtual ~AActive_Brick();

	virtual void Clear(HDC hdc, RECT& paint_area);

	double Get_Brick_X_Pos(bool of_center);
	double Get_Brick_Y_Pos(bool of_center);

	EBrick_Type Brick_Type;
	RECT Brick_Rect;
};

class AActive_Brick_Pink_Blue: public AActive_Brick
{
public:
	AActive_Brick_Pink_Blue(EBrick_Type brick_type, int level_x, int level_y);
	~AActive_Brick_Pink_Blue();	

	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual void Act();
	virtual bool Is_Finished();

	static void Setup_Color();
	static void Draw_In_Level(HDC hdc, RECT brick_rect, EBrick_Type brick_type);

private:
	int fade_Step;

	static const int MAX_FADE_STEP = AsConfig::FPS;

	static AColor Fading_Blue_Brick_Colors[MAX_FADE_STEP];
	static AColor Fading_Pink_Brick_Colors[MAX_FADE_STEP];


	static const int MAX_ANIMATION_STEP = 5;
	

	static void Get_Fading_Color(const AColor& origin_color, int step, AColor & result_color);

	static unsigned char Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step);
};

class AActive_Brick_Unbreakable : public AActive_Brick
{
public:
	AActive_Brick_Unbreakable(int level_x, int level_y);
	~AActive_Brick_Unbreakable();	

	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual void Act();
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC hdc, RECT brick_rect);

private:
	int Animation_Step;

	HRGN Region;

	static const int MAX_ANIMATION_STEP = 12;
};

class AActive_Brick_Multihit : public AActive_Brick
{
public:
	AActive_Brick_Multihit(int level_x, int level_y);
	~AActive_Brick_Multihit();

	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual void Act();
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC hdc, RECT brick_rect, EBrick_Type brick_type);
	

private:
	int Rotation_Step;

	static const int Steps_Per_Turn = 16;
	static const int MAX_ROTATION_STEP = Steps_Per_Turn * 4;

	static void Draw_Stage(HDC hdc, RECT brick_rect, int x, int width);
};


enum ETeleport_State
{
	ETS_Starting,
	ETS_Finishing,
	ETS_Done
};


enum EDirection_Type
{
	EDT_Left,
	EDT_Up,
	EDT_Right,
	EDT_Down
};

class AActive_Brick_Teleport : public AActive_Brick
{
public:
	EDirection_Type Relese_Direction;

	AActive_Brick_Teleport(int level_x, int level_y, ABall* ball, AActive_Brick_Teleport* destination_teleport);
	~AActive_Brick_Teleport();

	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual void Act();
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC hdc, RECT brick_rect, int step = 0);


private:
	int Animation_Step;

	ETeleport_State Teleport_State;

	AActive_Brick_Teleport* Destination_Teleport;
	ABall* Ball;

	static const int Max_Animation_Step = 12;

	void Set_Ball(ABall* ball);
};


class AAdvertisement : public AGraphics_Object
{
public:
	AAdvertisement(int level_x, int level_y, int width, int height);
	~AAdvertisement();

	virtual void Clear(HDC hdc, RECT& paint_area);
	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual void Act();
	virtual bool Is_Finished();

	void Show_Under_Brick(int Level_X, int Level_Y);
	bool Has_Brick_At(int level_x, int level_y);

private:
	int Level_X, Level_Y;
	int Width, Height;
	int Ball_X, Ball_Y;
	int Ball_Width, Ball_Height;
	int Ball_Y_Offset;
	double Falling_Speed, Acceleration_Step;
	double Deformation_Ratio;
	HRGN *Brick_Regions;

	HRGN Empt_Region;
	RECT Ad_Rect;

	static const int Ball_Size = 12;
	static const int High_Ball_Threshold = 12 * AsConfig::GLOBAL_SCALE;
	static const int Low_Ball_Threshold = -3 * AsConfig::GLOBAL_SCALE;
	static const int Deformation_Height = 2 * AsConfig::GLOBAL_SCALE;
};


class AActive_Brick_Ad : public AActive_Brick
{
public:
	AActive_Brick_Ad(int level_x, int level_y, AAdvertisement* advertisement);
	~AActive_Brick_Ad();

	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual void Act();
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC hdc, RECT brick_rect);


private:
	static const int Circle_Size = 7;

	AAdvertisement* Advertisement;
};


