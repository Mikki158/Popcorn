#pragma once

#include "Common.h"

//
class AColor
{
public:
    AColor();
    AColor(unsigned char r, unsigned char g, unsigned char b);
    AColor(const AColor& color, int pen_size);
    AColor(const AColor& pen_color, const AColor& brush_color, int pen_size);
    AColor(unsigned char r, unsigned char g, unsigned char b, int pen_size);
    ~AColor();

    void operator = (const AColor& another);

    void Select(HDC hdc) const;
    void Select_Pen(HDC hdc) const;
    void Set_As(unsigned char r, unsigned char g, unsigned char b);

    int Get_RGB() const;
    HBRUSH Get_Brush() const;

    unsigned char R, G, B;


private:
    HPEN Pen;
    HBRUSH Brush;
};

//
class AColor_Fade
{
public:
    AColor_Fade(const AColor& origin_color, int max_fade_step);
    AColor_Fade(const AColor& origin_color, const AColor& base_color, int max_fade_step);
    ~AColor_Fade();

    AColor* Get_Color(int fade_step);

private:
    std::vector<AColor*> Fading_Colors;

};

//
class AFont
{
public:
    AFont(int height, int weight, int family, const wchar_t* face_name, bool is_italic = false);
    ~AFont();

    void Select(HDC hdc) const;

private:
    HFONT Font_Handle;
};

//
class AsConfig
{
public:
    // Lvl
    static const int LEVEL_X_OFFSET = 8;
    static const int LEVEL_Y_OFFSET = 6;
    static const int CELL_WIDTH = 16;
    static const int CELL_HEIGHT = 8;
    static const int LEVEL_WIDTH = 12; // ширина уровня в кирпичах
    static const int LEVEL_HEIGHT = 14; // высота уровня в кирпичах
    static const AColor BG_Color;
    static bool Level_Has_Floor;


    // Engine
    static const int GLOBAL_SCALE = 3;
    static const int FPS = 20;
    static const int MAX_X_POS = LEVEL_X_OFFSET + CELL_WIDTH * LEVEL_WIDTH;
    static const int MAX_Y_POS = 199;
    static const int Max_Movers_Count = 10;
    static const int Initial_Life_Count = 0;
    static const int Max_Life_Count = 12;
    static const double D_GLOBAL_SCALE;
    static const AFont Name_Font, Score_Font, Logo_Pop_Font, Logo_Corn_Font, Game_Over_Font;
    static int Current_Timer_Tick;
    static HWND HWnd;

    // Border
    static const int BORDER_X_OFFSET = 6;
    static const int BORDER_Y_OFFSET = 4;
    static const int Floor_Y_Pos = MAX_Y_POS - 1;
    static const int Gate_Count = 8;

    // Ball
    static const int Max_Balls_Count = 10;
    static const double Ball_RADIUS;
    static const double Normal_Ball_Speed;
    static const double Moving_STEP_SIZE;
    static const double START_BALL_Y_POS;
    static const double Ball_Accelerate;
    static const double Min_Ball_Angle;

    // Brick
    static const int BRICK_WIDTH = 15;
    static const int BRICK_HEIGHT = 7;
    static const int Max_Active_Bricks_Count = 10;
    static const int Max_Falling_Letters_Count = 10;
    static const int Hits_Per_Letter = 10; // Вероятность выбить букву = 1.0 / Hits_Per_Letter
    
    static const AColor Pink_Color, Blue_Color, White_Color, Letter_Color, Teleport_Portal_Color, 
        Blue_Highlight_Unbreakable, Pink_Highlight_Unbreakable, Advertisement_Pink_Table, Advertisement_Blue_Table, 
        Laser_Color, Gate_Color, Monster_Dark_Pink_Color, Monster_Cornea_Color, Monster_Iris_Color, Monster_Comet_Tail,
        BG_Outline_Color, Explosion_Pink_Color, Explosion_Blue_Color, Shadow_Color, Highlight_Color;


    // Platform
    static const int Platform_Y_POS = 185;
    static const int Meltdown_Speed = 3;
    static const int Platform_NORMAL_WIDTH = 28;
    static const int Platform_CIRCLE_SIZE = 7;
    static const int Platform_NORMAL_INNER_WIDTH = Platform_NORMAL_WIDTH - Platform_CIRCLE_SIZE;
    static const int Platform_Height = 7;
    static const int Platform_Expanding_Inner_Width = 12;

    static void Throw();
};

//
class AsTools
{
public:
    static void Rect(HDC hdc, RECT& rect, const AColor& color);
    static void Rect(HDC hdc, int x, int y, int width, int height, const AColor& color);
    static void Round_Rect(HDC hdc, RECT& rect, int corner_radius = 2);
    static void Ellipse(HDC hdc, RECT& rect, const AColor& color);
    static void Invalidate_Rect(RECT& rect);
    static int Rand(int range);

    static AColor* Get_Fading_Color(const AColor& origin_color, int step, int max_step);
    static AColor* Get_Fading_Color(const AColor& origin_color, const AColor& base_color, int step, int max_step);
    static unsigned char Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step, int max_step);
    static bool Reflect_On_Circle(double next_x_pos, double next_y_pos, double circle_x, double circle_y, double circle_radius, ABall_Object* ball);

};

//
class AHit_Checker
{
public:
    virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall_Object* ball) = 0;
    virtual bool Check_Hit(double next_x_pos, double next_y_pos);
    virtual bool Check_Hit(RECT& rect);

    bool Hit_Circle_On_Line(double y, double next_x_pos, double left_x, double right_x, double radius, double& x);
};

//
class AHit_Checker_List
{
public:
    AHit_Checker_List();

    void Add_Hit_Checker(AHit_Checker* hit_checker);
    bool Check_Hit(double x_pos, double y_pos, ABall_Object* ball);
    bool Check_Hit(double x_pos, double y_pos);
    bool Check_Hit(RECT& rect);

private:
    int Hit_Checkers_Count;
    std::vector<AHit_Checker*> Hit_checkers;
};
