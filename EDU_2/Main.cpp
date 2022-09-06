#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <windows.h>
#include <debugapi.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include "Game_engine.h"
#include "Menu_engine.h"


int main()
{
    //��������� ���� - � ������� ����
    enum class States { MAIN_MENU, STARTING_GAME, RUNNING_GAME, PAUSED, CHANGE_EQUIPMENT};
    States game_state = States::MAIN_MENU;
    States game_last_state; //�� ���������������

    //�������������� ����������� ���������
    int GAME_WINDOW_X_RES = 2560; //2560
    int GAME_WINDOW_Y_RES = 1440; //1440
    int GAME_RENDER_SCALE = 2;

    sf::RenderWindow main_window;
    int my_display_H = sf::VideoMode::getDesktopMode().height;
    int my_display_W = sf::VideoMode::getDesktopMode().width;
    main_window.create(sf::VideoMode(GAME_WINDOW_X_RES, GAME_WINDOW_Y_RES), L"������ ����", sf::Style::Titlebar | sf::Style::Close);
    main_window.setFramerateLimit(60);
    main_window.setPosition({ my_display_W / 2 - GAME_WINDOW_X_RES /2, my_display_H / 2 - GAME_WINDOW_Y_RES /2});
    main_window.setMouseCursorVisible(0);
    main_window.setKeyRepeatEnabled(0);
    main_window.setActive(true);
    
    //������������� �������� ������
    Game main_game (&main_window);
    //main_game.set_render_resolution(GAME_WINDOW_X_RES, GAME_WINDOW_Y_RES);  //�������
    //������������� ������ ����
    Menu main_menu (&main_window);

    //��������� ������
    sf::Clock clock;

    // �������� ���� ���������
    while (main_window.isOpen())
    {
        //����� �����
        sf::Time elapsed = clock.restart();
        // �������� �������
        sf::Event event;
        while (main_window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                { 
                OutputDebugString(L"������ ������ ��������\n");
                main_window.close();
                }
            if (event.type == sf::Event::LostFocus)
                {
                    OutputDebugString(L"������� ����� �������� ����. �����.\n");
                    if (game_state == States::RUNNING_GAME)
                    {
                        game_last_state = game_state;
                        game_state = States::PAUSED;
                    }
                }
            if (event.type == sf::Event::GainedFocus)
                {
                    OutputDebugString(L"������� �����\n");
                }
        }

        //�������� ���������� � ������� �����
        
        if (game_state == States::STARTING_GAME)
            {
                int start_result_code = main_game.start();
                //���� ��������: 0 - ������, 1 - �����
                main_game.set_render_scale(GAME_RENDER_SCALE);//������������� �������
                if (start_result_code) game_state = States::RUNNING_GAME;
                else main_window.close();
            }

        if (game_state == States::RUNNING_GAME)
            {
                int run_result_code = main_game.run(elapsed);
                //���� ��������: 0 - ����������, 1 - ��������� �� �����, 2 - ����� ������������
                if (run_result_code==1)
                {
                    game_last_state = game_state;
                    game_state = States::PAUSED;
                }

                if (run_result_code == 2)
                {
                    game_state = States::CHANGE_EQUIPMENT;
                }

            }

        if (game_state == States::CHANGE_EQUIPMENT)
        {
            int result_code = main_game.take_equipment();
            //���� ��������: 0 - ����������, 1 - ��������� � �������� ����� ����
            if (result_code == 1) 
            {
                game_state = States::RUNNING_GAME;
            }
        }


        if (game_state == States::PAUSED)
            {
                int pause_result_code = main_game.pause(elapsed);
                //���� ��������: 0 - ����������, 1 - ����� �����, 2 - ����� � ������� ����
                if (pause_result_code == 1)
                {
                    game_state = game_last_state;//��������������� ��������� ���� �� �����
                }
                if (pause_result_code == 2) game_state = States::MAIN_MENU;
            }
        
        if (game_state == States::MAIN_MENU)
        {
            int menu_result_code = main_menu.run();
            // ���� ��������: 0 - ������   1 - ����� ����   2 - �����

            if (menu_result_code == 1) game_state = States::STARTING_GAME;

            if (menu_result_code == 2) main_window.close();
        
        }
    }

    //���������� ���������
    return 0;  
}