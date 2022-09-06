#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Menu_engine.h"
#include <windows.h>
#include <debugapi.h>
#include <stdio.h>

Menu::Menu(sf::RenderWindow* window)
{
	//����������� ������
	//OutputDebugString(L"������� ����������� ����\n");
	game_window = window;

	Current_Menu = Menu_states::MAIN_MENU;
	
	//�������� �������
	bg_main_menu_Texture.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Pictures\\Menu_Background_1.png");
	bg_main_menu_Sprite.setTexture(bg_main_menu_Texture);

	mouse_pointer_Texture.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Pointers\\Mouse_pointer_1.png");
	mouse_pointer_Sprite.setTexture(mouse_pointer_Texture);

	bg_options_menu_Texture.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Pictures\\Menu_Background_2.png");
	bg_options_menu_Sprite.setTexture(bg_options_menu_Texture);

};

int Menu::run()
{
	//�������� ���� ����
	
	if (Current_Menu == Menu_states::MAIN_MENU)
	{
		//�� ������ � ������� ����
		game_window->clear(sf::Color::Black);
		game_window->draw(bg_main_menu_Sprite);

		//������ ���������� ��������
		int New_game_area[4] = { 200, 400, 300, 1620 };//top,bottom,left,right
		int Options_area[4] = { 400, 600, 300, 1620 };
		int Exit_area[4] = { 600, 800, 300, 1620 };

		//��������� ���������� ����
		// get the local mouse position (relative to a window)
		sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);
		//������ ������
		mouse_pointer_Sprite.setPosition((float)localPosition.x, (float)localPosition.y);
		game_window->draw(mouse_pointer_Sprite);
		
		//��������� ���������� ���� ��� �������� ����
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &New_game_area[0])) 
			{
				//OutputDebugString(L"������ - ����� ����\n");
			}
		
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Options_area[0]))
			{
				//OutputDebugString(L"������ - ���������\n");
			}

		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0])) 
			{
					//OutputDebugString(L"������ - �����\n");
			}

		game_window->display();

		//��������� ��������� ������� LMB
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			// ��������� ������� �� ������
			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &New_game_area[0])) return (1);
			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Options_area[0]))
			{
				Current_Menu = Menu_states::OPTIONS_MENU;
				return(0);
			}
			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0])) return(2);
		}
		return (0);//����������
	}


	if (Current_Menu == Menu_states::OPTIONS_MENU)
	{
		//�� ������ � ���� ��������
		game_window->clear(sf::Color::Black);
		//bg_menu_Sprite.setPosition(0, 0);
		//bg_menu_Sprite.setScale(1, 1);
		game_window->draw(bg_options_menu_Sprite);
		
		//������ ���������� ��������
		//int New_game_area[4] = { 200, 400, 300, 1620 };
		//int Options_area[4] = { 400, 600, 300, 1620 };
		int Exit_area[4] = { 800, 1000, 300, 1620 };
		
		
		//��������� ���������� ����
		// get the local mouse position (relative to a window)
		sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);
		//������ ������
		mouse_pointer_Sprite.setPosition((float)localPosition.x, (float)localPosition.y);
		game_window->draw(mouse_pointer_Sprite);

		//��������� ��������� �� ������� ����

		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0]))
		{
			//OutputDebugString(L"������ - �����\n");
		}

		game_window->display();

		//��������� ��������� ������� LMB
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			// ��������� ������� �� ������
			if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0])) Current_Menu = Menu_states::MAIN_MENU;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) Current_Menu = Menu_states::MAIN_MENU;//����� � ������� ���� �� ������ ESC


		return (0);//����������

	}

	return(0);
};

int Menu::check_mouse_hover(int x, int y, int *area)
{
	if (y > area[0] && y < area[1] && x > area[2] && x< area[3]) return 1;
	return 0;
}