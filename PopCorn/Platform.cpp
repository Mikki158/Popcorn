#include "Platform.h"


AsPlatform::AsPlatform()
    : X_Pos(100), X_Step(AsConfig::GLOBAL_SCALE), Width(NORMAL_WIDTH), Inner_width(NORMAL_WIDTH - CIRCLE_SIZE), 
    Platform_Rect(), Prev_Platform_Rect(), Platform_Circle_Pen(), Platform_Inner_Pen(), Platform_Circle_Brush(), 
    Platform_Inner_Brush(), Platform_State(EPS_Normal), Meltdown_Y_Pos(0), Rolling_Step(0), Normal_Platform_Image(nullptr),
    Normal_Platform_Image_Width(0), Normal_Platform_Image_Height(0), Platform_Circle_Pen_Color(255, 85, 255), 
    Platform_Inner_Pen_Color(85, 255, 255)
{

}

AsPlatform::~AsPlatform()
{
    delete[] Normal_Platform_Image;
}

bool AsPlatform::Check_Hit(double next_x_pos, double next_y_pos, ABall* ball)
{
    

    double inner_top_y, inner_low_y;;
    double inner_left_x, inner_right_x;
    double inner_y;
    double reflection_pos;

    



    // отражаем шарик от платформы
    if (next_y_pos + ball->RADIUS < AsConfig::Platform_Y_POS)
        return false;

    inner_top_y = (double)(AsConfig::Platform_Y_POS - 1);
    inner_low_y = (double)(AsConfig::Platform_Y_POS + Height - 1);
    inner_left_x = (double)(X_Pos + (CIRCLE_SIZE - 1));
    inner_right_x = (double)(X_Pos + Width - (CIRCLE_SIZE - 1));

    // Проверяем отражение от боковых шариков
    if (Reflect_On_Circle(next_x_pos, next_y_pos, 0.0, ball))
        return true;// От левого

    if (Reflect_On_Circle(next_x_pos, next_y_pos, Width - CIRCLE_SIZE, ball))
        return true;// От правого


    // Проверяем отражение от центральной части
    if (ball->Is_Moving_Up())
        inner_y = inner_low_y; // От нижней грани
    else
        inner_y = inner_top_y; // От верхней грани
    
    if (Hit_Circle_On_Line(next_y_pos - inner_y, next_x_pos, inner_left_x, inner_right_x, ball->RADIUS, reflection_pos))
    {
        ball->Reflect(true);
        return true;
    }
    

    return false;
}

bool AsPlatform::Reflect_On_Circle(double next_x_pos, double next_y_pos, double platform_ball_x_offset, ABall* ball)
{
    double dx, dy;
    double platform_ball_x, platform_ball_y, platform_ball_radius;
    double distance, two_radius;

    double alpha, beta, gamma;
    double realted_ball_direction;

    const double pi_2 = 2.0 * M_PI;

    platform_ball_radius = (double)(CIRCLE_SIZE / 2.0);
    platform_ball_x = (double)(X_Pos + platform_ball_radius + platform_ball_x_offset);
    platform_ball_y = (double)AsConfig::Platform_Y_POS + platform_ball_radius;

    dx = next_x_pos - platform_ball_x;
    dy = next_y_pos - platform_ball_y;

    distance = sqrt((dx * dx) + (dy * dy));
    two_radius = platform_ball_radius + ball->RADIUS;

    if (fabs(distance - two_radius) < AsConfig::STEP_SIZE)
    {// Мячик коснулся бокового шарика
        beta = atan2(-dy, dx);

        realted_ball_direction = ball->Get_Direction();
        realted_ball_direction -= beta;

        if (realted_ball_direction > pi_2)
            realted_ball_direction -= pi_2;

        if (realted_ball_direction < 0)
            realted_ball_direction += pi_2;

        if (realted_ball_direction > M_PI_2 && realted_ball_direction < M_PI + M_PI_2)
        {
            alpha = beta + M_PI - ball->Get_Direction();
            gamma = beta + alpha;

            ball->Set_Direction(gamma);

            return true;
        }
    }

    return false;
}


void AsPlatform::Init()
{
    AsConfig::Create_Pen_Brush(Platform_Circle_Pen_Color, Platform_Circle_Pen, Platform_Circle_Brush);
    AsConfig::Create_Pen_Brush(Platform_Inner_Pen_Color, Platform_Inner_Pen, Platform_Inner_Brush);
}

void AsPlatform::Redraw_Platform()
{
    HWND hwnd = AsConfig::HWnd;

    int platform_width;

    Prev_Platform_Rect = Platform_Rect;

    if (Platform_State == EPS_Roll_In)
    {
        platform_width = CIRCLE_SIZE;
    }
    else
    {
        platform_width = Width;
    }

    Platform_Rect.left = X_Pos * AsConfig::GLOBAL_SCALE;
    Platform_Rect.top = AsConfig::Platform_Y_POS * AsConfig::GLOBAL_SCALE;
    Platform_Rect.right = Platform_Rect.left + platform_width * AsConfig::GLOBAL_SCALE;
    Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::GLOBAL_SCALE;
    
    if (Platform_State == EPS_Meltdown)
    {
        Prev_Platform_Rect.bottom = (AsConfig::MAX_X_POS + 1) * AsConfig::GLOBAL_SCALE;
    }

    InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
    InvalidateRect(hwnd, &Platform_Rect, FALSE);
}

void AsPlatform::Draw(HDC hdc, RECT& paint_area)
{
    RECT intersection_rect;

    if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
    {
        return;
    }

    switch (Platform_State)
    {
    case EPS_Normal:
        Draw_Normal_State(hdc, paint_area);
        break;

    case EPS_Meltdown:
        Draw_Meltdown_State(hdc, paint_area);
        break;

    case EPS_Roll_In:
        Draw_Roll_In_State(hdc, paint_area);
        break;

    case EPS_Expand_Roll_In:
        Draw_Expanding_Roll_In_State(hdc, paint_area);
        break;

    case EPS_Missing:
        break;

    default:
        break;
    }

    
}

void AsPlatform::Clear_BG(HDC hdc) // Очищаем фоном прежнее место
{
    SelectObject(hdc, AsConfig::BG_Pen);
    SelectObject(hdc, AsConfig::BG_Brush);

    Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top,
        Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);
}

void AsPlatform::Draw_Normal_State(HDC hdc, RECT& paint_area)
{
    int offset = 0;
    int x = X_Pos;
    int y = AsConfig::Platform_Y_POS;

    Clear_BG(hdc);

    // Рисуем боковые шарики
    SelectObject(hdc, Platform_Circle_Pen);
    SelectObject(hdc, Platform_Circle_Brush);

    Ellipse(hdc, x * AsConfig::GLOBAL_SCALE, y * AsConfig::GLOBAL_SCALE, (x + CIRCLE_SIZE) * AsConfig::GLOBAL_SCALE - 1,
        (y + CIRCLE_SIZE) * AsConfig::GLOBAL_SCALE - 1);
    Ellipse(hdc, (x + Inner_width) * AsConfig::GLOBAL_SCALE, y * AsConfig::GLOBAL_SCALE,
        (x + Inner_width + CIRCLE_SIZE) * AsConfig::GLOBAL_SCALE - 1, (y + CIRCLE_SIZE) * AsConfig::GLOBAL_SCALE - 1);

    // Рисуем среднюю платформу
    SelectObject(hdc, Platform_Inner_Pen);
    SelectObject(hdc, Platform_Inner_Brush);

    RoundRect(hdc, (x + 4) * AsConfig::GLOBAL_SCALE, (y + 1) * AsConfig::GLOBAL_SCALE,
        (x + 4 + Inner_width - 1) * AsConfig::GLOBAL_SCALE - 1, (y + 1 + 5) * AsConfig::GLOBAL_SCALE - 1,
        3 * AsConfig::GLOBAL_SCALE, 3 * AsConfig::GLOBAL_SCALE);

    x *= AsConfig::GLOBAL_SCALE;
    y *= AsConfig::GLOBAL_SCALE;

    if (Normal_Platform_Image == nullptr)
    {
        Normal_Platform_Image_Width = Width * AsConfig::GLOBAL_SCALE;
        Normal_Platform_Image_Height = Height * AsConfig::GLOBAL_SCALE;

        Normal_Platform_Image = new int[Normal_Platform_Image_Width * Normal_Platform_Image_Height];

        for (int i = 0; i < Normal_Platform_Image_Height; i++)
        {
            for (int j = 0; j < Normal_Platform_Image_Width; j++)
            {
                Normal_Platform_Image[offset++] = GetPixel(hdc, x + j, y + i);
            }
        }
    }
}

void AsPlatform::Set_State(EPlatform_State new_state)
{
    int len;

    if (Platform_State == new_state)
    {
        return;
    }

    switch (new_state)
    {
    case EPS_Missing:
        break;
    case EPS_Normal:
        break;
    case EPS_Meltdown:
        Platform_State = EPS_Meltdown;

        len = sizeof(Meltdown_Platform_Y_Pos) / sizeof(Meltdown_Platform_Y_Pos[0]);

        for (int i = 0; i < len; i++)
        {
            Meltdown_Platform_Y_Pos[i] = Platform_Rect.top;
        }
        break;
    case EPS_Roll_In:
        X_Pos = AsConfig::MAX_X_POS;
        Rolling_Step = MAX_ROLLING_STEP - 2;
        break;
    default:
        break;
    }

    Platform_State = new_state;
}

void AsPlatform::Act()
{
    HWND hwnd = AsConfig::HWnd;

    switch (Platform_State)
    {
    case EPS_Meltdown:
        Redraw_Platform();
        break;

    case EPS_Roll_In:
        Redraw_Platform();
        break;

    case EPS_Expand_Roll_In:
        Redraw_Platform();
        break;

    default:
        break;
    }
}

void AsPlatform::Move(bool to_left)
{
    if (Platform_State != EPS_Normal)
        return;

    if (to_left)
    {
        X_Pos -= X_Step;

        if (X_Pos <= AsConfig::BORDER_X_OFFSET)
        {
            X_Pos = AsConfig::BORDER_X_OFFSET;
        }

        Redraw_Platform();
    }
    else
    {
        X_Pos += X_Step;

        if (X_Pos >= AsConfig::MAX_X_POS - Width + 1)
        {
            X_Pos = AsConfig::MAX_X_POS - Width + 1;
        }

        Redraw_Platform();
    }
}

bool AsPlatform::Hit_by(AFalling_Letter* falling_letter)
{
    RECT intersection_rect, falling_letter_rect;

    falling_letter->Get_Letter_Cell(falling_letter_rect);

    if (IntersectRect(&intersection_rect, &falling_letter_rect, &Platform_Rect))
        return true;
    else
        return false;
}

void AsPlatform::Draw_Meltdown_State(HDC hdc, RECT& paint_area)
{
    int x, y, j;
    int y_offset;
    int stroke_len;
    int moved_columns_count = 0;
    int max_platform_y = (AsConfig::MAX_Y_POS + 1)* AsConfig::GLOBAL_SCALE;
    HPEN color_pen;
    COLORREF bg_pixel = RGB(AsConfig::BG_Color.R, AsConfig::BG_Color.G, AsConfig::BG_Color.B);


    for (int i = 0; i < Normal_Platform_Image_Width; i++)
    {
        if (Meltdown_Platform_Y_Pos[i] > max_platform_y + 5)
        {
            continue;
        }

        moved_columns_count += 1;

        x = Platform_Rect.left + i;
        
        y_offset = AsConfig::Rand(AsConfig::Meltdown_Speed) + 1;

        j = 0;
        y = Meltdown_Platform_Y_Pos[i];

        MoveToEx(hdc, x, y, 0);

        // Рисуем последовательность вертикальных штрихов разного цвета (согласно прообразу, сохраненному в Normal_Platform_Image)
        while (Get_Platform_Image_Stroke_Color(i, j, color_pen, stroke_len))
        {
            SelectObject(hdc, color_pen);
            LineTo(hdc, x, y + stroke_len);

            y += stroke_len;
            j += stroke_len;
        }

        // Стираем фоном пиксели над штрихом
        y = Meltdown_Platform_Y_Pos[i];
        MoveToEx(hdc, x, y, 0);
        SelectObject(hdc, AsConfig::BG_Pen);
        LineTo(hdc, x, y + y_offset);

        Meltdown_Platform_Y_Pos[i] += y_offset;
    }

    if (moved_columns_count == 0)
    {
        Platform_State = EPS_Missing;
    }
}

bool AsPlatform::Get_Platform_Image_Stroke_Color(int x, int y, HPEN &color_pen, int &stroke_len)
{// Вычисляет длину очередного вертикального штриха
    int offset = y * Normal_Platform_Image_Width + x; // Позиция в массиве Normal_Platform_Image_Height соответствующая смещению (x, y)
    int color;  

    stroke_len = 0;

    if (y >= Normal_Platform_Image_Height)
        return false;

    for (int i = y; i < Normal_Platform_Image_Height; i++)
    {
        if (i == y)
        {
            color = Normal_Platform_Image[offset];
            stroke_len = 1;
        }
        else
        {
            if (color == Normal_Platform_Image[offset])
                stroke_len++;
            else
                break;
        }

        offset += Normal_Platform_Image_Width; // Переходим на строку ниже
    }

    if (color == Platform_Circle_Pen_Color.Get_RGB())
        color_pen = Platform_Circle_Pen;
    else if (color == Platform_Inner_Pen_Color.Get_RGB())
        color_pen = Platform_Inner_Pen;
    else if (color == AsConfig::BG_Color.Get_RGB())
        color_pen = AsConfig::BG_Pen;
    else
        color_pen = 0;

    return true;

    //switch (color)
    //{
    //case BG:
    //    color_pen = AsConfig::BG_Pen;
    //    break;
    //default:
    //    break;
    //}
}

void AsPlatform::Draw_Roll_In_State(HDC hdc, RECT paint_area)
{
    int x = X_Pos * AsConfig::GLOBAL_SCALE;
    int y = AsConfig::Platform_Y_POS * AsConfig::GLOBAL_SCALE;
    int roller_size = CIRCLE_SIZE * AsConfig::GLOBAL_SCALE;
    double alpha;

    XFORM xform, old_xform;

    Clear_BG(hdc);

    // 1. Шарик
    SelectObject(hdc, Platform_Circle_Pen);
    SelectObject(hdc, Platform_Circle_Brush);

    Ellipse(hdc, x , y , x + roller_size - 1, y + roller_size - 1);

    // 2. Разделительная линия


    alpha = -2.0 * M_PI / (double)MAX_ROLLING_STEP * (double)Rolling_Step;

    xform.eM11 = (float)cos(alpha);
    xform.eM12 = (float)sin(alpha);
    xform.eM21 = (float)-sin(alpha);
    xform.eM22 = (float)cos(alpha);
    xform.eDx  = (float)(x + roller_size / 2);
    xform.eDy  = (float)(y + roller_size / 2);

    GetWorldTransform(hdc, &old_xform);
    SetWorldTransform(hdc, &xform);

    SelectObject(hdc, AsConfig::BG_Pen);
    SelectObject(hdc, AsConfig::BG_Brush);

    Rectangle(hdc, - AsConfig::GLOBAL_SCALE / 2, -roller_size / 2, AsConfig::GLOBAL_SCALE / 2 - 1, roller_size / 2 - 1);

    SetWorldTransform(hdc, &old_xform);

    Rolling_Step += 1;

    if (Rolling_Step > MAX_ROLLING_STEP)
    {
        Rolling_Step -= MAX_ROLLING_STEP;
    }

    X_Pos -= ROLLING_PLATFORM_SPEED;

    if (X_Pos <= ROLL_IN_PLATFORM_END_X_POS)
    {
        X_Pos += ROLLING_PLATFORM_SPEED;
        Platform_State = EPS_Expand_Roll_In;
        Inner_width = 1;
    }    
}

void AsPlatform::Draw_Expanding_Roll_In_State(HDC hdc, RECT paint_area)
{
    Draw_Normal_State(hdc, paint_area);

    X_Pos -= 1;

    Inner_width += 2;

    if (Inner_width >= NORMAL_PLATFORM_INNER_WIDTH)
    {
        Inner_width = NORMAL_PLATFORM_INNER_WIDTH;
        Platform_State = EPS_Ready;
        Redraw_Platform();
    }
}

EPlatform_State AsPlatform::Get_State()
{
    return Platform_State;
}

