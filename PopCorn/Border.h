﻿#pragma once

#include <Windows.h>

#include "Ball.h"

enum class EGate_State : unsigned char
{
    Closed,
    Short_Open,
    Long_Open
};

enum class EGate_Tranformation : unsigned char
{
    Unknow,

    Init,
    Active,
    Finalize
};

class AGate: public AGraphics_Object
{
public:
    AGate(int x_pos, int y_pos);

    virtual void Act();
    virtual void Clear(HDC hdc, RECT& paint_area);
    virtual void Draw(HDC hdc, RECT& paint_area);
    virtual bool Is_Finished();

    void Open_Gate(bool short_open);
    bool Is_Opened();
    void Get_Y_Size(int& gate_top_y, int& gate_low_y);

private:
    int X_Pos;
    double Y_Pos, Origin_Y_Pos;
    int Edge_Count;
    int Gate_Close_Tick;
    double Gap_Height;
    RECT Gate_Rect;
    EGate_State Gate_State;
    EGate_Tranformation Gate_Tranformation;

    static const double Max_Gap_Short_Height, Gap_Height_Short_Step;
    static const double Max_Gap_Long_Height, Gap_Height_Long_Step;
    static const int Width = 6;
    static const int Height = 19;
    static const int Short_Opening_Timeout = AsConfig::FPS; // 1 секунда
    static const int Long_Opening_Timeout = AsConfig::FPS * 3; // 2 секунда

    bool Act_For_Open(bool short_open, bool& correct_pos);
    void Draw_Cup(HDC hdc, bool top_cup);
    void Draw_Edges(HDC hdc);
    void Draw_Long_Opening_Edges(HDC hdc);
    void Draw_Pink_Edge(HDC hdc, int edge_y_offset, bool long_edge, bool has_highlight);
    void Draw_Short_Opening_Edges(HDC hdc);
    void Draw_One_Edge(HDC hdc, int edge_y_offset, bool long_edge);
    void Draw_Charge(HDC hdc);
    void Redraw_Gate();
};

class AsBorder: public AHit_Checker, public AGraphics_Object
{
public:

    AsBorder();
    ~AsBorder();

    virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall* ball);

    virtual void Act();
    virtual void Clear(HDC hdc, RECT& paint_area);
    virtual void Draw(HDC hdc, RECT& paint_area);
    virtual bool Is_Finished();

    void Redraw_Floor();
    void Open_Gate(int gate_index, bool short_open);
    bool Is_Gate_Opened(int gate_index);

private:
    RECT Floor_Rect;
    AGate *Gates[AsConfig::Gate_Count];


    void Draw_Floor(HDC hdc, RECT& paint_area);
    void Draw_Element(HDC hdc, RECT& paint_area, int x, int y, bool top_border);
};