#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <debugapi.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <cmath>
#include "Custom_def.h"
//#include "Mission.h"
//#include "Menu.h"
#include "Game.h"


Game::Game(sf::RenderWindow* window, std::string path, Menu *menu_object)
{
    game_window = window;
    game_path = path;
    game_menu = menu_object;
    
    //набор данных для рендеринга окна
    render_res_X = floor(game_window->getView().getSize().x);
    render_res_Y = floor(game_window->getView().getSize().y);

    //состояние игры - на орбите
     game_state = GameStates::ORBIT;
    //States game_last_state; //не инициализирован

    //открываем файл с музыкой
    if (!music_orbit.openFromFile(game_path + "Assets\\Music\\Orbit music.ogg")) printf("Error load music\n");
    music_orbit.setLoop(true);
    
     //картинка на орбитальной базе
     bg_Planet_Texture.loadFromFile(game_path + "Assets\\Pictures\\Planet_1.png");
     bg_Planet_Texture.setSmooth(1);
     bg_Planet_Sprite.setTexture(bg_Planet_Texture);

     bg_SpaceShip_Texture.loadFromFile(game_path + "Assets\\Pictures\\Orbital_base_1.png");
     bg_SpaceShip_Texture.setSmooth(1);
     bg_SpaceShip_Sprite.setTexture(bg_SpaceShip_Texture);

     Rover_Texture.loadFromFile(game_path + "Assets\\Pictures\\Rover_1.png");
     Rover_Texture.setSmooth(1);
     Rover_Sprite.setTexture(Rover_Texture);
     
     Interface_Texture.loadFromFile(game_path + "Assets\\Pictures\\Orbital_Interface.png");
     Interface_Texture.setSmooth(1);
     Interface_Sprite.setTexture(Interface_Texture);
     
     mouse_pointer_Texture.loadFromFile(game_path + "Assets\\Pointers\\Mouse_pointer_1.png");
     mouse_pointer_Texture.setSmooth(1);
     mouse_pointer_Sprite.setTexture(mouse_pointer_Texture);
     
     bg_mission_launch_Texture.loadFromFile(game_path + "Assets\\Pictures\\Starting_Mission_Background.png");
     bg_mission_launch_Texture.setSmooth(1);
     bg_mission_launch_Sprite.setTexture(bg_mission_launch_Texture);

     suit_Texture.loadFromFile(game_path + "Assets\\Pictures\\Suit.png");
     suit_Texture.setSmooth(1);
     suit_Sprite.setTexture(suit_Texture);

     Dialog_Base_Texture.loadFromFile(game_path + "Assets\\Dialogs\\Base_dialog.png");
     Dialog_Base_Texture.setSmooth(1);
     Dialog_Base_Sprite.setTexture(Dialog_Base_Texture);

     Dialog_Ship_Texture.loadFromFile(game_path + "Assets\\Dialogs\\Ship_dialog.png");
     Dialog_Ship_Texture.setSmooth(1);
     Dialog_Ship_Sprite.setTexture(Dialog_Ship_Texture);

     Dialog_Fly_Texture.loadFromFile(game_path + "Assets\\Dialogs\\Dialog_fly.png");
     Dialog_Fly_Texture.setSmooth(1);
     Dialog_Fly_Sprite.setTexture(Dialog_Fly_Texture);

     Dialog_Rover_Texture.loadFromFile(game_path + "Assets\\Dialogs\\Dialog_rover_up.png");
     Dialog_Rover_Texture.setSmooth(1);
     Dialog_Rover_Sprite.setTexture(Dialog_Rover_Texture);

     Dialog_Suit_Texture.loadFromFile(game_path + "Assets\\Dialogs\\Dialog_suit.png");
     Dialog_Suit_Texture.setSmooth(1);
     Dialog_Suit_Sprite.setTexture(Dialog_Suit_Texture);

     Planet_Jump_Texture.loadFromFile(game_path + "Assets\\Pictures\\Jump.png");
     Planet_Jump_Texture.setSmooth(1);
     Planet_Jump_Sprite.setTexture(Planet_Jump_Texture);

     Dialog_weapon_Texture.loadFromFile(game_path + "Assets\\Dialogs\\Dialog_weapon.png");
     Dialog_weapon_Texture.setSmooth(1);
     Dialog_weapon_Sprite.setTexture(Dialog_weapon_Texture);

     Dialog_Upgrade_Texture.loadFromFile(game_path + "Assets\\Dialogs\\Dialog_upgrade.png");
     Dialog_Upgrade_Texture.setSmooth(1);
     Dialog_Upgrade_Sprite.setTexture(Dialog_Upgrade_Texture);

     Dialog_Upgrade_Text_Texture.loadFromFile(game_path + "Assets\\Dialogs\\Upgrade_Text.png");
     Dialog_Upgrade_Text_Texture.setSmooth(1);
     Dialog_Upgrade_Text_Sprite.setTexture(Dialog_Upgrade_Text_Texture);

     weapon_tile_Texture.loadFromFile(game_path + "Assets\\Textures\\Weapon_tiles.png");
     weapon_tile_Texture.setSmooth(1);
     weapon_tile_Sprite.setTexture(weapon_tile_Texture);

     Dialog_Death_Texture.loadFromFile(game_path + "Assets\\Dialogs\\Death_dialog.png");
     Dialog_Death_Texture.setSmooth(1);
     Dialog_Death_Sprite.setTexture(Dialog_Death_Texture);

     Dialog_Help1_Texture.loadFromFile(game_path + "Assets\\Dialogs\\Dialog_Help_1.png");
     Dialog_Help1_Texture.setSmooth(1);
     Dialog_Help1_Sprite.setTexture(Dialog_Help1_Texture);

     Dialog_Help2_Texture.loadFromFile(game_path + "Assets\\Dialogs\\Dialog_Help_2.png");
     Dialog_Help2_Texture.setSmooth(1);
     Dialog_Help2_Sprite.setTexture(Dialog_Help2_Texture);

     //загружаем шрифт
     if (!font.loadFromFile(game_path + "Assets\\Fonts\\arialnb.ttf")) printf("font load error\n");
     text.setFont(font); // font is a sf::Font

     click_block = true;
}

int Game::Run(sf::Time elapsed)
{
    //printf("Run_Game\n");
    //sf::sleep(sf::milliseconds(100));

    //прошло времени с микросекундах
    elapsed_ms = elapsed.asMilliseconds();

    //запрашиваем размеры окна
    sf::Vector2u new_win_size = game_window->getSize();
    double kx = new_win_size.x / 1920.0;
    double ky = new_win_size.y / 1080.0;

    // get the local mouse position (relative to a window)
    sf::Vector2i CursorPosition = sf::Mouse::getPosition(*game_window);
    
    if (game_state == GameStates::ORBIT)
    {

        //рисуем интерфейс
        draw_orbit_interface(kx, ky);

        //запускаем музыку если она не играет
        if (music_orbit.getStatus() != sf::SoundSource::Status::Playing)
        {
            printf("Game -> Play music\n");
            music_orbit.play();
            music_orbit.setVolume(game_options->music_volume);
        }

        if (show_Help_1) { game_state = GameStates::ORBIT_HELP_1; return 0; } //если окно подсказок еще не отображалось - переходим на вспомогательную ветку

        //задаем координаты областей
        int Exit_area[4] = { int(19.0 * ky), int(75.0 * ky), int(21.0 * kx), int(195.0 * kx) };//top,bottom,left,right
        int Start_mission_area[4] = { int(548.0 * ky), int(648.0 * ky), int(139.0 * kx), int(345.0 * kx) };//top,bottom,left,right
        int Rover_area[4] = { int(562.0 * ky), int(632.0 * ky), int(1669.0 * kx), int(1832.0 * kx) };//top,bottom,left,right
        int Fly_area[4] = { int(222.0 * ky), int(319.0 * ky), int(956.0 * kx), int(1239.0 * kx) };//top,bottom,left,right
        int Suit_area[4] = { int(1008.0 * ky), int(1072.0 * ky), int(182.0 * kx), int(348.0 * kx) };//top,bottom,left,right

        //проверяем нахождение мыши над пунктами меню
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Exit_area[0])) draw_box(&Exit_area[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Start_mission_area[0])) draw_box(&Start_mission_area[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Rover_area[0])) draw_box(&Rover_area[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Fly_area[0])) draw_box(&Fly_area[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Suit_area[0])) draw_box(&Suit_area[0], sf::Color(0, 0, 0, 128));

        //рисуем курсор
        mouse_pointer_Sprite.setPosition((float)CursorPosition.x, (float)CursorPosition.y);
        game_window->draw(mouse_pointer_Sprite);

        //финальная отрисовка окна
        game_window->display();

        //проверяем нажатия клавишь
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click_block)
        {
            // проверяем нажатие на кнопки
            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Rover_area[0]))
            {
                //меню улучшений
                click_block = true;
                game_state = GameStates::ROVER_ORBIT;
                
                //делаем скриншот
                sf::Vector2u windowSize = game_window->getSize();
                Screenshot_Texture.create(windowSize.x, windowSize.y);
                Screenshot_Texture.update(*game_window);
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Start_mission_area[0]))
            {
                printf("Game -> Start MISSION\n");  //новая высадка
                music_orbit.stop();
                bg_mission_launch_Sprite.setScale(kx, ky);
                game_window->draw(bg_mission_launch_Sprite);
                game_window->display();
                //создаем объект миссии
                current_mission = new Mission(game_window, game_path);
                current_mission->set_options_object(game_options, planet_type, planet_level, &suit_upgrades[0], &rover_upgrades[0]);
                current_mission->start();
                click_block = true;
                game_state = GameStates::MISSION;
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Fly_area[0]))
            {
                //меню улучшений
                click_block = true;
                game_state = GameStates::FLY_AWAY_ORBIT;
                //делаем скриншот
                sf::Vector2u windowSize = game_window->getSize();
                Screenshot_Texture.create(windowSize.x, windowSize.y);
                Screenshot_Texture.update(*game_window);
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Suit_area[0]))
            {
                //меню улучшений
                click_block = true;
                game_state = GameStates::SUIT_ORBIT;
                //делаем скриншот
                sf::Vector2u windowSize = game_window->getSize();
                Screenshot_Texture.create(windowSize.x, windowSize.y);
                Screenshot_Texture.update(*game_window);
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Exit_area[0]))
            {
                printf("Game -> Exit\n");  //выход в главное меню
                printf("Game -> Save(code %i)", Save());
                click_block = true;
                music_orbit.stop();
                return(1);
            }
        }

        //обработка нажатия ESC
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !click_block)
        {
            game_state = GameStates::PAUSE_ORBIT;
        }

        //снимаем блокировку даблкликов
        if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
        {
            click_block = false;
        }
    }

    if (game_state == GameStates::MISSION)
        {
            //printf("MISSION\n");
            //sf::sleep(sf::milliseconds(100));
            int result = current_mission->run(elapsed);

            if (result == 0)
            {
                //ничего не делаем
                click_block = true;
                if (show_Help_2) game_state = GameStates::MISSION_HELP_1; //при первом запуске показываем помощь
            }

            if (result == 1)
            {
                //переход в состояние паузы
                printf("Game -> PAUSE ON\n");
                click_block = true;
                game_state = GameStates::PAUSE_MISSION;
            }

            if (result == 2)
            {
                //диалог подбора оружия
                printf("Game -> Take weapon\n");
                click_block = true;
                game_state = GameStates::TAKE_WEAPON_MISSION;
            }


            if (result == 3)
            {
                //миссия закончена штатно
                printf("Game -> Mission finished\n");
                printf("Game -> Save(code %i)", Save());
                click_block = true;
                game_state = GameStates::ORBIT;
                //добавляем собранные ресурсы
                resource_money += current_mission->get_collected_money();
                resource_metal += current_mission->get_collected_metal();
                resource_crystal += current_mission->get_collected_crystal();
                resource_uran += current_mission->get_collected_uran();
                delete current_mission; //уничтожаем объект
                game_options->ptr_music_mission = NULL; //обнуляем ссылку на объект музыки
            }
            if (result == 4)
            {
                //игрок зашел на базу
                click_block = true;
                game_state = GameStates::BASE_MISSION;
                //передаем ресурсы на базу
                current_mission->transfer_to_base();
            }

            if (result == 5)
            {
                //игрок нашел корабль
                click_block = true;
                game_state = GameStates::SHIP_MISSION;
            }

            if (result == 6)
            {
                //диалог подбора апгрейдов
                printf("Game -> take UP\n");
                click_block = true;
                game_state = GameStates::TAKE_UPGRADE_MISSION;
            }

            if (result == 7)
            {
                //смерть игрока
                printf("Game -> Death\n");
                click_block = true;
                game_state = GameStates::DEATH_MISSION;
            }




        }
      
    if (game_state == GameStates::PAUSE_MISSION)
    {
        //sf::sleep(sf::milliseconds(100));
        //printf("PAUSE\n");
        //int result = current_mission->pause(elapsed);
        int result = game_menu->run(true, false);

        if (result == 0)
        {
            //ничего не делаем
        }

        if (result == 1)
        {
            //снимаем паузу
            game_state = GameStates::MISSION;
            click_block = true;
            printf("Game -> PAUSE OFF\n");
        }

        if (result == 2)
        {
            //эвакуация с планеты по ESC
            //сохраняем только деньги
            printf("Game -> Save money (base+rover)\n");
            resource_money += current_mission->get_collected_money();
            printf("Game -> Delete mission\n");
            delete current_mission;
            game_options->ptr_music_mission = NULL; //обнуляем ссылку на объект музыки
            game_state = GameStates::ORBIT;
            printf("Game -> EXIT to MENU\n");
            click_block = true;
            return(0);
        }

    }
    
    if (game_state == GameStates::PAUSE_ORBIT)
    {
        int result = game_menu->run(true, true);//true - меню паузы вызвано во время игру

        if (result == 0)
        {
            //ничего не делаем
        }

        if (result == 1)
        {
            //снимаем паузу
            click_block = true;
            game_state = GameStates::ORBIT;
            printf("Game -> PAUSE OFF\n");
        }

        if (result == 2)
        {
            //выход в главное меню
            click_block = true;
            game_state = GameStates::ORBIT;
            printf("Game -> Exit\n");
            printf("Game -> Save(code %i)", Save());
            music_orbit.stop();
            return(1);
        }

    }

    if (game_state == GameStates::BASE_MISSION)
    {
        //игрок зашел на базу поддержки

        game_window->clear(sf::Color::Black);

        Screenshot_Sprite.setTexture(*current_mission->getScreenshot());
        game_window->draw(Screenshot_Sprite);
        
        Dialog_Base_Sprite.setScale(kx, ky);
        game_window->draw(Dialog_Base_Sprite);
        
        //рисуем данные по ресурсам
        text.setCharacterSize(24 * ky); // in pixels, not points!

        //деньги
        text.setFillColor(sf::Color::Yellow);
        text.setStyle(sf::Text::Bold);
        char buffer[50];
        sprintf_s(buffer, "%i RUB", current_mission->get_collected_money());
        text.setPosition(744.0 * kx, 380.0 * ky);
        text.setString(buffer);
        text.setOrigin(0, 0);
        game_window->draw(text);

        //металл
        sprintf_s(buffer, "%i", current_mission->get_collected_metal());
        text.setPosition(596.0 * kx, 480.0 * ky);
        text.setString(buffer);
        text.setOrigin(0, 0);
        game_window->draw(text);


        //уран
        sprintf_s(buffer, "%i", current_mission->get_collected_uran());
        text.setPosition(784.0 * kx, 480.0 * ky);
        text.setString(buffer);
        text.setOrigin(0, 0);
        game_window->draw(text);

        //кристаллы
        sprintf_s(buffer, "%i", current_mission->get_collected_crystal());
        text.setPosition(973.0 * kx, 480.0 * ky);
        text.setString(buffer);
        text.setOrigin(0, 0);
        game_window->draw(text);

        //здоровье и заряд
        //позиции заданы явно при изменении графики пересчитать
        int health[4] = { int(558.0 * ky), int(573.0 * ky), int(1175.0 * kx), int(1267.0 * kx) };//top,bottom,left,right
        int energy[4] = { int(558.0 * ky), int(573.0 * ky), int(1272.0 * kx), int(1365.0 * kx) };//top,bottom,left,right
        health[3] = current_mission->get_player_health_prc() / 100.0 * (health[3] - health[2]) + health[2];
        energy[3] = current_mission->get_player_energy_prc() / 100.0 * (energy[3] - energy[2]) + energy[2];

        draw_box(&health[0], sf::Color(0, 255, 0, 255));
        draw_box(&energy[0], sf::Color(0, 255, 255, 255));

        //устанавливаем цены ремонта
        int price_for_health = 0;
        int price_for_recharge = 0;

        if (current_mission->get_player_health_prc() < 100)
        {
            //цена пропорциональна уровню планеты
            price_for_health = (int)floor((100 - current_mission->get_player_health_prc())* planet_level);
            sprintf_s(buffer, "%i RUB", price_for_health);
            text.setCharacterSize(20);
            text.setPosition(1219.0 * kx, 613.0 * ky);
            text.setString(buffer);
            text.setOrigin(text.getLocalBounds().width / 2.0, 10);
            game_window->draw(text);
        }

        if (current_mission->get_player_energy_prc() < 100)
        {
            //цена пропорциональна уровню планеты
            price_for_recharge = (int)floor((100 - current_mission->get_player_energy_prc()) * planet_level);
            sprintf_s(buffer, "%i RUB", price_for_recharge);
            text.setCharacterSize(20);
            text.setPosition(1320.0 * kx, 613.0 * ky);
            text.setString(buffer);
            text.setOrigin(text.getLocalBounds().width / 2.0, 10);
            game_window->draw(text);
        }

        //задаем координаты областей
        int Res_to_orbit_area[4] = { int(565.0 * ky), int(607.0 * ky), int(549.0 * kx), int(937.0 * kx) };//top,bottom,left,right
        int Go_mission_area[4] = { int(699.0 * ky), int(737.0 * ky), int(551.0 * kx), int(790.0 * kx) };//top,bottom,left,right
        int Evacuation_area[4] = { int(704.0 * ky), int(746.0 * ky), int(1003.0 * kx), int(1314.0 * kx) };//top,bottom,left,right
        int Repair_area[4] = { int(586.0 * ky), int(630.0 * ky), int(1181.0 * kx), int(1261.0 * kx) };//top,bottom,left,right
        int Charge_area[4] = { int(586.0 * ky), int(630.0 * ky), int(1279.0 * kx), int(1359.0 * kx) };//top,bottom,left,right

        //проверяем нахождение мыши над пунктами меню
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Res_to_orbit_area[0])) draw_box(&Res_to_orbit_area[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Go_mission_area[0])) draw_box(&Go_mission_area[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Evacuation_area[0])) draw_box(&Evacuation_area[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Repair_area[0])) draw_box(&Repair_area[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Charge_area[0])) draw_box(&Charge_area[0], sf::Color(0, 0, 0, 128));

        //рисуем курсор
        mouse_pointer_Sprite.setPosition((float)CursorPosition.x, (float)CursorPosition.y);
        game_window->draw(mouse_pointer_Sprite);

        //финальная отрисовка окна
        game_window->display();


        //проверяем нажатия клавишь
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click_block)
        {
            // проверяем нажатие на кнопки
            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Res_to_orbit_area[0]))
            {
                //загружаем ресурсы на орбиту
                click_block = true;
                printf("Base -> ResUP\n");
                unsigned int arr[4];
                current_mission->transfer_res_to_orbit(&arr[0]);
                resource_money += arr[0];
                resource_metal += arr[1];
                resource_crystal += arr[2];
                resource_uran += arr[3];
                
                //printf("Base -> ResUP (money)  %i\n", arr[0]);
                //printf("Base -> ResUP (metal) %i\n", arr[1]);
                //printf("Base -> ResUP (crystal) %i\n", arr[2]);
                //printf("Base -> ResUP (uran) %i\n", arr[3]);

            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Go_mission_area[0]))
            {
                printf("Base -> Start MISSION\n"); 
                //выезжаем на планету
                click_block = true;
                game_state = GameStates::MISSION;
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Evacuation_area[0]))
            {
                //эвакуация с планеты по кнопке на базе
                //сохраняем деньги
                printf("Base -> Save all res (base_rover)y\n");
                resource_money += current_mission->get_collected_money();
                resource_metal += current_mission->get_collected_metal();
                resource_crystal += current_mission->get_collected_crystal();
                resource_uran += current_mission->get_collected_uran();
                printf("Base -> Delete mission\n");
                delete current_mission;
                game_options->ptr_music_mission = NULL; //обнуляем ссылку на объект музыки
                game_state = GameStates::ORBIT;
                printf("Base -> EXIT to ORBIT\n");
                click_block = true;
                //return(0);
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Repair_area[0]))
            {
                //ремонт
                click_block = true;
                current_mission->repair(price_for_health);
                printf("Base -> Repair\n");

            }


            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Charge_area[0]))
            {
                //зарядка
                printf("Base -> Charge\n");
                current_mission->recharge(price_for_recharge);
                click_block = true;
                
            }
        }

        //обработка нажатия ESC
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !click_block)
        {
            //game_state = GameStates::PAUSE_ORBIT;
        }

        //снимаем блокировку даблкликов
        if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
        {
            click_block = false;
        }

        return(0);

    }

    if (game_state == GameStates::SHIP_MISSION)
    {
        //заходим на покинутый корабль
        game_window->clear(sf::Color::Black);

        Screenshot_Sprite.setTexture(*current_mission->getScreenshot());
        game_window->draw(Screenshot_Sprite);

        Dialog_Ship_Sprite.setScale(kx, ky);
        game_window->draw(Dialog_Ship_Sprite);

        //задаем координаты областей
        int Run_away_area[4] = { int(682.0 * ky), int(764.0 * ky), int(515.0 * kx), int(777.0 * kx) };//top,bottom,left,right
        int Go_into_area[4] = { int(641.0 * ky), int(686.0 * ky), int(805.0 * kx), int(1013.0 * kx) };//top,bottom,left,right
        int Evacuation_area[4] = { int(705.0 * ky), int(744.0 * ky), int(1003.0 * kx), int(1314.0 * kx) };//top,bottom,left,right

        //проверяем нахождение мыши над пунктами меню
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Run_away_area[0])) draw_box(&Run_away_area[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Go_into_area[0])) draw_box(&Go_into_area[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Evacuation_area[0])) draw_box(&Evacuation_area[0], sf::Color(0, 0, 0, 128));

        //рисуем курсор
        mouse_pointer_Sprite.setPosition((float)CursorPosition.x, (float)CursorPosition.y);
        game_window->draw(mouse_pointer_Sprite);

        //финальная отрисовка окна
        game_window->display();


        //проверяем нажатия клавишь
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click_block)
        {
            // проверяем нажатие на кнопки
            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Run_away_area[0]))
            {
                //просто уходим
                click_block = true;
                printf("Ship -> Runaway\n");
                game_state = GameStates::MISSION;

            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Go_into_area[0]))
            {
                printf("Ship -> Go into...\n");
                //заходим внутрь
                click_block = true;
                game_state = GameStates::MISSION;
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Evacuation_area[0]))
            {
                //эвакуация с планеты по кнопке на базе
                //сохраняем деньги
                printf("Ship -> Save all res (base_rover)y\n");
                resource_money += current_mission->get_collected_money();
                resource_metal += current_mission->get_collected_metal();
                resource_crystal += current_mission->get_collected_crystal();
                resource_uran += current_mission->get_collected_uran();
                printf("Ship -> Delete mission\n");
                delete current_mission;
                game_options->ptr_music_mission = NULL; //обнуляем ссылку на объект музыки
                game_state = GameStates::ORBIT;
                printf("Ship -> EXIT to ORBIT\n");
                click_block = true;
                
            }

        }

        //обработка нажатия ESC
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !click_block)
        {
            game_state = GameStates::MISSION;
        }

        //снимаем блокировку даблкликов
        if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
        {
            click_block = false;
        }

        return(0);


    }

    if (game_state == GameStates::ROVER_ORBIT)
    {
        //рисуем интерфейс
        draw_orbit_interface(kx, ky);
        /*
        Screenshot_Sprite.setTexture(Screenshot_Texture);
        Screenshot_Sprite.setColor(sf::Color(255, 255, 255, 128));
        game_window->clear(sf::Color::Black);
        game_window->draw(Screenshot_Sprite);
        */
        Dialog_Rover_Sprite.setScale(kx, ky);
        game_window->draw(Dialog_Rover_Sprite);
        
        //задаем координаты областей кнопок
        int Butt_1[4] =     { int(406.0 * ky), int(442.0 * ky), int(899.0 * kx), int(934.0 * kx) };//top,bottom,left,right
        int Butt_2[4] =     { int(528.0 * ky), int(564.0 * ky), int(899.0 * kx), int(934.0 * kx) };//top,bottom,left,right
        int Butt_3[4] =     { int(649.0 * ky), int(685.0 * ky), int(899.0 * kx), int(934.0 * kx) };//top,bottom,left,right
        int Butt_4[4] =     { int(406.0 * ky), int(442.0 * ky), int(1329.0 * kx), int(1363.0 * kx) };//top,bottom,left,right
        int Butt_5[4] =     { int(528.0 * ky), int(564.0 * ky), int(1329.0 * kx), int(1363.0 * kx) };//top,bottom,left,right
        int Butt_6[4] =     { int(649.0 * ky), int(685.0 * ky), int(1329.0 * kx), int(1363.0 * kx) };//top,bottom,left,right
        int Exit_area[4] =  { int(771.0 * ky), int(810.0 * ky), int(900.0 * kx), int(1021.0 * kx) };//top,bottom,left,right

        //рассчитываем цену апгрейдов
        unsigned int prices[6][4] = { 0 }; //цены: деньги-металл-кристалл-уран
        for (int i=0;i<6;++i) // i - номер апгрейда
        {
            prices[i][0] = rover_upgrades[i] * 100 + 100; // 100руб за каждый уровень
            prices[i][1] = rover_upgrades[i] * 5 + 5; // 5 металла за каждый уровень
            if (rover_upgrades[i] > 3) prices[i][2] = (rover_upgrades[i] - 4) * 2 + 2; // 2 кристалла за каждый уровень, начиная с 4-го
            if (rover_upgrades[i] > 8) prices[i][3] = (rover_upgrades[i] - 9) * 1 + 1; // 1 уран за каждый уровень, начиная с 9-го
            if (i == 5)
            {
                //отдельный алгоритм для технологий
                prices[i][0] = rover_upgrades[i] * 20000 + 10000;
                prices[i][1] = rover_upgrades[i] * 100 + 50; 
                prices[i][2] = rover_upgrades[i] * 50 + 25; 
                prices[i][3] = rover_upgrades[i] * 20 + 10; 
            }

        }
        
        //рассчитываем наличие ресурсов для апгрейдов и закрашиваем неактивные стрелки
        if (!check_prices(&prices[0][0]) || rover_upgrades[0] == 50)   draw_box(&Butt_1[0], sf::Color(115, 95, 53, 255));
        if (!check_prices(&prices[1][0]) || rover_upgrades[1] == 50)   draw_box(&Butt_2[0], sf::Color(115, 95, 53, 255));
        if (!check_prices(&prices[2][0]) || rover_upgrades[2] == 50)   draw_box(&Butt_3[0], sf::Color(115, 95, 53, 255));
        if (!check_prices(&prices[3][0]) || rover_upgrades[3] == 50)   draw_box(&Butt_4[0], sf::Color(115, 95, 53, 255));
        if (!check_prices(&prices[4][0]) || rover_upgrades[4] == 50)   draw_box(&Butt_5[0], sf::Color(115, 95, 53, 255));
        if (!check_prices(&prices[5][0]) || rover_upgrades[5] == 3)   draw_box(&Butt_6[0], sf::Color(115, 95, 53, 255));
                                                                                                   
        //заполняем информацию об апгрейдах
        {
            char buffer[50];
            text.setCharacterSize(28); // in pixels, not points!
            text.setFillColor(sf::Color::Cyan);
            text.setStyle(sf::Text::Bold);
            text.setOrigin(0, 0);

            //Upgrade 1
            sprintf_s(buffer, "lv.%i [%i]", rover_upgrades[0], (int)floor(100.0 + 20.0 * rover_upgrades[0]));
            text.setPosition(751.0 * kx, 411.0 * ky); text.setString(buffer); game_window->draw(text);
            if (rover_upgrades[0]<50)
            {
                sprintf_s(buffer, "%i", prices[0][0]); text.setPosition(680.0 * kx, 447.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[0][3]); text.setPosition(749.0 * kx, 447.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[0][2]); text.setPosition(816.0 * kx, 447.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[0][1]); text.setPosition(882.0 * kx, 447.0 * ky); text.setString(buffer); game_window->draw(text);
            }

            if (rover_upgrades[0] < 50) sprintf_s(buffer, "[%i]", (int)floor(100 + 20 * (rover_upgrades[0] + 1)));
            else sprintf_s(buffer, "[%s]", "MAX");
            text.setPosition(856.0 * kx, 478.0 * ky); text.setString(buffer); game_window->draw(text);

            //Upgrade 2
            sprintf_s(buffer, "lv.%i [%i]", rover_upgrades[1], (int)floor(100.0  + 20.0 * rover_upgrades[1]));
            text.setPosition(775.0 * kx, 531.0 * ky); text.setString(buffer); game_window->draw(text);

            if (rover_upgrades[1] < 50)
            {
                sprintf_s(buffer, "%i", prices[1][0]); text.setPosition(680.0 * kx, 570.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[1][3]); text.setPosition(749.0 * kx, 570.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[1][2]); text.setPosition(816.0 * kx, 570.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[1][1]); text.setPosition(882.0 * kx, 570.0 * ky); text.setString(buffer); game_window->draw(text);
            }
            if (rover_upgrades[1] < 50) sprintf_s(buffer, "[%i]", (int)floor(100 + 20 * (rover_upgrades[1] + 1)));
            else sprintf_s(buffer, "[%s]", "MAX");
            text.setPosition(856.0 * kx, 598.0 * ky); text.setString(buffer); game_window->draw(text);

            //Upgrade 3
            sprintf_s(buffer, "lv.%i[%0.1f/s]", rover_upgrades[2], 1.0 + 0.5 * rover_upgrades[2]);
            text.setPosition(785.0 * kx, 653.0 * ky); text.setString(buffer); game_window->draw(text);

            if (rover_upgrades[2] < 50)
            {
                sprintf_s(buffer, "%i", prices[2][0]); text.setPosition(680.0 * kx, 690.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[2][3]); text.setPosition(749.0 * kx, 690.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[2][2]); text.setPosition(816.0 * kx, 690.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[2][1]); text.setPosition(882.0 * kx, 690.0 * ky); text.setString(buffer); game_window->draw(text);
            }

            if (rover_upgrades[2] < 50) sprintf_s(buffer, "[%0.1f/s]", 1.0 + 0.5 * (rover_upgrades[2] + 1));
            else sprintf_s(buffer, "[%s]", "MAX");
            text.setPosition(856.0 * kx, 718.0 * ky); text.setString(buffer); game_window->draw(text);

            //Upgrade 4
            sprintf_s(buffer, "lv.%i [%i%c]", rover_upgrades[3], (int)(100.0 * pow(1.0185f, rover_upgrades[3])), '%');
            text.setPosition(1213.0 * kx, 411.0 * ky); text.setString(buffer); game_window->draw(text);

            if (rover_upgrades[3] < 50)
            {
                sprintf_s(buffer, "%i", prices[3][0]); text.setPosition(1110.0 * kx, 447.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[3][3]); text.setPosition(1179.0 * kx, 447.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[3][2]); text.setPosition(1246.0 * kx, 447.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[3][1]); text.setPosition(1312.0 * kx, 447.0 * ky); text.setString(buffer); game_window->draw(text);
            }
            if (rover_upgrades[3] < 50) sprintf_s(buffer, "[%i%c]", (int)(100.0 * pow(1.0185f, rover_upgrades[3]+1)), '%');
            else sprintf_s(buffer, "[%s]", "MAX");
            text.setPosition(1286.0 * kx, 478.0 * ky); text.setString(buffer); game_window->draw(text);

            //Upgrade 5
            sprintf_s(buffer, "lv.%i [%i%c]", rover_upgrades[4], (int)(100.0 * pow(1.0185f, rover_upgrades[4])), '%');
            text.setPosition(1193.0 * kx, 531.0 * ky); text.setString(buffer); game_window->draw(text);
            
            if (rover_upgrades[4] < 50)
            {
                sprintf_s(buffer, "%i", prices[4][0]); text.setPosition(1110.0 * kx, 570.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[4][3]); text.setPosition(1179.0 * kx, 570.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[4][2]); text.setPosition(1246.0 * kx, 570.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[4][1]); text.setPosition(1312.0 * kx, 570.0 * ky); text.setString(buffer); game_window->draw(text);
            }
            if (rover_upgrades[4] < 50) sprintf_s(buffer, "[%i%c]", (int)(100.0 * pow(1.0185f, rover_upgrades[4] + 1)), '%');
            else sprintf_s(buffer, "[%s]", "MAX");
            text.setPosition(1286.0 * kx, 598.0 * ky); text.setString(buffer); game_window->draw(text);

            //Upgrade 6
            sprintf_s(buffer, "lv.%i [%i]", rover_upgrades[5], rover_upgrades[5]);
            text.setPosition(1227.0 * kx, 652.0 * ky); text.setString(buffer); game_window->draw(text);

            if (rover_upgrades[5] < 3)
            {
                sprintf_s(buffer, "%i", prices[5][0]); text.setPosition(1106.0 * kx, 690.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[5][3]); text.setPosition(1179.0 * kx, 690.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[5][2]); text.setPosition(1246.0 * kx, 690.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[5][1]); text.setPosition(1312.0 * kx, 690.0 * ky); text.setString(buffer); game_window->draw(text);
            }
            if (rover_upgrades[5] < 3) sprintf_s(buffer, "[+1 slot]");
            else sprintf_s(buffer, "[%s]", "MAX");
            text.setPosition(1286.0 * kx, 718.0 * ky); text.setString(buffer); game_window->draw(text);

        }
        
        //sf::sleep(sf::seconds(10));
        
        //проверяем нахождение мыши над пунктами меню
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_1[0]) && check_prices(&prices[0][0]) && rover_upgrades[0] < 50)    draw_box(&Butt_1[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_2[0]) && check_prices(&prices[1][0]) && rover_upgrades[1] < 50)    draw_box(&Butt_2[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_3[0]) && check_prices(&prices[2][0]) && rover_upgrades[2] < 50)    draw_box(&Butt_3[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_4[0]) && check_prices(&prices[3][0]) && rover_upgrades[3] < 50)    draw_box(&Butt_4[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_5[0]) && check_prices(&prices[4][0]) && rover_upgrades[4] < 50)    draw_box(&Butt_5[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_6[0]) && check_prices(&prices[5][0]) && rover_upgrades[5] < 3)    draw_box(&Butt_6[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Exit_area[0])) draw_box(&Exit_area[0], sf::Color(0, 0, 0, 128));

        //рисуем курсор
        mouse_pointer_Sprite.setPosition((float)CursorPosition.x, (float)CursorPosition.y);
        game_window->draw(mouse_pointer_Sprite);

        //финальная отрисовка окна
        game_window->display();

        //проверяем нажатия клавишь
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click_block)
        {
            // проверяем нажатие на кнопки
            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_1[0]) && check_prices(&prices[0][0]) && rover_upgrades[0] < 50)
            {
                //Апгрейд № 1
                click_block = true;
                //если уровень <50,то повышаем уровень
                rover_upgrades[0]++;
                spend_money(&prices[0][0]);
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_2[0]) && check_prices(&prices[1][0]) && rover_upgrades[1] < 50)
            {
                //Апгрейд № 2
                click_block = true;
                //если уровень <50,то повышаем уровень
                rover_upgrades[1]++;
                spend_money(&prices[1][0]);
            }
            
            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_3[0]) && check_prices(&prices[2][0]) && rover_upgrades[2] < 50)
            {
                //Апгрейд № 3
                click_block = true;
                //если уровень <50,то повышаем уровень
                rover_upgrades[2]++;
                spend_money(&prices[2][0]);
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_4[0]) && check_prices(&prices[3][0]) && rover_upgrades[3] < 50)
            {
                //Апгрейд № 4
                click_block = true;
                //если уровень <50,то повышаем уровень
                rover_upgrades[3]++;
                spend_money(&prices[3][0]);
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_5[0]) && check_prices(&prices[4][0]) && rover_upgrades[4] < 50)
            {
                //Апгрейд № 5
                click_block = true;
                //если уровень <50,то повышаем уровень
                rover_upgrades[4]++;
                spend_money(&prices[4][0]);
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_6[0]) && check_prices(&prices[5][0]) && rover_upgrades[5] < 3)
            {
                //Апгрейд № 6
                click_block = true;
                //если уровень <3,то повышаем уровень
                rover_upgrades[5]++;
                spend_money(&prices[5][0]);
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Exit_area[0]))
            {
                printf("Rover -> Exit\n");  //выход
                click_block = true;
                game_state = GameStates::ORBIT;
            }
        }

        //обработка нажатия ESC
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !click_block)
        {
            //game_state = GameStates::PAUSE_ORBIT;
        }

        //снимаем блокировку даблкликов
        if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
        {
            click_block = false;
        }
        
        
    }

    if (game_state == GameStates::SUIT_ORBIT) 
    {
        //рисуем интерфейс
        draw_orbit_interface(kx, ky);
        Dialog_Suit_Sprite.setScale(kx, ky);
        game_window->draw(Dialog_Suit_Sprite);

        //задаем координаты областей кнопок
        int Butt_1[4] = { int(406.0 * ky), int(442.0 * ky), int(899.0 * kx), int(934.0 * kx) };//top,bottom,left,right
        int Butt_2[4] = { int(528.0 * ky), int(564.0 * ky), int(899.0 * kx), int(934.0 * kx) };//top,bottom,left,right
        int Butt_3[4] = { int(649.0 * ky), int(685.0 * ky), int(899.0 * kx), int(934.0 * kx) };//top,bottom,left,right
        int Butt_4[4] = { int(406.0 * ky), int(442.0 * ky), int(1329.0 * kx), int(1363.0 * kx) };//top,bottom,left,right
        int Butt_5[4] = { int(528.0 * ky), int(564.0 * ky), int(1329.0 * kx), int(1363.0 * kx) };//top,bottom,left,right
        int Butt_6[4] = { int(649.0 * ky), int(685.0 * ky), int(1329.0 * kx), int(1363.0 * kx) };//top,bottom,left,right
        int Exit_area[4] = { int(771.0 * ky), int(810.0 * ky), int(900.0 * kx), int(1021.0 * kx) };//top,bottom,left,right

        //рассчитываем цену апгрейдов
        unsigned int prices[6][4] = { 0 }; //цены: деньги-металл-кристалл-уран
        for (int i = 0; i < 6; ++i) // i - номер апгрейда
        {
            prices[i][0] = suit_upgrades[i] * 100 + 100; // 100руб за каждый уровень
            prices[i][1] = suit_upgrades[i] * 5 + 5; // 5 металла за каждый уровень
            if (suit_upgrades[i] > 3) prices[i][2] = (suit_upgrades[i] - 4) * 2 + 2; // 2 кристалла за каждый уровень, начиная с 4-го
            if (suit_upgrades[i] > 8) prices[i][3] = (suit_upgrades[i] - 9) * 1 + 1; // 1 уран за каждый уровень, начиная с 9-го
            if (i == 2)
            {
                //отдельный алгоритм для чипов
                prices[i][0] = suit_upgrades[i] * 1000 + 1000;
                prices[i][1] = suit_upgrades[i] * 5 + 5;
                prices[i][2] = suit_upgrades[i] * 10 + 10;
                prices[i][3] = suit_upgrades[i] * 3 + 5;
            }
            if (i == 4)
            {
                //отдельный алгоритм для ракет
                prices[i][0] = suit_upgrades[i] * 2000 + 2000;
                prices[i][1] = suit_upgrades[i] * 10 + 10;
                prices[i][2] = suit_upgrades[i] * 5 + 5;
                prices[i][3] = suit_upgrades[i] * 2 + 1;
            }
            if (i == 6)
            {
                //отдельный алгоритм для технологий
                prices[i][0] = suit_upgrades[i] * 20000 + 10000;
                prices[i][1] = suit_upgrades[i] * 100 + 50;
                prices[i][2] = suit_upgrades[i] * 50 + 25;
                prices[i][3] = suit_upgrades[i] * 20 + 10;
            }



        }

        //рассчитываем наличие ресурсов для апгрейдов и закрашиваем неактивные стрелки
        if (!check_prices(&prices[0][0]) || suit_upgrades[0] == 50)   draw_box(&Butt_1[0], sf::Color(115, 95, 53, 255));
        if (!check_prices(&prices[1][0]) || suit_upgrades[1] == 50)   draw_box(&Butt_2[0], sf::Color(115, 95, 53, 255));
        if (!check_prices(&prices[2][0]) || suit_upgrades[2] == 3)   draw_box(&Butt_3[0], sf::Color(115, 95, 53, 255));
        if (!check_prices(&prices[3][0]) || suit_upgrades[3] == 50)   draw_box(&Butt_4[0], sf::Color(115, 95, 53, 255));
        if (!check_prices(&prices[4][0]) || suit_upgrades[4] == 4)   draw_box(&Butt_5[0], sf::Color(115, 95, 53, 255));
        if (!check_prices(&prices[5][0]) || suit_upgrades[5] == 3)   draw_box(&Butt_6[0], sf::Color(115, 95, 53, 255));

        //заполняем информацию об апгрейдах
        {
            char buffer[50];
            text.setCharacterSize(28); // in pixels, not points!
            text.setFillColor(sf::Color::Cyan);
            text.setStyle(sf::Text::Bold);
            text.setOrigin(0, 0);
            wchar_t buf[50];

            //Upgrade 1
            swprintf_s(buf, L"ур.%i [%0.1f %c]", suit_upgrades[0], 100.0 * (1.0f - pow(0.98f, suit_upgrades[0])), '%');
            text.setPosition(751.0 * kx, 411.0 * ky); text.setString(buf); game_window->draw(text);
            if (suit_upgrades[0] < 50)
            {
                sprintf_s(buffer, "%i", prices[0][0]); text.setPosition(680.0 * kx, 447.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[0][3]); text.setPosition(749.0 * kx, 447.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[0][2]); text.setPosition(816.0 * kx, 447.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[0][1]); text.setPosition(882.0 * kx, 447.0 * ky); text.setString(buffer); game_window->draw(text);
            }

            if (suit_upgrades[0] < 50) sprintf_s(buffer, "[%0.1f %c]", 100.0 * (1.0f - pow(0.98f, suit_upgrades[0] + 1)), '%');
            else sprintf_s(buffer, "[%s]", "MAX");
            text.setPosition(846.0 * kx, 477.0 * ky); text.setString(buffer); game_window->draw(text);

            //Upgrade 2
            sprintf_s(buffer, "lv.%i [%0.1f/s]", suit_upgrades[1], 2.0 * pow(1.0222, suit_upgrades[1]));
            text.setPosition(775.0 * kx, 531.0 * ky); text.setString(buffer); game_window->draw(text);

            if (suit_upgrades[1] < 50)
            {
                sprintf_s(buffer, "%i", prices[1][0]); text.setPosition(680.0 * kx, 570.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[1][3]); text.setPosition(749.0 * kx, 570.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[1][2]); text.setPosition(816.0 * kx, 570.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[1][1]); text.setPosition(882.0 * kx, 570.0 * ky); text.setString(buffer); game_window->draw(text);
            }
            if (suit_upgrades[1] < 50) sprintf_s(buffer, "lv.%i[%0.1f/s]", suit_upgrades[1], 2.0 * pow(1.0222, (suit_upgrades[1]+1)));
            else sprintf_s(buffer, "[%s]", "MAX");
            text.setPosition(832.0 * kx, 599.0 * ky); text.setString(buffer); game_window->draw(text);

            //Upgrade 3
            sprintf_s(buffer, "lv.%i [%i]", suit_upgrades[2], suit_upgrades[2]);
            text.setPosition(823.0 * kx, 653.0 * ky); text.setString(buffer); game_window->draw(text);

            if (suit_upgrades[2] < 3)
            {
                sprintf_s(buffer, "%i", prices[2][0]); text.setPosition(680.0 * kx, 690.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[2][3]); text.setPosition(749.0 * kx, 690.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[2][2]); text.setPosition(816.0 * kx, 690.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[2][1]); text.setPosition(882.0 * kx, 690.0 * ky); text.setString(buffer); game_window->draw(text);
            }

            if (suit_upgrades[2] < 3) sprintf_s(buffer, "[%i]", (suit_upgrades[2] + 1));
            else sprintf_s(buffer, "[%s]", "MAX");
            text.setPosition(856.0 * kx, 718.0 * ky); text.setString(buffer); game_window->draw(text);

            //Upgrade 4
            sprintf_s(buffer, "lv.%i [%i%c]", suit_upgrades[3], (int)(100.0 * pow(1.0082f, suit_upgrades[3])), '%');
            text.setPosition(1213.0 * kx, 411.0 * ky); text.setString(buffer); game_window->draw(text);

            if (suit_upgrades[3] < 50)
            {
                sprintf_s(buffer, "%i", prices[3][0]); text.setPosition(1110.0 * kx, 447.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[3][3]); text.setPosition(1179.0 * kx, 447.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[3][2]); text.setPosition(1246.0 * kx, 447.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[3][1]); text.setPosition(1312.0 * kx, 447.0 * ky); text.setString(buffer); game_window->draw(text);
            }
            if (suit_upgrades[3] < 50) sprintf_s(buffer, "[%i%c]", (int)(100.0 * pow(1.0082f, suit_upgrades[3] + 1)), '%');
            else sprintf_s(buffer, "[%s]", "MAX");
            text.setPosition(1286.0 * kx, 477.0 * ky); text.setString(buffer); game_window->draw(text);

            //Upgrade 5
            sprintf_s(buffer, "lv.%i [%i]", suit_upgrades[4], suit_upgrades[4]);
            text.setPosition(1193.0 * kx, 531.0 * ky); text.setString(buffer); game_window->draw(text);

            if (suit_upgrades[4] < 4)
            {
                sprintf_s(buffer, "%i", prices[4][0]); text.setPosition(1110.0 * kx, 570.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[4][3]); text.setPosition(1179.0 * kx, 570.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[4][2]); text.setPosition(1246.0 * kx, 570.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[4][1]); text.setPosition(1312.0 * kx, 570.0 * ky); text.setString(buffer); game_window->draw(text);
            }
            if (suit_upgrades[4] < 4) sprintf_s(buffer, "+1 rocket");
            else sprintf_s(buffer, "[%s]", "MAX");
            text.setPosition(1266.0 * kx, 599.0 * ky); text.setString(buffer); game_window->draw(text);

            //Upgrade 6
            sprintf_s(buffer, "lv.%i [%i]", suit_upgrades[5], suit_upgrades[5]);
            text.setPosition(1239.0 * kx, 652.0 * ky); text.setString(buffer); game_window->draw(text);

            if (suit_upgrades[5] < 3)
            {
                sprintf_s(buffer, "%i", prices[5][0]); text.setPosition(1106.0 * kx, 690.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[5][3]); text.setPosition(1179.0 * kx, 690.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[5][2]); text.setPosition(1246.0 * kx, 690.0 * ky); text.setString(buffer); game_window->draw(text);
                sprintf_s(buffer, "%i", prices[5][1]); text.setPosition(1312.0 * kx, 690.0 * ky); text.setString(buffer); game_window->draw(text);
            }
            if (suit_upgrades[5] < 3) sprintf_s(buffer, "[+1 slot]");
            else sprintf_s(buffer, "[%s]", "MAX");
            text.setPosition(1276.0 * kx, 718.0 * ky); text.setString(buffer); game_window->draw(text);

        }

        //sf::sleep(sf::seconds(10));

        //проверяем нахождение мыши над пунктами меню
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_1[0]) && check_prices(&prices[0][0]) && suit_upgrades[0] < 50)    draw_box(&Butt_1[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_2[0]) && check_prices(&prices[1][0]) && suit_upgrades[1] < 50)    draw_box(&Butt_2[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_3[0]) && check_prices(&prices[2][0]) && suit_upgrades[2] < 3)    draw_box(&Butt_3[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_4[0]) && check_prices(&prices[3][0]) && suit_upgrades[3] < 50)    draw_box(&Butt_4[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_5[0]) && check_prices(&prices[4][0]) && suit_upgrades[4] < 4)    draw_box(&Butt_5[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_6[0]) && check_prices(&prices[5][0]) && suit_upgrades[5] < 3)    draw_box(&Butt_6[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Exit_area[0])) draw_box(&Exit_area[0], sf::Color(0, 0, 0, 128));

        //рисуем курсор
        mouse_pointer_Sprite.setPosition((float)CursorPosition.x, (float)CursorPosition.y);
        game_window->draw(mouse_pointer_Sprite);

        //финальная отрисовка окна
        game_window->display();

        //проверяем нажатия клавишь
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click_block)
        {
            // проверяем нажатие на кнопки
            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_1[0]) && check_prices(&prices[0][0]) && suit_upgrades[0] < 50)
            {
                //Апгрейд № 1
                click_block = true;
                //если уровень <50,то повышаем уровень
                suit_upgrades[0]++;
                spend_money(&prices[0][0]);
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_2[0]) && check_prices(&prices[1][0]) && suit_upgrades[1] < 50)
            {
                //Апгрейд № 2
                click_block = true;
                //если уровень <50,то повышаем уровень
                suit_upgrades[1]++;
                spend_money(&prices[1][0]);
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_3[0]) && check_prices(&prices[2][0]) && suit_upgrades[2] < 3)
            {
                //Апгрейд № 3
                click_block = true;
                //если уровень <50,то повышаем уровень
                suit_upgrades[2]++;
                spend_money(&prices[2][0]);
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_4[0]) && check_prices(&prices[3][0]) && suit_upgrades[3] < 50)
            {
                //Апгрейд № 4
                click_block = true;
                //если уровень <50,то повышаем уровень
                suit_upgrades[3]++;
                spend_money(&prices[3][0]);
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_5[0]) && check_prices(&prices[4][0]) && suit_upgrades[4] < 4)
            {
                //Апгрейд № 5
                click_block = true;
                //если уровень <50,то повышаем уровень
                suit_upgrades[4]++;
                spend_money(&prices[4][0]);
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Butt_6[0]) && check_prices(&prices[5][0]) && suit_upgrades[5] < 3)
            {
                //Апгрейд № 6
                click_block = true;
                //если уровень <3,то повышаем уровень
                suit_upgrades[5]++;
                spend_money(&prices[5][0]);
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Exit_area[0]))
            {
                printf("suit -> Exit\n");  //выход
                click_block = true;
                game_state = GameStates::ORBIT;
            }
        }

        //обработка нажатия ESC
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !click_block)
        {
            //game_state = GameStates::PAUSE_ORBIT;
        }

        //снимаем блокировку даблкликов
        if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
        {
            click_block = false;
        }


    }

    if (game_state == GameStates::FLY_AWAY_ORBIT) 
    {
        //рисуем интерфейс
        draw_orbit_interface(kx, ky);
        Dialog_Fly_Sprite.setScale(kx, ky);
        game_window->draw(Dialog_Fly_Sprite);

        //задаем координаты областей кнопок
        int OK_area[4] = { int(669.0 * ky), int(768.0 * ky), int(574.0 * kx), int(842.0 * kx) };//top,bottom,left,right
        int Exit_area[4] = { int(669.0 * ky), int(768.0 * ky), int(934.0 * kx), int(1335.0 * kx) };//top,bottom,left,right

        //рассчитываем цену полета
        unsigned int price_money = (planet_level + 1) * 10000;
        unsigned int price_uran = (planet_level + 1) * 10;

        //рассчитываем наличие ресурсов
        if ((price_money > resource_money) || (price_uran > resource_uran)) draw_box(&OK_area[0], sf::Color(0, 0, 0, 240));

        //заполняем информацию об апгрейдах
            char buffer[50];
            text.setCharacterSize(60); // in pixels, not points!
            text.setFillColor(sf::Color::Yellow);
            text.setStyle(sf::Text::Bold);
            text.setOrigin(0, 0);

            //money
            sprintf_s(buffer, "%i RUB", price_money);
            text.setPosition(1158.0 * kx, 528.0 * ky); text.setString(buffer); game_window->draw(text);

            //uran
            sprintf_s(buffer, "%i", price_uran);
            text.setPosition(654.0 * kx, 528.0 * ky); text.setString(buffer); game_window->draw(text);

        //sf::sleep(sf::seconds(10));

        //проверяем нахождение мыши над пунктами меню
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &OK_area[0]) && ((price_money < resource_money) && (price_uran < resource_uran)))    draw_box(&OK_area[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Exit_area[0])) draw_box(&Exit_area[0], sf::Color(0, 0, 0, 128));

        //рисуем курсор
        mouse_pointer_Sprite.setPosition((float)CursorPosition.x, (float)CursorPosition.y);
        game_window->draw(mouse_pointer_Sprite);

        //финальная отрисовка окна
        game_window->display();

        //проверяем нажатия клавишь
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click_block)
        {
            // проверяем нажатие на кнопки
            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &OK_area[0]) && ((price_money < resource_money) || (price_uran < resource_uran)))
            {
                //вылет
                click_block = true;
                //если уровень <50,то повышаем уровень
                unsigned int prices[4] = { price_money ,0 ,0 , price_uran };
                spend_money(&prices[0]);
                //скриншот полета
                Planet_Jump_Sprite.setScale(kx, ky);
                game_window->draw(Planet_Jump_Sprite);
                game_window->display();
                //драматическая пауза
                sf::sleep(sf::seconds(1));
                planet_number++;
                
                planet_level = 1 + floor(rand() * (planet_number / 2) / (RAND_MAX + 1.0));
                if (planet_level > 9) planet_level = 9;
                planet_type = 1 + floor(rand() * 5.0 / (RAND_MAX + 1.0));
                
                if (planet_type == 1) bg_Planet_Texture.loadFromFile(game_path + "Assets\\Pictures\\Planet_1.png");
                if (planet_type == 2) bg_Planet_Texture.loadFromFile(game_path + "Assets\\Pictures\\Planet_2.png");
                if (planet_type == 3) bg_Planet_Texture.loadFromFile(game_path + "Assets\\Pictures\\Planet_3.png");
                if (planet_type == 4) bg_Planet_Texture.loadFromFile(game_path + "Assets\\Pictures\\Planet_4.png");
                if (planet_type == 5) bg_Planet_Texture.loadFromFile(game_path + "Assets\\Pictures\\Planet_5.png");
                bg_Planet_Sprite.setTexture(bg_Planet_Texture);
                game_state = GameStates::ORBIT;

            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Exit_area[0]))
            {
                printf("fly -> Exit\n");  //выход
                click_block = true;
                game_state = GameStates::ORBIT;
            }
        }

        //обработка нажатия ESC
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !click_block)
        {
            //game_state = GameStates::PAUSE_ORBIT;
        }

        //снимаем блокировку даблкликов
        if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
        {
            click_block = false;
        }

    }
    
    if (game_state == GameStates::TAKE_WEAPON_MISSION)
    {

        //sf::sleep(sf::milliseconds(100));

        Screenshot_Sprite.setTexture(*current_mission->getScreenshot());
        Screenshot_Sprite.setColor(sf::Color(255, 255, 255, 255));
        game_window->clear();
        game_window->draw(Screenshot_Sprite);

        //рисуем интерфейс
        Dialog_weapon_Sprite.setScale(kx, ky);
        game_window->draw(Dialog_weapon_Sprite);

        //задаем координаты областей кнопок
        int Get_resources_area[4] = { int(833.0 * ky), int(926.0 * ky), int(256.0 * kx), int(651.0 * kx) };//top,bottom,left,right
        int Change_weapon_area[4] = { int(833.0 * ky), int(926.0 * ky), int(762.0 * kx), int(1156.0 * kx) };//top,bottom,left,right
        int Level_UP_area[4] = { int(833.0 * ky), int(926.0 * ky), int(1264.0 * kx), int(1659.0 * kx) };//top,bottom,left,right
        
        WeaponData new_weapon;
        new_weapon.sprite_N = current_mission->get_weapon_to_take() & 255;
        new_weapon.type = current_mission->get_weapon_to_take() & 255;
        new_weapon.color = current_mission->get_weapon_to_take() >> 8;
        new_weapon.fire_rate = calculate_fire_rate(new_weapon.type, new_weapon.color, 0); //рассчитывается в функции (double)
        new_weapon.power = calculate_damage(new_weapon.type, new_weapon.color, 0); //рассчитывается в функции (double)
        if (new_weapon.type == 2) new_weapon.damageArea = 100.0; else new_weapon.damageArea = 0.0;
        if (new_weapon.type < 3) new_weapon.hasSpecialEffect = false; else new_weapon.hasSpecialEffect = true;
        new_weapon.level = 0;

        //рисуем спрайт
        weapon_tile_Sprite.setTextureRect(sf::IntRect(0, new_weapon.sprite_N * 200, 200, 200));
        weapon_tile_Sprite.setPosition((int)(1560.0 * kx),(int)((256.0 + 50.0) * ky));
        weapon_tile_Sprite.setScale(kx, ky);
        weapon_tile_Sprite.setOrigin(100, 100);
        weapon_tile_Sprite.setRotation(-90.0);
        game_window->draw(weapon_tile_Sprite);

        //параметры текста

        wchar_t buffer[50];
        text.setCharacterSize((int)(30.0 * ky)); // in pixels, not points!
        text.setFillColor(sf::Color(78,69,46,255));
        text.setStyle(sf::Text::Bold);
        text.setOrigin(0, (int)(30.0 * ky));
        
        //заполняем информацию о найденном оружии

        get_weapon_name(new_weapon.type, buffer);
        text.setPosition(1000.0 * kx, 505.0 * ky); text.setString(buffer); game_window->draw(text);
        
        text.setFillColor(get_color_by_weapon_quality(new_weapon.color));
        get_weapon_quality_name(new_weapon.color, buffer);
        text.setPosition(1000.0 * kx, 551.0 * ky); text.setString(buffer); game_window->draw(text);
        text.setFillColor(sf::Color(78, 69, 46, 255));
        
        swprintf_s(buffer,L"%i", new_weapon.level);
        text.setPosition(1000.0 * kx, 602.0 * ky); text.setString(buffer); game_window->draw(text);
        swprintf_s(buffer, L"%0.2f", new_weapon.power);
        text.setPosition(1000.0 * kx, 653.0 * ky); text.setString(buffer); game_window->draw(text);
        swprintf_s(buffer, L"%0.2f", new_weapon.fire_rate);
        text.setPosition(1000.0 * kx, 699.0 * ky); text.setString(buffer); game_window->draw(text);
        if (new_weapon.damageArea > 0.0)swprintf_s(buffer, L"ДА"); else swprintf_s(buffer, L"НЕТ");
        text.setPosition(1000.0 * kx, 748.0 * ky); text.setString(buffer); game_window->draw(text);
        if (new_weapon.hasSpecialEffect)swprintf_s(buffer, L"ДА"); else swprintf_s(buffer, L"НЕТ");
        text.setPosition(1000.0 * kx, 797.0 * ky); text.setString(buffer); game_window->draw(text);

        //Получаем информацию о текущем оружии
        WeaponData cur_weapon = current_mission->get_weapon_data();

        //заполняем информацию о текущем оружии
        get_weapon_name(cur_weapon.type, buffer);
        text.setPosition(720.0 * kx, 505.0 * ky); text.setString(buffer); game_window->draw(text);
        text.setFillColor(get_color_by_weapon_quality(cur_weapon.color));
        get_weapon_quality_name(cur_weapon.color, buffer);
        text.setPosition(720.0 * kx, 551.0 * ky); text.setString(buffer); game_window->draw(text);
        text.setFillColor(sf::Color(78, 69, 46, 255));
        swprintf_s(buffer, L"%i", cur_weapon.level);
        text.setPosition(720.0 * kx, 602.0 * ky); text.setString(buffer); game_window->draw(text);
        swprintf_s(buffer, L"%0.2f", cur_weapon.power);
        text.setPosition(720.0 * kx, 653.0 * ky); text.setString(buffer); game_window->draw(text);
        swprintf_s(buffer, L"%0.2f", cur_weapon.fire_rate);
        text.setPosition(720.0 * kx, 699.0 * ky); text.setString(buffer); game_window->draw(text);
        if (cur_weapon.damageArea > 0.0)swprintf_s(buffer, L"ДА"); else swprintf_s(buffer, L"НЕТ");
        text.setPosition(720.0 * kx, 748.0 * ky); text.setString(buffer); game_window->draw(text);
        if (cur_weapon.hasSpecialEffect)swprintf_s(buffer, L"ДА"); else swprintf_s(buffer, L"НЕТ");
        text.setPosition(720.0 * kx, 797.0 * ky); text.setString(buffer); game_window->draw(text);

        //заполняем информацию об апгрейде текущего оружия

        get_weapon_name(cur_weapon.type, buffer);
        text.setPosition(1275.0 * kx, 505.0 * ky); text.setString(buffer); game_window->draw(text);
        text.setFillColor(get_color_by_weapon_quality(cur_weapon.color));
        get_weapon_quality_name(cur_weapon.color, buffer);
        text.setPosition(1275.0 * kx, 551.0 * ky); text.setString(buffer); game_window->draw(text);
        text.setFillColor(sf::Color(78, 69, 46, 255));
        swprintf_s(buffer, L"%i", cur_weapon.level + 1);
        text.setPosition(1275.0 * kx, 602.0 * ky); text.setString(buffer); game_window->draw(text);
        swprintf_s(buffer, L"%0.2f", calculate_damage(cur_weapon.type, cur_weapon.color, cur_weapon.level + 1));
        text.setPosition(1275.0 * kx, 653.0 * ky); text.setString(buffer); game_window->draw(text);
        swprintf_s(buffer, L"%0.2f", calculate_fire_rate(cur_weapon.type, cur_weapon.color, cur_weapon.level + 1));
        text.setPosition(1275.0 * kx, 699.0 * ky); text.setString(buffer); game_window->draw(text);
        if (cur_weapon.damageArea > 0.0)swprintf_s(buffer, L"ДА"); else swprintf_s(buffer, L"НЕТ");
        text.setPosition(1275.0 * kx, 748.0 * ky); text.setString(buffer); game_window->draw(text);
        if (cur_weapon.hasSpecialEffect)swprintf_s(buffer, L"ДА"); else swprintf_s(buffer, L"НЕТ");
        text.setPosition(1275.0 * kx, 797.0 * ky); text.setString(buffer); game_window->draw(text);

        //проверяем нахождение мыши над пунктами меню
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Change_weapon_area[0]))    draw_box(&Change_weapon_area[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Level_UP_area[0]))         draw_box(&Level_UP_area[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Get_resources_area[0]))    draw_box(&Get_resources_area[0], sf::Color(0, 0, 0, 128));

        //рисуем курсор
        mouse_pointer_Sprite.setPosition((float)CursorPosition.x, (float)CursorPosition.y);
        game_window->draw(mouse_pointer_Sprite);

        //финальная отрисовка окна
        game_window->display();
        
        //проверяем нажатия клавишь
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click_block)
        {
            // проверяем нажатие на кнопки
            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Change_weapon_area[0]))
            {
                //заменяем оружие
                click_block = true;
                current_mission->set_weapon(new_weapon);

                game_state = GameStates::MISSION;

            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Level_UP_area[0]))
            {
                click_block = true;
                printf("level UP \n");
                cur_weapon.level++;
                cur_weapon.power = calculate_damage(cur_weapon.type, cur_weapon.color, cur_weapon.level);
                cur_weapon.fire_rate = calculate_fire_rate(cur_weapon.type, cur_weapon.color, cur_weapon.level);
                current_mission->set_weapon(cur_weapon);
                game_state = GameStates::MISSION;
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Get_resources_area[0]))
            {
                click_block = true;
                if (new_weapon.color == 0)
                {
                    int res[4]{ 0, 10, 0, 0 };
                    current_mission->get_resources(&res[0]);
                }
                if (new_weapon.color == 1)
                {
                    int res[4]{ 0, 10, 5, 0 };
                    current_mission->get_resources(&res[0]);
                }
                if (new_weapon.color == 2)
                {
                    int res[4]{ 0, 15, 10, 1 };
                    current_mission->get_resources(&res[0]);
                }
                if (new_weapon.color == 3)
                {
                    int res[4]{ 0, 20, 12, 3 };
                    current_mission->get_resources(&res[0]);
                }
                if (new_weapon.color == 4)
                {
                    int res[4]{ 0, 20, 15, 5 };
                    current_mission->get_resources(&res[0]);
                }
                game_state = GameStates::MISSION;
            }

        }

        //снимаем блокировку даблкликов
        if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
        {
            click_block = false;
        }
    }

    if (game_state == GameStates::TAKE_UPGRADE_MISSION)
    {

        //sf::sleep(sf::milliseconds(100));

        Screenshot_Sprite.setTexture(*current_mission->getScreenshot());
        Screenshot_Sprite.setColor(sf::Color(255, 255, 255, 255));
        game_window->clear();
        game_window->draw(Screenshot_Sprite);

        //рисуем интерфейс
        Dialog_Upgrade_Sprite.setScale(kx, ky);
        game_window->draw(Dialog_Upgrade_Sprite);
        
        //задаем координаты областей кнопок
        int Get_resources_area[4] = { int(861.0 * ky), int(933.0 * ky), int(249.0 * kx), int(548.0 * kx) };//top,bottom,left,right
        int slot_1_area[4] = { int(861.0 * ky), int(933.0 * ky), int(616.0 * kx), int(915.0 * kx) };//top,bottom,left,right
        int slot_2_area[4] = { int(861.0 * ky), int(933.0 * ky), int(997.0 * kx), int(1296.0 * kx) };//top,bottom,left,right
        int slot_3_area[4] = { int(861.0 * ky), int(933.0 * ky), int(1369.0 * kx), int(1668.0 * kx) };//top,bottom,left,right

        int new_upgrade = current_mission->get_upgrade_to_take();

        //рисуем спрайт
        Dialog_Upgrade_Text_Sprite.setTextureRect(sf::IntRect(0, new_upgrade * 200, 200, 200));
        Dialog_Upgrade_Text_Sprite.setPosition((int)(1560.0 * kx), (int)((256.0) * ky));
        Dialog_Upgrade_Text_Sprite.setScale(kx, ky);
        Dialog_Upgrade_Text_Sprite.setOrigin(100, 100);
        game_window->draw(Dialog_Upgrade_Text_Sprite);

        int active_slots = current_mission->get_slots_number();
        int slots[3];
        current_mission->get_slots_array(&slots[0]);
        //slots[0] = 1;
        //printf("slots = %i\n", active_slots);
        //printf("slots (%i, %i, %i)\n", slots[0], slots[1], slots[2]);
         
        //проверяем нахождение мыши над пунктами меню
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Get_resources_area[0]))                 draw_box(&Get_resources_area[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &slot_1_area[0]) && active_slots > 0)    draw_box(&slot_1_area[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &slot_2_area[0]) && active_slots > 1)    draw_box(&slot_2_area[0], sf::Color(0, 0, 0, 128));
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &slot_3_area[0]) && active_slots > 2)    draw_box(&slot_3_area[0], sf::Color(0, 0, 0, 128));

        //рисуем иконки апгрейдов
        //сначала нерабочие ячейки
        for (int i = active_slots + 1; i < 4; ++i)
        {
            Dialog_Upgrade_Text_Sprite.setTextureRect(sf::IntRect(0, 5 * 200, 200, 200));
            Dialog_Upgrade_Text_Sprite.setPosition((int)((291.0 + 379.0 * i) * kx), (int)(604.0 * ky));
            Dialog_Upgrade_Text_Sprite.setScale(kx, ky);
            Dialog_Upgrade_Text_Sprite.setOrigin(0, 0);
            game_window->draw(Dialog_Upgrade_Text_Sprite);
        }

        //теперь занятые
        for (int i = 0; i < active_slots; ++i)
        {
            if (slots[i] == -1)
            {
                Dialog_Upgrade_Text_Sprite.setTextureRect(sf::IntRect(0, 6 * 200, 200, 200));
            }
            else
            {
                Dialog_Upgrade_Text_Sprite.setTextureRect(sf::IntRect(0, slots[i] * 200, 200, 200));
            }
            
            Dialog_Upgrade_Text_Sprite.setPosition((int)((291.0 + 379.0 * (i+1)) * kx), (int)(604.0 * ky));
            Dialog_Upgrade_Text_Sprite.setScale(kx, ky);
            Dialog_Upgrade_Text_Sprite.setOrigin(0, 0);
            game_window->draw(Dialog_Upgrade_Text_Sprite);
        }


        //рисуем курсор
        mouse_pointer_Sprite.setPosition((float)CursorPosition.x, (float)CursorPosition.y);
        game_window->draw(mouse_pointer_Sprite);

        //финальная отрисовка окна
        game_window->display();

        //проверяем нажатия клавишь
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click_block)
        {
            // проверяем нажатие на кнопки
            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &slot_1_area[0]) && active_slots >= 1)
            {
                //вставка в слот 1
                click_block = true;
                //current_mission->set_weapon(new_weapon);
                current_mission->install_upgrade(1, new_upgrade);
                game_state = GameStates::MISSION;
            }

            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &slot_2_area[0]) && active_slots >= 2)
            {
                //вставка в слот 1
                click_block = true;
                current_mission->install_upgrade(2, new_upgrade);
                //current_mission->set_weapon(new_weapon);
                game_state = GameStates::MISSION;
            }
            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &slot_3_area[0]) && active_slots >= 3)
            {
                //вставка в слот 1
                click_block = true;
                current_mission->install_upgrade(3, new_upgrade);
                //current_mission->set_weapon(new_weapon);
                game_state = GameStates::MISSION;
            }


            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Get_resources_area[0]))
            {
                click_block = true;
                int res[4]{ 0, 10, 10, 2 };
                current_mission->get_resources(&res[0]);
                game_state = GameStates::MISSION;
            }

        }

        //снимаем блокировку даблкликов
        if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
        {
            click_block = false;
        }
        
    }

    if (game_state == GameStates::DEATH_MISSION)
    {
        //игрок умер

        game_window->clear(sf::Color::Black);

        Screenshot_Sprite.setTexture(*current_mission->getScreenshot());
        game_window->draw(Screenshot_Sprite);

        Dialog_Death_Sprite.setScale(kx, ky);
        game_window->draw(Dialog_Death_Sprite);

        //рисуем данные по ресурсам
        text.setCharacterSize(30 * ky); // in pixels, not points!

        //деньги
        text.setFillColor(sf::Color::Yellow);
        text.setStyle(sf::Text::Bold);
        char buffer[50];
        sprintf_s(buffer, "%i RUB", current_mission->get_rover_money());
        text.setPosition(850.0 * kx, 521.0 * ky);
        text.setString(buffer);
        text.setOrigin(0, 0);
        game_window->draw(text);


        //задаем координаты областей
        int Exit_area[4] = { int(774.0 * ky), int(845.0 * ky), int(808.0 * kx), int(1106.0 * kx) };//top,bottom,left,right

        //проверяем нахождение мыши над пунктами меню
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Exit_area[0])) draw_box(&Exit_area[0], sf::Color(0, 0, 0, 128));

        //рисуем курсор
        mouse_pointer_Sprite.setPosition((float)CursorPosition.x, (float)CursorPosition.y);
        game_window->draw(mouse_pointer_Sprite);

        //финальная отрисовка окна
        game_window->display();


        //проверяем нажатия клавишь
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click_block)
        {
            // проверяем нажатие на кнопки
            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &Exit_area[0]))
            {
                //загружаем ресурсы на орбиту
                click_block = true;
                //unsigned int arr[4];
                current_mission->transfer_to_base();
                //resource_money += arr[0];
                current_mission->reset_Rover();
                game_state = GameStates::BASE_MISSION;
            }
        }

        //обработка нажатия ESC
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !click_block)
        {
            //game_state = GameStates::PAUSE_ORBIT;
        }

        //снимаем блокировку даблкликов
        if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
        {
            click_block = false;
        }

        return(0);

    }

    if (game_state == GameStates::ORBIT_HELP_1)
    {
        //рисуем интерфейс
        draw_orbit_interface(kx, ky);
        //рисуем инструктаж
        Dialog_Help1_Sprite.setScale(kx, ky);
        game_window->draw(Dialog_Help1_Sprite);
        
        //задаем координаты областей
        int OK_area[4] = { int(801.0 * ky), int(865.0 * ky), int(809.0 * kx), int(1095.0 * kx) };//top,bottom,left,right

        //проверяем нахождение мыши над пунктами меню
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &OK_area[0])) draw_box(&OK_area[0], sf::Color(0, 0, 0, 128));

        //рисуем курсор
        mouse_pointer_Sprite.setPosition((float)CursorPosition.x, (float)CursorPosition.y);
        game_window->draw(mouse_pointer_Sprite);

        //финальная отрисовка окна
        game_window->display();

        //проверяем нажатия клавишь
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click_block)
        {
            // проверяем нажатие на кнопки
            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &OK_area[0]))
            {
                click_block = true;
                show_Help_1 = false;
                game_state = GameStates::ORBIT;
                //return(0);
            }
        }

        //обработка нажатия ESC
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !click_block)
        {
            //game_state = GameStates::PAUSE_ORBIT;
        }

        //снимаем блокировку даблкликов
        if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
        {
            click_block = false;
        }
    }

    if (game_state == GameStates::MISSION_HELP_1)
    {
        //рисуем скриншот
        Screenshot_Sprite.setTexture(*current_mission->getScreenshot());
        //printf("tex2 x %i \n", Screenshot_Sprite.getTexture()->getSize().x);
        Screenshot_Sprite.setColor(sf::Color(255, 255, 255, 255));
        game_window->clear(sf::Color::Black);
        game_window->draw(Screenshot_Sprite);
        //printf("tex2");

        //рисуем инструктаж
        Dialog_Help2_Sprite.setScale(kx, ky);
        game_window->draw(Dialog_Help2_Sprite);

        //задаем координаты областей
        int OK_area[4] = { int(801.0 * ky), int(865.0 * ky), int(818.0 * kx), int(1104.0 * kx) };//top,bottom,left,right

        //проверяем нахождение мыши над пунктами меню
        if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &OK_area[0])) draw_box(&OK_area[0], sf::Color(0, 33, 0, 128));

        //рисуем курсор
        mouse_pointer_Sprite.setPosition((float)CursorPosition.x, (float)CursorPosition.y);
        game_window->draw(mouse_pointer_Sprite);

        //финальная отрисовка окна
        game_window->display();

        //проверяем нажатия клавишь
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click_block)
        {
            // проверяем нажатие на кнопки
            if (check_mouse_hover((int)CursorPosition.x, (int)CursorPosition.y, &OK_area[0]))
            {
                click_block = true;
                show_Help_2 = false;
                game_state = GameStates::MISSION;
                //return(0);
            }
        }

        //обработка нажатия ESC
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !click_block)
        {
            //game_state = GameStates::PAUSE_ORBIT;
        }

        //снимаем блокировку даблкликов
        if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
        {
            click_block = false;
        }
    }

    return(0); //продолжаем
}

int Game::Reset()
{
    printf("Game -> reset game\n");
    resource_money = 0;
    resource_metal = 0;
    resource_crystal = 0;
    resource_uran = 0;
    planet_number = 0;
    planet_type = 1;
    planet_level = 1;
    for (int i = 0; i < 6; ++i)
    {
        rover_upgrades[i] = 0;
        suit_upgrades[i] = 0;
    }
    GameExists = true;
    game_menu->set_gameExists(true);
    show_Help_1 = true;
    show_Help_2 = true;
    return(1); //все ОК
}

int Game::check_mouse_hover(int x, int y, int* area)
{
    if (y > area[0] && y < area[1] && x > area[2] && x < area[3]) return 1;
    return 0;
}

bool Game::check_game()
{
    printf("Game -> check game\n");
    return (GameExists);
}

bool Game::Load()
{
    printf("Game -> load game\n");
    std::ifstream fd;
    fd.open(game_path + "\\savegame.dat", std::ios_base::in | std::ios_base::binary);
    if (!fd) return (false);
    char buffer[8] = { 0 };
    unsigned int hash = 0; //контрольная сумма
    fd.read(buffer, 8);
    char_to_int(buffer, &resource_money); hash = h_function(resource_money + 25698);
    printf("money = %i\n", resource_money);
    
    fd.read(buffer, 8);
    char_to_int(buffer, &resource_metal);hash = h_function(hash + resource_metal);
    printf("metal = %i\n", resource_metal);

    fd.read(buffer, 8);
    char_to_int(buffer, &resource_crystal);hash = h_function(hash + resource_crystal);
    printf("crystal = %i\n", resource_crystal);

    fd.read(buffer, 8);
    char_to_int(buffer, &resource_uran);hash = h_function(hash + resource_uran);
    printf("uran = %i\n", resource_uran);

    fd.read(buffer, 8);
    char_to_int(buffer, &planet_number);hash = h_function(hash + planet_number);
    printf("planet_number = %i\n", planet_number);

    fd.read(buffer, 8);
    char_to_int(buffer, &planet_type);hash = h_function(hash + planet_type);
    printf("planet_type = %i\n", planet_type);

    fd.read(buffer, 8);
    char_to_int(buffer, &planet_level); hash = h_function(hash + planet_level);
    printf("planet_level = %i\n", planet_level);

    //считываем значения апгрейдов
    for (int i = 0; i < 6; ++i)
    {
        fd.read(buffer, 8);
        char_to_int(buffer, &suit_upgrades[i]); hash = h_function(hash + suit_upgrades[i]);
        fd.read(buffer, 8);
        char_to_int(buffer, &rover_upgrades[i]); hash = h_function(hash + rover_upgrades[i]);
        printf("suit_upgrade = %i rover_upgrade = %i\n", suit_upgrades[i], rover_upgrades[i]);
    }
    
    unsigned int tmp = 0;
    fd.read(buffer, 8);
    char_to_int(buffer, &tmp); hash = h_function(hash + tmp);
    show_Help_1 = (bool)tmp;
    printf("help 1 = %i\n", show_Help_1);

    fd.read(buffer, 8);
    char_to_int(buffer, &tmp); hash = h_function(hash + tmp);
    show_Help_2 = (bool)tmp;
    printf("help 2 = %i\n", show_Help_1);
    
    //проверяем хеш
    unsigned int hash_from_file;
    fd.read(buffer, 8);
    char_to_int(buffer, &hash_from_file);
    if (hash_from_file != hash)
    {
        printf("hash data error");
        return(false);
    }
    
    //значения для отладки
    /*
    resource_money = 1000000000;
    resource_metal = 10000000;
    resource_crystal = 100000000;
    resource_uran = 100000000;
    planet_number = 0;
    planet_type = 1;
    planet_level = 1;
    //for (int i = 0; i < 6; ++i) suit_upgrades[i] = i;
    //for (int i = 0; i < 6; ++i) rover_upgrades[i] = i;
    */
    
    GameExists = true;
    game_menu->set_gameExists(true);
    return (true);
}

int Game::Save()
{
    printf("Game -> save game\n");
    std::ofstream fd;
    char buffer[8] = { 0 };
    unsigned int hash = 0;
    fd.open(game_path + "\\savegame.dat", std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
    //printf("size = %i\n", sizeof(resource_money));
    int_to_char(&resource_money, &buffer[0]); hash = h_function(resource_money + 25698);
    fd.write(buffer, 8);
    int_to_char(&resource_metal, &buffer[0]);hash = h_function(hash + resource_metal);
    fd.write(buffer, 8);
    int_to_char(&resource_crystal, &buffer[0]);hash = h_function(hash + resource_crystal);
    fd.write(buffer, 8);
    int_to_char(&resource_uran, &buffer[0]);hash = h_function(hash + resource_uran);
    fd.write(buffer, 8);
    int_to_char(&planet_number, &buffer[0]);hash = h_function(hash + planet_number);
    fd.write(buffer, 8);
    int_to_char(&planet_type, &buffer[0]);hash = h_function(hash + planet_type);
    fd.write(buffer, 8);
    int_to_char(&planet_level, &buffer[0]);hash = h_function(hash + planet_level);
    fd.write(buffer, 8);
    for(int i=0;i<6;++i)
        {
            int_to_char(&suit_upgrades[i], &buffer[0]); hash = h_function(hash + suit_upgrades[i]);
            fd.write(buffer, 8);
            int_to_char(&rover_upgrades[i], &buffer[0]); hash = h_function(hash + rover_upgrades[i]);
            fd.write(buffer, 8);
        }
    unsigned int tmp = (int)show_Help_1;
    int_to_char(&tmp, &buffer[0]); hash = h_function(hash + tmp);
    fd.write(buffer, 8);
    tmp = (int)show_Help_2;
    int_to_char(&tmp, &buffer[0]); hash = h_function(hash + tmp);
    fd.write(buffer, 8);
    int_to_char(&hash, &buffer[0]);
    fd.write(buffer, 8);

    fd.close();
    return (0);
}

void Game::int_to_char(unsigned int *number, char *buffer)
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
        buffer[i] = n >> (i*8);
        //printf("%i = %c \n", (n >> (i*8)) & 256, buffer[i]);
    }
}

void Game::char_to_int(char* buffer, unsigned int* number)
{
    unsigned int n = 0;
    //преобразуем 8-байтный массив в число

    //считываем каждый байт отдельно, но не более кол-ва байт в беззнаковом целом
    for (int i = 0; i < sizeof(unsigned int); ++i)
    {
        //проходим по битам и считываем их
        int tmp = buffer[i];
        if (tmp < 0) tmp += 256;
        n += tmp * std::pow(256,i);
        //printf("%i ", tmp);
    }
    //printf("n = %i\n", n);
    *number = n;
}

int Game::h_function(unsigned int x)
{
    double k1 = (double)x * 0.6311033345632;
    double k2 = fmod(k1, 1);
    return (floor(k2 * 25600.0));
}

void Game::set_options_object(Options* game_options_from_main)
{
    game_options = game_options_from_main;
    music_orbit.setVolume(game_options->music_volume);
    game_options->ptr_music_game = &music_orbit;
}

void Game::draw_box(int* area_coord, sf::Color color)
{
    sf::RectangleShape rectangle(sf::Vector2f(area_coord[3] - area_coord[2], area_coord[1] - area_coord[0]));
    rectangle.setFillColor(color);
    rectangle.setPosition(area_coord[2], area_coord[0]);
    game_window->draw(rectangle);
}

bool Game::check_prices(unsigned int* prices)
{
    //sf::sleep(sf::milliseconds(100));
    //printf("check %i=%i %i=%i %i=%i %i=%i (%i)\n", resource_money, prices[0], resource_metal, prices[1], resource_crystal, prices[2], resource_uran, prices[3] , (resource_money >= prices[0] && resource_metal >= prices[1] && resource_crystal >= prices[2] && resource_uran >= prices[3]));
    return (resource_money >= prices[0] && resource_metal >= prices[1] && resource_crystal >= prices[2] && resource_uran >= prices[3]);
}

void Game::spend_money(unsigned int* prices)
{
    resource_money -= prices[0];
    resource_metal -= prices[1];
    resource_crystal -= prices[2];
    resource_uran -= prices[3];
    printf("checkuot!\n");
}

void Game::draw_orbit_interface(double kx, double ky)
{
    game_window->clear(sf::Color::Black);
    bg_Planet_Sprite.setScale(kx, ky);
    game_window->draw(bg_Planet_Sprite);
    bg_SpaceShip_Sprite.setScale(kx, ky);
    game_window->draw(bg_SpaceShip_Sprite);
    Rover_Sprite.setScale(kx, ky);
    game_window->draw(Rover_Sprite);
    suit_Sprite.setScale(kx, ky);
    game_window->draw(suit_Sprite);
    Interface_Sprite.setScale(kx, ky);
    game_window->draw(Interface_Sprite);

    //рисуем данные по ресурсам
    text.setCharacterSize(45); // in pixels, not points!

    //деньги
    text.setFillColor(sf::Color(153,203,253,255));
    text.setStyle(sf::Text::Bold);
    char buffer[50];
    sprintf_s(buffer, "%i", resource_money);
    text.setPosition(949.0 * kx, 18.0 * ky);
    text.setString(buffer);
    text.setOrigin(0, 0);
    game_window->draw(text);
    //уран
    text.setFillColor(sf::Color(153, 203, 253, 255));
    text.setStyle(sf::Text::Bold);
    sprintf_s(buffer, "%i", resource_uran);
    text.setPosition(1210.0 * kx, 18.0 * ky);
    text.setString(buffer);
    text.setOrigin(0, 0);
    game_window->draw(text);
    //кристаллы
    text.setFillColor(sf::Color(153, 203, 253, 255));
    text.setStyle(sf::Text::Bold);
    sprintf_s(buffer, "%i", resource_crystal);
    text.setPosition(1461.0 * kx, 18.0 * ky);
    text.setString(buffer);
    text.setOrigin(0, 0);
    game_window->draw(text);
    //металл
    text.setFillColor(sf::Color(153, 203, 253, 255));
    text.setStyle(sf::Text::Bold);
    sprintf_s(buffer, "%i", resource_metal);
    text.setPosition(1717.0 * kx, 18.0 * ky);
    text.setString(buffer);
    text.setOrigin(0, 0);
    game_window->draw(text);

    //рисуем данные по планете
    sf::RectangleShape rectangle(sf::Vector2f(340.0,180.0));
    rectangle.setFillColor(sf::Color(0, 0, 0, 128));
    rectangle.setPosition(0,330);
    rectangle.setRotation(5);
    game_window->draw(rectangle);


    text.setFillColor(sf::Color(192,192,192,128));
    text.setStyle(sf::Text::Bold);
    sprintf_s(buffer, "Planet level   %i", planet_level);
    text.setOrigin(0, 0);
    text.setPosition(28.0 * kx, 264.0 * ky);
    text.setString(buffer);
    text.setRotation(5);
    game_window->draw(text);

    text.setFillColor(sf::Color(192, 192, 192, 128));
    text.setStyle(sf::Text::Bold);
    sprintf_s(buffer, "Planet type   %i", planet_type);
    text.setPosition(28.0 * kx, 324.0 * ky);
    text.setString(buffer);
    text.setOrigin(0, 0);
    text.setRotation(5);
    game_window->draw(text);

    text.setRotation(0);
}

double Game::calculate_damage(int weapon_type, int weapon_color, int weapon_level)
{
    double damage = 0.0;
    //определяем базовый уровень
    if (weapon_type == 0) damage = 1.0;
    if (weapon_type == 1) damage = 0.5;
    if (weapon_type == 2) damage = 2.0;
    if (weapon_type == 3) damage = 1.3;
    if (weapon_type == 4) damage = 1.2;
    if (weapon_type == 5) damage = 1.1;

    damage *= (1.0 + weapon_color * 0.08);
    damage *= (1.0 + weapon_level * 0.02);
    return damage;
}

double Game::calculate_fire_rate(int weapon_type, int weapon_color, int weapon_level)
{
    double fire_rate = 0.0;
    //определяем базовый уровень
    if (weapon_type == 0) fire_rate = 2.0;
    if (weapon_type == 1) fire_rate = 4.0;
    if (weapon_type == 2) fire_rate = 1.0;
    if (weapon_type == 3) fire_rate = 1.8;
    if (weapon_type == 4) fire_rate = 1.9;
    if (weapon_type == 5) fire_rate = 2.0;

    fire_rate *= (1.0 + weapon_color * 0.05);
    fire_rate *= (1.0 + weapon_level * 0.01);
    //printf("%0.0f\n", fire_rate);
    //sf::sleep(sf::seconds(1));
    return fire_rate;
}

void Game::get_weapon_name(int weapon_type, wchar_t *buffer)
{
    // размер буфера 50
    wchar_t buf[50];
    if (weapon_type == 0) swprintf_s(buf, L"Пушка");
    if (weapon_type == 1) swprintf_s(buf, L"Пулемет");
    if (weapon_type == 2) swprintf_s(buf, L"Ракетница");
    if (weapon_type == 3) swprintf_s(buf, L"Плазмаган");
    if (weapon_type == 4) swprintf_s(buf, L"Пушка Тесла");
    if (weapon_type == 5) swprintf_s(buf, L"Замораживатель");

    for (int i = 0; i < 50; ++i) buffer[i] = buf[i];
}

void Game::get_weapon_quality_name(int color, wchar_t *buffer)
{
    // размер буфера 50
    wchar_t buf[50];
    if (color == 0) swprintf_s(buf, L"Обычное[0]");
    if (color == 1) swprintf_s(buf, L"Хорошее[1]");
    if (color == 2) swprintf_s(buf, L"Отличное[2]");
    if (color == 3) swprintf_s(buf, L"Супер-пупер[3]");
    if (color == 4) swprintf_s(buf, L"Божественное[4]");

    for (int i = 0; i < 50; ++i) buffer[i] = buf[i];
}

sf::Color Game::get_color_by_weapon_quality(int quality)
{
    if (quality == 0) return sf::Color(210,210,210,255);
    if (quality == 1) return sf::Color(0, 255, 0, 255);
    if (quality == 2) return sf::Color(0, 0, 255, 255);
    if (quality == 3) return sf::Color(255, 0, 255, 255);
    return sf::Color(255, 255, 0, 255);
    
}