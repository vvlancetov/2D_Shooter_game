#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Custom_def.h"
#include "Menu.h"
#include <fstream>
#include <windows.h>
#include <debugapi.h>
#include <stdio.h>
#include <string.h>

Menu::Menu(sf::RenderWindow* window, std::string path)
{
	//конструктор класса
	//OutputDebugString(L"Запущен конструктор меню\n");
	game_window = window;
	std::string game_path = path;
	Current_Menu = Menu_states::MAIN_MENU;
	
	//буфер для ложных кликов
	click_block = true;

	//пауза ли сейчас
	isPause = false;

	//загрузка текстур
	bg_main_menu_Texture.loadFromFile(game_path + "Assets\\Pictures\\Menu_Background_picture.png");
	bg_main_menu_Sprite.setTexture(bg_main_menu_Texture);

	bg_options_menu_Texture.loadFromFile(game_path + "Assets\\Pictures\\Menu_Background_picture_2.png");
	bg_options_menu_Sprite.setTexture(bg_options_menu_Texture);

	bg_pause_menu_Texture.loadFromFile(game_path + "Assets\\Pictures\\Pause_Background_picture.png");
	bg_pause_menu_Sprite.setTexture(bg_pause_menu_Texture);

	main_menu_Texture.loadFromFile(game_path + "Assets\\Pictures\\Main_Menu.png");
	main_menu_Sprite.setTexture(main_menu_Texture);

	options_menu_Texture.loadFromFile(game_path + "Assets\\Pictures\\Options_Menu.png");
	options_menu_Sprite.setTexture(options_menu_Texture);

	options_menu_Controls_Texture.loadFromFile(game_path + "Assets\\Pictures\\Options_Menu_Controls.png");
	options_menu_Controls_Sprite.setTexture(options_menu_Controls_Texture);

	options_menu_Audio_Texture.loadFromFile(game_path + "Assets\\Pictures\\Options_Menu_Audio.png");
	options_menu_Audio_Sprite.setTexture(options_menu_Audio_Texture);

	options_menu_Graphics_Texture.loadFromFile(game_path + "Assets\\Pictures\\Options_Menu_Graphics.png");
	options_menu_Graphics_Sprite.setTexture(options_menu_Graphics_Texture);

	options_menu_Language_Texture.loadFromFile(game_path + "Assets\\Pictures\\Language_Menu.png");
	options_menu_Language_Sprite.setTexture(options_menu_Language_Texture);

	pause_menu_Mission_Texture.loadFromFile(game_path + "Assets\\Pictures\\Pause_Menu_Mission.png");
	pause_menu_Mission_Sprite.setTexture(pause_menu_Mission_Texture);

	pause_menu_Orbit_Texture.loadFromFile(game_path + "Assets\\Pictures\\Pause_Menu_Orbit.png");
	pause_menu_Orbit_Sprite.setTexture(pause_menu_Orbit_Texture);

	mouse_pointer_Texture.loadFromFile(game_path + "Assets\\Pointers\\Mouse_pointer_1.png");
	mouse_pointer_Sprite.setTexture(mouse_pointer_Texture);

};

int Menu::run(bool pause, bool on_orbit)
{

	//флаг вызова из игры (пауза)
	isPause = pause;

	//запрашиваем размеры окна
	sf::Vector2u new_win_size = game_window->getSize();
	double kx = new_win_size.x / 1920.0;
	double ky = new_win_size.y / 1080.0;
	
	//основной цикл меню
	
	//Главное меню (вне паузы)
	if (Current_Menu == Menu_states::MAIN_MENU && !isPause)
	{
		
		//рисуем фон
		bg_main_menu_Sprite.setScale(kx,  ky);
		main_menu_Sprite.setScale(kx, ky);
		
		game_window->clear(sf::Color::Black);
		game_window->draw(bg_main_menu_Sprite);
		game_window->draw(main_menu_Sprite);

		//задаем координаты областей
		int Continue_area[4] = { int(200.0 * ky), int(300.0 * ky), int(500.0 * kx), int(1420.0 * kx) };//top,bottom,left,right
		int New_game_area[4] = { int(300.0 * ky), int(400.0 * ky), int(500.0 * kx), int(1420.0 * kx) };//top,bottom,left,right
		int Options_area[4]  = { int(400.0 * ky), int(500.0 * ky), int(500.0 * kx), int(1420.0 * kx) };
		int Language_area[4] = { int(500.0 * ky), int(600.0 * ky), int(500.0 * kx), int(1420.0 * kx) };
		int Exit_area[4]     = { int(600.0 * ky), int(700.0 * ky), int(500.0 * kx), int(1420.0 * kx) };

		//проверяем координаты мыши
		// get the local mouse position (relative to a window)
		sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);
		//рисуем курсор
		mouse_pointer_Sprite.setPosition((float)localPosition.x, (float)localPosition.y);
		game_window->draw(mouse_pointer_Sprite);
		
		//проверяем нахождение мыши над пунктами меню
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Continue_area[0]))
			{
				sf::RectangleShape rectangle(sf::Vector2f(Continue_area[3] - Continue_area[2], Continue_area[1] - Continue_area[0]));
				rectangle.setFillColor(sf::Color(0, 0, 0, 128));
				rectangle.setPosition(Continue_area[2], Continue_area[0]);
				game_window->draw(rectangle);
			}
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &New_game_area[0])) 
			{
				sf::RectangleShape rectangle(sf::Vector2f(New_game_area[3] - New_game_area[2], New_game_area[1] - New_game_area[0]));
				rectangle.setFillColor(sf::Color(0, 0, 0, 128));
				rectangle.setPosition(New_game_area[2], New_game_area[0]);
				game_window->draw(rectangle);
			}
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Options_area[0]))
			{
				sf::RectangleShape rectangle(sf::Vector2f(Options_area[3] - Options_area[2], Options_area[1] - Options_area[0]));
				rectangle.setFillColor(sf::Color(0, 0, 0, 128));
				rectangle.setPosition(Options_area[2], Options_area[0]);
				game_window->draw(rectangle);
			}
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Language_area[0]))
			{
				sf::RectangleShape rectangle(sf::Vector2f(Language_area[3] - Language_area[2], Language_area[1] - Language_area[0]));
				rectangle.setFillColor(sf::Color(0, 0, 0, 128));
				rectangle.setPosition(Language_area[2], Language_area[0]);
				game_window->draw(rectangle);
			}
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0]))
			{
				sf::RectangleShape rectangle(sf::Vector2f(Exit_area[3] - Exit_area[2], Exit_area[1] - Exit_area[0]));
				rectangle.setFillColor(sf::Color(0, 0, 0, 128));
				rectangle.setPosition(Exit_area[2], Exit_area[0]);
				game_window->draw(rectangle);
			}

		game_window->display();

		//проверяем проверяем нажатие LMB
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click_block)
		{
			
			// проверяем нажатие на кнопки
			
				if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Continue_area[0]))
				{
					click_block = true;
					return (1); //продолжить игру
				}
				
				if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &New_game_area[0]))
				{
					click_block = true;
					return (2); //новая игра
				}
				if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Options_area[0])) //переключение на меню опций
				{
					click_block = true;
					Current_Menu = Menu_states::OPTIONS_MENU;
					return(0);//продолжать
				}
				if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Language_area[0])) //переключение на меню опций
				{
					click_block = true;
					Current_Menu = Menu_states::LANGUAGE_MENU;
					return(0);//продолжать
				}
				if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0]))
				{
					click_block = true;
					return(3); //выход на рабочий стол
				}
			
		}

		//снимаем блокировку ввода если нет нажатий
		if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
		{
			click_block = false;
		}

		return (0);//продолжать
	}
	
	//Меню настроек
	if (Current_Menu == Menu_states::OPTIONS_MENU)
	{
		//мы попали в меню настроек
		game_window->clear(sf::Color::Black);
		if (!isPause)
		{
			bg_options_menu_Sprite.setScale(new_win_size.x / 1920.0, new_win_size.y / 1080.0);
			game_window->draw(bg_options_menu_Sprite);
		}
		else
		{
			bg_pause_menu_Sprite.setScale(new_win_size.x / 1920.0, new_win_size.y / 1080.0);
			game_window->draw(bg_pause_menu_Sprite);
		}
		options_menu_Sprite.setScale(new_win_size.x / 1920.0, new_win_size.y / 1080.0);
		game_window->draw(options_menu_Sprite);
		
		//задаем координаты областей
		int Graphics_area[4] = { int(200.0 * ky), int(300.0 * ky), int(500.0 * kx), int(1420.0 * kx) };
		int Audio_area[4] = { int(300.0 * ky), int(400.0 * ky), int(500.0 * kx), int(1420.0 * kx) };
		int Controls_area[4] = { int(400.0 * ky), int(500.0 * ky), int(500.0 * kx), int(1420.0 * kx) };
		int Exit_area[4] = { int(500.0 * ky), int(600.0 * ky), int(500.0 * kx), int(1420.0 * kx) };
		
		
		//проверяем координаты мыши
		sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);
		//рисуем курсор
		mouse_pointer_Sprite.setPosition((float)localPosition.x, (float)localPosition.y);
		game_window->draw(mouse_pointer_Sprite);

		//проверяем попадание по пунктам меню
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Graphics_area[0])) draw_box(&Graphics_area[0], sf::Color(0, 0, 0, 128));
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Audio_area[0])) draw_box(&Audio_area[0], sf::Color(0, 0, 0, 128));
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Controls_area[0]))  draw_box(&Controls_area[0], sf::Color(0, 0, 0, 128));
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0]))  draw_box(&Exit_area[0], sf::Color(0, 0, 0, 128));
		game_window->display();

		//проверяем проверяем нажатие LMB
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click_block)
		{
			// проверяем проверяем попадание по кнопке
			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0])) 
			{
				click_block = true;
				Current_Menu = Menu_states::MAIN_MENU;//выход в главное меню по нажатию кнопки
			}

			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Graphics_area[0]))
			{
				click_block = true;
				Current_Menu = Menu_states::GRAPHICS_MENU;//выход в главное меню по нажатию кнопки
			}

			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Audio_area[0]))
			{
				click_block = true;
				Current_Menu = Menu_states::AUDIO_MENU;//выход в главное меню по нажатию кнопки
			}

			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Controls_area[0]))
			{
				click_block = true;
				Current_Menu = Menu_states::CONTROLS_MENU;//выход в главное меню по нажатию кнопки
			}

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !click_block)
		{
			click_block = true;
			Current_Menu = Menu_states::MAIN_MENU;//выход в главное меню по кнопке ESC
		}
		
		//снимаем блокировку двойных нажатий
		if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))) click_block = false;
		return (0);//продолжать
	}
	
	//Главное меню в режиме паузы
	if (Current_Menu == Menu_states::MAIN_MENU && isPause)
	{
		//главное меню в режиме паузы
		game_window->clear(sf::Color::Black);
		bg_pause_menu_Sprite.setScale(kx, ky);
		game_window->draw(bg_pause_menu_Sprite);
		
		if (on_orbit)
		{
			pause_menu_Orbit_Sprite.setScale(kx, ky);
			game_window->draw(pause_menu_Orbit_Sprite);
		}
		else 
		{
			pause_menu_Mission_Sprite.setScale(kx, ky);
			game_window->draw(pause_menu_Mission_Sprite);
		}

		//задаем координаты областей
		int Continue_area[4] = { int(200.0 * ky), int(300.0 * ky), int(500.0 * kx), int(1420.0 * kx) };
		int Graphics_area[4] = { int(300.0 * ky), int(400.0 * ky), int(500.0 * kx), int(1420.0 * kx) };
		int Audio_area[4] = { int(400.0 * ky), int(500.0 * ky), int(500.0 * kx), int(1420.0 * kx) };
		int Control_area[4] = { int(500.0 * ky), int(600.0 * ky), int(500.0 * kx), int(1420.0 * kx) };
		int Exit_area[4]     = { int(600.0 * ky), int(700.0 * ky), int(500.0 * kx), int(1420.0 * kx) };

		//проверяем координаты мыши
		// get the local mouse position (relative to a window)
		sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);
		//рисуем курсор
		mouse_pointer_Sprite.setPosition((float)localPosition.x, (float)localPosition.y);
		game_window->draw(mouse_pointer_Sprite);

		//проверяем нахождение мыши над пунктами меню
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Continue_area[0])) draw_box(&Continue_area[0], sf::Color(0, 0, 0, 128));
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Graphics_area[0])) draw_box(&Graphics_area[0], sf::Color(0, 0, 0, 128));
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Audio_area[0])) draw_box(&Audio_area[0], sf::Color(0, 0, 0, 128));
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Control_area[0])) draw_box(&Control_area[0], sf::Color(0, 0, 0, 128));
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0])) draw_box(&Exit_area[0], sf::Color(0, 0, 0, 128));

		game_window->display();

		//проверяем проверяем нажатие LMB
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click_block)
		{
			// проверяем нажатие на кнопки
				if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Continue_area[0]))
				{
					click_block = true;
					return (1); //продолжить игру
				}

				if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Graphics_area[0]))
				{
					click_block = true;
					Current_Menu = Menu_states::GRAPHICS_MENU;
				}

				if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Audio_area[0]))
				{
					click_block = true;
					Current_Menu = Menu_states::AUDIO_MENU;
				}
				
				if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Control_area[0]))
				{
					click_block = true;
					Current_Menu = Menu_states::CONTROLS_MENU;
				}


				if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0]))
				{
					click_block = true;
					return(2); //выход в главное меню
				}

		}

		//проверяем нажатие ESC
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !click_block)
		{
			//OutputDebugString(L"Пауза x\n");
			//printf("%i", pause_delay_timer);
			//pause_delay_timer = 200; //включаем задержку заново
			click_block = true;
			return(1);				//снять паузу
		}

		//снимаем блокировку ввода если нет нажатий
		if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
		{
			click_block = false;
		}

		return (0);//продолжать
	}
	
	//подраздел графических настроек
	if (Current_Menu == Menu_states::GRAPHICS_MENU)
	{
		//мы попали в меню графических настроек
		game_window->clear(sf::Color::Black);
		if (!isPause) {
			bg_options_menu_Sprite.setScale(kx, ky);
			game_window->draw(bg_options_menu_Sprite);
		}
		else 
		{
			bg_pause_menu_Sprite.setScale(kx, ky);
			game_window->draw(bg_pause_menu_Sprite); 
		}

		options_menu_Graphics_Sprite.setScale(new_win_size.x / 1920.0, new_win_size.y / 1080.0);
		game_window->draw(options_menu_Graphics_Sprite);

		//задаем координаты областей
		int Opt_0[4] = { int(310.0 * ky), int(356.0 * ky), int(546.0 * kx), int(592.0 * kx) };
		int Opt_1[4] = { int(390.0 * ky), int(436.0 * ky), int(546.0 * kx), int(592.0 * kx) };
		int Opt_2[4] = { int(470.0 * ky), int(516.0 * ky), int(546.0 * kx), int(592.0 * kx) };
		int Opt_3[4] = { int(551.0 * ky), int(596.0 * ky), int(546.0 * kx), int(592.0 * kx) };
		int Opt_4[4] = { int(630.0 * ky), int(676.0 * ky), int(546.0 * kx), int(592.0 * kx) };
		int Exit_area[4] = { int(725.0 * ky), int(815.0 * ky), int(500.0 * kx), int(1420.0 * kx) };
		
		//закрашиваем активные опции
		if (game_options->Videomode_4K_enable) draw_box(&Opt_0[0], sf::Color(51, 51, 51, 255));
		if (game_options->Videomode_2K_enable) draw_box(&Opt_1[0], sf::Color(51, 51, 51, 255));
		if (game_options->Videomode_FHD_enable) draw_box(&Opt_2[0], sf::Color(51, 51, 51, 255));
		if (game_options->Videomode_1600_enable) draw_box(&Opt_3[0], sf::Color(51, 51, 51, 255));
		if (game_options->Videomode_1280_enable) draw_box(&Opt_4[0], sf::Color(51, 51, 51, 255));

		//закрашиваем текущий режим
		if (game_options->current_video_mode == 0) draw_box(&Opt_0[0], sf::Color(0, 200, 200, 255));
		if (game_options->current_video_mode == 1) draw_box(&Opt_1[0], sf::Color(0, 200, 200, 255));
		if (game_options->current_video_mode == 2) draw_box(&Opt_2[0], sf::Color(0, 200, 200, 255));
		if (game_options->current_video_mode == 3) draw_box(&Opt_3[0], sf::Color(0, 200, 200, 255));
		if (game_options->current_video_mode == 4) draw_box(&Opt_4[0], sf::Color(0, 200, 200, 255));

		//проверяем координаты мыши
		sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);
		//рисуем курсор
		mouse_pointer_Sprite.setPosition((float)localPosition.x, (float)localPosition.y);
		game_window->draw(mouse_pointer_Sprite);

		//проверяем попадание по пунктам меню и подкрашиваем текущий
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Opt_0[0]) && game_options->Videomode_4K_enable) draw_box(&Opt_0[0], sf::Color(0, 0, 0, 100));
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Opt_1[0]) && game_options->Videomode_2K_enable) draw_box(&Opt_1[0], sf::Color(0, 0, 0, 100));
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Opt_2[0]) && game_options->Videomode_FHD_enable) draw_box(&Opt_2[0], sf::Color(0, 0, 0, 100));
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Opt_3[0]) && game_options->Videomode_1600_enable) draw_box(&Opt_3[0], sf::Color(0, 0, 0, 100));
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Opt_4[0]) && game_options->Videomode_1280_enable) draw_box(&Opt_4[0], sf::Color(0, 0, 0, 100));

		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0]))
		{
			sf::RectangleShape rectangle(sf::Vector2f(Exit_area[3] - Exit_area[2], Exit_area[1] - Exit_area[0]));
			rectangle.setFillColor(sf::Color(0, 0, 0, 128));
			rectangle.setPosition(Exit_area[2], Exit_area[0]);
			game_window->draw(rectangle);
		}

		game_window->display();

		//проверяем проверяем нажатие LMB
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click_block)
		{
			// проверяем проверяем попадание по кнопке
			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0]))
			{
				click_block = true;
				if (!isPause) Current_Menu = Menu_states::OPTIONS_MENU;//выход в главное меню по кнопке ESC
				if (isPause) Current_Menu = Menu_states::MAIN_MENU;//выход в меню паузы по кнопке ESC

			}
			
			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Opt_0[0]) && game_options->Videomode_4K_enable)
			{
				click_block = true;
				game_options->current_video_mode = 0;
				game_window->create(sf::VideoMode(3840 - 11, 2160 - 105), L"Крутая игра"); // , sf::Style::Titlebar | sf::Style::Close);
				game_window->setPosition({ (int)game_options->display_Wigth / 2 - 3840 / 2, (int)game_options->display_Height / 2 - 2160 / 2 });
				game_window->setFramerateLimit(60);
				game_window->setMouseCursorVisible(0);
				game_window->setKeyRepeatEnabled(0);
			}
			
			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Opt_1[0]) && game_options->Videomode_2K_enable)
			{
				click_block = true;
				game_options->current_video_mode = 1;
				if (!game_options->Videomode_4K_enable)
				{
					//оставляем отступы если это разрешение максимальное
					game_window->create(sf::VideoMode(2560 - 8, 1440 - 71), L"Крутая игра"); // , sf::Style::Titlebar | sf::Style::Close);
				}
				else
				{
					game_window->create(sf::VideoMode(2560, 1440), L"Крутая игра"); // , sf::Style::Titlebar | sf::Style::Close);
				}
				game_window->setPosition({ (int)game_options->display_Wigth / 2 - 2560 / 2, (int)game_options->display_Height / 2 - 1440 / 2 });
				game_window->setFramerateLimit(60);
				game_window->setMouseCursorVisible(0);
				game_window->setKeyRepeatEnabled(0);
			}

			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Opt_2[0]) && game_options->Videomode_FHD_enable)
			{
				click_block = true;
				game_options->current_video_mode = 2;
				if (!game_options->Videomode_2K_enable)
				{
					//оставляем отступы если это разрешение максимальное
					game_window->create(sf::VideoMode(1920 - 8, 1080 - 71), L"Крутая игра"); // , sf::Style::Titlebar | sf::Style::Close);
				}
				else
				{
					game_window->create(sf::VideoMode(1920, 1080), L"Крутая игра"); // , sf::Style::Titlebar | sf::Style::Close);
				}
				game_window->setPosition({ (int)game_options->display_Wigth / 2 - 1920 / 2, (int)game_options->display_Height / 2 - 1080 / 2 });
				game_window->setFramerateLimit(60);
				game_window->setMouseCursorVisible(0);
				game_window->setKeyRepeatEnabled(0);
			}
			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Opt_3[0]) && game_options->Videomode_1600_enable)
			{
				click_block = true;
				game_options->current_video_mode = 3;
				if (!game_options->Videomode_FHD_enable)
				{
					game_window->create(sf::VideoMode(1600 - 8, 900 - 71), L"Крутая игра"); // , sf::Style::Titlebar | sf::Style::Close);
				}
				else
				{
					game_window->create(sf::VideoMode(1600, 900), L"Крутая игра"); // , sf::Style::Titlebar | sf::Style::Close);
				}
				game_window->setPosition({ (int)game_options->display_Wigth / 2 - 1600 / 2, (int)game_options->display_Height / 2 - 900 / 2 });
				game_window->setFramerateLimit(60);
				game_window->setMouseCursorVisible(0);
				game_window->setKeyRepeatEnabled(0);
			}
			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Opt_4[0]) && game_options->Videomode_1280_enable)
			{
				click_block = true;
				game_options->current_video_mode = 4;
				if (!game_options->Videomode_1600_enable)
				{
					game_window->create(sf::VideoMode(1280 - 8, 720 - 71), L"Крутая игра"); // , sf::Style::Titlebar | sf::Style::Close);
				}
				else
				{
					game_window->create(sf::VideoMode(1280, 720), L"Крутая игра"); // , sf::Style::Titlebar | sf::Style::Close);
				}
				game_window->setPosition({ (int)game_options->display_Wigth / 2 - 1280 / 2, (int)game_options->display_Height / 2 - 720 / 2 });
				game_window->setFramerateLimit(60);
				game_window->setMouseCursorVisible(0);
				game_window->setKeyRepeatEnabled(0);
			}

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !click_block)
		{
			click_block = true;
			if (!isPause) Current_Menu = Menu_states::OPTIONS_MENU;//выход в главное меню по кнопке ESC
			if (isPause) Current_Menu = Menu_states::MAIN_MENU;//выход в меню паузы по кнопке ESC
		}

		//снимаем блокировку двойных нажатий
		if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))) click_block = false;
		return (0);//продолжать
	}

	//подраздел звука
	if (Current_Menu == Menu_states::AUDIO_MENU)
	{
		game_window->clear(sf::Color::Black);
		if (!isPause) {
			bg_options_menu_Sprite.setScale(kx, ky);
			game_window->draw(bg_options_menu_Sprite);
		}
		else
		{
			bg_pause_menu_Sprite.setScale(kx, ky);
			game_window->draw(bg_pause_menu_Sprite);
		}

		options_menu_Audio_Sprite.setScale(kx, ky);
		game_window->draw(options_menu_Audio_Sprite);

		//задаем координаты областей
		int Music_area[4] = { int(231.0 * ky), int(262.0 * ky), int(823.0 * kx), int(1413.0 * kx) };
		int Music_area_color[4] = { Music_area[0], Music_area[1], Music_area[2], Music_area[3] };
		Music_area_color[3] = Music_area[2] + ((double)game_options->music_volume / 100.0) * (Music_area[3] - Music_area[2]);

		int Sound_area[4] = { int(337.0 * ky), int(367.0 * ky), int(823.0 * kx), int(1413.0 * kx) };
		int Sound_area_color[4] = { Sound_area[0], Sound_area[1], Sound_area[2], Sound_area[3] };
		Sound_area_color[3] = Sound_area[2] + ((double)game_options->sound_volume / 100.0) * (Sound_area[3] - Sound_area[2]);

		int Exit_area[4] = { int(406.0 * ky), int(489.0 * ky), int(751.0 * kx), int(1178.0 * kx) };

		//проверяем координаты мыши
		// get the local mouse position (relative to a window)
		sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);
		//рисуем курсор
		mouse_pointer_Sprite.setPosition((float)localPosition.x, (float)localPosition.y);
		
		//отображаем шкалы
		draw_box(&Music_area_color[0], sf::Color(0, 255, 255, 255));
		draw_box(&Sound_area_color[0], sf::Color(0, 255, 255, 255));

		game_window->draw(mouse_pointer_Sprite);

		//проверяем нахождение мыши над пунктами меню
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0])) draw_box(&Exit_area[0], sf::Color(0, 0, 0, 128));

		game_window->display();

		//проверяем проверяем нажатие LMB
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click_block)
		{

			// проверяем нажатие на кнопки
			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0]))
				{
					click_block = true;
					if (!isPause) Current_Menu = Menu_states::OPTIONS_MENU;//выход в главное меню по кнопке ESC
					if (isPause) Current_Menu = Menu_states::MAIN_MENU;//выход в меню паузы по кнопке ESC
				}

			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Music_area[0]))
			{
				int mus_vol = floor(100.0 * (localPosition.x - Music_area[2]) / (Music_area[3] - Music_area[2] + 1)) + 1;
				game_options->music_volume = mus_vol;
				//регулируем музыку во всех объектах
				if(game_options->ptr_music_game) game_options->ptr_music_game->setVolume(mus_vol);
				if(game_options->ptr_music_menu) game_options->ptr_music_menu->setVolume(mus_vol);
				if(game_options->ptr_music_mission) game_options->ptr_music_mission->setVolume(mus_vol);
			}

			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Sound_area[0]))
			{
				int sound_vol = floor(100.0 * (localPosition.x - Sound_area[2]) / (Sound_area[3] - Sound_area[2] + 1)) + 1;
				game_options->sound_volume = sound_vol;
			}


		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !click_block)
		{
			click_block = true;
			if (!isPause) Current_Menu = Menu_states::OPTIONS_MENU;//выход в главное меню по кнопке ESC
			if (isPause) Current_Menu = Menu_states::MAIN_MENU;//выход в меню паузы по кнопке ESC

		}

		//снимаем блокировку двойных нажатий
		if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))) click_block = false;





		return (0);//продолжать

	}

	//подраздел настроек управления
	if (Current_Menu == Menu_states::CONTROLS_MENU)
	{
		game_window->clear(sf::Color::Black);
		if (!isPause) {
			bg_options_menu_Sprite.setScale(kx, ky);
			game_window->draw(bg_options_menu_Sprite);
		}
		else
		{
			bg_pause_menu_Sprite.setScale(kx, ky);
			game_window->draw(bg_pause_menu_Sprite);
		}

		options_menu_Controls_Sprite.setScale(kx, ky);
		game_window->draw(options_menu_Controls_Sprite);

		

		//задаем координаты областей
		int Exit_area[4] = { int(730.0 * ky), int(810.0 * ky), int(700.0 * kx), int(1230.0 * kx) };

		//проверяем координаты мыши
		// get the local mouse position (relative to a window)
		sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);
		//рисуем курсор
		mouse_pointer_Sprite.setPosition((float)localPosition.x, (float)localPosition.y);
		game_window->draw(mouse_pointer_Sprite);

		//проверяем нахождение мыши над пунктами меню
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0])) draw_box(&Exit_area[0], sf::Color(0, 0, 0, 128));

		game_window->display();

		//проверяем проверяем нажатие LMB
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click_block)
		{

			// проверяем нажатие на кнопки
		
				if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0]))
				{
					click_block = true;
					if (!isPause) Current_Menu = Menu_states::OPTIONS_MENU;//выход в главное меню по кнопке ESC
					if (isPause) Current_Menu = Menu_states::MAIN_MENU;//выход в меню паузы по кнопке ESC
				}
			
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !click_block)
		{
			click_block = true;
			if (!isPause) Current_Menu = Menu_states::OPTIONS_MENU;//выход в главное меню по кнопке ESC
			if (isPause) Current_Menu = Menu_states::MAIN_MENU;//выход в меню паузы по кнопке ESC

		}

		//снимаем блокировку двойных нажатий
		if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))) click_block = false;

		return (0);//продолжать



	}

	//подменю выболра языка
	if (Current_Menu == Menu_states::LANGUAGE_MENU)
	{
		game_window->clear(sf::Color::Black);
		bg_options_menu_Sprite.setScale(kx, ky);
		game_window->draw(bg_options_menu_Sprite);

		options_menu_Language_Sprite.setScale(kx, ky);
		game_window->draw(options_menu_Language_Sprite);

		//задаем координаты областей
		int RU_area[4] =	{ int(200.0 * ky), int(300.0 * ky), int(500.0 * kx), int(1420.0 * kx) };//top,bottom,left,right
		int CN_area[4] =	{ int(300.0 * ky), int(400.0 * ky), int(500.0 * kx), int(1420.0 * kx) };//top,bottom,left,right
		int EN_area[4] =	{ int(400.0 * ky), int(500.0 * ky), int(500.0 * kx), int(1420.0 * kx) };
		int SP_area[4] =	{ int(500.0 * ky), int(600.0 * ky), int(500.0 * kx), int(1420.0 * kx) };
		int Exit_area[4] =	{ int(600.0 * ky), int(700.0 * ky), int(500.0 * kx), int(1420.0 * kx) };

		//проверяем координаты мыши
		// get the local mouse position (relative to a window)
		sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);
		//рисуем курсор
		mouse_pointer_Sprite.setPosition((float)localPosition.x, (float)localPosition.y);
		game_window->draw(mouse_pointer_Sprite);

		//проверяем нахождение мыши над пунктами меню
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &RU_area[0]))		draw_box(&RU_area[0], sf::Color(0, 0, 0, 128));
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &CN_area[0]))		draw_box(&CN_area[0], sf::Color(0, 0, 0, 128));
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &EN_area[0]))		draw_box(&EN_area[0], sf::Color(0, 0, 0, 128));
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &SP_area[0]))		draw_box(&SP_area[0], sf::Color(0, 0, 0, 128));
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0]))	draw_box(&Exit_area[0], sf::Color(0, 0, 0, 128));

		game_window->display();

		//проверяем проверяем нажатие LMB
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !click_block)
		{

			// проверяем нажатие на кнопки
			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &RU_area[0]))
			{
				click_block = true;
				game_options->language = 0;
				Current_Menu = Menu_states::MAIN_MENU;//выход в меню паузы по кнопке ESC
			}
			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &CN_area[0]))
			{
				click_block = true;
				game_options->language = 1;
				Current_Menu = Menu_states::MAIN_MENU;//выход в меню паузы по кнопке ESC
			}
			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &EN_area[0]))
			{
				click_block = true;
				game_options->language = 2;
				Current_Menu = Menu_states::MAIN_MENU;//выход в меню паузы по кнопке ESC
			}
			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &SP_area[0]))
			{
				click_block = true;
				game_options->language = 3;
				Current_Menu = Menu_states::MAIN_MENU;//выход в меню паузы по кнопке ESC
			}
			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0]))
			{
				click_block = true;
				game_options->language = 4;
				Current_Menu = Menu_states::MAIN_MENU;//выход в меню паузы по кнопке ESC
			}

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !click_block)
		{
			click_block = true;
			Current_Menu = Menu_states::MAIN_MENU;//выход в меню паузы по кнопке ESC

		}

		//снимаем блокировку двойных нажатий
		if (!(sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))) click_block = false;

		return (0);//продолжать



	}

	return(0);
};

int Menu::check_mouse_hover(int x, int y, int *area)
{
	if (y > area[0] && y < area[1] && x > area[2] && x< area[3]) return 1;
	return 0;
}

void Menu::set_options_object(Options* game_options_from_main)
{
	game_options = game_options_from_main;
	
}

void Menu::draw_box(int* area_coord, sf::Color color)
{
	sf::RectangleShape rectangle(sf::Vector2f(area_coord[3] - area_coord[2], area_coord[1] - area_coord[0]));
	rectangle.setFillColor(color);
	rectangle.setPosition(area_coord[2], area_coord[0]);
	game_window->draw(rectangle);
}