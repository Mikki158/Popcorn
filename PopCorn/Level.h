#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

#include "Active_Brick.h"
#include "Falling_Letter.h"

struct SPoint
{
    int X, Y;
};



class AsLevel: public AHit_Checker
{
public:

    static char Level_01[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Test_Level[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];

    AsLevel();
    ~AsLevel();

    virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball);

    void Init();
    void Act();
    void Set_Current_Level(char Level[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH]);
    void Draw(HDC hdc, RECT& paint_area);
    void Stop();

    bool Get_Next_Falling_Leter(AFalling_Letter **falling_letter, int &index);


private:
    int Teleport_Bricks_Count;
    double Current_Brick_Left_X, Current_Brick_Right_X;
    double Current_Brick_Top_Y, Current_Brick_Low_Y;
    bool Need_To_Cancel_All;


    AColor Parachute_Color;

    char Current_Level[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];

    SPoint* Teleport_Bricks_Pos;

    int Active_Bricks_Count;
    AActive_Brick* Active_Briks[AsConfig::Max_Active_Bricks_Count];
    int Falling_Letters_Count;
    AFalling_Letter* Falling_Letters[AsConfig::Max_Falling_Letters_Count];

    RECT Level_Rect;

    AAdvertisement* Advertisement;

    void Act_Objects(AGraphics_Object** object_array, int objects_max_count, int& count);
    void Clear_Objects(HDC hdc, RECT& paint_area, AGraphics_Object** object_array, int objects_max_count);
    void Delete_Objects(AGraphics_Object** object_array, int& objects_count, int objects_max_count);
    void Draw_Objects(HDC hdc, RECT& paint_area, AGraphics_Object** object_array, int max_count);
    void Draw_Brick(HDC hdc, RECT Brick_Rect, int level_x, int level_y);
    void Draw_Parachute_in_Level(HDC hdc, RECT brick_rect);
    void Draw_Parachute_Part(HDC hdc, RECT brick_rect, int offset, int width);
    void Redraw_Brick(int brick_x, int brick_y);
    void Add_Active_Brick_Teleport(int brick_x, int brick_y, ABall* ball, bool vertical_hit);
    void Add_Active_Brick(AActive_Brick* active_brick);
    void Cancel_All_Activity();
    
    bool On_Hit(int brick_x, int brick_y, ABall* ball, bool vertical_hit);
    bool Create_Active_Brick(int brick_x, int brick_y, EBrick_Type brick_type, ABall* ball, bool vertical_hit);
    bool Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall* ball, double& reflection_pos);
    bool Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall* ball, double& reflection_pos);
    bool Add_Falling_Letter(int brick_x, int brick_y, EBrick_Type brick_type);

    AActive_Brick_Teleport *Select_Destination_Teleport(int source_x, int source_y);
};
