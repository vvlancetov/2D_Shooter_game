#pragma once

#include <SFML/Graphics.hpp>
//#include <SFML/Window/Window.hpp>
//#include <SFML/Audio/Music.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <debugapi.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include "Custom_def.h"
#include "Game.h"
#include "Menu.h"


//объявления функций
void load_config(std::string game_path, Options* game_options);
void char_to_int(char* buffer, unsigned int* number);
void int_to_char(unsigned int* number, char* buffer);
void save_config(std::string game_path, Options* game_options);



//главная программа
int main(int argc, char* argv[])
{

    //вычисляем путь к текущему каталогу
    std::string game_path = argv[0];
    int l_symb = (int)game_path.find_last_of('\\');
    game_path.resize(++l_symb);

    //загружаем файл конфигурации
    Options game_options;
    load_config(game_path, &game_options);

    //состояние игры - ЗАГРУЗКА (сразу пытаемся загрузить сохранение)
    enum class States { IN_MENU, LOADING_GAME, RUNNING_GAME };
    States programm_state = States::LOADING_GAME;
    //States game_last_state; //не инициализирован


    //получаем данные о текущем дисплее
    int my_display_H = sf::VideoMode::getDesktopMode().height;
    int my_display_W = sf::VideoMode::getDesktopMode().width;
    game_options.display_Wigth = my_display_W;
    game_options.display_Height = my_display_H;

    //проверяем доступные видеорежимы и выставляем флаги
    if (my_display_W >= 1280 && my_display_H >= 720) { game_options.Videomode_1280_enable = true; printf("1280 ok\n"); }  //4
    if (my_display_W >= 1600 && my_display_H >= 900) { game_options.Videomode_1600_enable = true;  printf("1600 ok\n"); } //3
    if (my_display_W >= 1920 && my_display_H >= 1080) { game_options.Videomode_FHD_enable = true;  printf("FHD ok\n"); } //2
    if (my_display_W >= 2560 && my_display_H >= 1440) { game_options.Videomode_2K_enable = true;  printf("2K ok\n"); } //1
    if (my_display_W >= 3840 && my_display_H >= 2160) { game_options.Videomode_4K_enable = true;  printf("4K ok\n"); } //0

    if (game_options.current_video_mode == 0 && game_options.Videomode_4K_enable == false) game_options.current_video_mode = 1; //уменьшаем разрешение
    if (game_options.current_video_mode == 1 && game_options.Videomode_2K_enable == false) game_options.current_video_mode = 2;
    if (game_options.current_video_mode == 2 && game_options.Videomode_FHD_enable == false) game_options.current_video_mode = 3;
    if (game_options.current_video_mode == 3 && game_options.Videomode_1600_enable == false) game_options.current_video_mode = 4;
    
    //инициализируем графические константы
    int GAME_WINDOW_X_RES = 1280; //2560
    int GAME_WINDOW_Y_RES = 720; //1440

    if (game_options.current_video_mode == 0) { GAME_WINDOW_X_RES = 3840; GAME_WINDOW_Y_RES = 2160;}
    if (game_options.current_video_mode == 1) { GAME_WINDOW_X_RES = 2560; GAME_WINDOW_Y_RES = 1440; }
    if (game_options.current_video_mode == 2) { GAME_WINDOW_X_RES = 1920; GAME_WINDOW_Y_RES = 1080; }
    if (game_options.current_video_mode == 3) { GAME_WINDOW_X_RES = 1600; GAME_WINDOW_Y_RES = 900; }
    if (game_options.current_video_mode == 4) { GAME_WINDOW_X_RES = 1280; GAME_WINDOW_Y_RES = 720; }
    //добавить отступы для полноэеранных режимов

    int GAME_RENDER_SCALE = 2; // подобрать масштаб позже

    //создаем главное окно
    sf::RenderWindow main_window;
    main_window.create(sf::VideoMode(GAME_WINDOW_X_RES, GAME_WINDOW_Y_RES), L"Cool 2D game", sf::Style::Titlebar | sf::Style::Close);
    main_window.setPosition({ my_display_W / 2 - GAME_WINDOW_X_RES /2, my_display_H / 2 - GAME_WINDOW_Y_RES /2});
    main_window.setFramerateLimit(60);
    main_window.setMouseCursorVisible(0);
    main_window.setKeyRepeatEnabled(0);
    //main_window.setVerticalSyncEnabled(1);
    main_window.setActive(true);
    
    //Mission mission (&main_window, game_path);
    //mission.set_render_resolution(GAME_WINDOW_X_RES, GAME_WINDOW_Y_RES);  //удалить
    
    //создание объекта главного меню
    Menu main_menu (&main_window, game_path);
    main_menu.set_options_object(&game_options);
    //инициализация самой игры как объекта
    Game main_game(&main_window, game_path, &main_menu);
    main_game.set_options_object(&game_options);


    //запускаем таймер
    sf::Clock clock;

    //объект - музыка
    sf::Music music_menu;
    if (!music_menu.openFromFile(game_path + "Assets\\Music\\Menu music.ogg")) printf("Error load menu music\n");
    else music_menu.play();
    music_menu.setLoop(true);
    music_menu.setVolume(game_options.music_volume);
    game_options.ptr_music_menu = &music_menu;

    // основной цикл программы
    while (main_window.isOpen())
    {
        //время кадра
        sf::Time elapsed = clock.restart();
        
        // проверка событий
        sf::Event event;
        while (main_window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                { 
                //OutputDebugString(L"Нажата кнопка закрытия\n");
                printf("close main window \n");
                main_window.close();
                }
            if (event.type == sf::Event::LostFocus)
                {
                printf("lost focus \n");
                game_options.game_in_focus = false;
                    // можно поставить на паузу если пользователь переключил окна
                }
            if (event.type == sf::Event::GainedFocus)
                {
                    printf("get focus \n");
                    game_options.game_in_focus = true;
                    //выход из паузы при получении фокуса, к примеру
                    //OutputDebugString(L"Получен фокус\n");
                }

            if (event.type == sf::Event::Resized)
            {
                printf("resize \n");
                //изменяем параметры при изменении размеров
                int new_width = event.size.width;
                int new_heigth = event.size.height;
                if ((double)event.size.width / (double)event.size.height > 16.0 / 9.0) new_width = (double)new_heigth / 9.0 * 16.0;
                else new_heigth = (double)new_width / 16.0 * 9.0;
                main_window.create(sf::VideoMode(new_width, new_heigth), L"Крутая игра", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
                main_window.setFramerateLimit(60);
                main_window.setMouseCursorVisible(0);
                main_window.setKeyRepeatEnabled(0);
                //mission.set_render_resolution((int)new_width, (int)new_heigth);
                GAME_WINDOW_X_RES = (int)new_width; 
                GAME_WINDOW_Y_RES = (int)new_heigth; 
            }
        }

        //передача управления в главные ветки

        if (programm_state == States::IN_MENU)
        {
            //printf("Main -> IN_MENU\n");
            int result = main_menu.run(false, false);
            
            if (result) printf("main_menu.run() returned %i\n", result);

            if (result == 1)
            {
                //продолжить текущую игру (она должна быть загружена)
                if (main_game.check_game())
                {
                    printf("Main -> Continue game...\n");
                    music_menu.stop();
                    programm_state = States::RUNNING_GAME;
                }
            }

            if (result == 2)
            {
                //начать новую игру
                music_menu.stop();
                int start_result_code = main_game.Reset(); //обнуляем данные игры
                if (start_result_code) programm_state = States::RUNNING_GAME;
                else printf("Main -> Reset error!\n");
            }

            if (result == 3)
            {
                //выйти из игры
                music_menu.stop();
                printf("Main -> Exit...\n");
                main_window.close();
            }

            if (result == 4)
            {
                //выйти из игры
                //music_menu.stop();
                printf("Main -> Confirm new game ...\n");
                programm_state = States::IN_MENU;
            }

        }

        if (programm_state == States::LOADING_GAME)
        {
            //пробуем загрузить игру из сохранения при запуске программы
            
            if (main_game.Load())
            {
                printf("Main -> loading game - OK\n");
            }
            else
            {
                //если загрузка неудачна
                printf("Main -> loading error \n");
            }
            programm_state = States::IN_MENU;
        }

        if (programm_state == States::RUNNING_GAME)
            {
                //printf("Main -> RUNNING_GAME\n");
                int run_result_code = main_game.Run(elapsed);
                //Коды возврата: 0 - продолжать, 1 - выход в меню  
                //DEL поставить на паузу, 2 - взять оборудование, 3 - эвакуация с уровня, 4 - заход на базу, 5 - смерть игрока
                if (run_result_code == 1)
                {
                    //game_last_state = game_state;
                    music_menu.play();
                    programm_state = States::IN_MENU;
                    printf("Main -> State::IN_MENU\n");
                }
            }
}

    //завершение программы
    //сохраняем конфигурацию в файл
    save_config(game_path, &game_options);
    return 0;  
}

void load_config(std::string game_path, Options *game_options)
{
    std::ifstream fd;
    fd.open(game_path + "\\config.dat", std::ios_base::in | std::ios_base::binary);
    if (fd) 
        {
            printf("Game -> Load config\n");
            char buffer[8] = { 0 };
            unsigned int i;
            if (fd.read(buffer, 8))
            {
                //считываем громкость звука
                char_to_int(buffer, &i);
                if (i > 100)i = 100;
                printf("Sound %i\n", i);
                game_options->sound_volume = i;
            }

            //считываем громкость музыки
            if(fd.read(buffer, 8))
            {
                char_to_int(buffer, &i);
                if (i > 100)i = 100;
                printf("Sound %i\n", i);
                game_options->music_volume = i;
            }

            //считываем видеорежим
            if (fd.read(buffer, 8))
            {
                char_to_int(buffer, &i);
                if (i > 4)i = 4;
                printf("Videomode %i\n", i);
                game_options->current_video_mode = i;
            }

        }
    else
        {
            //если не удалось открыть файл
            game_options->sound_volume = 50;
            game_options->music_volume = 50;
            game_options->current_video_mode = 4;
            printf("load config fail\n");
        }
    fd.close();
}

void save_config(std::string game_path, Options *game_options)
{
    printf("Game -> save config\n");
    std::ofstream fd;
    char buffer[8] = { 0 };
    fd.open(game_path + "\\config.dat", std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
    //printf("size = %i\n", sizeof(resource_money));
    if (fd)
    {
        unsigned int i;
        i = game_options->sound_volume;
        int_to_char(&i, &buffer[0]);
        fd.write(buffer, 8);
        i = game_options->music_volume;
        int_to_char(&i, &buffer[0]);
        fd.write(buffer, 8);
        i = game_options->current_video_mode;
        int_to_char(&i, &buffer[0]);
        fd.write(buffer, 8);
        fd.close();
    }
    else
    {
        printf("save config fail\n");
    }
}

void char_to_int(char* buffer, unsigned int* number)
{
    unsigned int n = 0;
    //преобразуем 8-байтный массив в число

    //считываем каждый байт отдельно, но не более кол-ва байт в беззнаковом целом
    for (int i = 0; i < sizeof(unsigned int); ++i)
    {
        //проходим по битам и считываем их
        int tmp = buffer[i];
        if (tmp < 0) tmp += 256;
        n += tmp * std::pow(256, i);
        //printf("%i ", tmp);
    }
    //printf("n = %i\n", n);
    *number = n;
}

void int_to_char(unsigned int* number, char* buffer)
{
    unsigned int n = *number;
    //преобразуем число в 8-байтный массив
    //для начала заполним массив нулями
    for (int i = 0; i < 8; ++i)
    {
        buffer[i] = 0;
    }
    //записываем каждый байт отдельно, но не более кол-ва байт в беззнаковом целом

    for (int i = 0; i < sizeof(unsigned int); ++i)
    {
        //проходим по битам и записываем в буфер
        buffer[i] = n >> (i * 8);
        //printf("%i = %c \n", (n >> (i*8)) & 256, buffer[i]);
    }
}