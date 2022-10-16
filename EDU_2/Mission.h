#pragma once


class Mission
{
private:
	sf::RenderWindow* game_window;

	// ������� ��������
	sf::Sprite bg_game_Sprite;
	sf::Texture bg_game_Texture;
	// �������� ��� ������ �����������
	sf::Sprite bg_tile_Sprite;
	sf::Texture bg_tile_Texture;
	//�������� ��� ���������
	sf::Sprite bg_player_Sprite;
	sf::Texture bg_player_Texture;
	//�������� ��� ���� �����
	sf::Texture pause_menu_Texture;
	sf::Sprite pause_menu_Sprite;
	//������� ���� � ������ ���������
	sf::Sprite mouse_pointer_Sprite;
	sf::Texture mouse_pointer_Texture;
	sf::Sprite mouse_pointer_2_Sprite;
	sf::Texture mouse_pointer_2_Texture;
	sf::Sprite target_pointer_Sprite;
	sf::Texture target_pointer_Texture;


	//�������� ���� � ������
	sf::Sprite body_tile_Sprite;
	sf::Texture body_tile_Texture;
	sf::Sprite weapon_tile_Sprite;
	sf::Texture weapon_tile_Texture;
	//�������� ����, ������ � ��
	sf::Texture loot_Texture;
	sf::Sprite loot_Sprite;
	//�������� ���������
	sf::Texture interface_Texture;
	sf::Sprite interface_Sprite;
	//�������� ��� ����������
	sf::Texture screenshot_Texture;
	//�������� ������� ����� ������������
	sf::Texture interface_equip_Texture;
	sf::Sprite interface_equip_Sprite;
	//�������� ����
	sf::Texture player_bullet_Texture;
	sf::Sprite player_bullet_Sprite;
	//�������� ��������
	sf::Texture monster_Texture_1;
	sf::Sprite monster_Sprite;
	//�������� ��������
	sf::Texture VFX_Texture;
	sf::Sprite VFX_Sprite;
	//�������� ��������
	sf::Texture VFX_remains_Texture;
	sf::Sprite VFX_remains_Sprite;
	//�������� ���� ��������
	sf::Texture monster_bullet_Texture;
	sf::Sprite monster_bullet_Sprite;
	//�������� ����
	sf::Texture base_Texture;
	sf::Sprite base_Sprite;
	//�������� �������
	sf::Texture ship_Texture;
	sf::Sprite ship_Sprite;
	//�������� ������
	sf::Texture nest_Texture;
	sf::Sprite nest_Sprite;
	//�������� ������
	sf::Texture tires_Texture;
	sf::Sprite tires_Sprite;
	//�������� �������� ����
	sf::Texture shield_Texture;
	sf::Sprite shield_Sprite;

	//������� ��������
	sf::Texture death_dialog_Texture;
	sf::Sprite death_dialog_Sprite;
	sf::Texture evac_dialog_Texture;
	sf::Sprite evac_dialog_Sprite;
	sf::Texture base_dialog_Texture;
	sf::Sprite base_dialog_Sprite;

	//������� ���������
	sf::Texture int_upgrade_Texture;
	sf::Sprite int_upgrade_Sprite;

	//������ ��� ����������
	double render_res_X;
	double render_res_Y;
	double player_max_offset_X;
	double player_max_offset_Y;
	int land_tile_size;
	double GAME_RENDER_SCALE; //������� ����
	
	//��������� �����
	int elapsed_ms;

	//���� � ������
	std::string game_path;

	//������� ���������� ������� �������
	double visited_min_X, visited_max_X, visited_min_Y, visited_max_Y;
	//������� ���������������� ����
	double gen_loot_min_X, gen_loot_max_X, gen_loot_min_Y, gen_loot_max_Y;

	//������� 
	double pause_delay_timer; //������ �������� ��� ��������� �����
	double loot_gen_timer; //������ ��������� ����
	double monster_gen_timer; //������ ��������� ����
	
	//��������� ������
	enum class Game_stage {PLAYER_INIT, FREE_RUN, INTERACTION, DYING};
	Game_stage Current_stage; // = Game_stage::PLAYER_INIT;

	//��������� �� ���� �������
	unsigned int base_money;
	unsigned int base_metal;
	unsigned int base_crystal;
	unsigned int base_uran;

	//������ �������
	unsigned int planet_type, planet_level;

	//���� ��� ������ � ������ ������
	int first_launch = 1;

	//������ ���������
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
		double fire_rate_boost;		//�������� � �������
		double gun_damage;		//���� �� ��������
		unsigned int active_slots = 0;  //�������� �����
		int slots[3]{ -1 }; //������ ������ ��� ����������
		double player_coord_X;
		double player_coord_Y;	//���������� � ����
		double player_offset_X;
		double player_offset_Y;	//�������� �� ������ ������
		int body_direction;		//����������� �������� � ������
		double body_direction_rad;
		int weapon_direction;
		double animation_time;		//�������� �������� � ��������
		double animation_time_MAX;
		double weapon_animation_time;		//�������� �������� � ��������
		double weapon_animation_time_MAX;
		double weapon_direction_rad;
		unsigned int collected_money;
		unsigned int collected_metal;
		unsigned int collected_crystal;
		unsigned int collected_uran;
		WeaponData weapon; 
		double tires_XY[2];
		
	};
	player Rover;
	
	//��������� ��� �������� ����
	struct loot
	{
		int isValid;
		int type;// 0 - health, 1 - energy, 2 - health+energy, 3 - metal, 4 - crystal, 5 - uran, 6 - upgrade, 7 - weapon 
		int power; //������� ������������� 50 - 250, ���-�� ��������, ��� ������  � �.�.
		double X; 
		double Y;
	};
	
	loot* loot_array;
	int loot_count;
	
	//��������� ��� ����������� ����
	struct flying_loot
	{
		int isValid;
		int type;// 0 - health, 1 - energy, 2 - health+energy, 3 - metal, 4 - crystal, 5 - uran, 6 - upgrade, 7 - weapon 
		int power; //������� ������������� 50 - 250, ���-�� ��������, ��� ������  � �.�.
		double X;
		double Y;
		double dX;
		double dY;
		double dV;
		double V_Velocity;
		double ttl;
	};
	
	flying_loot* flying_loot_array;
	
	//������ ����� ��������: ����, ����� ���������, �����
	struct stationary_object
	{
		int isValid;
		int type;
		double size_X;
		double size_Y;
		double X;
		double Y;
		double Health;
		double Health_MAX;
		int spec_data;
	};
	stationary_object* obj_array;

	//������ ��� ������������ (������, ��������, �����)    DEL
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
	int weapon_to_take;//����� ������ ��� ������� (������� � ��������� �����)
	int upgrage_to_take;//����� �������� ��� ������� (������� � ��������� �����)
	
	//������ ��������
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
		unsigned char attack_type; //  0 - ���������/����, 1 - ������� ����, 2 - �������� �������/������������ ������
		double attack_time; //����� ����������� �����
		double attack_cooldown;//�������
		double attack_distance;//���������� ��� �����
		double health;
		double X;
		double Y;
		double chase_distance;
	};
	monster* monster_array;
	int monster_count;
	int sector_count[16];

	//������ ��������� ������
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
		int lives;
		int monters_hit[3];
	};
	player_bullet* player_bullet_array;
	int player_bullet_count;
	double player_weapon_cooldown;
	double player_weapon_cooldown_G;
	double player_shield_cooldown;

	//������ ��������� ��������
	struct monster_bullet
	{
		char isAlive = 0;
		unsigned char type; // 0 - ���������/����, 1 - ������� ����, 2 - �������� �������/������������ ������
		unsigned char sprite_N;
		double power;
		double speed;
		double direction;
		double ttl; //���������� ����� �����
		double X;
		double Y;
	};
	monster_bullet* monster_bullet_array;
	int monster_bullet_count;

	//������ ��������
	struct VFX_effect
	{
		char isAlive = 0; // 0 - ��������� ����, 1 - ���������� ������
		unsigned char number;//����� � ������. ������ � ���� 100*100
		unsigned char last_frame;
		unsigned char keep_alive; // 0 ��� 1
		double total_time; //� �������������
		double remaining_time;
		unsigned char remains_num;//����� ������� � ��������� ���� > 0
		double scale;
		double X;
		double Y;
	};
	VFX_effect* VFX_array;
	int VFX_effect_count;

	//������ ��������
	struct VFX_remains
	{
		char isAlive = 0; // 0 - ��������� ����, 1 - ������
		unsigned char number;//����� � ������. ������ � ���� 100*100
		double X;
		double Y;
		double rotation;
	};
	VFX_remains* VFX_remains_array;
	int VFX_remains_pointer;

	//������ ������
	struct Grenade
	{
		char isAlive = 0; // 0 - ��������� ����, 1 - ������
		double X;
		double Y;
		double V;
		double ttl;
		double dX;
		double dY;
		double V_Velocity;
		double power;
	};
	Grenade* grenades_array;

	sf::Font font;
	sf::Text text;

	//������
	sf::Music music_action;

	//�����
	sf::SoundBuffer buffer_gun_1; //����� ������
	sf::Sound sound_gun_1;
	sf::SoundBuffer buffer_gun_2; //����� ������
	sf::Sound sound_gun_2;
	sf::SoundBuffer buffer_gun_3; //����� ������
	sf::Sound sound_gun_3;
	sf::SoundBuffer buffer_gun_4; //����� ������
	sf::Sound sound_gun_4;
	sf::SoundBuffer buffer_gun_5; //����� ������
	sf::Sound sound_gun_5;

	sf::SoundBuffer buffer_bite_1; //���� �������
	sf::Sound sound_bite_1;

	sf::SoundBuffer buffer_death_1; //������ �������
	sf::Sound sound_death_1;

	sf::SoundBuffer buffer_hit_1; //���� ������� � ������
	sf::Sound sound_hit_1;

	sf::SoundBuffer buffer_hit_2; //���� ������� � �����
	sf::Sound sound_hit_2;

	sf::SoundBuffer buffer_hit_3; //���� �������������
	sf::Sound sound_hit_3;

	sf::SoundBuffer buffer_hit_4; //���� �����
	sf::Sound sound_hit_4;

	sf::SoundBuffer buffer_explode_1; //�����
	sf::Sound sound_explode_1;

	sf::SoundBuffer buffer_monster_fire_1; //������� �������
	sf::Sound sound_monster_fire_1;

	sf::SoundBuffer buffer_monster_fire_2; //������� �������
	sf::Sound sound_monster_fire_2;
	
	//������ � �������
	Options* game_options;

	//�������� ����� ������ ������
	double death_delay;

	//���������� �������
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
	void create_VFX_remains(double VFX_X, double VFX_Y, int VFX_type, double angle = 0.0);
	void draw_VFX_remains();
	void draw_monster_bullets();
	void make_explosion(double X, double Y, double power, bool player_damage, bool monster_damage);
	void generate_stationary_obj();
	void draw_stationary_obj();
	void move_player(int acceleration);
	int check_obj_collision(double X, double Y, double offset);
	
	void reset_world();
	void draw_flying_loot();
	void add_flying_loot(int type, int X, int Y);
	void spawn_monster(int i, double X, double Y);
	void draw_flying_grenades();
	void draw_shield();
	sf::Vector3f find_nearest_enemy();
	sf::Vector3f find_intersection(double rad, double X, double Y, double dist);
	sf::Vector3f find_line_cross(double X11, double Y11, double X12, double Y12, double X21, double Y21, double X22, double Y22);

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
	int get_rover_money();
	int get_collected_metal();
	int get_collected_crystal();
	int get_collected_uran();
	void set_options_object(Options* game_options, unsigned int planet_type, unsigned int planet_level, unsigned int *suit_upgrades, unsigned int *rover_upgrades);
	sf::Texture* getScreenshot();
	int get_player_health_prc();
	int get_player_energy_prc();
	void transfer_to_base();
	void transfer_res_to_orbit(unsigned int *res_arr);
	void get_resources(int *res_arr);
	int get_weapon_to_take();
	int get_upgrade_to_take();
	WeaponData get_weapon_data();
	void set_weapon(WeaponData weapon_data);
	int get_slots_number();
	void get_slots_array(int *slots_arr);
	void install_upgrade(int slot_num, int upgrade_type);
	void drop_flying_loot(int metal_drop, int crystal_drop, int uran_drop, double X, double Y);
	void reset_Rover();
	void repair(int price);
	void recharge(int price);
};