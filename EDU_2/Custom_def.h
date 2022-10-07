#pragma once
#include <SFML/Audio.hpp>

//объявления типов и вспомогательных структур
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

class WeaponData
{
public:
    unsigned int sprite_N = 0;
    unsigned int type = 0;
    unsigned int color = 0;
    unsigned int level = 0;
    double fire_rate = 0.0;
    double power = 0.0;
    double damageArea = 0.0;
    bool hasSpecialEffect = false;

    int get_ammo_type()
    {
        return type;
    }

    int get_ammo_ttl()
    {
        return 4000;
    }

    int get_ammo_speed()
    {
        if (type == 0) return 2000; //пушка
        if (type == 1) return 2000; //пулемет
        if (type == 2) return 800; //ракета
        if (type == 3) return 1200; //плазма
        if (type == 4) return 1200;  //электро
        if (type == 5) return 1200; //холод

        return 2000; //микросекунды
    }

    int get_shot_sound()
    {
        if (type == 0) return 1;
        if (type == 1) return 2;
        if (type == 2) return 5;
        if (type == 3) return 4;
        if (type == 4) return 4;
        if (type == 5) return 4;

        return 3; //default
    }


};