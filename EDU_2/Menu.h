#pragma once

class Menu
{
private:
	
	sf::RenderWindow* game_window;
	
	sf::Texture bg_main_menu_Texture;
	sf::Sprite bg_main_menu_Sprite;
	
	sf::Texture bg_options_menu_Texture;	
	sf::Sprite bg_options_menu_Sprite;	
	
	sf::Texture bg_pause_menu_Texture;
	sf::Sprite bg_pause_menu_Sprite;

	sf::Texture main_menu_Texture;
	sf::Sprite main_menu_Sprite;

	sf::Texture options_menu_Texture;
	sf::Sprite options_menu_Sprite;

	sf::Texture options_menu_Controls_Texture;
	sf::Sprite options_menu_Controls_Sprite;

	sf::Texture options_menu_Audio_Texture;
	sf::Sprite options_menu_Audio_Sprite;

	sf::Texture options_menu_Graphics_Texture;
	sf::Sprite options_menu_Graphics_Sprite;

	sf::Texture options_menu_Language_Texture;
	sf::Sprite options_menu_Language_Sprite;

	sf::Texture pause_menu_Texture;
	sf::Sprite pause_menu_Sprite;

	sf::Sprite mouse_pointer_Sprite;
	sf::Texture mouse_pointer_Texture;

	Options* game_options;
	
	
	enum class Menu_states { MAIN_MENU, OPTIONS_MENU, GRAPHICS_MENU, AUDIO_MENU, CONTROLS_MENU, LANGUAGE_MENU };
	Menu_states Current_Menu;
	int check_mouse_hover(int x, int y, int *area);

	bool click_block;
	bool isPause;
	void draw_box(int* area_coord, sf::Color color);

public:
	Menu(sf::RenderWindow* window, std::string game_path);
	int run(bool pause);
	void set_options_object(Options* game_options_from_main);
	
};

