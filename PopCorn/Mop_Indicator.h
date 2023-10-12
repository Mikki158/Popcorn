#pragma once
#include "Config.h"

//
class AMop_Indicator : public AGraphics_Object
{
public:
    AMop_Indicator(int x_pos, int y_pos, int time_offset);

    virtual void Act();
    virtual void Clear(HDC hdc, RECT& paint_area);
    virtual void Draw(HDC hdc, RECT& paint_area);
    virtual bool Is_Finished();

    void Set_Y_Pos(int y_pos);

private:
    int X_Pos, Y_Pos;
    int Time_Offset;

    static const int Width = 17;
    static const int Height = 5;
    static const int MAX_FADE_STEP = AsConfig::FPS * 4 / 10; // 0.4 секунды
    static const int Normal_Timeout = AsConfig::FPS; // 1 секунда
    static AColor_Fade Fading_Blue_Colors;

    RECT Indicator_Rect;
    const AColor* Current_Color;
};
