#pragma once


class Mission
{
private:
	sf::RenderWindow* game_window;

	// фоновая текстура
	sf::Sprite bg_game_Sprite;
	sf::Texture bg_game_Texture;
	// текстура для тайлов поверхности
	sf::Sprite bg_tile_Sprite;
	sf::Texture bg_tile_Texture;
	//текстуры для персонажа
	sf::Sprite bg_player_Sprite;
	sf::Texture bg_player_Texture;
	//текстура для меню паузы
	sf::Texture pause_menu_Texture;
	sf::Sprite pause_menu_Sprite;
	//курсоры мыши
	sf::Sprite mouse_pointer_Sprite;
	sf::Texture mouse_pointer_Texture;
	sf::Sprite mouse_pointer_2_Sprite;
	sf::Texture mouse_pointer_2_Texture;
	//текстуры тела и оружия
	sf::Sprite body_tile_Sprite;
	sf::Texture body_tile_Texture;
	sf::Sprite weapon_tile_Sprite;
	sf::Texture weapon_tile_Texture;
	//текстуры лута, оружия и тп
	sf::Texture loot_Texture;
	sf::Sprite loot_Sprite;
	//текстуры интефейса
	sf::Texture interface_Texture;
	sf::Sprite interface_Sprite;
	//текстура для скриншотов
	sf::Texture screnshot_Texture;
	//текстуры диалога смены оборудования
	sf::Texture interface_equip_Texture;
	sf::Sprite interface_equip_Sprite;
	//текстуры пуль
	sf::Texture player_bullet_Texture;
	sf::Sprite player_bullet_Sprite;
	//текстуры монстров
	sf::Texture monster_Texture_1;
	sf::Sprite monster_Sprite;
	//текстуры эффектов
	sf::Texture VFX_Texture;
	sf::Sprite VFX_Sprite;
	//текстуры останков
	sf::Texture VFX_remains_Texture;
	sf::Sprite VFX_remains_Sprite;
	//текстуры пуль монстров
	sf::Texture monster_bullet_Texture;
	sf::Sprite monster_bullet_Sprite;
	//текстура базы
	sf::Texture base_Texture;
	sf::Sprite base_Sprite;
	//текстура корабля
	sf::Texture ship_Texture;
	sf::Sprite ship_Sprite;
	//текстура гнезда
	sf::Texture nest_Texture;
	sf::Sprite nest_Sprite;
	//спрайты диалогов
	sf::Texture death_dialog_Texture;
	sf::Sprite death_dialog_Sprite;
	sf::Texture evac_dialog_Texture;
	sf::Sprite evac_dialog_Sprite;
	sf::Texture base_dialog_Texture;
	sf::Sprite base_dialog_Sprite;

	//данные для рендеринга
	double render_res_X;
	double render_res_Y;
	double player_max_offset_X;
	double player_max_offset_Y;
	int land_tile_size;
	double GAME_RENDER_SCALE; //масштаб игры
	
	//прошедшее время
	int elapsed_ms;

	//путь к файлам
	std::string game_path;

	//границы посещенной игроком области
	double visited_min_X, visited_max_X, visited_min_Y, visited_max_Y;
	//границы сгенерированного лута
	double gen_loot_min_X, gen_loot_max_X, gen_loot_min_Y, gen_loot_max_Y;

	//таймеры 
	double pause_delay_timer; //таймер задержки при установке паузы
	double loot_gen_timer; //таймер генерации лута
	double monster_gen_timer; //таймер генерации лута
	
	//состояние игрока
	enum class Game_stage {PLAYER_INIT, FREE_RUN, INTERACTION, DYING};
	Game_stage Current_stage; // = Game_stage::PLAYER_INIT;

	//собранные на базе ресурсы
	unsigned int base_money;
	unsigned int base_metal;
	unsigned int base_crystal;
	unsigned int base_uran;

	//данные планеты
	unsigned int planet_type, planet_level;

	//данные персонажа
	struct player
	{
		double Health;
		double Health_MAX = 0;
		double Energy;
		double Energy_MAX = 0;
		double Generate_speed;
		double Speed;
		double Speed_MAX = 0;
		double Speed_direction;
		double fire_rate;		//выстрелы в секунду
		double gun_damage;		//урон от выстрела
		unsigned int slots = 0;  //слоты под артефакты
		double player_coord_X;
		double player_coord_Y;	//координаты в мире
		double player_offset_X;
		double player_offset_Y;	//смещение от центра экрана
		int body_direction;		//направление движения и оружия
		int weapon_direction;
		double animation_time;		//тайминги анимации в секундах
		double animation_time_MAX;
		double weapon_animation_time;		//тайминги анимации в секундах
		double weapon_animation_time_MAX;
		double weapon_direction_rad;
		int collected_money;
		int collected_metal;
		int collected_crystal;
		int collected_uran;
	};
	player Rover;
	
	//структуры для хранения лута
	struct loot
	{
		int isValid;
		int type;// 0 - health, 1 - energy, 2 - health+energy
		int power; //сколько восстановится 50 - 250
		double X; 
		double Y;
	};
	
	loot* loot_array;
	int loot_count;

	//массив точек интереса: база, точки эвакуации
	struct stationary_object
	{
		int isValid;
		int type;
		double size_X;
		double size_Y;
		double X;
		double Y;
		double Health;
	};
	stationary_object* obj_array;

	//массив для оборудования (оружие, апгрейды, броня)
	struct equipment
	{
		int isValid;
		int type; //0 - upgrade, 1 - weapon 
		int power;
		double X;
		double Y;
	};
	equipment* equipment_array;
	int equipment_count;
	int equipment_to_take;//номер оборудования для подбора (переход в отдельную ветку)

	//массив монстров
	struct monster
	{
		char isAlive = 0;
		char isJugger = 0;
		unsigned char sprite_N;
		double scale;
		int level;
		double anim_time_MAX;
		double anim_time;
		unsigned char power;
		double moving_speed;
		double direction;
		unsigned char attack_type; //  0 - рукопашка/укус, 1 - обычная пуля, 2 - взрывная граната/ограниченный радиус
		double attack_time; //время перезарядки атаки
		double attack_cooldown;//счетчик
		double attack_distance;//расстояние для атаки
		double health;
		double X;
		double Y;
	};
	monster* monster_array;
	int monster_count;

	//массив выстрелов игрока
	struct player_bullet
	{
		char isAlive = 0;
		unsigned char type;
		double power;
		double speed;
		double direction;
		double X;
		double Y;
		double ttl;
	};
	player_bullet* player_bullet_array;
	int player_bullet_count;
	double player_weapon_cooldown;

	//массив выстрелов монстров
	struct monster_bullet
	{
		char isAlive = 0;
		unsigned char type; // 0 - рукопашка/укус, 1 - обычная пуля, 2 - взрывная граната/ограниченный радиус
		unsigned char sprite_N;
		double power;
		double speed;
		double direction;
		double ttl; //оставшееся время жизни
		double X;
		double Y;
	};
	monster_bullet* monster_bullet_array;
	int monster_bullet_count;

	//массив эффектов
	struct VFX_effect
	{
		char isAlive = 0; // 0 - свободный слот, 1 - работающий эффект
		unsigned char number;//номер в атласе. размер у всех 100*100
		unsigned char last_frame;
		unsigned char keep_alive; // 0 или 1
		double total_time; //в микросекундах
		double remaining_time;
		unsigned char remains_num;//номер спрайта с останками если > 0
		double scale;
		double X;
		double Y;
	};
	VFX_effect* VFX_array;
	int VFX_effect_count;

	//массив останков
	struct VFX_remains
	{
		char isAlive = 0; // 0 - свободный слот, 1 - занято
		unsigned char number;//номер в атласе. размер у всех 100*100
		double X;
		double Y;
	};
	VFX_remains* VFX_remains_array;
	int VFX_remains_pointer;

	sf::Font font;
	sf::Text text;

	//музыка
	sf::Music music_action;

	//звуки
	sf::SoundBuffer buffer_gun_1; //пушка игрока
	sf::Sound sound_gun_1;

	sf::SoundBuffer buffer_bite_1; //укус монстра
	sf::Sound sound_bite_1;

	sf::SoundBuffer buffer_death_1; //смерть монстра
	sf::Sound sound_death_1;

	sf::SoundBuffer buffer_hit_1; //удар снаряда в металл
	sf::Sound sound_hit_1;

	sf::SoundBuffer buffer_hit_2; //удар снаряда в плоть
	sf::Sound sound_hit_2;

	sf::SoundBuffer buffer_explode_1; //взрыв
	sf::Sound sound_explode_1;

	sf::SoundBuffer buffer_monster_fire_1; //выстрел монстра
	sf::Sound sound_monster_fire_1;

	sf::SoundBuffer buffer_monster_fire_2; //выстрел монстра
	sf::Sound sound_monster_fire_2;
	
	//объект с опциями
	Options* game_options;


	//внутренние функции
	int check_mouse_hover(int x, int y, int* area);
	int rnd_from_coord(int x, int y, int range);
	int generate_loot();
	void generate_monsters();
	void draw_interface(sf::Vector2i localPosition);
	void draw_terrain();
	void draw_loot();
	void draw_equipment();
	void draw_character();
	void controls_processing(sf::Vector2i localPosition);
	void draw_bullets();
	void draw_monsters();
	void create_VFX(double VFX_X, double VFX_Y, int VFX_type, double scale);
	void draw_VFX();
	void create_VFX_remains(double VFX_X, double VFX_Y, int VFX_type);
	void draw_VFX_remains();
	void draw_monster_bullets();
	void make_explosion(double X, double Y, double power, bool player_damage, bool monster_damage);
	void make_stationary_obj();
	void draw_stationary_obj();
	void move_player(int acceleration);
	int check_obj_collision(double X, double Y, double offset);
	void reset_Rover();
	void reset_world();


public:
	Mission(sf::RenderWindow *window, std::string path);
	int run(sf::Time elapsed);
	int start();
	int pause(sf::Time elapsed);
	void set_render_resolution(int x, int y);
	void set_render_scale(double scale);
	int take_equipment();
	int evacuate();
	int in_base();
	int player_death();
	int get_collected_money();
	int get_collected_metal();
	int get_collected_crystal();
	int get_collected_uran();
	void set_options_object(Options* game_options, unsigned int planet_type, unsigned int planet_level, unsigned int *suit_upgrades, unsigned int *rover_upgrades);
	sf::Texture* getScreenshot();
	int get_player_health_prc();
	int get_player_energy_prc();
	void transfer_to_base();
	void transfer_res_to_orbit(unsigned int *res_arr);
};