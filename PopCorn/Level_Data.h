#pragma once

#include "Active_Brick.h"

//
class ALevel_Data
{
public:
    ALevel_Data(int level_number);

    char* Level;
    AAdvertisement* Advertisement;

    static const int Max_Level_Number = 50;

    int Get_Available_Bricks_Count();

private:

    static char Test_Level[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];

    static char Level_01[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_02[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_03[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_04[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_05[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_06[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_07[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_08[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_09[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_10[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_11[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_12[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_13[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_14[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_15[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_16[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_17[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_18[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_19[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_20[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_21[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_22[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_23[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_24[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_25[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_26[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_27[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_28[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_29[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_30[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_31[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_32[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_33[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_34[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_35[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_36[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_37[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_38[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_39[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_40[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_41[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_42[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_43[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_44[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_45[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_46[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_47[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_48[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_49[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];
    static char Level_50[AsConfig::LEVEL_HEIGHT][AsConfig::LEVEL_WIDTH];

    static char* Level_Array[Max_Level_Number];
};