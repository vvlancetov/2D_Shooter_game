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


	//музыка
	sf::Music music_orbit;


	int render_res_X;
	int render_res_Y;

	bool GameExists = false;
	std::string game_path;

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

	enum class GameStates { ORBIT, MISSION, PAUSE_MISSION, PAUSE_ORBIT, BASE_MISSION, SHIP_MISSION, ROVER_ORBIT, SUIT_ORBIT, FLY_AWAY_ORBIT};
	GameStates game_state;
	int check_mouse_hover(int x, int y, int* area);
	void draw_box(int* area_coord, sf::Color color);
	void draw_orbit_interface(double kx, double ky);

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
};


