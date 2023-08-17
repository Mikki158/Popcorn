#include "Falling_Letter.h"

int AFalling_Letter::Letters_Popularity[ELT_MAX] = {7, 7, 7, 7, 7, 7, 7,  3, 3, 3,  1};
int AFalling_Letter::All_Letters_Popularity;


AFalling_Letter::AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y)
    :brick_type(brick_type), letter_type(letter_type), X(x), Y(y), Rotation_step(2),
    Next_Rotation_Tick(AsConfig::Current_Timer_Tick + Ticks_Per_Step), Falling_Latter_State(EFLS_Normal)
{
    Letter_Cell.left = X;
    Letter_Cell.top = Y;
    Letter_Cell.right = Letter_Cell.left + AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE;
    Letter_Cell.bottom = Letter_Cell.top + AsConfig::BRICK_HEIGHT * AsConfig::GLOBAL_SCALE;

    Prev_Letter_Cell = Letter_Cell;
}

void AFalling_Letter::Draw_Brick_Letter(HDC hdc)
{// Вывод падающих букв
    double offSet;
    double rotation_angle;
    double y_ration;
    int back_part_offset;
    bool switch_color;
    HPEN front_pen, back_pen;
    HBRUSH front_brush, back_brush;
    XFORM xForm, old_xForm;

    if (!(brick_type == EBT_Blue || brick_type == EBT_Pink))
    {
        return;
    }

    // корректируем шаг вращения и угол поворота
    Rotation_step = Rotation_step % Max_Rotation_Step;
    if (Rotation_step < 8)
    {
        rotation_angle = 2.0 * M_PI / (double)Max_Rotation_Step * (double)Rotation_step;
    }
    else
    {
        rotation_angle = 2.0 * M_PI / (double)Max_Rotation_Step * (double)(8 - Rotation_step);
    }

    if (Rotation_step > 4 && Rotation_step <= 12)
    {
        if (brick_type == EBT_Pink)
            switch_color = true;
        else
            switch_color = false;
    }
    else
    {
        if (brick_type == EBT_Blue)
            switch_color = true;
        else
            switch_color = false;
    }

    Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);

    if (Rotation_step == 4 || Rotation_step == 12)
    {
        // выводим фон
        
        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);


        Rectangle(hdc, X, Y + Brick_Half_Height - AsConfig::GLOBAL_SCALE,
            X + AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE, Y + Brick_Half_Height);

        // выводим передний план
        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);

        Rectangle(hdc, X, Y + Brick_Half_Height,
            X + AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE, Y + Brick_Half_Height + AsConfig::GLOBAL_SCALE);
    }
    else
    {
        y_ration = cos(rotation_angle);

        xForm.eM11 = 1.0;
        xForm.eM12 = 0.0;
        xForm.eM21 = 0.0;
        xForm.eM22 = (float)fabs(cos(rotation_angle));
        xForm.eDx = (float)X;
        xForm.eDy = (float)Y + (float)(Brick_Half_Height);

        GetWorldTransform(hdc, &old_xForm);
        SetWorldTransform(hdc, &xForm);


        offSet = 3.0 * (1.0 - fabs(xForm.eM22)) * (double)AsConfig::GLOBAL_SCALE;
        back_part_offset = (int)round(offSet);

        // выводим фон
        SelectObject(hdc, front_pen);
        SelectObject(hdc, front_brush);

        if (y_ration < 0.0)
            back_part_offset = -back_part_offset;

        Rectangle(hdc, 0, -Brick_Half_Height - back_part_offset, AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE - 1, Brick_Half_Height - back_part_offset);

        // выводим передний план
        SelectObject(hdc, back_pen);
        SelectObject(hdc, back_brush);

        Rectangle(hdc, 0, -Brick_Half_Height, AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE - 1, Brick_Half_Height);

        if (Rotation_step > 4 && Rotation_step <= 12)
        {
            SelectObject(hdc, AsConfig::Letter_Pen);
            switch (letter_type)
            {
            case ELT_O: // "Отмена"
                Ellipse(hdc, 0 + 5 * AsConfig::GLOBAL_SCALE, 1 * AsConfig::GLOBAL_SCALE - Brick_Half_Height - 1,
                    0 + 10 * AsConfig::GLOBAL_SCALE, 6 * AsConfig::GLOBAL_SCALE - Brick_Half_Height);
                break;

            case ELT_I: // "Инверсия"
                Draw_Line(hdc, 5, 1, 5, 6);
                Draw_Line_To(hdc, 9, 1);
                Draw_Line_To(hdc, 9, 6);
                break;

            case ELT_C: // "Скорость"
                Draw_C(hdc);
                break;

            case ELT_M: // "Монстры"
                Draw_Line(hdc, 5, 6, 5, 1);
                Draw_Line_To(hdc, 7, 3);
                Draw_Line_To(hdc, 9, 1);
                Draw_Line_To(hdc, 9, 6);
                break;

            case ELT_G: // "Жизнь"
                Draw_Line(hdc, 7, 1, 7, 6);
                Draw_Line(hdc, 5, 3, 9, 3);
                Draw_Line(hdc, 4, 1, 5, 3);              
                Draw_Line(hdc, 5, 3, 4, 6);
                Draw_Line(hdc, 10, 1, 9, 3);
                Draw_Line(hdc, 9, 3, 10, 6);           
                break;

            case ELT_K: // "Клей"
                Draw_Line(hdc, 6, 1, 6, 6);
                Draw_Line(hdc, 9, 1, 6, 4);
                Draw_Line(hdc, 9, 6, 6, 3);
                
                break;
            
            case ELT_W: // "Шире"
                Draw_Line(hdc, 4, 1, 4, 6);
                Draw_Line_To(hdc, 10, 6);
                Draw_Line_To(hdc, 10, 1);
                Draw_Line(hdc, 7, 1, 7, 6);
                break;

            case ELT_P: // "Пол"
                Draw_Line(hdc, 5, 6, 5, 1);
                Draw_Line_To(hdc, 9, 1);
                Draw_Line_To(hdc, 9, 6);
                break;

            case ELT_L: // "Лазер"
                Draw_Line(hdc, 5, 6, 7, 1);
                Draw_Line_To(hdc, 9, 6);
                break;

            case ELT_T: // "Три"
                Draw_Line(hdc, 5, 1, 9, 1);
                Draw_Line(hdc, 7, 1, 7, 6);
                break;

            case ELT_PLUS: // "Переход на следующий уровень"
                Draw_Line(hdc, 7, 1, 7, 5);
                Draw_Line(hdc, 5, 3, 9, 3);
                break;

            default:
                break;
            }
        }
        SetWorldTransform(hdc, &old_xForm);
    }
}

void AFalling_Letter::Draw_C(HDC hdc)
{
    int arc_x = 5 * AsConfig::GLOBAL_SCALE;
    int arc_y = 1 * AsConfig::GLOBAL_SCALE - Brick_Half_Height;
    int arc_size = 5 * AsConfig::GLOBAL_SCALE - 1;

    Arc(hdc, arc_x, arc_y, arc_x + arc_size, arc_y + arc_size, arc_x + arc_size, arc_y, arc_x + arc_size, arc_y + arc_size);
}

void AFalling_Letter::Draw_Line_To(HDC hdc, int x, int y)
{
    int end_y;

    end_y = y * AsConfig::GLOBAL_SCALE - Brick_Half_Height;
    if (y == 6)
        end_y = end_y - 1;

    LineTo(hdc, x * AsConfig::GLOBAL_SCALE + 1, y * AsConfig::GLOBAL_SCALE - Brick_Half_Height);
}

void AFalling_Letter::Draw_Line(HDC hdc, int x_1, int y_1, int x_2, int y_2)
{
    int start_y, end_y;

    start_y = y_1 * AsConfig::GLOBAL_SCALE - Brick_Half_Height;
    if (y_1 == 6)
        start_y = start_y - 1;

    end_y = y_2 * AsConfig::GLOBAL_SCALE - Brick_Half_Height;
    if (y_2 == 6)
        end_y = end_y - 1;

    MoveToEx(hdc, x_1 * AsConfig::GLOBAL_SCALE + 1, y_1 * AsConfig::GLOBAL_SCALE - Brick_Half_Height, 0);
    LineTo(hdc, x_2 * AsConfig::GLOBAL_SCALE + 1, y_2 * AsConfig::GLOBAL_SCALE - Brick_Half_Height);
}

void AFalling_Letter::Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush)
{
    if (is_switch_color)
    {
        front_pen = AsConfig::Brick_Pink_Pen;
        front_brush = AsConfig::Brick_Pink_Brush;

        back_pen = AsConfig::Brick_Blue_Pen;
        back_brush = AsConfig::Brick_Blue_Brush;
    }
    else
    {
        front_pen = AsConfig::Brick_Blue_Pen;
        front_brush = AsConfig::Brick_Blue_Brush;

        back_pen = AsConfig::Brick_Pink_Pen;
        back_brush = AsConfig::Brick_Pink_Brush;
    }
}

void AFalling_Letter::Draw(HDC hdc, RECT& paint_area)
{
    RECT intersection_rect;

    if (IntersectRect(&intersection_rect, &paint_area, &Prev_Letter_Cell))
    {
        // очищаем фон
        SelectObject(hdc, AsConfig::BG_Pen);
        SelectObject(hdc, AsConfig::BG_Brush);

        Rectangle(hdc, Prev_Letter_Cell.left, Prev_Letter_Cell.top, Prev_Letter_Cell.right, Prev_Letter_Cell.bottom);
    }

    if (Falling_Latter_State == EFLS_Finalizing)
    {
        Falling_Latter_State = EFLS_Finished;
        return;
    }

    if (IntersectRect(&intersection_rect, &paint_area, &Letter_Cell))
        Draw_Brick_Letter(hdc);
}


//
void AFalling_Letter::Act()
{
    if (Falling_Latter_State != EFLS_Normal)
        return;

    if (Letter_Cell.top >= AsConfig::MAX_Y_POS * AsConfig::GLOBAL_SCALE)
    {
        Finalize();
        return;
    }

    Prev_Letter_Cell = Letter_Cell;

    Y += AsConfig::GLOBAL_SCALE;

    Letter_Cell.top += AsConfig::GLOBAL_SCALE;
    Letter_Cell.bottom += AsConfig::GLOBAL_SCALE;

    if (AsConfig::Current_Timer_Tick >= Next_Rotation_Tick)
    {
        Rotation_step++;

        Next_Rotation_Tick += Ticks_Per_Step;
    }

    InvalidateRect(AsConfig::HWnd, &Prev_Letter_Cell, FALSE);
    InvalidateRect(AsConfig::HWnd, &Letter_Cell, FALSE);
}

bool AFalling_Letter::Is_Finished()
{
    if (Falling_Latter_State == EFLS_Finished)
        return true;
    else
        return false;
}

void AFalling_Letter::Get_Letter_Cell(RECT& rect)
{
    rect = Letter_Cell;
}

void AFalling_Letter::Finalize()
{
    Falling_Latter_State = EFLS_Finalizing;
    InvalidateRect(AsConfig::HWnd, &Prev_Letter_Cell, FALSE);
    InvalidateRect(AsConfig::HWnd, &Letter_Cell, FALSE);
}

void AFalling_Letter::Test_Draw_All_Steps(HDC hdc)
{
    Rotation_step = 0;

    for (int i = 0; i < Max_Rotation_Step; i++)
    {
        Draw_Brick_Letter(hdc);

        Rotation_step++;
        X += AsConfig::CELL_WIDTH * AsConfig::GLOBAL_SCALE;
        Letter_Cell.left += AsConfig::CELL_WIDTH * AsConfig::GLOBAL_SCALE;
        Letter_Cell.right += AsConfig::CELL_WIDTH * AsConfig::GLOBAL_SCALE;
    }
}

void AFalling_Letter::Init()
{
    All_Letters_Popularity = 0;

    for (int i = 0; i < ELT_MAX; i++)
    {
        All_Letters_Popularity += Letters_Popularity[i];
    }
}

ELetter_Type AFalling_Letter::Get_Random_Letter_Type()
{
    int letters_popularity;

    letters_popularity = AsConfig::Rand(All_Letters_Popularity);

    for (int i = 0; i < ELT_MAX; i++)
    {
        if (letters_popularity < Letters_Popularity[i])
            return (ELetter_Type)i; 

        letters_popularity -= Letters_Popularity[i];
    }

    return ELT_O;
}


