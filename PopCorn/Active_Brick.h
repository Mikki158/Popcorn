#pragma once

#include <Windows.h>

#include "Config.h"

class AGraphics_Object
{
public:
	virtual ~AGraphics_Object();

	virtual void Draw(HDC hdc, RECT& paint_area) = 0;
	virtual void Act() = 0;
	virtual bool Is_Finished() = 0;
};

class AActive_Brick: public AGraphics_Object
{
protected:
	AActive_Brick(EBrick_Type brick_type, int level_x, int level_y);

	virtual ~AActive_Brick();

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

	static AColor Blue_Highlight, Pink_Highlight;
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

	//static AColor Blue_Highlight, Pink_Highlight;
	//HRGN Region;

	static const int Steps_Per_Turn = 16;
	static const int MAX_ROTATION_STEP = Steps_Per_Turn * 4;

	static void Draw_Stage(HDC hdc, RECT brick_rect, int x, int width);
};

