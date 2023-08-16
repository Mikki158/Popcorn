#pragma once

#include <Windows.h>

#include "Config.h"

class AGraphics_Object
{
public:
	virtual void Draw(HDC hdc, RECT& paint_area) = 0;
	virtual void Act() = 0;
	virtual bool Is_Finished() = 0;
};

class AActive_Brick: public AGraphics_Object
{
public:
	AActive_Brick(EBrick_Type brick_type, int level_x, int level_y);

	virtual void Draw(HDC hdc, RECT& paint_area);
	virtual void Act();
	virtual bool Is_Finished();

	static void Setup_Color();

	EBrick_Type Brick_Type;
	int level_x, level_y;

private:
	static HPEN Fading_Blue_Brick_Pens[AsConfig::MAX_FADE_STEP];
	static HBRUSH Fading_Blue_Brick_Brushes[AsConfig::MAX_FADE_STEP];

	static HPEN Fading_Pink_Brick_Pens[AsConfig::MAX_FADE_STEP];
	static HBRUSH Fading_Pink_Brick_Brushes[AsConfig::MAX_FADE_STEP];



	RECT Brick_Rect;
	




	static unsigned char Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step);
	static void Get_Fading_Color(const AColor& color, int step, HPEN& pen, HBRUSH& brush);




private:
	int Fade_Step;
};
