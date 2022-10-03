#pragma once
#include <SFML/Audio.hpp>

//объ€влени€ типов и вспомогательных структур
struct Options
{
    unsigned int sound_volume = 0;
    unsigned int music_volume = 0;
    unsigned int current_video_mode = 4;
    bool Videomode_4K_enable = false; //0
    bool Videomode_2K_enable = false; //1
    bool Videomode_FHD_enable = false; //2
    bool Videomode_1600_enable = false; //3
    bool Videomode_1280_enable = false; //4
    unsigned int display_Wigth;
    unsigned int display_Height;
    sf::Music *ptr_music_menu = NULL;
    sf::Music *ptr_music_game = NULL;
    sf::Music *ptr_music_mission = NULL;
    unsigned int language = 0;
};