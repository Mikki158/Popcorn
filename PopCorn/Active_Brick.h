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

private:
	int fade_Step;
	int level_x, level_y;


	static HPEN Fading_Blue_Brick_Pens[AsConfig::MAX_FADE_STEP];
	static HBRUSH Fading_Blue_Brick_Brushes[AsConfig::MAX_FADE_STEP];

	static HPEN Fading_Pink_Brick_Pens[AsConfig::MAX_FADE_STEP];
	static HBRUSH Fading_Pink_Brick_Brushes[AsConfig::MAX_FADE_STEP];


	static const int MAX_UNBREAKABLE_ANIMATION_STEP = 5;
	

	static void Get_Fading_Color(const AColor& color, int step, HPEN& pen, HBRUSH& brush);

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

private:
	int level_x, level_y;
	int Unbreakable_Animation_Step;

	EBrick_Type Brick_Type;
	RECT Brick_Rect;

	static const int MAX_UNBREAKABLE_ANIMATION_STEP = 5;
};

