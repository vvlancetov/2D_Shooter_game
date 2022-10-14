#pragma once
#include "Mission.h"
#include "Menu.h"

class Game
{
private:

	sf::RenderWindow* game_window;
	double elapsed_ms = 0.0;
	
	Mission *current_mission;
	Menu *game_menu;

	//текстуры
	sf::Texture bg_Planet_Texture;
	sf::Sprite bg_Planet_Sprite;

	sf::Texture bg_SpaceShip_Texture;
	sf::Sprite bg_SpaceShip_Sprite;

	sf::Texture Rover_Texture;
	sf::Sprite Rover_Sprite;

	sf::Texture Interface_Texture;
	sf::Sprite Interface_Sprite;

	sf::Texture mouse_pointer_Texture;
	sf::Sprite mouse_pointer_Sprite;

	sf::Texture bg_mission_launch_Texture;
	sf::Sprite bg_mission_launch_Sprite;

	sf::Texture suit_Texture;
	sf::Sprite suit_Sprite;

	sf::Texture Dialog_Base_Texture;
	sf::Sprite Dialog_Base_Sprite;

	sf::Texture Dialog_Ship_Texture;
	sf::Sprite Dialog_Ship_Sprite;

	sf::Texture Dialog_Fly_Texture;
	sf::Sprite Dialog_Fly_Sprite;

	sf::Texture Dialog_Rover_Texture;
	sf::Sprite Dialog_Rover_Sprite;

	sf::Texture Dialog_Suit_Texture;
	sf::Sprite Dialog_Suit_Sprite;

	sf::Texture Screenshot_Texture;
	sf::Sprite Screenshot_Sprite;

	sf::Texture Planet_Jump_Texture;
	sf::Sprite Planet_Jump_Sprite;

	sf::Texture Dialog_weapon_Texture;
	sf::Sprite Dialog_weapon_Sprite;

	sf::Texture Dialog_Upgrade_Texture;
	sf::Sprite Dialog_Upgrade_Sprite;

	sf::Texture Dialog_Upgrade_Text_Texture;
	sf::Sprite Dialog_Upgrade_Text_Sprite;

	sf::Texture Dialog_Death_Texture;
	sf::Sprite Dialog_Death_Sprite;

	sf::Texture Dialog_Help1_Texture;
	sf::Sprite Dialog_Help1_Sprite;

	sf::Texture Dialog_Help2_Texture;
	sf::Sprite Dialog_Help2_Sprite;


	//картинки оружия
	sf::Sprite weapon_tile_Sprite;
	sf::Texture weapon_tile_Texture;

	//музыка
	sf::Music music_orbit;


	int render_res_X;
	int render_res_Y;

	bool GameExists = true;
	std::string game_path;
	bool show_Help_1 = true;
	bool show_Help_2 = true;

	//добытые ресурсы
	unsigned int resource_money = 0;
	unsigned int resource_metal = 0;
	unsigned int resource_crystal = 0;
	unsigned int resource_uran = 0;

	//данные текущей планеты
	unsigned int planet_number = 1;
	unsigned int planet_type = 1;
	unsigned int planet_level = 1;

	//уровни апгрейдов скафандра
	unsigned int suit_upgrades[6];
	//уровни апгрейдов вездехода
	unsigned int rover_upgrades[6];

	enum class GameStates { ORBIT, ORBIT_HELP_1, MISSION, MISSION_HELP_1, PAUSE_MISSION, PAUSE_ORBIT, BASE_MISSION, DEATH_MISSION, SHIP_MISSION, ROVER_ORBIT, SUIT_ORBIT, FLY_AWAY_ORBIT, TAKE_UPGRADE_MISSION, TAKE_WEAPON_MISSION };
	GameStates game_state;
	int check_mouse_hover(int x, int y, int* area);
	void draw_box(int* area_coord, sf::Color color);
	void draw_orbit_interface(double kx, double ky);
	void get_weapon_name(int weapon_type, wchar_t *buffer);
	void get_weapon_quality_name(int weapon_type, wchar_t* buffer);
	sf::Color get_color_by_weapon_quality(int quality);

	sf::Font font;
	sf::Text text;

	//блокировкщик двойных кликов
	bool click_block;

	//объект с опциями
	Options* game_options;

public:
	Game(sf::RenderWindow* window, std::string path, Menu *menu_object); //конструктор
	int Run(sf::Time elapsed);//основной цикл игры
	int Reset();//перезапуск игры
	bool check_game();
	bool Load();
	int Save();
	void int_to_char(unsigned int* number, char* buffer);
	void char_to_int(char* buffer, unsigned int* number);
	int h_function(unsigned int x);
	void set_options_object(Options* game_options);
	bool check_prices(unsigned int* prices);
	void spend_money(unsigned int* prices);
	double calculate_damage(int weapon_type, int weapon_color, int weapon_level);
	double calculate_fire_rate(int weapon_type, int weapon_color, int weapon_level);
};


