#include "Active_Brick.h"


AColor AActive_Brick_Pink_Blue::Fading_Blue_Brick_Colors[MAX_FADE_STEP];
AColor AActive_Brick_Pink_Blue::Fading_Pink_Brick_Colors[MAX_FADE_STEP];





 
// AActive_Brick
//
void AActive_Brick::Get_Level_Pos(int& dest_brick_x, int& dest_brick_y)
{
    dest_brick_x = Level_X;
    dest_brick_y = Level_Y;
}


//
AActive_Brick::AActive_Brick(EBrick_Type brick_type, int level_x, int level_y)
    :Brick_Rect(), Brick_Type(brick_type), Level_X(level_x), Level_Y(level_y)
{
    Brick_Rect.left = (AsConfig::LEVEL_X_OFFSET + level_x * AsConfig::CELL_WIDTH) * AsConfig::GLOBAL_SCALE;
    Brick_Rect.top = (AsConfig::LEVEL_Y_OFFSET + level_y * AsConfig::CELL_HEIGHT) * AsConfig::GLOBAL_SCALE;
    Brick_Rect.right = Brick_Rect.left + AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE;
    Brick_Rect.bottom = Brick_Rect.top + AsConfig::BRICK_HEIGHT * AsConfig::GLOBAL_SCALE;
}


//
AActive_Brick::~AActive_Brick()
{
}


//
void AActive_Brick::Clear(HDC hdc, RECT& paint_area)
{

}


//
double AActive_Brick::Get_Brick_X_Pos(bool of_center)
{
    double pos = (double)(AsConfig::LEVEL_X_OFFSET + Level_X * AsConfig::CELL_WIDTH);

    if (of_center)
        pos += (double)AsConfig::BRICK_WIDTH / 2;

    return pos;
}


//
double AActive_Brick::Get_Brick_Y_Pos(bool of_center)
{
    double pos = (double)(AsConfig::LEVEL_Y_OFFSET + Level_Y * AsConfig::CELL_HEIGHT);

    if (of_center)
        pos += (double)AsConfig::BRICK_HEIGHT / 2;

    return pos;
}



// AActive_Brick_Pink_Blue
//
AActive_Brick_Pink_Blue::AActive_Brick_Pink_Blue(EBrick_Type brick_type, int level_x, int level_y)
    :AActive_Brick(brick_type, level_x, level_y), fade_Step(0)
{
    if (! (brick_type == EBT_Blue || brick_type == EBT_Pink))
        throw 13;
}


//
AActive_Brick_Pink_Blue::~AActive_Brick_Pink_Blue()
{

}


//
void AActive_Brick_Pink_Blue::Act()
{
    if (fade_Step <= MAX_FADE_STEP - 1)
    {
        fade_Step += 1;
        InvalidateRect(AsConfig::HWnd, &Brick_Rect, FALSE);
    }
}


//
void AActive_Brick_Pink_Blue::Draw(HDC hdc, RECT& paint_area)
{
    AColor* color = nullptr;

    switch (Brick_Type)
    {
    case EBT_Pink:
        color = &Fading_Pink_Brick_Colors[fade_Step];
        break;

    case EBT_Blue:
        color = &Fading_Blue_Brick_Colors[fade_Step];
        break;

    default:
        break;
    }

    if (color != nullptr)
        color->Select(hdc); 

    RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right - 1, Brick_Rect.bottom - 1,
        2 * AsConfig::GLOBAL_SCALE, 2 * AsConfig::GLOBAL_SCALE);
}


//
bool AActive_Brick_Pink_Blue::Is_Finished()
{
    if (fade_Step >= MAX_FADE_STEP - 1)
        return true;
    else
        return false;
}


//
void AActive_Brick_Pink_Blue::Setup_Color()
{
    for (int i = 0; i < MAX_FADE_STEP; i++)
    {
        Get_Fading_Color(AsConfig::Blue_Color, i, Fading_Blue_Brick_Colors[i]);
        Get_Fading_Color(AsConfig::Pink_Color, i, Fading_Pink_Brick_Colors[i]);
    }
}


//
void AActive_Brick_Pink_Blue::Draw_In_Level(HDC hdc, RECT brick_rect, EBrick_Type brick_type)
{// Вывод неактивного кирпича на уровень
    const AColor* color = 0;

    switch (brick_type)
    {
    case EBT_None:
        color = &AsConfig::BG_Color;
        break;

    case EBT_Pink:
        color = &AsConfig::Pink_Color;
        break;

    case EBT_Blue:
        color = &AsConfig::Blue_Color;
        break;

    default:
        AsConfig::Throw();
        return;
    }

    if(color != nullptr)
        color->Select(hdc);
    
    AsConfig::Round_Rect(hdc, brick_rect);
}


//
unsigned char AActive_Brick_Pink_Blue::Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step)
{
    return color - step * (color - bg_color) / (MAX_FADE_STEP - 1);
}


//
void AActive_Brick_Pink_Blue::Get_Fading_Color(const AColor &origin_color, int step, AColor& result_color)
{
    unsigned char r, g, b;

    r = Get_Fading_Channel(origin_color.R, AsConfig::BG_Color.R, step);
    g = Get_Fading_Channel(origin_color.G, AsConfig::BG_Color.G, step);
    b = Get_Fading_Channel(origin_color.B, AsConfig::BG_Color.B, step);

    result_color = AColor(r, g, b);
}



// AActive_Brick_Unbreakable
//
AActive_Brick_Unbreakable::AActive_Brick_Unbreakable(int level_x, int level_y)
    :AActive_Brick(EBT_Unbreakable, level_x, level_y), Animation_Step(0), Region(0)
{
    Region = CreateRoundRectRgn(Brick_Rect.left, Brick_Rect.top, Brick_Rect.right + 1, Brick_Rect.bottom + 1, 
        2 * AsConfig::GLOBAL_SCALE - 1, 2 * AsConfig::GLOBAL_SCALE - 1);
}


//
AActive_Brick_Unbreakable::~AActive_Brick_Unbreakable()
{
    DeleteObject(Region);
}


//
void AActive_Brick_Unbreakable::Act()
{
    if (Animation_Step <= MAX_ANIMATION_STEP)
    {
        Animation_Step++;
        InvalidateRect(AsConfig::HWnd, &Brick_Rect, FALSE);
    }
}


//
void AActive_Brick_Unbreakable::Draw(HDC hdc, RECT& paint_area)
{
    int scale = AsConfig::GLOBAL_SCALE;
    int offset;

    Draw_In_Level(hdc, Brick_Rect);

    AsConfig::Pink_Color.Select(hdc);

    SelectClipRgn(hdc, Region);

    offset = 2 * Animation_Step * scale - AsConfig::BRICK_WIDTH * scale;

    AsConfig::Blue_Highlight_Unbreakable.Select_Pen(hdc);
    MoveToEx(hdc, Brick_Rect.left + 4 * scale + offset, Brick_Rect.bottom + scale, 0);
    LineTo(hdc, Brick_Rect.left + 13 * scale - 1 + offset, Brick_Rect.top - 1 * scale);

    AsConfig::Pink_Highlight_Unbreakable.Select_Pen(hdc);
    MoveToEx(hdc, Brick_Rect.left + 6 * scale + offset, Brick_Rect.bottom + scale, 0);
    LineTo(hdc, Brick_Rect.left + 15 * scale - 1 + offset, Brick_Rect.top - 1 * scale);

    SelectClipRgn(hdc, 0);
}


//
bool AActive_Brick_Unbreakable::Is_Finished()
{
    if (Animation_Step >= MAX_ANIMATION_STEP - 1)
        return true;
    else
        return false;
}


//
void AActive_Brick_Unbreakable::Draw_In_Level(HDC hdc, RECT brick_rect)
{// Вывод неактивного кирпича на уровень

    AsConfig::White_Color.Select(hdc);
    AsConfig::Round_Rect(hdc, brick_rect);
}



// AActive_Brick_Multihit
//
AActive_Brick_Multihit::AActive_Brick_Multihit(int level_x, int level_y)
    :AActive_Brick(EBT_Multihit_1, level_x, level_y), Rotation_Step(0)
{

}


//
AActive_Brick_Multihit::~AActive_Brick_Multihit()
{
    //DeleteObject(Region);
}


//
void AActive_Brick_Multihit::Act()
{
    if (Rotation_Step <= MAX_ROTATION_STEP)
    {
        Rotation_Step++;
        InvalidateRect(AsConfig::HWnd, &Brick_Rect, FALSE);
    }
}


//
void AActive_Brick_Multihit::Draw(HDC hdc, RECT& paint_area)
{
    int scale = AsConfig::GLOBAL_SCALE;
    int step;
    double rotation_angle, x_ratio;
    RECT zero_rect;
    XFORM xForm, old_xForm;

    // 1. Очищаем фон
    AsConfig::BG_Color.Select(hdc);
    AsConfig::Round_Rect(hdc, Brick_Rect);

    // 2. Настраиваем матрицу "поворота 100"
    step = Rotation_Step % Steps_Per_Turn;
    rotation_angle = M_PI_4 / 2.0 * (double)step;
    x_ratio = cos(rotation_angle);

    xForm.eM11 = (float)cos(rotation_angle);
    xForm.eM12 = 0.0;
    xForm.eM21 = 0.0;
    xForm.eM22 = 1.0;
    xForm.eDx = (float)Brick_Rect.left + (float)(1.0 - x_ratio) * (float)(AsConfig::BRICK_WIDTH * AsConfig::GLOBAL_SCALE) / 2.0f;
    xForm.eDy = (float)Brick_Rect.top;

    GetWorldTransform(hdc, &old_xForm);
    SetWorldTransform(hdc, &xForm);


    // 3. Рисуем "100"
    AsConfig::Letter_Color.Select_Pen(hdc);

    MoveToEx(hdc, 0 + 1 * scale + 1, 0 + 3 * scale, 0);
    LineTo(hdc, 0 + 3 * scale + 1, 0 + 1 * scale);
    LineTo(hdc, 0 + 3 * scale + 1, 0 + 6 * scale - 1);

    zero_rect.left = 0 + 5 * scale + 1;
    zero_rect.top = 0 + 1 * scale;
    zero_rect.right = zero_rect.left + 3 * scale + 1;
    zero_rect.bottom = zero_rect.top + 5 *scale;
    AsConfig::Round_Rect(hdc, zero_rect);


    zero_rect.left += 5 * scale;
    zero_rect.right += 5 * scale;
    AsConfig::Round_Rect(hdc, zero_rect);

    SetWorldTransform(hdc, &old_xForm);
}


//
bool AActive_Brick_Multihit::Is_Finished()
{
    if (Rotation_Step >= MAX_ROTATION_STEP)
        return true;
    else
        return false;
}


//
void AActive_Brick_Multihit::Draw_In_Level(HDC hdc, RECT brick_rect, EBrick_Type brick_type)
{// Вывод неактивного кирпича на уровень

    int scale = AsConfig::GLOBAL_SCALE;

    // 1. Рисуем фон
    AsConfig::White_Color.Select(hdc);
    AsConfig::Round_Rect(hdc, brick_rect);

    AsConfig::Pink_Color.Select(hdc);
    Rectangle(hdc, brick_rect.left + scale, brick_rect.top + scale, 
        brick_rect.right - scale - 1, brick_rect.bottom - scale - 1);

    // 2. Рисуем внутренние прямоугольники
    switch (brick_type)
    {
    case EBT_Multihit_1:
        Draw_Stage(hdc, brick_rect, 2, 10);
        break;

    case EBT_Multihit_2:
        Draw_Stage(hdc, brick_rect, 2, 4);
        Draw_Stage(hdc, brick_rect, 8, 4);
        break;

    case EBT_Multihit_3:
        Draw_Stage(hdc, brick_rect, 2, 2);
        Draw_Stage(hdc, brick_rect, 6, 2);
        Draw_Stage(hdc, brick_rect, 10, 2);
        break;

    case EBT_Multihit_4:
        Draw_Stage(hdc, brick_rect, 2, 2);
        Draw_Stage(hdc, brick_rect, 5, 2);
        Draw_Stage(hdc, brick_rect, 8, 2);
        Draw_Stage(hdc, brick_rect, 11, 2);
        break;

    default:
        AsConfig::Throw();
        break;
    }
}


//
void AActive_Brick_Multihit::Draw_Stage(HDC hdc, RECT brick_rect, int x, int width)
{// Рисуем внутренний прямоугольник

    int scale = AsConfig::GLOBAL_SCALE;
    RECT stage_rect;

    stage_rect.left = brick_rect.left + x * scale;
    stage_rect.top = brick_rect.top + 2 * scale;
    stage_rect.right = stage_rect.left + width * scale;
    stage_rect.bottom = stage_rect.top + 3 * scale;

    AsConfig::BG_Color.Select(hdc);
    Rectangle(hdc, stage_rect.left + scale, stage_rect.top + scale, stage_rect.right + scale - 1, stage_rect.bottom + scale - 1);

    AsConfig::Blue_Color.Select(hdc);
    Rectangle(hdc, stage_rect.left, stage_rect.top, stage_rect.right - 1, stage_rect.bottom - 1);

}



// AActive_Brick_Teleport
//
AActive_Brick_Teleport::AActive_Brick_Teleport(int level_x, int level_y, ABall *ball, AActive_Brick_Teleport *destination_teleport)
    :AActive_Brick(EBT_Teleport, level_x, level_y), Animation_Step(0), Ball(0), Destination_Teleport(destination_teleport),
    Teleport_State(ETS_Starting)
{
    Set_Ball(ball);
}


//
AActive_Brick_Teleport::~AActive_Brick_Teleport()
{
    //DeleteObject(Region);
}


//
void AActive_Brick_Teleport::Act()
{
    double ball_x, ball_y;
    double direction;

    //if (AsConfig::Current_Timer_Tick % 5 != 0)
    //    return; 

    if (Animation_Step <= Max_Animation_Step)
    {
        Animation_Step++;
        InvalidateRect(AsConfig::HWnd, &Brick_Rect, FALSE);
    }
    else
    {
        switch (Teleport_State)
        {
        case ETS_Starting:
            Animation_Step = 0;
            Teleport_State = ETS_Finishing;
            if (Destination_Teleport != nullptr)
            {
                Destination_Teleport->Set_Ball(Ball);
                Ball = nullptr;
            }
            break;

        case ETS_Finishing:
            Teleport_State = ETS_Done;

            if (Ball != nullptr)
            {
                switch (Relese_Direction)
                {
                case EDT_Left:
                    ball_x = Get_Brick_X_Pos(false) - ABall::RADIUS;
                    ball_y = Get_Brick_Y_Pos(true);
                    break;

                case EDT_Up:
                    ball_x = Get_Brick_X_Pos(true);
                    ball_y = Get_Brick_Y_Pos(false) - ABall::RADIUS;
                    break;

                case EDT_Right:
                    ball_x = Get_Brick_X_Pos(false) + (double)AsConfig::BRICK_WIDTH + ABall::RADIUS;
                    ball_y = Get_Brick_Y_Pos(true);
                    break;

                case EDT_Down:
                    ball_x = Get_Brick_X_Pos(true);
                    ball_y = Get_Brick_Y_Pos(false) + (double)AsConfig::BRICK_HEIGHT + ABall::RADIUS;
                    break;

                default:
                    AsConfig::Throw();
                    break;
                }

                direction = Ball->Get_Direction();
                Ball->Set_State(EBS_Normal, ball_x, ball_y);
                Ball->Set_Direction(direction);

                Ball = nullptr; // Отмечаем мячик как отсутствующий в телепорте
                InvalidateRect(AsConfig::HWnd, &Brick_Rect, FALSE);
            }
            break;
        }
    }
}


//
void AActive_Brick_Teleport::Draw(HDC hdc, RECT& paint_area)
{
    int step;


    switch (Teleport_State)
    {
    case ETS_Starting:
        step = Animation_Step;
        break;

    case ETS_Finishing:
        step = Max_Animation_Step - Animation_Step;
        break;

    default:
        step = 0;
    }

    Draw_In_Level(hdc, Brick_Rect, step);


    if(Ball != nullptr)
        Ball->Draw_Teleporting(hdc, step);
}


//
bool AActive_Brick_Teleport::Is_Finished()
{
    if(Teleport_State == ETS_Done)
        return true;
    else
        return false;
}


//
void AActive_Brick_Teleport::Draw_In_Level(HDC hdc, RECT brick_rect, int step)
{// Вывод неактивного кирпича на уровень

    const int scale = AsConfig::GLOBAL_SCALE;
    int left_x = brick_rect.left + 3 * scale + 1;
    int top_y = brick_rect.top + step / 2 + 1;
    int right_x = brick_rect.left + 11 * scale + 1;
    int bottom_y = brick_rect.top + 6 * scale - step / 2 + 1;

    // 1. Фон
    AsConfig::Pink_Color.Select(hdc);
    AsConfig::Round_Rect(hdc, brick_rect);

    // 2. Тень
    AsConfig::BG_Color.Select(hdc);
    Ellipse(hdc, brick_rect.left + AsConfig::BRICK_WIDTH / 2 * scale, top_y, right_x + scale, bottom_y);

    // 2. Портал
    AsConfig::Teleport_Portal_Color.Select(hdc);
    Ellipse(hdc, left_x, top_y, right_x, bottom_y);
}


//
void AActive_Brick_Teleport::Set_Ball(ABall* ball)
{
    double ball_x, ball_y;

    // Ставим мячик в центр кирпича
    ball_x = Get_Brick_X_Pos(true);
    ball_y = Get_Brick_Y_Pos(true);

    if (ball != nullptr)
    {
        ball->Set_State(EBS_Teleporting, ball_x, ball_y);
    }
    Ball = ball;
    
}



// AAdvertisement
//
AAdvertisement::AAdvertisement(int level_x, int level_y, int width, int height)
    :Level_X(level_x), Level_Y(level_y), Width(width), Height(height), Brick_Regions(nullptr), Empt_Region(0), 
    Ball_X(0), Ball_Y(0), Ball_Width(Ball_Size * AsConfig::GLOBAL_SCALE), Ball_Height(Ball_Size * AsConfig::GLOBAL_SCALE),
    Ball_Y_Offset(0), Falling_Speed(0), Acceleration_Step(0.2), Deformation_Ratio(0)
{
    const int scale = AsConfig::GLOBAL_SCALE;

    Empt_Region = CreateRectRgn(0, 0, 0, 0);

    Brick_Regions = new HRGN[Width * Height];
    memset(Brick_Regions, 0, sizeof(HRGN) * Width * Height);

    Ad_Rect.left = (AsConfig::LEVEL_X_OFFSET + Level_X * AsConfig::CELL_WIDTH) * scale;
    Ad_Rect.top = (AsConfig::LEVEL_Y_OFFSET + Level_Y * AsConfig::CELL_HEIGHT) * scale;
    Ad_Rect.right = Ad_Rect.left + ((width - 1) * AsConfig::CELL_WIDTH + AsConfig::BRICK_WIDTH) * scale;
    Ad_Rect.bottom = Ad_Rect.top + ((height - 1) * AsConfig::CELL_HEIGHT + AsConfig::BRICK_HEIGHT) * scale;

    Ball_X = Ad_Rect.left + 9 * scale + Ball_Width / 2;
    Ball_Y = Ad_Rect.top + 2 * scale + Ball_Height / 2;


    //for (int i = 0; i < Height; i++)
    //    for (int j = 0; j < Width; j++)
    //        Show_Under_Brick(Level_X + j, Level_Y + i);
}


//
AAdvertisement::~AAdvertisement()
{
    HRGN region;

    for (int i = 0; i < Height; i++)
        for (int j = 0; j < Width; j++)
        {
            region = Brick_Regions[i * Width + j];
            if (region != 0)
                DeleteObject(region);
        }

    delete[] Brick_Regions;
}


//
void AAdvertisement::Clear(HDC hdc, RECT& paint_area)
{

}


//
void AAdvertisement::Act()
{
    int cell_width = AsConfig::CELL_WIDTH * AsConfig::GLOBAL_SCALE;
    int cell_height = AsConfig::CELL_HEIGHT * AsConfig::GLOBAL_SCALE;
    RECT rect;

    //if (AsConfig::Current_Timer_Tick % 3 != 0)
    //    return;

    // 1. Заказываем перерисовкуфрагментов, надкоторыми пропали кирпичи
    for (int i = 0; i < Height; i++)
        for (int j = 0; j < Width; j++)
            if (Brick_Regions[i * Width + j] != 0)
            {
                rect.left = Ad_Rect.left + j * cell_width;
                rect.top = Ad_Rect.top + i * cell_height;
                rect.right = rect.left + cell_width;
                rect.bottom = rect.top + cell_height;

                InvalidateRect(AsConfig::HWnd, &rect, FALSE);
            }

    // 2. Смещаем шарик
    Falling_Speed += Acceleration_Step;
    Ball_Y_Offset = High_Ball_Threshold - (int)(Falling_Speed * Falling_Speed);

    if (Ball_Y_Offset <= Low_Ball_Threshold + Deformation_Height)
        Deformation_Ratio = (double)(Ball_Y_Offset - Low_Ball_Threshold) / (double)Deformation_Height;
    else
        Deformation_Ratio = 1.0;

    if (Ball_Y_Offset > High_Ball_Threshold || Ball_Y_Offset < Low_Ball_Threshold)
        Acceleration_Step = -Acceleration_Step;
}


//
void AAdvertisement::Draw(HDC hdc, RECT& paint_area)
{
    const int scale = AsConfig::GLOBAL_SCALE;
    int x, y;
    int circle_size = Ball_Size * scale;
    int deformation;
    int ball_width, ball_height;
    int shadow_width, shadow_height;
    HRGN region;
    RECT intersection_rect;
    POINT table_points[4] =
    {
        {Ad_Rect.left + 1, Ad_Rect.top + 15 * scale},
        {Ad_Rect.left + 15 * scale + 1, Ad_Rect.top + 10 * scale},
        {Ad_Rect.left + 30 * scale + 1, Ad_Rect.top + 15 * scale},
        {Ad_Rect.left + 15 * scale + 1, Ad_Rect.top + 20 * scale}
    };


    if (!IntersectRect(&intersection_rect, &paint_area, &Ad_Rect))
        return;

    SelectClipRgn(hdc, Empt_Region);

    for (int i = 0; i < Height; i++)
        for (int j = 0; j < Width; j++)
        {
            region = Brick_Regions[i * Width + j];
            if (region != 0)
                ExtSelectClipRgn(hdc, region, RGN_OR); // Выбираем регион            
        }

    // 1. Рамкой стираем предыдущее изображение
    // 1.1 Тонкая синяя рамка со скругленными углами
    AsConfig::BG_Color.Select(hdc);
    AsConfig::Blue_Color.Select_Pen(hdc);
    AsConfig::Round_Rect(hdc, Ad_Rect);

    // 2. Стол
    // 2.1 Белая поверхность

    AsConfig::White_Color.Select(hdc);
    Polygon(hdc, table_points, 4);

    // 3. Тень под шариком
    // 3.1 Синий элипс размер 8x6, пока шарик полностью над "столом"
    
    AsConfig::Blue_Color.Select(hdc);

    //Ellipse(hdc, Ad_Rect.left + 11 * scale + 1, Ad_Rect.top + 14 * scale,
    //    Ad_Rect.left + 20 * scale, Ad_Rect.top + 18 * scale - 1);

    shadow_width = Ball_Width - 4 * scale;
    shadow_height = 4 * scale;

    deformation = (int)((1.0 - Deformation_Ratio) * (double)scale * 3.0);

    ball_width = shadow_width + deformation;
    ball_height = shadow_height - deformation;

    x = Ball_X - ball_width / 2;
    y = Ball_Y - ball_height / 2 + Ball_Y_Offset / 6 + 8 * scale;

    Ellipse(hdc, x, y, x + ball_width, y + ball_height);


    // 3.2 Уезжает вниз, когда шарик в верхней точке
    // 3.3 Увеличивается, когда шарик плющится

    // 4. Борта
    // 4.2 Синяя кайма толщиной в 1 игровой пиксель
    
    AsConfig::Advertisement_Blue_Table.Select(hdc);

    MoveToEx(hdc, Ad_Rect.left + scale - 1, Ad_Rect.top + 15 * scale, 0);
    LineTo(hdc, Ad_Rect.left + 15 * scale + 1, Ad_Rect.top + 10 * scale);
    LineTo(hdc, Ad_Rect.left + 30 * scale, Ad_Rect.top + 15 * scale);
    LineTo(hdc, Ad_Rect.left + 15 * scale + 1, Ad_Rect.top + 20 * scale);
    LineTo(hdc, Ad_Rect.left + scale - 1, Ad_Rect.top + 15 * scale);


    // 4.3 Красный борт толщиной в 1 игровой пиксель

    AsConfig::Advertisement_Pink_Table.Select(hdc);
    MoveToEx(hdc, Ad_Rect.left + scale, Ad_Rect.top + 16 * scale, 0);
    LineTo(hdc, Ad_Rect.left + 15 * scale + 1, Ad_Rect.top + 21 * scale);
    LineTo(hdc, Ad_Rect.left + 30 * scale - 1, Ad_Rect.top + 16 * scale);

    // 5. Шарик
    // 5.1 Красный элипс 12x12

    ball_width = Ball_Width + deformation;
    ball_height = Ball_Height - deformation;

    x = Ball_X - ball_width / 2;
    y = Ball_Y - ball_height / 2 - Ball_Y_Offset;


    AsConfig::Pink_Color.Select(hdc);

    Ellipse(hdc, x, y, x + ball_width, y + ball_height);

    // 5.2 Блик сверху

    AsConfig::Letter_Color.Select(hdc);
    Arc(hdc, x + scale, y + scale, x + ball_width - scale, y + ball_height - scale,
        x + 4 * scale, y + scale, x + scale, y + 4 * scale);

    // 5.3 Летает вверх/вниз (по затухающей траектории)
    // 5.4 Сплющивается внизу до 16x9
    
    

    SelectClipRgn(hdc, 0);

}


//
bool AAdvertisement::Is_Finished()
{
    return false; // Реклама не заканчивается никогда
}


//
void AAdvertisement::Show_Under_Brick(int level_X, int level_Y)
{
    int x, y;
    int cell_width = AsConfig::CELL_WIDTH * AsConfig::GLOBAL_SCALE;
    int cell_height = AsConfig::CELL_HEIGHT * AsConfig::GLOBAL_SCALE;
    RECT rect;

    x = level_X - Level_X;
    y = level_Y - Level_Y;

    if (x < 0 || x >= Width)
        AsConfig::Throw();

    if (y < 0 || y >= Height)
        AsConfig::Throw();

    rect.left = Ad_Rect.left + x * cell_width;
    rect.top = Ad_Rect.top + y * cell_height;
    rect.right = rect.left + cell_width;
    rect.bottom = rect.top + cell_height;

    Brick_Regions[y * Width + x] = CreateRectRgnIndirect(&rect);
}


//
bool AAdvertisement::Has_Brick_At(int level_x, int level_y)
{
    if (level_x >= Level_X && level_x <= Level_X + Width)
        if (level_y >= Level_Y && level_y <= Level_Y + Height)
            return true;

    return false; 
}



// AActive_Brick_Ad
//
AActive_Brick_Ad::AActive_Brick_Ad(int level_x, int level_y, AAdvertisement * advertisement)
    :AActive_Brick(EBT_Unbreakable, level_x, level_y), Advertisement(advertisement)
{
    if(Advertisement != nullptr)
        Advertisement->Show_Under_Brick(Level_X, Level_Y);
}


//
AActive_Brick_Ad::~AActive_Brick_Ad()
{
}


//
void AActive_Brick_Ad::Act()
{
    //if (Animation_Step <= MAX_ANIMATION_STEP)
    //{
    //    Animation_Step++;
    InvalidateRect(AsConfig::HWnd, &Brick_Rect, FALSE);
    //}
}


//
void AActive_Brick_Ad::Draw(HDC hdc, RECT& paint_area)
{

}


//
bool AActive_Brick_Ad::Is_Finished()
{
    //if (Animation_Step >= MAX_ANIMATION_STEP - 1)
    //    return true;
    //else
        return false;
}


//
void AActive_Brick_Ad::Draw_In_Level(HDC hdc, RECT brick_rect)
{
    const int scale = AsConfig::GLOBAL_SCALE;
    int x = brick_rect.left;
    int y = brick_rect.top;
    int size = (Circle_Size - 1) * scale - 1;

    // 1. Стираем предыдущее изображение
    AsConfig::BG_Color.Select(hdc);
    Rectangle(hdc, brick_rect.left, brick_rect.top, brick_rect.right + scale - 1, brick_rect.bottom + scale - 1);

    // 2. Рисуем шарики
    for (int i = 0; i < 2; i++)
    {
        // 1. Рисуем шарик
        AsConfig::Pink_Color.Select(hdc);
        Ellipse(hdc, x, y, x + 7 * scale - 1, brick_rect.bottom - 1);

        // 2. Рисуем блик
        AsConfig::Letter_Color.Select(hdc);
        Arc(hdc, x + scale, y + scale, x + size, y + size, x + 2 * scale, y + scale, x + scale, y + 3 * scale);

        x += 8 * scale;
    }    
}