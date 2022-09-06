#pragma once

class Menu
{
private:
	
	sf::RenderWindow* game_window;
	
	sf::Sprite bg_main_menu_Sprite;
	sf::Sprite bg_options_menu_Sprite;
	sf::Sprite mouse_pointer_Sprite;
	
	sf::Texture bg_main_menu_Texture;
	sf::Texture bg_options_menu_Texture;
	sf::Texture mouse_pointer_Texture;
	
	enum class Menu_states { MAIN_MENU, OPTIONS_MENU, GRAPHICS_MENU, SOUND_MENU, CONTROLS_MENU };
	Menu_states Current_Menu;
	int check_mouse_hover(int x, int y, int *area);

public:
	Menu(sf::RenderWindow* window);
	int run();
	
};