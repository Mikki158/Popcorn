#pragma once

#include <Windows.h>

#include "Config.h"

class AActive_Brick
{
public:
	AActive_Brick(EBrick_Type brick_type);

	void Draw(HDC hdc, RECT& paint_area);
	void Act();

	static void setup_Colors();

private:
	static HPEN Fading_Blue_Brick_Pens[AsConfig::MAX_FADE_STEP];
	static HBRUSH Fading_Blue_Brick_Brushes[AsConfig::MAX_FADE_STEP];

	static HPEN Fading_Pink_Brick_Pens[AsConfig::MAX_FADE_STEP];
	static HBRUSH Fading_Pink_Brick_Brushes[AsConfig::MAX_FADE_STEP];



	RECT Brick_Rect;
	EBrick_Type Brick_Type;




	static unsigned char Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step);
	static void Get_Fading_Color(const AColor& color, int step, HPEN& pen, HBRUSH& brush);




private:
	int Fade_Step;
};
