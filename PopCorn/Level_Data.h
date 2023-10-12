#pragma once

#include "Active_Brick.h"

//
class ALevel_Data
{
public:
    ALevel_Data(int level_number);
    char* Level;
    AAdvertisement* Advertisement;

    static const int Max_Level_Number = 10;

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

    static char* Level_Array[Max_Level_Number];
};