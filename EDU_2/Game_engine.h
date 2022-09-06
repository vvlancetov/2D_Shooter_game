#pragma once


class Game
{
private:
	sf::RenderWindow* game_window;

	// фонова€ текстура
	sf::Sprite bg_game_Sprite;
	sf::Texture bg_game_Texture;
	
	// текстура дл€ тайлов поверхности
	sf::Sprite bg_tile_Sprite;
	sf::Texture bg_tile_Texture;
	
	//текстуры дл€ персонажа
	sf::Sprite bg_player_Sprite;
	sf::Texture bg_player_Texture;

	//текстура дл€ меню паузы
	sf::Texture pause_menu_Texture;
	sf::Sprite pause_menu_Sprite;

	//курсоры мыши
	sf::Sprite mouse_pointer_Sprite;
	sf::Texture mouse_pointer_Texture;

	sf::Sprite mouse_pointer_2_Sprite;
	sf::Texture mouse_pointer_2_Texture;

	//текстуры тела и оружи€
	sf::Sprite body_tile_Sprite;
	sf::Texture body_tile_Texture;

	sf::Sprite weapon_tile_Sprite;
	sf::Texture weapon_tile_Texture;

	//текстуры лута, оружи€ и тп
	sf::Texture loot_Texture;
	sf::Sprite loot_Sprite;

	//текстуры интефейса
	sf::Texture interface_Texture;
	sf::Sprite interface_Sprite;
	
	//текстура дл€ скриншотов
	sf::Texture screnshot_Texture;

	//текстуры диалога смены оборудовани€
	sf::Texture interface_equip_Texture;
	sf::Sprite interface_equip_Sprite;

	//текстуры пуль
	sf::Texture player_bullet_Texture;
	sf::Sprite player_bullet_Sprite;
	
	//текстуры монстров
	sf::Texture monster_Texture_array[5];
	sf::Sprite monster_Sprite_array[5];

	//текстуры эффектов
	sf::Texture VFX_Texture;
	sf::Sprite VFX_Sprite;

	//текстуры останков
	sf::Texture VFX_remains_Texture;
	sf::Sprite VFX_remains_Sprite;

	//текстуры пуль монстров
	sf::Texture monster_bullet_Texture;
	sf::Sprite monster_bullet_Sprite;

	//данные дл€ рендеринга
	double render_res_X;
	double render_res_Y;
	double player_max_offset_X;
	double player_max_offset_Y;
	int land_tile_size;
	double GAME_RENDER_SCALE;
	
	//прошедшее врем€
	int elapsed_ms;

	//границы посещенной игроком области
	double visited_min_X, visited_max_X, visited_min_Y, visited_max_Y;
	//границы сгенерированного лута
	double gen_loot_min_X, gen_loot_max_X, gen_loot_min_Y, gen_loot_max_Y;

	//таймеры 
	double pause_delay_timer; //таймер задержки при установке паузы
	double loot_gen_timer; //таймер генерации лута
	double monster_gen_timer; //таймер генерации лута

	
	//состо€ние игрока
	enum class Game_stage {PLAYER_INIT, FREE_RUN, INTERACTION, DYING};
	Game_stage Current_stage; // = Game_stage::PLAYER_INIT;

	//данные персонажа
	struct player
	{
		double Health;
		double Health_MAX;
		double Armor;
		double Armor_MAX;
		double Energy;
		double Energy_MAX;
		double Speed;
		double Speed_MAX;
		double fire_rate;		//выстрелы в секунду
		double gun_damage;		//урон от выстрела
		double player_coord_X;
		double player_coord_Y;	//координаты в мире
		double player_offset_X;
		double player_offset_Y;	//смещение от центра экрана
		int body_direction;		//направление движени€ и оружи€
		int weapon_direction;
		double animation_time;		//тайминги анимации в секундах
		double animation_time_MAX;
		double weapon_animation_time;		//тайминги анимации в секундах
		double weapon_animation_time_MAX;
		double weapon_direction_rad;
	};
	player Player_instance;
	
	//структуры дл€ хранени€ лута
	struct loot
	{
		int isValid;
		int type;// 0 - health, 1 - energy, 2 - health+energy
		int power; //сколько восстановитс€ 50 - 250
		double X; 
		double Y;
	};
	
	loot* loot_array;
	int loot_count;

	//массив точек эвакуации
	struct esc_point
	{
		int isValid;
		double X;
		double Y;
	};
	esc_point* array[8];

	//массив дл€ оборудовани€ (оружие, апгрейды, брон€)
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
	int equipment_to_take;//номер оборудовани€ дл€ подбора (переход в отдельную ветку)

	//массив монстров
	struct monster
	{
		char isAlive = 0;
		unsigned char sprite_N;
		unsigned char power;
		double moving_speed;
		double direction;
		unsigned char attack_type; //  0 - рукопашка/укус, 1 - обычна€ пул€, 2 - взрывна€ граната/ограниченный радиус
		double attack_time; //врем€ перезар€дки атаки
		double attack_cooldown;//счетчик
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
		unsigned char type; // 0 - рукопашка/укус, 1 - обычна€ пул€, 2 - взрывна€ граната/ограниченный радиус
		unsigned char sprite_N;
		double power;
		double speed;
		double direction;
		double ttl; //оставшеес€ врем€ жизни
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
		double X;
		double Y;
	};
	VFX_effect* VFX_array;
	int VFX_effect_count;

	//массив останков
	struct VFX_remains
	{
		char isAlive = 0; // 0 - свободный слот, 1 - зан€то
		unsigned char number;//номер в атласе. размер у всех 100*100
		double X;
		double Y;
	};
	VFX_remains* VFX_remains_array;
	int VFX_remains_pointer;

	sf::Font font;
	sf::Text text;

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
	void create_VFX(double VFX_X, double VFX_Y, int VFX_type);
	void draw_VFX();
	void create_VFX_remains(double VFX_X, double VFX_Y, int VFX_type);
	void draw_VFX_remains();
	void draw_monster_bullets();

public:
	Game(sf::RenderWindow *window);
	int run(sf::Time elapsed);
	int start();
	int pause(sf::Time elapsed);
	void set_render_resolution(int x, int y);
	void set_render_scale(double scale);
	int take_equipment();
};