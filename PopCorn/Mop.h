#pragma once
#include "Mop_Cylinder.h"
#include "Mop_Indicator.h"

//
enum class EMop_State : unsigned char
{
    Idle,

    Ascending,
    Cleaning,
    Clean_Done,
    Showing,
    Descending,
    Descend_Done
};

//
class AsMop : public AGraphics_Object
{
public:
    AsMop();
    ~AsMop();

    virtual void Act();
    virtual void Clear(HDC hdc, RECT& paint_area);
    virtual void Draw(HDC hdc, RECT& paint_area);
    virtual bool Is_Finished();

    void Activate(bool cleaning);
    void Set_Mop();
    void Clean_Area(HDC hdc);
    bool Is_Mopping_Done();
    bool Is_Cleaning_Done();

private:
    bool Mopping_Is_Done;
    int Y_Pos, MAX_Y_POS;
    int Start_Tick;
    int Lifting_Height;
    RECT Prev_Mop_Rect, Mop_Rect;
    EMop_State Mop_State;

    std::vector<AMop_Indicator*> Mop_Indicators;
    std::vector<AMop_Cylinder*> Mop_Cylinders;

    static const int Width = (AsConfig::LEVEL_WIDTH - 1) * AsConfig::CELL_WIDTH + AsConfig::BRICK_WIDTH;
    static const int Height = AsConfig::BRICK_HEIGHT;
    static const int Expansion_Timeout = AsConfig::FPS * 2;
    static const int Lifting_Timeout = AsConfig::FPS;

    void Act_Lifring(bool lift_up);
    int Get_Cylinders_Height();
};