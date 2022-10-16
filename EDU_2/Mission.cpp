#pragma warning( disable : 4244 )
#pragma warning( disable : 4838 )

#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>
//#include <SFML/Audio/Music.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <debugapi.h>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include "Custom_def.h"
#include "Mission.h"
#include "Game.h"

Mission::Mission(sf::RenderWindow* window, std::string path)
{
	//конструктор класса
	//OutputDebugString(L"Запущен конструктор игры\n");
	//printf("%s \n", game_path);
	game_window = window;
	game_path = path;
	//printf("%f - width", window->getView().getSize().x);
	//printf("%f - height", window->getView().getSize().y);
	//набор данных для рендеринга окна
	render_res_X = floor(game_window->getView().getSize().x);
	render_res_Y = floor(game_window->getView().getSize().y);
	player_max_offset_Y = render_res_Y / 2 - 400;
	if (player_max_offset_Y < 0) player_max_offset_Y = 0;
	player_max_offset_X = render_res_X / 2 - 800;
	if (player_max_offset_X < 0) player_max_offset_X = 0;
	//GAME_RENDER_SCALE = 2.0;
	GAME_RENDER_SCALE = float((3.0 * 2560 / render_res_X) * 10.0) / 10.0;

	//прошедшее время в мс
	elapsed_ms = 0;

	//размер тайла
	land_tile_size = 100;

	//границы исследованных областей
	visited_min_X = 0; visited_max_X = 0; visited_min_Y = 0; visited_max_Y = 0;
	gen_loot_min_X = 0, gen_loot_max_X = 0, gen_loot_min_Y = 0, gen_loot_max_Y = 0;

	//переменная задержки (для паузы)
	pause_delay_timer = 200.0;
	//задержка генерации лута
	loot_gen_timer = 3000.0;
	//задержка генерации монстров
	monster_gen_timer = 1000.0;

	Current_stage = Game_stage::PLAYER_INIT;

	//загружаем текстуры
	{
		bg_player_Texture.loadFromFile(game_path + "Assets\\Textures\\Player_tiles.png");
		bg_player_Texture.setSmooth(1);
		bg_player_Sprite.setTexture(bg_player_Texture);

		bg_tile_Texture.loadFromFile(game_path + "Assets\\Textures\\Ground_tiles.png");
		//bg_tile_Texture.setSmooth(1);
		bg_tile_Sprite.setTexture(bg_tile_Texture);

		bg_game_Texture.loadFromFile(game_path + "Assets\\Pictures\\Starting_Background.png");
		bg_game_Sprite.setTexture(bg_game_Texture);

		pause_menu_Texture.loadFromFile(game_path + "Assets\\Pictures\\Pause_Menu.png");
		pause_menu_Sprite.setTexture(pause_menu_Texture);

		mouse_pointer_Texture.loadFromFile(game_path + "Assets\\Pointers\\Mouse_pointer_1.png");
		mouse_pointer_Sprite.setTexture(mouse_pointer_Texture);

		body_tile_Texture.loadFromFile(game_path + "Assets\\Textures\\Player_tiles.png");
		body_tile_Sprite.setTexture(body_tile_Texture);

		weapon_tile_Texture.loadFromFile(game_path + "Assets\\Textures\\Weapon_tiles.png");
		weapon_tile_Sprite.setTexture(weapon_tile_Texture);

		mouse_pointer_2_Texture.loadFromFile(game_path + "Assets\\Pointers\\Mouse_pointer_2.png");
		mouse_pointer_2_Sprite.setTexture(mouse_pointer_2_Texture);

		target_pointer_Texture.loadFromFile(game_path + "Assets\\Pointers\\Target_pointer.png");
		target_pointer_Sprite.setTexture(target_pointer_Texture);

		loot_Texture.loadFromFile(game_path + "Assets\\Textures\\Loot.png");
		loot_Sprite.setTexture(loot_Texture);

		interface_Texture.loadFromFile(game_path + "Assets\\Pictures\\Interface.png");
		interface_Texture.setSmooth(1);
		interface_Sprite.setTexture(interface_Texture);

		interface_equip_Texture.loadFromFile(game_path + "Assets\\Pictures\\Dialog_equipment.png");
		interface_equip_Sprite.setTexture(interface_equip_Texture);

		player_bullet_Texture.loadFromFile(game_path + "Assets\\Textures\\Player_Bullets.png");
		player_bullet_Sprite.setTexture(player_bullet_Texture);

		monster_Texture_1.loadFromFile(game_path + "Assets\\Textures\\Monsters_1.png");
		monster_Texture_1.setSmooth(1);
		monster_Sprite.setTexture(monster_Texture_1);

		//monster_Texture_array[1].loadFromFile(game_path + "Assets\\Textures\\Monsters_2.png");
		//monster_Sprite_array[1].setTexture(monster_Texture_array[1]);

		VFX_Texture.loadFromFile(game_path + "Assets\\Textures\\VFX.png");
		VFX_Sprite.setTexture(VFX_Texture);

		VFX_remains_Texture.loadFromFile(game_path + "Assets\\Textures\\Remains.png");
		VFX_remains_Sprite.setTexture(VFX_remains_Texture);

		monster_bullet_Texture.loadFromFile(game_path + "Assets\\Textures\\Monster_bullets.png");
		monster_bullet_Sprite.setTexture(monster_bullet_Texture);

		base_Texture.loadFromFile(game_path + "Assets\\Pictures\\base.png");
		base_Sprite.setTexture(base_Texture);

		ship_Texture.loadFromFile(game_path + "Assets\\Textures\\Spaceship.png");
		ship_Sprite.setTexture(ship_Texture);

		death_dialog_Texture.loadFromFile(game_path + "Assets\\Pictures\\Death_dialog.png");
		death_dialog_Sprite.setTexture(death_dialog_Texture);

		evac_dialog_Texture.loadFromFile(game_path + "Assets\\Pictures\\Evac_dialog.png");
		evac_dialog_Sprite.setTexture(evac_dialog_Texture);

		base_dialog_Texture.loadFromFile(game_path + "Assets\\Pictures\\Base_dialog.png");
		base_dialog_Sprite.setTexture(base_dialog_Texture);

		nest_Texture.loadFromFile(game_path + "Assets\\Pictures\\Nest.png");
		nest_Sprite.setTexture(nest_Texture);

		tires_Texture.loadFromFile(game_path + "Assets\\Textures\\Tires.png");
		tires_Sprite.setTexture(tires_Texture);

		int_upgrade_Texture.loadFromFile(game_path + "Assets\\Dialogs\\Upgrade_Text.png");
		int_upgrade_Sprite.setTexture(int_upgrade_Texture);

		shield_Texture.loadFromFile(game_path + "Assets\\Textures\\shield.png");
		shield_Sprite.setTexture(shield_Texture);
	}

	//загружаем шрифт
	if (!font.loadFromFile(game_path + "Assets\\Fonts\\arialnb.ttf")) printf("font load error\n");
	text.setFont(font); // font is a sf::Font

	//создаем создаем структуры данных
	{
		loot_array = (loot*)malloc(1000 * sizeof(loot));
		if (loot_array == NULL) printf("malloc error loot array");

		flying_loot_array = (flying_loot*)malloc(100 * sizeof(flying_loot));
		if (flying_loot_array == NULL) printf("malloc error flying_loot array");

		obj_array = (stationary_object*)malloc(100 * sizeof(stationary_object));
		if (obj_array == NULL) printf("malloc error obj array");

		equipment_array = (equipment*)malloc(1000 * sizeof(equipment));
		if (equipment_array == NULL) printf("malloc error equipment array");

		monster_array = (monster*)malloc(1000 * sizeof(monster));
		if (monster_array == NULL) printf("malloc error monster array");

		player_bullet_array = (player_bullet*)malloc(100 * sizeof(player_bullet));
		if (player_bullet_array == NULL) printf("malloc error p_bullet array");
		player_weapon_cooldown = 100;

		monster_bullet_array = (monster_bullet*)malloc(100 * sizeof(monster_bullet));
		if (monster_bullet_array == NULL) printf("malloc error m_bullet array");

		VFX_array = (VFX_effect*)malloc(100 * sizeof(VFX_effect));
		if (VFX_array == NULL) printf("malloc error VFX_effect array");

		VFX_remains_array = (VFX_remains*)malloc(1000 * sizeof(VFX_remains));
		if (VFX_remains_array == NULL) printf("malloc error VFX_remains array");

		grenades_array = (Grenade*)malloc(10 * sizeof(Grenade));
		if (grenades_array == NULL) printf("malloc error grenades_array array");
	}

	//открываем файл с музыкой
	if (!music_action.openFromFile(game_path + "Assets\\Music\\Mission music.ogg")) printf("Error load music\n");
	music_action.setLoop(true);
	music_action.setVolume(20);

	//открываем звуковые файлы
	{
		if (!buffer_gun_1.loadFromFile(game_path + "Assets\\Sound\\shot_sound_1.wav"))  printf("Error load shot_sound_1.wav\n");
		sound_gun_1.setBuffer(buffer_gun_1);//выстрел из пушки
		sound_gun_1.setVolume(50);

		if (!buffer_gun_2.loadFromFile(game_path + "Assets\\Sound\\shot_sound_2.wav"))  printf("Error load shot_sound_2.wav\n");
		sound_gun_2.setBuffer(buffer_gun_2);//выстрел из пушки
		sound_gun_2.setVolume(50);

		if (!buffer_gun_3.loadFromFile(game_path + "Assets\\Sound\\shot_sound_3.wav"))  printf("Error load shot_sound_3.wav\n");
		sound_gun_3.setBuffer(buffer_gun_3);//выстрел из пушки
		sound_gun_3.setVolume(50);

		if (!buffer_gun_4.loadFromFile(game_path + "Assets\\Sound\\shot_sound_4.wav"))  printf("Error load shot_sound_4.wav\n");
		sound_gun_4.setBuffer(buffer_gun_4);//выстрел из пушки
		sound_gun_4.setVolume(50);

		if (!buffer_gun_5.loadFromFile(game_path + "Assets\\Sound\\shot_sound_5.wav"))  printf("Error load shot_sound_5.wav\n");
		sound_gun_5.setBuffer(buffer_gun_5);//выстрел из пушки
		sound_gun_5.setVolume(50);

		if (!buffer_bite_1.loadFromFile(game_path + "Assets\\Sound\\bite_1.wav"))  printf("Error load bite_1.wav\n");
		sound_bite_1.setBuffer(buffer_bite_1);//укус монстра
		sound_bite_1.setVolume(50);

		if (!buffer_hit_1.loadFromFile(game_path + "Assets\\Sound\\hit_1.wav"))  printf("Error load hit_1.wav\n");
		sound_hit_1.setBuffer(buffer_hit_1);//удар снаряда в металл
		sound_hit_1.setVolume(50);

		if (!buffer_hit_2.loadFromFile(game_path + "Assets\\Sound\\hit_2.wav"))  printf("Error load hit_2.wav\n");
		sound_hit_2.setBuffer(buffer_hit_2);//удар снаряда в плоть
		sound_hit_2.setVolume(50);

		if (!buffer_hit_3.loadFromFile(game_path + "Assets\\Sound\\hit_3.wav"))  printf("Error load hit_3.wav\n");
		sound_hit_3.setBuffer(buffer_hit_3);//удар снаряда в плоть
		sound_hit_3.setVolume(50);

		if (!buffer_hit_4.loadFromFile(game_path + "Assets\\Sound\\hit_4.wav"))  printf("Error load hit_4.wav\n");
		sound_hit_4.setBuffer(buffer_hit_4);//удар снаряда в плоть
		sound_hit_4.setVolume(50);

		if (!buffer_death_1.loadFromFile(game_path + "Assets\\Sound\\death_1.wav"))  printf("Error load death_1.wav\n");
		sound_death_1.setBuffer(buffer_death_1);//смерть монстра
		sound_death_1.setVolume(50);

		if (!buffer_explode_1.loadFromFile(game_path + "Assets\\Sound\\explode_1.wav"))  printf("Error load explode_1.wav\n");
		sound_explode_1.setBuffer(buffer_explode_1);//взрыв
		sound_explode_1.setVolume(80);

		if (!buffer_monster_fire_1.loadFromFile(game_path + "Assets\\Sound\\monster_fire_1.wav"))  printf("Error load monster_fire_1.wav\n");
		sound_monster_fire_1.setBuffer(buffer_monster_fire_1);//выстрел монстра
		sound_monster_fire_1.setVolume(50);

		if (!buffer_monster_fire_2.loadFromFile(game_path + "Assets\\Sound\\monster_fire_2.wav"))  printf("Error load monster_fire_2.wav\n");
		sound_monster_fire_2.setBuffer(buffer_monster_fire_2);//выстрел монстра
		sound_monster_fire_2.setVolume(50);
	}

};

int Mission::run(sf::Time elapsed)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) return 0;
	
	//основной цикл игры
	render_res_X = floor(game_window->getView().getSize().x);
	render_res_Y = floor(game_window->getView().getSize().y);
	GAME_RENDER_SCALE = float((2.5 * 2560 / render_res_X) * 10.0) / 10.0;
	//запускаем музыку если она не играет
	if (music_action.getStatus() != sf::SoundSource::Status::Playing) music_action.play();

	//прошло времени с микросекундах
	elapsed_ms = elapsed.asMilliseconds();

	//подзарядка генератора
	Rover.Energy += Rover.Generate_speed / 1000.0 * elapsed_ms;
	if (Rover.Energy > Rover.Energy_MAX) Rover.Energy = Rover.Energy_MAX;

	//уменьшаем значения счетчиков
	if (pause_delay_timer > 0) pause_delay_timer -= elapsed_ms;
	loot_gen_timer -= elapsed_ms;
	if (loot_gen_timer <= 0) generate_loot(); //генерация лута
	monster_gen_timer -= elapsed_ms;
	if (monster_gen_timer <= 0) generate_monsters();//генерация монстров

	//сбрасываем взятые предметы
	weapon_to_take = - 1;
	upgrage_to_take = -1;

	//очистка окна
	game_window->clear(sf::Color::Black); //очистка окна

	//запрашиваем позицию курсора мыши
	sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);

	//рисуем территорию тайлами
	draw_terrain();

	//рисуем останки
	draw_VFX_remains();

	//рисуем корабли
	draw_stationary_obj();

	//рисуем лут
	draw_loot();

	//рисуем персонажа
	if (Rover.Health > 0) draw_character();

	//рисуем мобов
	draw_monsters();

	//рисуем разлетающиеся предметы
	draw_flying_loot();

	//рисуем выстрелы и эффекты
	draw_bullets();
	draw_monster_bullets();
	draw_VFX();

	//рисуем гранаты
	draw_flying_grenades();

	//рисуем щит
	if (Rover.Health > 0) draw_shield();

	//рисуем интерфейс
	draw_interface(localPosition);

	//game_window->display();
	//sf::sleep(sf::milliseconds(500));
	if (first_launch)
	{
		//делаем скриншот
		screenshot_Texture.create(render_res_X, render_res_Y);
		screenshot_Texture.update(*game_window);
		--first_launch;
		return 0;
	}
	
	game_window->display();

	//обрабатываем нажатия клавиш
	if (Rover.Health > 0) controls_processing(localPosition);

	//обработка нажатия ESC в самом конце
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && (Rover.Health > 0))
	{
		//делаем скриншот
		sf::Vector2u windowSize = game_window->getSize();
		screenshot_Texture.create(windowSize.x, windowSize.y);
		screenshot_Texture.update(*game_window);
		return(1);				//и ставим на паузу
	}
	if (weapon_to_take != -1 && (Rover.Health > 0))
	{
		//делаем скриншот экрана
		sf::Vector2u windowSize = game_window->getSize();
		screenshot_Texture.create(windowSize.x, windowSize.y);
		screenshot_Texture.update(*game_window);
		//printf("Screenshot!\n");
		return (2); //если берем оружие - меняем состояние игры
	}

	if (upgrage_to_take != -1 && (Rover.Health > 0))
	{
		//делаем скриншот экрана
		sf::Vector2u windowSize = game_window->getSize();
		screenshot_Texture.create(windowSize.x, windowSize.y);
		screenshot_Texture.update(*game_window);
		//printf("Screenshot!\n");
		return (6); //если берем upgrage - меняем состояние игры
	}


	//обработка нажатия кнопки Е

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && (Rover.Health > 0))
	{
		//активация объекта нажатием на Е
		int obj_near = check_obj_collision(Rover.player_coord_X, Rover.player_coord_Y, 300.0f);
		if (obj_near != 0)
			if (obj_array[obj_near - 1].type == 0)
			{
				//вход в базу
				//printf("[E] Enter Base");
				sf::Vector2u windowSize = game_window->getSize();
				screenshot_Texture.create(windowSize.x, windowSize.y);
				screenshot_Texture.update(*game_window);
				return (4);
			}

		if (obj_array[obj_near - 1].type == 1)
		{
			//рядом выход
			//printf("[E] Evacuate");
			sf::Vector2u windowSize = game_window->getSize();
			screenshot_Texture.create(windowSize.x, windowSize.y);
			screenshot_Texture.update(*game_window);
			//music_action.stop();//вырубаем музон
			return (5);
		}
	}

	//проверка здоровья
	if (Rover.Health <= 0) //смерть
	{
		if (death_delay == 3000)
		{
			printf("Mission - > Player dead\n");
			//раскидать ресурсы возле места смерти
			make_explosion(Rover.player_coord_X,Rover.player_coord_Y, 10, 0 ,1);
			drop_flying_loot(Rover.collected_metal, Rover.collected_crystal, Rover.collected_uran, Rover.player_coord_X, Rover.player_coord_Y);
			//обнуляем ресурсы в ровере
			Rover.collected_metal = 0; Rover.collected_crystal = 0; Rover.collected_uran = 0;
		}
		death_delay -= elapsed_ms;
		if (death_delay <= 0) 
		{
			sf::Vector2u windowSize = game_window->getSize();
			screenshot_Texture.create(windowSize.x, windowSize.y);
			screenshot_Texture.update(*game_window);
			return (7); //смерть
		}
	}



	return(0);//просто продолжаем цикл
};

int Mission::start()
{
	//инициализация игры
	//game_window->clear(sf::Color::Green);
	//game_window->draw(bg_game_Sprite);
	//game_window->display();
	generate_loot();
	sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);
	srand(1);//перезапускаем генератор RND
	reset_Rover();
	reset_world(); //перезапускаем мир
	return(1); //в случае успеха возвращаем 1, иначе 0
}

int Mission::check_mouse_hover(int x, int y, int* area)
{
	if (y > area[0] && y < area[1] && x > area[2] && x < area[3]) return 1;
	return 0;
}

void Mission::set_render_resolution(int x, int y)
{
	//удалить
	render_res_X = floor(game_window->getView().getSize().x);
	render_res_Y = floor(game_window->getView().getSize().y);
	player_max_offset_Y = render_res_Y / 2 - 400;
	if (player_max_offset_Y < 0) player_max_offset_Y = 0;
	player_max_offset_X = render_res_X / 2 - 800;
	if (player_max_offset_X < 0) player_max_offset_X = 0;

}

int Mission::rnd_from_coord(int x, int y, int range)
{
	//пытаемся получить рандомное число их 2-мерных координат
	int code_1 = ((double)abs(x + 11) / 17 * 73);
	int code_2 = ((double)abs(y + 29) / 13 * 101);
	code_1 = code_1 ^ 42;  //227
	code_2 = code_2 ^ 21;  //51213

	//	% 13 + (x % 5) + (y % 7) + (abs(y) * 13) % 17; //you can use any prime number
	return ((code_1 + code_2) % range); //Make sure it's in range 0-max

}

int Mission::generate_loot()
{

	loot_gen_timer = 1000;//переделать под константы

	//генерируем лут и апгрейды вокруг игрока

	int LOOT_ITEMS_TO_GEN = 10;
	int LOOT_ITEMS_ADDED = 0;

	//цикл генерации для лута
	for (int i = 0; i < LOOT_ITEMS_TO_GEN; ++i)
	{
		double NEW_X = (rand() * 4000.0 / RAND_MAX - 2000.0);// 4000  2000
		if (NEW_X < 0) NEW_X -= 120 * GAME_RENDER_SCALE; else NEW_X += 120 * GAME_RENDER_SCALE;  // 1200
		double NEW_Y = (rand() * 4000.0 / RAND_MAX - 2000.0);  //4000 2000
		if (NEW_Y < 0) NEW_Y -= 80 * GAME_RENDER_SCALE; else NEW_Y += 80 * GAME_RENDER_SCALE; //800
		NEW_X += (Rover.player_coord_X - Rover.player_offset_X);
		NEW_Y += (Rover.player_coord_Y - Rover.player_offset_Y);

		//если координаты не попадают в область уже генерированного лута - добавляем в массив

		if (NEW_X < gen_loot_min_X || NEW_X > gen_loot_max_X || NEW_Y < gen_loot_min_Y || NEW_Y > gen_loot_max_Y || 0)  //отключить потом
		{
			char type = 5 + floor(8.0 * rand() / (RAND_MAX+1)); //0 - health, 1 - energy, 2 - health+energy, 3 - metal, 4 - crystal, 5 - uran, 6 - upgrade, 7 - weapon
			int power_rnd = floor(100.0 * rand() / (RAND_MAX+1));
			unsigned int power = 0;
			if (type < 3)
			{
				//определяем мощность восстановителей
				if (power_rnd < 50) power = 50;
				else
				{
					if (power_rnd < 75) power = 100;
					else
					{
						if (power_rnd < 88) power = 150;
						else
						{
							if (power_rnd < 95) power = 200;
							else power = 250;
						}
					}
				}
			}
			
			if (type > 2 && type < 6)
			{
				//кол-во ресов
				power = 1 + floor(power_rnd / 50.0);
			}

			if (type == 6)
			{
				//апгрейд
				power = (int)floor(rand() * 5.0 / (RAND_MAX + 1)); //пулемет, ракета и т.п.;

			}

			if (type == 7)
			{
				//оружие
				int weapon_type = (int)floor(rand() * 6.0 / (RAND_MAX + 1)); //пулемет, ракета и т.п.
				int weapon_color = (int)floor(rand() * 5.0 / (RAND_MAX + 1)); //серое, золотое и т.п.
				power = weapon_type | (weapon_color << 8);
			}

			//добавляем данные в массив лута
			if (loot_count < 1000)
			{

				//ищем пустую ячейку в массиве
				int new_item = 0;
				for (new_item = 0; new_item < 1000 && loot_array[new_item].isValid == 1; ++new_item);
				loot_array[new_item].isValid = 1;
				loot_array[new_item].type = type;
				loot_array[new_item].power = power;
				loot_array[new_item].X = floor(NEW_X);
				loot_array[new_item].Y = floor(NEW_Y);
				//printf("loot added # %5i = %5i, %5i, (%0f, %0f) \n", new_item, type, power, NEW_X, NEW_Y);
				//обновляем информацию о границах области генерации лута
				if (NEW_X < gen_loot_min_X) gen_loot_min_X = NEW_X;
				if (NEW_X > gen_loot_max_X) gen_loot_max_X = NEW_X;
				if (NEW_Y < gen_loot_min_Y) gen_loot_min_Y = NEW_Y;
				if (NEW_Y > gen_loot_max_Y) gen_loot_max_Y = NEW_Y;
				//printf("min_X (%0f), max_X (%0f), min_Y (%0f), max_Y (%0f)) \n", gen_loot_min_X, gen_loot_max_X, gen_loot_min_Y, gen_loot_max_Y);
				loot_count++;
			}
			LOOT_ITEMS_ADDED++;
		}

	}


	return(LOOT_ITEMS_ADDED);
}

void Mission::draw_interface(sf::Vector2i localPosition)
{
	double kx = render_res_X / 1920.0;
	double ky = render_res_Y / 1080.0;
	
	interface_Sprite.setScale(kx, ky);
	
	//определяем включение автопилота
	bool autoPilot = false;
	for (int i = 0; i < Rover.active_slots; ++i)
	{
		if (Rover.slots[i] == 1) autoPilot = true;
	}

	//метка на цели
	if (autoPilot)
	{
		sf::Vector3f target = find_nearest_enemy();
		if (target.z >= 0)
		{
			target_pointer_Sprite.setScale(2.0 / GAME_RENDER_SCALE, 2.0 / GAME_RENDER_SCALE);
			target_pointer_Sprite.setPosition(target.x / GAME_RENDER_SCALE - (Rover.player_coord_X / GAME_RENDER_SCALE - Rover.player_offset_X - render_res_X / 2), Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y + render_res_Y / 2 - target.y / GAME_RENDER_SCALE);
			target_pointer_Sprite.setOrigin(50.0, 50.0);
			game_window->draw(target_pointer_Sprite);
			//printf("target cursor x=%0.0f y=%0.0f\n", target.x / GAME_RENDER_SCALE - (Rover.player_coord_X / GAME_RENDER_SCALE - Rover.player_offset_X - render_res_X / 2), Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y + render_res_Y / 2 - target.y / GAME_RENDER_SCALE);
		}
	}
	
	game_window->draw(interface_Sprite);

	//вывод текста в окно
	text.setCharacterSize(24); // in pixels, not points!
	text.setFillColor(sf::Color::Red);
	//text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	char buffer[50];
	sprintf_s(buffer, "%0.0f %0.0f monster=%i bullet=%i loot=%i", Rover.player_coord_X, Rover.player_coord_Y, monster_count, player_bullet_count, loot_count);
	text.setPosition(50.0, 50.0);
	text.setString(buffer);
	//game_window->draw(text);

	//рисуем здоровье
	if (Rover.Health < 0) Rover.Health = 0;
	sf::RectangleShape health_rectangle(sf::Vector2f(411.f * render_res_X / 1920.0 * Rover.Health / Rover.Health_MAX, 39.f * render_res_Y / 1080.0));
	health_rectangle.setFillColor(sf::Color::Green);
	health_rectangle.setPosition(29 * render_res_X / 1920.0, 955 * render_res_Y / 1080.0);
	game_window->draw(health_rectangle);

	//рисуем энергию
	if (Rover.Energy < 0) Rover.Energy = 0;
	sf::RectangleShape energy_rectangle(sf::Vector2f(411.f * render_res_X / 1920.0 * Rover.Energy / Rover.Energy_MAX, 39.f * render_res_Y / 1080.0));
	energy_rectangle.setFillColor(sf::Color::Cyan);
	energy_rectangle.setPosition(29 * render_res_X / 1920.0, 1017 * render_res_Y / 1080.0);
	game_window->draw(energy_rectangle);

	//печатаем кол-во денег
	
	text.setCharacterSize(25); // in pixels, not points!
	text.setScale(kx, ky);
	text.setFillColor(sf::Color::Yellow);
	text.setOrigin(0, 0);
	sprintf_s(buffer, "%i", Rover.collected_money);
	text.setString(buffer);
	text.setPosition(190.0 * kx, 15.0 * ky);
	game_window->draw(text);
	
	//уран
	sprintf_s(buffer, "%i", Rover.collected_uran);
	text.setString(buffer);
	text.setPosition(345.0 * kx, 15.0 * ky);
	game_window->draw(text);

	//кристалл
	sprintf_s(buffer, "%i", Rover.collected_crystal);
	text.setString(buffer);
	text.setPosition(494.0 * kx, 15.0 * ky);
	game_window->draw(text);

	//металл
	sprintf_s(buffer, "%i", Rover.collected_metal);
	text.setString(buffer);
	text.setPosition(643.0 * kx, 15.0 * ky);
	game_window->draw(text);

	sf::FloatRect bounds;
	//рисуем кнопку активации (Е)
	//проверяем наличие рядом объекта нужного типа
	int obj_near = check_obj_collision(Rover.player_coord_X, Rover.player_coord_Y, 300.0f);
	if (obj_near != 0)
	{
		//printf("type=%i\n", obj_array[obj_near-1].type);
		if (obj_array[obj_near - 1].type == 0)
		{
			//рядом база
			sprintf_s(buffer, "[E] Enter Base");
			text.setCharacterSize(24.0 / GAME_RENDER_SCALE); // in pixels, not points!
			text.setFillColor(sf::Color::White);
			text.setString(buffer);
			bounds = text.getLocalBounds();
			text.setOrigin(bounds.width / 2.0, bounds.height / 2.0);
			text.setPosition(render_res_X / 2 + Rover.player_offset_X, render_res_Y / 2 - Rover.player_offset_Y - 124.0 / GAME_RENDER_SCALE);
			game_window->draw(text);
		}

		if (obj_array[obj_near - 1].type == 1)
		{
			//рядом точка эвакуации
			//рядом база
			sprintf_s(buffer, "[E] Evacuate");
			text.setCharacterSize(24.0 / GAME_RENDER_SCALE); // in pixels, not points!
			text.setFillColor(sf::Color::White);
			text.setString(buffer);
			bounds = text.getLocalBounds();
			text.setOrigin(bounds.width / 2.0, bounds.height / 2.0);
			text.setPosition(render_res_X / 2 + Rover.player_offset_X, render_res_Y / 2 - Rover.player_offset_Y - 124.0 / GAME_RENDER_SCALE);
			game_window->draw(text);
		}
	}

	//рисуем точки на радаре
	for (int i = 0; i < 100; ++i)
	{
		if (obj_array[i].isValid == 1)
		{
			// рисуем точку
			double angle = atan2(obj_array[i].Y - Rover.player_coord_Y, obj_array[i].X - Rover.player_coord_X);
			double dist = sqrt((obj_array[i].Y - Rover.player_coord_Y) * (obj_array[i].Y - Rover.player_coord_Y) + (obj_array[i].X - Rover.player_coord_X) * (obj_array[i].X - Rover.player_coord_X));
			dist /= 100.0;
			int color_transp = 255;
			if (dist > 114)
			{
				color_transp = 255 - (dist - 114) * 10;
				if (color_transp < 0) color_transp = 0;
				dist = 114;
			}
			dist *= render_res_X / 1920.0;
			sf::CircleShape dot(4.f);
			dot.setOrigin(2.0, 2.0);
			dot.setPosition(1760.0 * render_res_X / 1920.0 + cos(angle) * dist, 156.0 * render_res_Y / 1080.0 - sin(angle) * dist);
			if (obj_array[i].type == 0)
			{
				dot.setFillColor(sf::Color(0, 255, 0, 255));  //база
				//printf("a = %0.1f d = %0.1f\n", angle, dist);
				//printf("type 0\n");
			}
			if (obj_array[i].type == 1)
			{
				dot.setFillColor(sf::Color(255, 0, 0, color_transp));  //корабль
				//printf("type 1\n");
			}
			if (obj_array[i].type == 2)
			{
				dot.setFillColor(sf::Color(255, 0, 255, color_transp));  //гнездо
				//printf("type 1\n");
			}

			game_window->draw(dot);
		}
	}
	//sf::sleep(sf::seconds(1));
	//рисуем иконку оружия

	weapon_tile_Sprite.setTextureRect(sf::IntRect(0, Rover.weapon.sprite_N * 200, 200, 200));
	weapon_tile_Sprite.setPosition((1768.0 + 60.0) * kx, (935.0 + 90.0) * ky);
	weapon_tile_Sprite.setScale(121.0 / 200.0 * 1.2 * kx, 121.0 / 200.0 * 1.2 * ky);
	weapon_tile_Sprite.setRotation(-90.0);
	game_window->draw(weapon_tile_Sprite);


	//рисуем иконки апгрейдов
	for(int i = 0; i < Rover.active_slots; ++i)
	{
		int_upgrade_Sprite.setTextureRect(sf::IntRect(200, Rover.slots[i] * 200, 200, 200));
		int_upgrade_Sprite.setPosition(54.0 * kx, (100.0 + 79.0 * i) * ky);
		int_upgrade_Sprite.setScale(50.0 / 200.0 * kx, 50.0 / 200.0 * ky);
		int_upgrade_Sprite.setOrigin(100,100);
		game_window->draw(int_upgrade_Sprite);

		if (Rover.slots[i] == 0)
		{
			//рисуем иконку пушки для ПКМ
			int_upgrade_Sprite.setPosition((1678.0 + 33.0) * kx, (990.0 + 33.0) * ky);
			int_upgrade_Sprite.setScale(66.0 / 200.0 * kx, 66.0 / 200.0 * ky);
			int_upgrade_Sprite.setOrigin(100, 100);
			game_window->draw(int_upgrade_Sprite);
		}
	}

	//рисуем иконку автопилота
	if (autoPilot)
	{
		int_upgrade_Sprite.setTextureRect(sf::IntRect(200, 200, 200, 200));
		int_upgrade_Sprite.setPosition((1627.0) * kx, (1023) * ky);
		int_upgrade_Sprite.setScale(66.0 / 200.0 * kx, 66.0 / 200.0 * ky);
		int_upgrade_Sprite.setOrigin(100, 100);
		game_window->draw(int_upgrade_Sprite);
	}




	//рисуем курсор в виде прицела
	mouse_pointer_2_Sprite.setPosition((float)localPosition.x - 10, (float)localPosition.y - 10);
	game_window->draw(mouse_pointer_2_Sprite);


}

void Mission::set_render_scale(double scale)
{
	GAME_RENDER_SCALE = scale;

}

void Mission::draw_terrain()
{
	int left_tile = floor((Rover.player_coord_X / GAME_RENDER_SCALE - Rover.player_offset_X - render_res_X / 2.0) / ((double)land_tile_size / GAME_RENDER_SCALE));
	int right_tile = left_tile + ceil((double)render_res_X / ((double)land_tile_size / GAME_RENDER_SCALE));
	int top_tile = floor((round(Rover.player_coord_Y / GAME_RENDER_SCALE) - round(Rover.player_offset_Y) + (double)render_res_Y / 2.0) / ((double)land_tile_size / GAME_RENDER_SCALE));
	int bottom_tile = top_tile - ceil((double)render_res_Y / ((double)land_tile_size / GAME_RENDER_SCALE));

	for (int i = left_tile; i <= right_tile; ++i)
	{
		for (int j = top_tile; j >= bottom_tile; --j)
		{
			int num = rnd_from_coord(i, j, 19);
			
			bg_tile_Sprite.setOrigin(0, 0);
			bg_tile_Sprite.setColor(sf::Color(255, 255, 255, 255));
			//if (i == 3 && j == 3) bg_tile_Sprite.setColor(sf::Color(0, 0, 255, 220));
			//if (i == 0 && j == 0) bg_tile_Sprite.setColor(sf::Color(0, 255, 0, 220));
			bg_tile_Sprite.setTextureRect(sf::IntRect(num * 100, (planet_type - 1)* 100, land_tile_size + 1, land_tile_size + 1));
			bg_tile_Sprite.setPosition(i * ((double)land_tile_size / GAME_RENDER_SCALE) + render_res_X / 2 - (Rover.player_coord_X / GAME_RENDER_SCALE - Rover.player_offset_X), -(j + 1) * ((double)land_tile_size / GAME_RENDER_SCALE) + (Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y) + render_res_Y / 2);
			bg_tile_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
			game_window->draw(bg_tile_Sprite);
			//дорисовываем кусты поверх
			//num = rnd_from_coord(i + 349, j + 829, 19);
			//bg_tile_Sprite.setTextureRect(sf::IntRect(num * 100, 100, land_tile_size, land_tile_size));
			//game_window->draw(bg_tile_Sprite);
		}
	}

}

void Mission::draw_loot()
{
	int loot_index = 0;
	int loot_processed = 0;
	while (loot_processed < loot_count)
	{
		//проходим по массиву лута
		if (loot_array[loot_index].isValid == 1)
		{
			if (abs(loot_array[loot_index].X - Rover.player_coord_X) < render_res_X * GAME_RENDER_SCALE && abs(loot_array[loot_index].Y - Rover.player_coord_Y) < render_res_Y * GAME_RENDER_SCALE)
			{
				//проверяем коллизию
				if (abs(loot_array[loot_index].X - Rover.player_coord_X) < 100 && abs(loot_array[loot_index].Y - Rover.player_coord_Y) < 100)
				{
					//"потребляем" лут
					//0 - health, 1 - energy, 2 - health+energy, 3 - metal, 4 - crystal, 5 - uran

					if (loot_array[loot_index].type == 0)
					{
						//пробуем съесть аптечку
						if (Rover.Health < Rover.Health_MAX)
						{
							Rover.Health += loot_array[loot_index].power;
							if (Rover.Health > Rover.Health_MAX) Rover.Health = Rover.Health_MAX;
							loot_array[loot_index].isValid = 0;
							loot_count--;
						}
						//printf("health = %0f\n", Rover.Health);
					}

					if (loot_array[loot_index].type == 1)
					{
						//пробуем съесть энергетик
						if (Rover.Energy < Rover.Energy_MAX)
						{
							Rover.Energy += loot_array[loot_index].power;
							if (Rover.Energy > Rover.Energy_MAX) Rover.Energy = Rover.Energy_MAX;
							loot_array[loot_index].isValid = 0;
							loot_count--;
						}

						//printf("energy = %0f\n", Rover.Energy);
					}

					if (loot_array[loot_index].type == 2)
					{
						//пробуем съесть комбо
						if (Rover.Health < Rover.Health_MAX || Rover.Energy < Rover.Energy_MAX)
						{
							Rover.Health += loot_array[loot_index].power;
							Rover.Energy += loot_array[loot_index].power;
							loot_array[loot_index].isValid = 0;
							loot_count--;
						}
						if (Rover.Health > Rover.Health_MAX) Rover.Health = Rover.Health_MAX;
						if (Rover.Energy > Rover.Energy_MAX) Rover.Energy = Rover.Energy_MAX;
						//printf("health = %0f, energy = %0f\n", Rover.Health, Rover.Energy);
					}

					if (loot_array[loot_index].type == 3)
					{
						//3 - metal, 4 - crystal, 5 - uran
							Rover.collected_metal += loot_array[loot_index].power;
							loot_array[loot_index].isValid = 0;
							loot_count--;
					}
					if (loot_array[loot_index].type == 4)
					{
						//3 - metal, 4 - crystal, 5 - uran
						Rover.collected_crystal += loot_array[loot_index].power;
						loot_array[loot_index].isValid = 0;
						loot_count--;
					}
					
					if (loot_array[loot_index].type == 5)
					{
						//3 - metal, 4 - crystal, 5 - uran
						Rover.collected_uran += loot_array[loot_index].power;
						loot_array[loot_index].isValid = 0;
						loot_count--;
					}

					if (loot_array[loot_index].type == 6 && upgrage_to_take == -1)
					{
						//6 - upgrade
						upgrage_to_take = loot_array[loot_index].power;
						loot_array[loot_index].isValid = 0;
						loot_count--;
					}
					if (loot_array[loot_index].type == 7 && upgrage_to_take == -1)
					{
						//7 - weapon
						weapon_to_take = loot_array[loot_index].power;
						loot_array[loot_index].isValid = 0;
						loot_count--;
					}
				}

				if (loot_array[loot_index].isValid == 1)
				{
					//рисуем лут
					int tile_offset = loot_array[loot_index].type * 100;
					loot_Sprite.setTextureRect(sf::IntRect(tile_offset, 0, 100, 100));
					loot_Sprite.setPosition(loot_array[loot_index].X / GAME_RENDER_SCALE - (Rover.player_coord_X / GAME_RENDER_SCALE - Rover.player_offset_X - render_res_X / 2), Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y + render_res_Y / 2 - loot_array[loot_index].Y / GAME_RENDER_SCALE);
					loot_Sprite.setOrigin(50,50);
					loot_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
					if (loot_array[loot_index].type == 7)
					{
						//дорисовываем цвет оружия
						int color = loot_array[loot_index].power >> 8;
						sf::CircleShape shape(60.f);
						shape.setOrigin(60.0, 60.0);
						shape.setPosition(loot_array[loot_index].X / GAME_RENDER_SCALE - (Rover.player_coord_X / GAME_RENDER_SCALE - Rover.player_offset_X - render_res_X / 2), Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y + render_res_Y / 2 - loot_array[loot_index].Y / GAME_RENDER_SCALE);
						shape.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
						if (color == 0) shape.setFillColor(sf::Color(128, 128, 128, 70));
						if (color == 1) shape.setFillColor(sf::Color(0,   255,   0, 70));
						if (color == 2) shape.setFillColor(sf::Color(0,   255, 255, 70));
						if (color == 3) shape.setFillColor(sf::Color(255,   0, 255, 70));
						if (color == 4) shape.setFillColor(sf::Color(255, 255,   0, 70));
						game_window->draw(shape);
					}
					game_window->draw(loot_Sprite);
				}


			}

			loot_processed++;
		}
		loot_index++;
	}
	//printf("loot procc = %i \n", loot_processed);
}

void Mission::draw_character()
{
	
	//привязываем скорость анимации к прошедшему времени
	Rover.animation_time = Rover.animation_time + elapsed_ms;
	if (Rover.animation_time > Rover.animation_time_MAX) Rover.animation_time -= Rover.animation_time_MAX;

	body_tile_Sprite.setPosition(render_res_X / 2 + Rover.player_offset_X, render_res_Y / 2 - Rover.player_offset_Y);
	//body_tile_Sprite.setTextureRect(sf::IntRect(round(Rover.animation_time / Rover.animation_time_MAX) * 7 * 200, (Rover.body_direction - 1) * 200, 200, 200));
	body_tile_Sprite.setTextureRect(sf::IntRect(round(Rover.animation_time / Rover.animation_time_MAX) * 7 * 200, 0, 200, 200));
	body_tile_Sprite.setOrigin(100.0, 100.0);
	body_tile_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
	body_tile_Sprite.setRotation(-Rover.Speed_direction * 180.0 / 3.1415926);
	game_window->draw(body_tile_Sprite);

	//рисуем пушку

	Rover.weapon_animation_time += elapsed_ms;
	if (Rover.weapon_animation_time > Rover.weapon_animation_time_MAX) Rover.weapon_animation_time -= Rover.weapon_animation_time_MAX;

	weapon_tile_Sprite.setPosition(render_res_X / 2 + Rover.player_offset_X, render_res_Y / 2 - Rover.player_offset_Y);
	//weapon_tile_Sprite.setTextureRect(sf::IntRect(round(Rover.weapon_animation_time / Rover.weapon_animation_time_MAX) * 7 * 200, (Rover.weapon_direction - 1) * 200, 200, 200));
	weapon_tile_Sprite.setTextureRect(sf::IntRect(round(Rover.weapon_animation_time / Rover.weapon_animation_time_MAX) * 7 * 200, Rover.weapon.sprite_N * 200, 200, 200));
	weapon_tile_Sprite.setOrigin(100.0, 100.0);
	weapon_tile_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
	weapon_tile_Sprite.setRotation(-Rover.weapon_direction_rad * 180.0 / 3.1415926);
	game_window->draw(weapon_tile_Sprite);

	//рассчитываем удаленность от последней точки и рисуем след

	double dist = sqrt((Rover.tires_XY[0] - Rover.player_coord_X) * (Rover.tires_XY[0] - Rover.player_coord_X) + (Rover.tires_XY[1] - Rover.player_coord_Y)* (Rover.tires_XY[1] - Rover.player_coord_Y));
	if (dist >= 230.0)
	{
		Rover.tires_XY[0] = Rover.player_coord_X;
		Rover.tires_XY[1] = Rover.player_coord_Y;
		create_VFX_remains(Rover.player_coord_X, Rover.player_coord_Y, 4, -Rover.Speed_direction * 180.0 / 3.1415926); // 3 - шины
		//printf("%f\n", Rover.Speed_direction * 180.0 / 3.1415926);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && 0) 
	{
		
		sf::Vector3f r = find_intersection(Rover.weapon_direction_rad, Rover.player_coord_X, Rover.player_coord_Y, 1000.0);
		//printf("(x y z) %0.0f %0.0f %0.0f \n", r.x, r.y, r.z);
		//sf::sleep(sf::milliseconds(100));
		if (r.z != -1)
		{
			sf::RectangleShape health_rectangle(sf::Vector2f(8.0,8.0));
			health_rectangle.setFillColor(sf::Color::Green);
			health_rectangle.setOrigin(4.0, 4.0);
			//health_rectangle.setPosition(r.x * render_res_X / 1920.0, r.y * render_res_Y / 1080.0);
			health_rectangle.setPosition(render_res_X / 2 + Rover.player_offset_X - Rover.player_coord_X / GAME_RENDER_SCALE + r.x / GAME_RENDER_SCALE, render_res_Y / 2 + Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y - r.y / GAME_RENDER_SCALE);
			game_window->draw(health_rectangle);
		}
	}
}

void Mission::controls_processing(sf::Vector2i localPosition)
{
	int player_D_x = 0;
	int player_D_y = 0;
	player_weapon_cooldown -= elapsed_ms; //охлаждаем ствол
	if (player_weapon_cooldown < 0) player_weapon_cooldown = 0;
	player_weapon_cooldown_G -= elapsed_ms; //охлаждаем гранатомет
	if (player_weapon_cooldown_G < 0) player_weapon_cooldown_G = 0;


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) player_D_y++;//движение вверх
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) player_D_y--;//движение вниз
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) player_D_x--;//движение влево
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) player_D_x++;//движение вправо
	
	//определение направления движения игрока
	if (player_D_x == 0 && player_D_y == 1) Rover.body_direction = 1;
	if (player_D_x == 1 && player_D_y == 1) Rover.body_direction = 2;
	if (player_D_x == 1 && player_D_y == 0) Rover.body_direction = 3;
	if (player_D_x == 1 && player_D_y == -1) Rover.body_direction = 4;
	if (player_D_x == 0 && player_D_y == -1) Rover.body_direction = 5;
	if (player_D_x == -1 && player_D_y == -1) Rover.body_direction = 6;
	if (player_D_x == -1 && player_D_y == 0) Rover.body_direction = 7;
	if (player_D_x == -1 && player_D_y == 1) Rover.body_direction = 8;

	move_player((player_D_x != 0 || player_D_y != 0) ? 1 : 0); //двигаем игрока с учетом инерции и препятствий

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) //масштаб +
	{
		GAME_RENDER_SCALE -= 0.05;
		if (GAME_RENDER_SCALE < 1) GAME_RENDER_SCALE = 1;
		set_render_scale(GAME_RENDER_SCALE);
		//printf("render scale = %1.1f\n", GAME_RENDER_SCALE);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) //масштаб -
	{
		GAME_RENDER_SCALE += 0.05;
		if (GAME_RENDER_SCALE > 4) GAME_RENDER_SCALE = 4;
		set_render_scale(GAME_RENDER_SCALE);
		//printf("render scale = %1.1f\n", GAME_RENDER_SCALE);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) GAME_RENDER_SCALE = 2;//масштаб 2
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))//тестовый разброс ресурсов
	{
		double x = ((Rover.player_coord_X / GAME_RENDER_SCALE - Rover.player_offset_X - render_res_X / 2) + localPosition.x) * GAME_RENDER_SCALE;
		double y = (Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y + render_res_Y / 2 - localPosition.y) * GAME_RENDER_SCALE;
		//drop_flying_loot(2, 2, 2, x, y); 
		//sf::sleep(sf::milliseconds(100));
	}

	// проверяем включение автопилота
	bool autoPilot = false;
	for (int i = 0; i < Rover.active_slots; ++i)
	{
		if (Rover.slots[i] == 1) autoPilot = true;
	}

	//считаем угол между игроком и курсором
	int D_x = localPosition.x - (render_res_X / 2 + Rover.player_offset_X);
	int D_y = (render_res_Y / 2 - Rover.player_offset_Y) - localPosition.y;
	if (!autoPilot)
	{
		Rover.weapon_direction_rad = std::atan2((double)D_y, (double)D_x);
		if (Rover.weapon_direction_rad < 0) Rover.weapon_direction_rad += 2 * 3.1415926;
	}
	
	//устанавливаем направление оружия для анимации
	if ((Rover.weapon_direction_rad >= 45 * 7.5 / 180 * 3.1415) || (Rover.weapon_direction_rad < 45 * 0.5 / 180 * 3.1415)) Rover.weapon_direction = 3;
	if ((Rover.weapon_direction_rad >= 45 * 0.5 / 180 * 3.1415) && (Rover.weapon_direction_rad < 45 * 1.5 / 180 * 3.1415)) Rover.weapon_direction = 2;
	if ((Rover.weapon_direction_rad >= 45 * 1.5 / 180 * 3.1415) && (Rover.weapon_direction_rad < 45 * 2.5 / 180 * 3.1415)) Rover.weapon_direction = 1;
	if ((Rover.weapon_direction_rad >= 45 * 2.5 / 180 * 3.1415) && (Rover.weapon_direction_rad < 45 * 3.5 / 180 * 3.1415)) Rover.weapon_direction = 8;
	if ((Rover.weapon_direction_rad >= 45 * 3.5 / 180 * 3.1415) && (Rover.weapon_direction_rad < 45 * 4.5 / 180 * 3.1415)) Rover.weapon_direction = 7;
	if ((Rover.weapon_direction_rad >= 45 * 4.5 / 180 * 3.1415) && (Rover.weapon_direction_rad < 45 * 5.5 / 180 * 3.1415)) Rover.weapon_direction = 6;
	if ((Rover.weapon_direction_rad >= 45 * 5.5 / 180 * 3.1415) && (Rover.weapon_direction_rad < 45 * 6.5 / 180 * 3.1415)) Rover.weapon_direction = 5;
	if ((Rover.weapon_direction_rad >= 45 * 6.5 / 180 * 3.1415) && (Rover.weapon_direction_rad < 45 * 7.5 / 180 * 3.1415)) Rover.weapon_direction = 4;



	//проверяем нажатие на кнопку LMB и доступность оружия (автопилот ВЫКЛ)
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && player_weapon_cooldown == 0 && game_options->game_in_focus && !autoPilot)
	{
		//если места в массиве патронов достаточно, создаем еще один
		if (player_bullet_count < 100)
		{
			//ищем пустую ячейку в массиве
			int new_item = 0;
			for (new_item = 0; new_item < 100 && player_bullet_array[new_item].isAlive == 1; ++new_item);
			if (player_bullet_array[new_item].isAlive == 0)//еще раз проверяем, что позиция не занята
			{
				player_bullet_array[new_item].isAlive = 1;
				player_bullet_array[new_item].type = Rover.weapon.get_ammo_type();
				player_bullet_array[new_item].power = Rover.weapon.power * 1;  //убрать множитель
				player_bullet_array[new_item].speed = Rover.weapon.get_ammo_speed();
				if (Rover.weapon_direction_rad > 3.1415926) player_bullet_array[new_item].direction = Rover.weapon_direction_rad - 2 * 3.141926;
				else player_bullet_array[new_item].direction = Rover.weapon_direction_rad;
				player_bullet_array[new_item].X = Rover.player_coord_X + cos(player_bullet_array[new_item].direction) * 100.0;
				player_bullet_array[new_item].Y = Rover.player_coord_Y + sin(player_bullet_array[new_item].direction) * 100.0;
				player_bullet_array[new_item].ttl = Rover.weapon.get_ammo_ttl();//time to live
				player_bullet_array[new_item].lives = 1;
				for (int t = 0; t < 3; ++t) 
				{ 
					if (Rover.active_slots > t && Rover.slots[t] == 2) ++player_bullet_array[new_item].lives; //добавляем жизни пулям если стоит апгрейд
					player_bullet_array[new_item].monters_hit[t] = -1; //обнуляем список попаданий
				}
				//printf("lives=%i", player_bullet_array[new_item].lives);
				player_bullet_count++;
				player_weapon_cooldown = 1000.0 / (Rover.weapon.fire_rate * Rover.fire_rate_boost);
				if (Rover.weapon.get_shot_sound() == 1) { sound_gun_1.setVolume(game_options->sound_volume); sound_gun_1.play(); } //звук выстрела
				if (Rover.weapon.get_shot_sound() == 2) { sound_gun_2.setVolume(game_options->sound_volume); sound_gun_2.play(); } //звук выстрела
				if (Rover.weapon.get_shot_sound() == 3) { sound_gun_3.setVolume(game_options->sound_volume); sound_gun_3.play(); } //звук выстрела
				if (Rover.weapon.get_shot_sound() == 4) { sound_gun_4.setVolume(game_options->sound_volume); sound_gun_4.play(); } //звук выстрела
				if (Rover.weapon.get_shot_sound() == 5) { sound_gun_5.setVolume(game_options->sound_volume); sound_gun_5.play(); } //звук выстрела
			}
		}
	}

	//работа автопилота
	if (autoPilot)
	{
		sf::Vector3f target = find_nearest_enemy();
		double angle_to_target = 0.0; //направление на цель
		double weapon_angle = 0.0; //направление орудия
		double dA = 0.0; // угол доворота орудия
		bool fire = false;

		if (target.z >= 0)
		{
			angle_to_target = std::atan2(target.y - Rover.player_coord_Y, target.x - Rover.player_coord_X);
			if (angle_to_target < 0) angle_to_target += 2 * 3.1415926;
			weapon_angle = Rover.weapon_direction_rad;
			if (weapon_angle < 0) weapon_angle += 2 * 3.1415926;
			if (weapon_angle > 2 * 3.1415926) weapon_angle -= 2 * 3.1415926;

			double dA = angle_to_target - weapon_angle;
			if (dA > 3.1415926) dA -= 2 * 3.1415926;
			if (dA < -3.1415926) dA += 2 * 3.1415926;
			if (abs(dA) < 3.1415926 / 90.0)
			{
				Rover.weapon_direction_rad = angle_to_target; //выставляем орудие ровно на цель
				fire = true;//разрешение стрелять
			}
			else
			{
				//доворот орудия
				if (dA > 0)
				{
					Rover.weapon_direction_rad += fmin(2.0 * 3.1415926 / 1000 * elapsed_ms,dA); //вправо
				}
				else 
				{
					Rover.weapon_direction_rad -= fmax(2.0 * 3.1415926 / 1000 * elapsed_ms, dA); //влево
				}
			}
		}

		//если основное оружие готово
		if (player_weapon_cooldown == 0 && fire)
		{
			//если места в массиве патронов достаточно, создаем еще один
			//printf("dist_near=%0.0f\n", sqrt((target.x - Rover.player_coord_X) * (target.x - Rover.player_coord_X) + (target.y - Rover.player_coord_Y) * (target.y - Rover.player_coord_Y)));
			if (player_bullet_count < 100 &&
				sqrt((target.x - Rover.player_coord_X) * (target.x - Rover.player_coord_X) + (target.y - Rover.player_coord_Y) * (target.y - Rover.player_coord_Y)) < 2200 &&
				target.z >= 0)
			{
					//printf("target # %i\n", (int)target.z);
					//printf("%f0.0\n", sqrt((target.x - Rover.player_coord_X) * (target.x - Rover.player_coord_X) + (target.y - Rover.player_coord_Y) * (target.y - Rover.player_coord_Y)));
					//ищем пустую ячейку в массиве
					int new_item = 0;
					for (new_item = 0; new_item < 100 && player_bullet_array[new_item].isAlive == 1; ++new_item);
					if (player_bullet_array[new_item].isAlive == 0)//еще раз проверяем, что позиция не занята
					{
						player_bullet_array[new_item].isAlive = 1;
						player_bullet_array[new_item].type = Rover.weapon.get_ammo_type();
						player_bullet_array[new_item].power = Rover.weapon.power * 10;  //убрать множитель
						player_bullet_array[new_item].speed = Rover.weapon.get_ammo_speed();
						player_bullet_array[new_item].direction = std::atan2(target.y - Rover.player_coord_Y, target.x - Rover.player_coord_X);
						if (player_bullet_array[new_item].direction < 0) player_bullet_array[new_item].direction += 2 * 3.141926;
						Rover.weapon_direction_rad = player_bullet_array[new_item].direction;
						player_bullet_array[new_item].X = Rover.player_coord_X + cos(player_bullet_array[new_item].direction) * 100.0;
						player_bullet_array[new_item].Y = Rover.player_coord_Y + sin(player_bullet_array[new_item].direction) * 100.0;
						player_bullet_array[new_item].ttl = Rover.weapon.get_ammo_ttl();//time to live
						player_bullet_array[new_item].lives = 1;
						for (int t = 0; t < 3; ++t)
						{
							if (Rover.active_slots > t && Rover.slots[t] == 2) ++player_bullet_array[new_item].lives; //добавляем жизни пулям если стоит апгрейд
							player_bullet_array[new_item].monters_hit[t] = -1; //обнуляем список попаданий
						}
						//printf("lives=%i", player_bullet_array[new_item].lives);
						player_bullet_count++;
						player_weapon_cooldown = 1000.0 / (Rover.weapon.fire_rate * Rover.fire_rate_boost);
						if (Rover.weapon.get_shot_sound() == 1) { sound_gun_1.setVolume(game_options->sound_volume); sound_gun_1.play(); } //звук выстрела
						if (Rover.weapon.get_shot_sound() == 2) { sound_gun_2.setVolume(game_options->sound_volume); sound_gun_2.play(); } //звук выстрела
						if (Rover.weapon.get_shot_sound() == 3) { sound_gun_3.setVolume(game_options->sound_volume); sound_gun_3.play(); } //звук выстрела
						if (Rover.weapon.get_shot_sound() == 4) { sound_gun_4.setVolume(game_options->sound_volume); sound_gun_4.play(); } //звук выстрела
						if (Rover.weapon.get_shot_sound() == 5) { sound_gun_5.setVolume(game_options->sound_volume); sound_gun_5.play(); } //звук выстрела
					}
				
			}
		}
	}
	
	//проверяем нажатие на кнопку RMB и доступность оружия
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && player_weapon_cooldown_G == 0 && game_options->game_in_focus)
	{
		//стреляем из гранатомета
		//подсчитываем кол-во гранат
		int grenades = 0;
		for (int i = 0; i < Rover.active_slots; ++i)
		{
			if (Rover.slots[i] == 0) grenades++;
		}
		
		if (grenades > 0)
		{
			//делаем выстрел
			int i;
			for (i = 0; i < 10; ++i)
			{
				if (grenades_array[i].isAlive == 0) break;
			}
			if (i < 10)
			{
				grenades_array[i].isAlive = 1;
				grenades_array[i].power = 5;
				grenades_array[i].X = Rover.player_coord_X;
				grenades_array[i].Y = Rover.player_coord_Y;
				grenades_array[i].V = 0;
				grenades_array[i].ttl = 2000;
				grenades_array[i].V_Velocity = 1600;

				//считаем угол между игроком и курсором
				int D_x = localPosition.x - (render_res_X / 2 + Rover.player_offset_X);
				int D_y = (render_res_Y / 2 - Rover.player_offset_Y) - localPosition.y;
				double fire_direction = std::atan2((double)D_y, (double)D_x);
				if (fire_direction < 0) fire_direction += 2 * 3.1415926;
				
				grenades_array[i].dX = cos(fire_direction) * 500.0;
				grenades_array[i].dY = sin(fire_direction) * 500.0;

				player_weapon_cooldown_G = 3000 / grenades; //время отката зависит от кол-ва гранат

				//printf("(X Y dX dY) %0.0f %0.0f %0.0f %0.0f\n", grenades_array[i].X,grenades_array[i].Y,grenades_array[i].dX,grenades_array[i].dY);
			}
		}
	}

	//тестовая информация
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Multiply))
	{
		/*
		printf("Flying loot\n");
		for (int i = 0; i < 100; ++i)
		{
			if (flying_loot_array[i].isValid)
			{
				printf("i=%4i  dX=%0.0f dY=%0.0f Vel=%0.0f ttl=%0.0f\n", i, flying_loot_array[i].dX, flying_loot_array[i].dY, flying_loot_array[i].V_Velocity, flying_loot_array[i].ttl);
			}
		}
		*/
		printf("monster count=%i \n", monster_count);
		for (int i = 0; i < 16; ++i)
		{
			printf("%i = %i\n", i, sector_count[i]);
		}
		sf::sleep(sf::milliseconds(300));
		/*
		printf("monster count=%i \n", monster_count);
		int c = 0;
		for (int i = 0; i < 1000; ++i)
		{
			if (monster_array[i].isAlive == 1) c++;
		}
		printf("in array = %i\n", c);
		printf("\n\nDebug info = Rover\n");
		printf("planet_type %i\n", planet_type);
		printf("planet_level %i\n", planet_level);
		printf("Rover.Health_MAX %0.1f\n", Rover.Health_MAX);
		printf("Rover.Health %0.1f\n", Rover.Health);
		printf("Rover.Energy_MAX %0.1f\n", Rover.Energy_MAX);
		printf("Rover.Energy %0.1f\n", Rover.Energy);
		printf("Rover.Generate_speed %0.1f\n", Rover.Generate_speed);
		printf("Rover.Speed_MAX %0.1f\n", Rover.Speed_MAX);
		printf("Rover.gun_damage %0.1f\n", Rover.gun_damage);
		printf("Rover.fire_rate %0.1f\n", Rover.weapon.fire_rate);
		printf("Rover.slots %i\n", Rover.slots);
		printf("Rover.weapon_direction_rad %0.1f\n", Rover.weapon_direction_rad);
		*/
	}
}

void Mission::draw_bullets()
{
	//рисуем пульки - 100 шт в массиве

	for (int i = 0; i < 100; ++i)
	{
		if (player_bullet_array[i].isAlive == 1)
		{
			//рассчитываем очередные координаты и уменьшаем время жизни
			player_bullet_array[i].X += player_bullet_array[i].speed * cos(player_bullet_array[i].direction) * elapsed_ms / 1000;
			player_bullet_array[i].Y += player_bullet_array[i].speed * sin(player_bullet_array[i].direction) * elapsed_ms / 1000;
			player_bullet_array[i].ttl -= elapsed_ms;
			if (player_bullet_array[i].ttl <= 0)
			{
				player_bullet_array[i].isAlive = 0; //пуля исчезает
				player_bullet_count--;
			}
			else
			{
				//проверяем коллизии со всеми монстрами, если пуля не исчезла
				//проходим по массиву с монстрами и сравниваем с координатами монстров
				int monsters_procecced = 0;
				for (int j = 0; j < 1000 && monsters_procecced < monster_count && player_bullet_array[i].isAlive == 1; ++j)
				{
					if (monster_array[j].isAlive == 1)
					{
						//если монстр жив, сравниваем координаты
						if (abs(player_bullet_array[i].X - monster_array[j].X) < 50 * monster_array[j].scale && abs(player_bullet_array[i].Y - monster_array[j].Y) < 50 * monster_array[j].scale)
						{
							//проверяем, не попадали ли в этого монстра еще раз
							for (int t = 0; t < 3; ++t)
							{ 
								if (player_bullet_array[i].monters_hit[t] == j) {
									//printf("skip\n");  
									goto Skip; //пропускаем код, если мы уже попали в этого монстра
								} 
							}

							//фиксируем попадание
							monster_array[j].health -= player_bullet_array[i].power; //уменьшаем здоровье монстра
							
							if (player_bullet_array[i].type == 0) { sound_hit_2.setVolume(game_options->sound_volume); sound_hit_2.play(); create_VFX(player_bullet_array[i].X, player_bullet_array[i].Y, 0, 1.0); } //попадание снаряда
							if (player_bullet_array[i].type == 1) { sound_hit_2.setVolume(game_options->sound_volume); sound_hit_2.play(); create_VFX(player_bullet_array[i].X, player_bullet_array[i].Y, 0, 1.0); }
							if (player_bullet_array[i].type == 2) {	make_explosion(player_bullet_array[i].X, player_bullet_array[i].Y, player_bullet_array[i].power, false, true);} //взрыв ракеты - отдельная процедура
							if (player_bullet_array[i].type == 3) { sound_hit_4.setVolume(game_options->sound_volume); sound_hit_4.play(); create_VFX(player_bullet_array[i].X, player_bullet_array[i].Y, 5, 2.0);} //взрыв плазмы
							if (player_bullet_array[i].type == 4) { sound_hit_3.setVolume(game_options->sound_volume); sound_hit_3.play(); create_VFX(player_bullet_array[i].X, player_bullet_array[i].Y, 6, 2.0);} //удар током
							if (player_bullet_array[i].type == 5) { sound_hit_2.setVolume(game_options->sound_volume); sound_hit_2.play(); create_VFX(player_bullet_array[i].X, player_bullet_array[i].Y, 6, 2.0);} //обычный звук / холод

							if (monster_array[j].health <= 0 && monster_array[j].isAlive == 1)
							{
								monster_array[j].isAlive = 0; //фиксируем смерть
								monster_count--;
								monsters_procecced--;
								//создаем спецэффект на месте монстра
								//if (player_bullet_array[i].type == 0 || player_bullet_array[i].type == 1) create_VFX(monster_array[j].X, monster_array[j].Y, 0, 1.0);
								//sound_death_1.play();
								Rover.collected_money += monster_array[j].power;//гонорар
								if (monster_array[j].isJugger)
								{
									//выбрасываем ресурсы, если монстр мощный
									drop_flying_loot(planet_level * 2, floor(planet_level / 3.0) * 2, (int)floor(planet_level / 5.0), monster_array[j].X, monster_array[j].Y);
									//издаем жуткий звук
									sound_death_1.setVolume(game_options->sound_volume);
									sound_death_1.play();
								}
							}
							if (player_bullet_array[i].lives == 1 || player_bullet_array[i].type == 2)
							{
								player_bullet_array[i].isAlive = 0;//пуля исчезает после попадания
								player_bullet_count--;
							}
							else
							{
								//если у пули > 1 "жизни"
								player_bullet_array[i].lives--;  //уменьшаем кол-во жизней
								for (int t = 0; t < 3; ++t) 
								{
									if (player_bullet_array[i].monters_hit[t] == -1)
									{
										player_bullet_array[i].monters_hit[t] = j; //запоминаем пораженного монстра (убираем повторные срабатывания)
										//printf("hit mem %i \n", j);
										break;
									}
								}
							}
						Skip:;
						}
						monsters_procecced++;
					}
				}
			}

			//проверка попадания в объекты
			if (player_bullet_array[i].isAlive == 1) //повторная проверка т.к. пуля могла попасть в монстра
			{
				int obj = check_obj_collision(player_bullet_array[i].X, player_bullet_array[i].Y, -20.0f);
				if (obj)
				{
					//фиксируем попадание
					player_bullet_array[i].isAlive = 0;//пуля исчезает после попадания
					player_bullet_count--;
					
					if (obj_array[obj - 1].type == 0 || obj_array[obj - 1].type == 1) //попадание в базу или корабль
					{

						if (player_bullet_array[i].type == 0 || player_bullet_array[i].type == 1)
						{
							//попадание снаряда
							sound_hit_1.setVolume(game_options->sound_volume);
							sound_hit_1.play();
							create_VFX(player_bullet_array[i].X, player_bullet_array[i].Y, 2, 1); // тип 2 - попадание пули
						}
						if (player_bullet_array[i].type == 2) make_explosion(player_bullet_array[i].X, player_bullet_array[i].Y, player_bullet_array[i].power, false, true);
						if (player_bullet_array[i].type == 3)
						{
							sound_hit_4.setVolume(game_options->sound_volume);
							sound_hit_4.play();  //взрыв плазмы
							create_VFX(player_bullet_array[i].X, player_bullet_array[i].Y, 5, 2); // тип 4 - взрыв плазмы
						}
						if (player_bullet_array[i].type == 4) 
						{
							sound_hit_3.setVolume(game_options->sound_volume); 
							sound_hit_3.play();
							create_VFX(player_bullet_array[i].X, player_bullet_array[i].Y, 6, 2); // тип 6 - удар током
						} 
						if (player_bullet_array[i].type == 5)
						{
							sound_hit_1.setVolume(game_options->sound_volume);
							sound_hit_1.play();
							create_VFX(player_bullet_array[i].X, player_bullet_array[i].Y, 6, 2); // тип 6 - удар током
						}


					}
					if (obj_array[obj-1].type == 2) //попадание в гнездо
					{
						create_VFX(player_bullet_array[i].X, player_bullet_array[i].Y, 0, 2); // тип 0 - взрыв монстра
						sound_hit_2.setVolume(game_options->sound_volume);
						sound_hit_2.play();
						obj_array[obj-1].Health -= player_bullet_array[i].power;
						//printf("%0.0f\n", obj_array[obj - 1].Health);
						if (obj_array[obj-1].Health <= 0)
						{
							//убиваем гнездо
							obj_array[obj-1].isValid = 0;
							sound_death_1.setVolume(game_options->sound_volume);
							sound_death_1.play();
							//выбрасываем ресурсы
							drop_flying_loot(planet_level * 4, planet_level * 2, planet_level, obj_array[obj - 1].X, obj_array[obj - 1].Y);
							Rover.collected_money += 20 * planet_level; //деньги
						}
					}

				}
			}

			if (player_bullet_array[i].isAlive == 1) //повторная проверка т.к. пуля могла попасть еще куда-нибудь
			{
				//рисуем спрайт пули
				player_bullet_Sprite.setTextureRect(sf::IntRect(0, player_bullet_array[i].type * 100.0, 100, 100));
				//printf("%i\n", Rover.weapon.get_ammo_type());
				player_bullet_Sprite.setOrigin(50, 50);
				player_bullet_Sprite.setPosition(render_res_X / 2 + Rover.player_offset_X - Rover.player_coord_X / GAME_RENDER_SCALE + player_bullet_array[i].X / GAME_RENDER_SCALE, render_res_Y / 2 + Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y - player_bullet_array[i].Y / GAME_RENDER_SCALE);
				player_bullet_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
				player_bullet_Sprite.setRotation(-player_bullet_array[i].direction * 180.0 / 3.1415926);
				game_window->draw(player_bullet_Sprite);
				//printf("%4i  %4i  %4i  %6.0f  %6.0f  %6.0f %6.6f\n", i, player_bullet_array[i].isAlive, player_bullet_array[i].type, player_bullet_array[i].power, player_bullet_array[i].X, player_bullet_array[i].Y, player_bullet_array[i].direction);
			}
		}
	}
}

void Mission::generate_monsters()
{
	monster_gen_timer = 1000;//переделать под константы
	//генерируем монстров вокруг игрока

	//рассчитываем кол-во монстров в секторе
	double dist_to_base = sqrt(Rover.player_coord_X * Rover.player_coord_X + Rover.player_coord_Y * Rover.player_coord_Y);
	double monster_by_sec = 1 * planet_level;
		if (dist_to_base > 12500) monster_by_sec = 0.5 * (dist_to_base / 12500.0) * planet_level;
	int monster_tot = 0;
	//int GENERATED_MONSTER = 0;
	double gen_dist = sqrt(pow((render_res_X / 2 + player_max_offset_X),2) + pow((render_res_Y / 2 + player_max_offset_Y), 2) )* GAME_RENDER_SCALE;
	double rnd_angle = 0.0;
	//цикл генерации
	for (int sec = 0; sec < 16; ++sec) { monster_tot += sector_count[sec]; }
	for (int sec = 0; sec < 16; ++sec)
	{
		while (sector_count[sec] < monster_by_sec && monster_count < 1000 && monster_tot < monster_by_sec * 16)
		{
			//вычисляем случайные координаты
			rnd_angle = 3.1415926 / 8.0 * rand() / (RAND_MAX + 1.00) + sec * 3.1415926 / 8.0;
			double rnd_dist = gen_dist + 400.0; // render_res_X / 2 + (gen_dist - render_res_X / 2) * rand() / RAND_MAX;
			double NEW_X = Rover.player_coord_X + cos(rnd_angle) * rnd_dist;
			double NEW_Y = Rover.player_coord_Y + sin(rnd_angle) * rnd_dist;
			//определяем попадание за пределы экрана
			double left = -(render_res_X / 2 + Rover.player_offset_X - Rover.player_coord_X / GAME_RENDER_SCALE) * GAME_RENDER_SCALE; 
			double right = left + render_res_X * GAME_RENDER_SCALE;
			double top = (render_res_Y / 2 + Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y) * GAME_RENDER_SCALE;
			double bottom = top - render_res_Y * GAME_RENDER_SCALE;
			if (NEW_X < left - 100 || NEW_X > right + 100 || NEW_Y < bottom - 100 || NEW_Y > top + 100)
			{
				//создаем еще одного монстра
				for (int i = 0; i < 1000 && sector_count[sec] < monster_by_sec; ++i)
				{
					if (monster_array[i].isAlive == 0)
					{
						spawn_monster(i, NEW_X, NEW_Y);
						sector_count[sec]++;
						monster_tot++;
						//printf("spawn monster (spawn)\n");
						break;
					}
				}
			}
		}
	}
	//printf("mons_tot=%i monster_by_sec=%0.1f \n", monster_tot++ , monster_by_sec);
	//printf("moster_count=%i\n", monster_count);
}

void Mission::draw_monsters()
{
	double PI = 3.14159265358979323846;
	//проходимся по массиву монстров и рисуем спрайты
	//также нужно проверить коллизии и отрисовать анимацию
	
	//обнуляем массив секторов
	for (int i = 0; i < 16; ++i)
	{
		sector_count[i] = 0;
	}

	for (int i = 0; i < 1000; ++i)
	{

		if (monster_array[i].isAlive == 1) // проверяем жив ли монстр
		{
			//рассчитываем расстояние до игрока
			double dist_to_player = sqrt((Rover.player_coord_X - monster_array[i].X) * (Rover.player_coord_X - monster_array[i].X) + (Rover.player_coord_Y - monster_array[i].Y) * (Rover.player_coord_Y - monster_array[i].Y));

			//меняем направление к игроку если нет рядом препятствия
			if (check_obj_collision(monster_array[i].X, monster_array[i].Y, 85.0f) == 0) monster_array[i].direction = atan2(Rover.player_coord_Y - monster_array[i].Y, Rover.player_coord_X - monster_array[i].X);




			//рассчитываем движение
			if (dist_to_player > monster_array[i].attack_distance && dist_to_player < 2000 * GAME_RENDER_SCALE) //если монстр далеко - движения нет
			{
				//Проверка на коллизии
				int pendulum = 1; // (int)((rand() % 2 - 0.5f) * 2); //маятник
				for (double D_angle = 0.0f; D_angle < PI; D_angle += PI / 90.0f)
				{
					//просчитываем начальное направление + отклонения в обе стороны если монстр попадает в объект
					monster_array[i].direction += D_angle * pendulum;
					
					double new_X = monster_array[i].X + monster_array[i].moving_speed * cos(monster_array[i].direction) * elapsed_ms / 1000;
					double new_Y = monster_array[i].Y + monster_array[i].moving_speed * sin(monster_array[i].direction) * elapsed_ms / 1000;

					//определяем наличие коллизии
					/*
					if (check_obj_collision(new_X, new_Y, 80.0f) == 0)
					{
						monster_array[i].Y = new_Y;
						monster_array[i].X = new_X;
						break;
					}
					else monster_array[i].moving_speed *= 1.00f; //замедляемся при столкновении
					
					*/
					//определяем наличие коллизии
					sf::Vector3f r = find_intersection(monster_array[i].direction, monster_array[i].X, monster_array[i].Y, 400.0);
					if (r.z == -1)
						{
							if (check_obj_collision(new_X, new_Y, 80.0f) == 0)
							{
								monster_array[i].Y = new_Y;
								monster_array[i].X = new_X;
								break;
							}
							else monster_array[i].moving_speed *= 1.00f; //замедляемся при столкновении
						}
					pendulum *= -1;
				}
			}


			//рассчет границ экрана на местности
			double left = -(render_res_X / 2 + Rover.player_offset_X - Rover.player_coord_X / GAME_RENDER_SCALE) * GAME_RENDER_SCALE;
			double right = left + render_res_X * GAME_RENDER_SCALE;
			double top = (render_res_Y / 2 + Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y) * GAME_RENDER_SCALE;
			double bottom = top - render_res_Y * GAME_RENDER_SCALE;

			//если монстр вблизи - рисуем его
			if (monster_array[i].X > left - 200 && monster_array[i].X < right + 200 && monster_array[i].Y > bottom - 200 && monster_array[i].Y < top + 200)
			{
				monster_array[i].anim_time += elapsed_ms;
				if (monster_array[i].anim_time > monster_array[i].anim_time_MAX) monster_array[i].anim_time -= monster_array[i].anim_time_MAX;

				int sprite_dir = 0;
				if (monster_array[i].direction > PI / 4.0 && monster_array[i].direction < PI * 3.0 / 4.0) sprite_dir = 1;
				if (monster_array[i].direction > PI * 3.0 / 4.0 || monster_array[i].direction < -PI * 3.0 / 4.0) sprite_dir = 2;
				if (monster_array[i].direction < -PI / 4.0 && monster_array[i].direction > -PI * 3.0 / 4.0) sprite_dir = 3;

				monster_Sprite.setTextureRect(sf::IntRect(sprite_dir * 300 + floor(3.0 * monster_array[i].anim_time / (monster_array[i].anim_time_MAX + 1)) * 100, monster_array[i].sprite_N * 100, 100, 100));;
				monster_Sprite.setOrigin(50, 50);
				monster_Sprite.setPosition(render_res_X / 2 + Rover.player_offset_X - Rover.player_coord_X / GAME_RENDER_SCALE + monster_array[i].X / GAME_RENDER_SCALE, render_res_Y / 2 + Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y - monster_array[i].Y / GAME_RENDER_SCALE);
				monster_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE * monster_array[i].scale, 1.0 / GAME_RENDER_SCALE * monster_array[i].scale));
				if (monster_array[i].isJugger)
				{
					//у джагернаута дорисовываем эффект
					VFX_Sprite.setTextureRect(sf::IntRect(0, 400, 100, 100));
					VFX_Sprite.setOrigin(50, 50);
					VFX_Sprite.setPosition(render_res_X / 2 + Rover.player_offset_X - Rover.player_coord_X / GAME_RENDER_SCALE + monster_array[i].X / GAME_RENDER_SCALE, render_res_Y / 2 + Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y - monster_array[i].Y / GAME_RENDER_SCALE);
					VFX_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE * monster_array[i].scale, 1.0 / GAME_RENDER_SCALE * monster_array[i].scale));
					VFX_Sprite.setRotation(monster_array[i].anim_time / monster_array[i].anim_time_MAX * 90.0);
					game_window->draw(VFX_Sprite);
				}

				//рисуем № монстра для отладки

				text.setCharacterSize(15 * render_res_X / 1920.0); // in pixels, not points!
				text.setFillColor(sf::Color::Red);
				//text.setStyle(sf::Text::Bold | sf::Text::Underlined);
				char buffer[50];
				sprintf_s(buffer, "%i", i);
				text.setOrigin(text.getLocalBounds().width / 2.0, text.getLocalBounds().height / 2.0);
				text.setPosition(render_res_X / 2 + Rover.player_offset_X - Rover.player_coord_X / GAME_RENDER_SCALE + monster_array[i].X / GAME_RENDER_SCALE, render_res_Y / 2 + Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y - monster_array[i].Y / GAME_RENDER_SCALE - 150.0 / GAME_RENDER_SCALE);
				text.setString(buffer);
				game_window->draw(text);
				
				//проверяем наличие включенного щита
				int shield_power = 0;
				for (int t = 0; t < 3; ++t)
				{
					if (Rover.active_slots > t && Rover.slots[t] == 4 && Rover.Energy > 0) 
					{
						shield_power++;
					}
				}

				//проверяем на попадание в силовое поле
				bool in_shield = false;
				if (dist_to_player < 250.0 && shield_power > 0) in_shield = true;

				//уменьшаем здоровье монстра
				if (in_shield && Rover.Energy > 0 && monster_array[i].health > 0)
				{
					monster_array[i].health -= elapsed_ms / 1000.0 * 0.5 * shield_power;
					if (monster_array[i].health < 0) monster_array[i].health = 0;
					Rover.Energy -= elapsed_ms / 1000.0 * 0.5 * shield_power;
					if (Rover.Energy < 0) Rover.Energy = 0;
					//printf("m health %0.1f \n", monster_array[i].health);
				}

				if (monster_array[i].health > 0) game_window->draw(monster_Sprite);
				else
				{
					//если здоровье < 0, убиваем монстра
					monster_array[i].isAlive = 0; //фиксируем смерть
					monster_count--;
					//создаем спецэффект на месте монстра
					create_VFX(monster_array[i].X, monster_array[i].Y, 0, monster_array[i].scale);
					//sound_death_1.play();
					Rover.collected_money += monster_array[i].power;//гонорар
					if (monster_array[i].isJugger)
					{
						//выбрасываем ресурсы, если монстр мощный
						drop_flying_loot((planet_level + 1) * 2, (planet_level + 1) * 1, (int)floor((planet_level + 1) * 0.5), monster_array[i].X, monster_array[i].Y);
						//издаем жуткий звук
						sound_death_1.setVolume(game_options->sound_volume);
						sound_death_1.play();
					}

				}

				monster_array[i].attack_cooldown -= elapsed_ms;

				//атака монстра
				//проверяем наличие объектов перед игроком
				sf::Vector3f r = find_intersection(monster_array[i].direction, monster_array[i].X, monster_array[i].Y, 1000.0);
				bool monster_attack = false;
				if (r.z == -1) monster_attack = true;
				else 
				{
					double dist_to_obj = sqrt((monster_array[i].X - r.x) * (monster_array[i].X - r.x) + (monster_array[i].Y - r.y) * (monster_array[i].Y - r.y));
					if (dist_to_obj > dist_to_player)
					{
						monster_attack = true;
				
					}
				}
				printf("#%i direct=%0.2f r.z=%0.0f\n", i, monster_array[i].direction, r.z);
				if (monster_attack && monster_array[i].attack_cooldown <= 0 && dist_to_player < monster_array[i].attack_distance && (monster_array[i].X > left && monster_array[i].X < right && monster_array[i].Y > bottom && monster_array[i].Y < top) && (Rover.Health > 0) && monster_array[i].health > 0)
				{
					//производим атаку расстояние до игрока меньше дальности атаки
					monster_array[i].attack_cooldown = monster_array[i].attack_time; //перезарядка таймера
					//ищем место в массиве пуль монстров
					for (int j = 0; j < 100; ++j)
					{
						if (monster_bullet_array[j].isAlive == 0)
						{
							//заполняем массив данными пули
							monster_bullet_array[j].isAlive = 1;
							monster_bullet_array[j].type = monster_array[i].attack_type;
							monster_bullet_array[j].sprite_N = 0;//дефолтный уровень
							if (monster_array[i].attack_type == 0) monster_bullet_array[j].sprite_N = 0;
							if (monster_array[i].attack_type == 1) monster_bullet_array[j].sprite_N = 1;
							if (monster_array[i].attack_type == 2) monster_bullet_array[j].sprite_N = 2;
							monster_bullet_array[j].power = monster_array[i].power;
							//скорость и время по-умолчанию (рукопашка)
							monster_bullet_array[j].speed = 500.0;//дефолтные данные
							monster_bullet_array[j].ttl = 300; //оставшееся время жизни
							if (monster_array[j].attack_type == 0) //укус
							{
								monster_bullet_array[j].speed = 800.0;
								monster_bullet_array[j].ttl = 5000; //оставшееся время жизни
							}
							if (monster_array[j].attack_type == 1) //плазма
							{
								monster_bullet_array[j].speed = 400.0;
								monster_bullet_array[j].ttl = 3000; //оставшееся время жизни
							}

							if (monster_array[j].attack_type == 2) //электричество
							{
								monster_bullet_array[j].speed = 400.0;
								monster_bullet_array[j].ttl = 3000; //оставшееся время жизни
							}
							monster_bullet_array[j].direction = atan2(Rover.player_coord_Y - monster_array[i].Y, Rover.player_coord_X - monster_array[i].X); //стреляем прямо в игрока
							monster_bullet_array[j].X = monster_array[i].X;
							monster_bullet_array[j].Y = monster_array[i].Y;
							//printf("monster shot type=%i sprite_N=%i dist=%0.0f\n", monster_bullet_array[j].type, monster_bullet_array[j].sprite_N, dist_to_player);
							break;
						}
					}
				}
			}
			//расчет количества монстров по секторам
			// рассчитываем номер сектора
			double ang = atan2((monster_array[i].Y - Rover.player_coord_Y), (monster_array[i].X - Rover.player_coord_X));
			if (ang < 0) ang += 2 * PI;
			int sec = (int)floor(16.0 * ang / (2 * PI+ 0.001));
			if (dist_to_player < 3000.0 || 1) sector_count[sec]++;
			//printf("(draw) i=%i ang=%0.2f sec=%i sec_quant=%i dist=%0.0f\n", i, ang, sec, sector_count[sec], dist_to_player);
		}
	}
}

void Mission::create_VFX(double VFX_X, double VFX_Y, int VFX_type, double scale)
{
	// 0 - взрыв монстра, 1 - укус, 2 - маленький взрыв снаряда, 3 - большой взрыв ракеты, 4 взрыв плазмы
	//поиск свободного слота	
	//printf("VFX  type=%i  scale = %i \n", VFX_type, scale);
	int i;
	for (i = 0; i < 100 && VFX_array[i].isAlive > 0; ++i);
	if (VFX_array[i].isAlive > 0) return; //если все слоты заняты - возврат

	if (VFX_type == 0) //взрыв монстра
	{
		//добавляем строку в массив
		VFX_array[i].isAlive = 1; // 0 - свободный слот, 1 - анимация
		VFX_array[i].number = 0;//номер в атласе. размер у всех 100*100
		VFX_array[i].last_frame = 16;
		VFX_array[i].keep_alive = 0; // 0 или 1 - убрать
		VFX_array[i].total_time = 500; //в микросекундах
		VFX_array[i].remaining_time = 500; // = total_time
		VFX_array[i].remains_num = 1;//после анимации оставить останки
		VFX_array[i].scale = scale;
		VFX_array[i].X = VFX_X;
		VFX_array[i].Y = VFX_Y;
		//printf("create VFX = 0\n");
	}

	if (VFX_type == 1) // укус монстра
	{
		//добавляем строку в массив
		VFX_array[i].isAlive = 1; // 0 - свободный слот, 1 - анимация
		VFX_array[i].number = 1;//номер в атласе. размер у всех 100*100
		VFX_array[i].last_frame = 5;
		VFX_array[i].keep_alive = 0; // 0 или 1 - убрать
		VFX_array[i].total_time = 500; //в микросекундах
		VFX_array[i].remaining_time = 500; // = total_time
		VFX_array[i].remains_num = 0;//после анимации оставить останки
		VFX_array[i].scale = scale;
		VFX_array[i].X = VFX_X;
		VFX_array[i].Y = VFX_Y;
		//printf("create VFX = 1\n");
	}

	if (VFX_type == 2) //маленький взрыв снаряда
	{
		//добавляем строку в массив
		VFX_array[i].isAlive = 1; // 0 - свободный слот, 1 - анимация
		VFX_array[i].number = 2;//номер в атласе. размер у всех 100*100
		VFX_array[i].last_frame = 16;
		VFX_array[i].keep_alive = 0; // 0 или 1 - убрать
		VFX_array[i].total_time = 500; //в микросекундах
		VFX_array[i].remaining_time = 500; // = total_time
		VFX_array[i].remains_num = 0;//после анимации оставить останки
		VFX_array[i].scale = scale;
		VFX_array[i].X = VFX_X;
		VFX_array[i].Y = VFX_Y;
		//printf("create VFX = 2\n");
	}

	if (VFX_type == 3) //большой взрыв ракеты/гранаты
	{
		//добавляем строку в массив
		VFX_array[i].isAlive = 1; // 0 - свободный слот, 1 - анимация
		VFX_array[i].number = 3;//номер в атласе. размер у всех 100*100
		VFX_array[i].last_frame = 36;
		VFX_array[i].keep_alive = 0; // 0 или 1 - убрать
		VFX_array[i].total_time = 1000; //в микросекундах
		VFX_array[i].remaining_time = 1000;
		VFX_array[i].remains_num = 3;//после анимации оставить останки
		VFX_array[i].scale = scale;
		VFX_array[i].X = VFX_X;
		VFX_array[i].Y = VFX_Y;
		//printf("create VFX = 2\n");
	}

	if (VFX_type == 5) // взрыв плазмы
	{
		//добавляем строку в массив
		VFX_array[i].isAlive = 1; // 0 - свободный слот, 1 - анимация
		VFX_array[i].number = 5;//номер в атласе. размер у всех 100*100
		VFX_array[i].last_frame = 25;
		VFX_array[i].keep_alive = 0; // 0 или 1 - убрать
		VFX_array[i].total_time = 600; //в микросекундах
		VFX_array[i].remaining_time = 600; // = total_time
		VFX_array[i].remains_num = 0;//после анимации оставить останки
		VFX_array[i].scale = scale;
		VFX_array[i].X = VFX_X;
		VFX_array[i].Y = VFX_Y;
		//printf("create VFX = 2\n");
	}
	
	if (VFX_type == 6) // взрыв эл-ва/холода
	{
		//добавляем строку в массив
		VFX_array[i].isAlive = 1; // 0 - свободный слот, 1 - анимация
		VFX_array[i].number = 6;//номер в атласе. размер у всех 100*100
		VFX_array[i].last_frame = 25;
		VFX_array[i].keep_alive = 0; // 0 или 1 - убрать
		VFX_array[i].total_time = 600; //в микросекундах
		VFX_array[i].remaining_time = 600; // = total_time
		VFX_array[i].remains_num = 0;//после анимации оставить останки
		VFX_array[i].scale = scale;
		VFX_array[i].X = VFX_X;
		VFX_array[i].Y = VFX_Y;
		//printf("create VFX = 2\n");
	}


}

void Mission::draw_VFX()
{
	//return;
	for (int i = 0; i < 100; ++i)
	{
		if (VFX_array[i].isAlive == 0) continue; //если это поле = 0, переход к следующему элементу
		int pos = floor((VFX_array[i].total_time - VFX_array[i].remaining_time) / VFX_array[i].total_time * (VFX_array[i].last_frame - 1)) * 100;
		VFX_Sprite.setTextureRect(sf::IntRect(pos, VFX_array[i].number * 100, 100, 100));
		VFX_Sprite.setOrigin(50, 50);
		VFX_Sprite.setPosition(render_res_X / 2 + Rover.player_offset_X - Rover.player_coord_X / GAME_RENDER_SCALE + VFX_array[i].X / GAME_RENDER_SCALE, render_res_Y / 2 + Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y - VFX_array[i].Y / GAME_RENDER_SCALE);
		VFX_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE * VFX_array[i].scale, 1.0 / GAME_RENDER_SCALE * VFX_array[i].scale));
		VFX_Sprite.setRotation(0.0);
		game_window->draw(VFX_Sprite);
		//увеличиваем счетчик времени
		VFX_array[i].remaining_time -= elapsed_ms;
		
		if (VFX_array[i].remaining_time <= 0)
		{
			VFX_array[i].isAlive = 0; //убиваем спрайт
			//рисуем останки
			create_VFX_remains(VFX_array[i].X, VFX_array[i].Y, VFX_array[i].remains_num);

		}
	}
}

void Mission::draw_VFX_remains()
{
	for (int i = 0; i < 1000; ++i)
	{
		if (VFX_remains_array[i].isAlive == 1)
		{
			//проверяем координаты
			//нумерация спрайтов начинается с 1
			double sprite_X = render_res_X / 2.0 + Rover.player_offset_X - ((Rover.player_coord_X - VFX_remains_array[i].X) / GAME_RENDER_SCALE);
			double sprite_Y = render_res_Y / 2.0 - Rover.player_offset_Y + ((Rover.player_coord_Y - VFX_remains_array[i].Y) / GAME_RENDER_SCALE);
			if (sprite_X > 0 && sprite_X < render_res_X && sprite_Y > 0 && sprite_Y < render_res_Y)
			{
				//рисуем спрайт
				if (VFX_remains_array[i].number != 4)
				{
					VFX_remains_Sprite.setTextureRect(sf::IntRect((VFX_remains_array[i].number - 1) * 100, 0, 100, 100));
					VFX_remains_Sprite.setOrigin(50, 50);
					VFX_remains_Sprite.setPosition(sprite_X, sprite_Y);
					VFX_remains_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
					VFX_remains_Sprite.setColor(sf::Color(255, 255, 255, 128));
					VFX_remains_Sprite.setRotation(VFX_remains_array[i].rotation);
					game_window->draw(VFX_remains_Sprite);
				}
				else
				{
					tires_Sprite.setOrigin(100, 100);
					tires_Sprite.setPosition(sprite_X, sprite_Y);
					tires_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
					tires_Sprite.setColor(sf::Color(255, 255, 255, 40));
					tires_Sprite.setRotation(VFX_remains_array[i].rotation);
					game_window->draw(tires_Sprite);
				}

			}
		}
	}
}

void Mission::create_VFX_remains(double VFX_X, double VFX_Y, int VFX_type, double angle)
{
	VFX_remains_array[VFX_remains_pointer].isAlive = 1;
	VFX_remains_array[VFX_remains_pointer].number = VFX_type;
	VFX_remains_array[VFX_remains_pointer].X = VFX_X;
	VFX_remains_array[VFX_remains_pointer].Y = VFX_Y;
	VFX_remains_array[VFX_remains_pointer].rotation = angle;
	VFX_remains_pointer++;
	if (VFX_remains_pointer == 1000) VFX_remains_pointer = 0;
}

void Mission::draw_monster_bullets()
{
	
	//проверяем наличие включенного щита
	int shield_power = 0;
	for (int t = 0; t < 3; ++t)
	{
		if (Rover.active_slots > t && Rover.slots[t] == 3 && Rover.Energy > 0) //добавляем радиус и урон
		{
			shield_power++;
		}
	}

	for (int i = 0; i < 100; ++i)
	{
		if (monster_bullet_array[i].isAlive == 1)
		{
			//движение пули
			monster_bullet_array[i].X += monster_bullet_array[i].speed * cos(monster_bullet_array[i].direction) * elapsed_ms / 1000;
			monster_bullet_array[i].Y += monster_bullet_array[i].speed * sin(monster_bullet_array[i].direction) * elapsed_ms / 1000;

			//проверяем координаты
			double sprite_X = render_res_X / 2.0 + Rover.player_offset_X - ((Rover.player_coord_X - monster_bullet_array[i].X) / GAME_RENDER_SCALE);
			double sprite_Y = render_res_Y / 2.0 - Rover.player_offset_Y + ((Rover.player_coord_Y - monster_bullet_array[i].Y) / GAME_RENDER_SCALE);
			monster_bullet_array[i].ttl -= elapsed_ms;

			if (monster_bullet_array[i].ttl <= 0 || monster_bullet_array[i].power == 0)
			{
				monster_bullet_array[i].isAlive = 0;//удаляем пулю из массива
				//если это граната, создаем взрыв
				//if (monster_bullet_array[i].type == 2) make_explosion(monster_bullet_array[i].X, monster_bullet_array[i].Y, monster_bullet_array[i].power, 1, 0);
				continue;
			}

			//если пуля внутри щита - уменьшаем ее силу и уменьшаем энергию щита
			double dist_to_player = 0.0;
			if (shield_power > 0) dist_to_player = sqrt((monster_bullet_array[i].X - Rover.player_coord_X) * (monster_bullet_array[i].X - Rover.player_coord_X) + (monster_bullet_array[i].Y - Rover.player_coord_Y) * (monster_bullet_array[i].Y - Rover.player_coord_Y));
			bool in_shield = false;
			if (dist_to_player < 250.0 && shield_power > 0) in_shield = true;
			//if (in_shield) printf("in shield\n");

			//уменьшаем силу пули
			if (in_shield && Rover.Energy > 0 && monster_bullet_array[i].type > 0)  //укусы не попадают под эффект
			{
				monster_bullet_array[i].power -= elapsed_ms / 1000.0 * 2.0 * shield_power;
				if (monster_bullet_array[i].power < 0) monster_bullet_array[i].power = 0;
				Rover.Energy -= elapsed_ms / 1000.0 * 2.0 * shield_power;
				if (Rover.Energy < 0) Rover.Energy = 0;
				//printf("%0.1f %0.1f \n", monster_bullet_array[i].power, Rover.Energy);
			}

			//проверка попадания в объекты
			if (monster_bullet_array[i].isAlive == 1) //повторная проверка т.к. пуля могла попасть в монстра
			{
				if (check_obj_collision(monster_bullet_array[i].X, monster_bullet_array[i].Y, -20.0f))
				{
					//фиксируем попадание
					monster_bullet_array[i].isAlive = 0;//пуля исчезает после попадания
					monster_bullet_count--;
					if (monster_bullet_array[i].type == 1)
					{
						create_VFX(monster_bullet_array[i].X, monster_bullet_array[i].Y, 5, 1); // тип 0 - взрыв снаряда
						sound_hit_4.setVolume(game_options->sound_volume);
						sound_hit_4.play();
					}
					if (monster_bullet_array[i].type == 2)
					{
						create_VFX(monster_bullet_array[i].X, monster_bullet_array[i].Y, 6, 1); // тип 0 - взрыв снаряда
						sound_hit_4.setVolume(game_options->sound_volume);
						sound_hit_4.play();
					}
				}
			}

			if (sprite_X > 0 && sprite_X < render_res_X && sprite_Y > 0 && sprite_Y < render_res_Y && monster_bullet_array[i].isAlive == 1)
			{
				//рисуем спрайт
				monster_bullet_Sprite.setTextureRect(sf::IntRect(0, (monster_bullet_array[i].sprite_N) * 100, 100, 100));
				monster_bullet_Sprite.setOrigin(50, 50);
				monster_bullet_Sprite.setPosition(sprite_X, sprite_Y);
				monster_bullet_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
				monster_bullet_Sprite.setColor(sf::Color(255, 255, 255, 255));
				monster_bullet_Sprite.setRotation(-monster_bullet_array[i].direction * 180.0 / 3.1415926);
				game_window->draw(monster_bullet_Sprite);

				//проверяем на попадание
				if (abs(monster_bullet_array[i].X - Rover.player_coord_X) < 50 && abs(monster_bullet_array[i].Y - Rover.player_coord_Y) < 50 && (Rover.Health > 0))
				{
					//фиксируем попадание
					Rover.Health -= monster_bullet_array[i].power; //уменьшаем здоровье игрока
					//printf("health=%0.0f", Rover.Health);
					if (Rover.Health <= 0)
					{
						//printf("Player died\n");
						//создаем спецэффект смерти на месте игрока
						//create_VFX(Rover.player_coord_X, Rover.player_coord_Y, 3, 4); // тип 2 - взрыв
						//sound_explode_1.play();
					}
					
					//printf("bullet hit type=%i \n", monster_bullet_array[i].type);
					monster_bullet_array[i].isAlive = 0;//пуля исчезает после попадания
					monster_bullet_count--;
					//создаем спецэффект месте попадания
					if (monster_bullet_array[i].type == 0)
					{
						create_VFX(monster_bullet_array[i].X, monster_bullet_array[i].Y, 1, 1); // тип 1 - укус
						sound_bite_1.setVolume(game_options->sound_volume);
						sound_bite_1.play();
					}
					if (monster_bullet_array[i].type == 1)
					{
						create_VFX(monster_bullet_array[i].X, monster_bullet_array[i].Y, 5, 1); // тип 2 - взрыв плазмы
						sound_hit_4.setVolume(game_options->sound_volume);
						sound_hit_4.play();
					}
					if (monster_bullet_array[i].type == 2)
					{
						create_VFX(monster_bullet_array[i].X, monster_bullet_array[i].Y, 6, 1); // тип 2 - взрыв электричества
						sound_hit_4.setVolume(game_options->sound_volume);
						sound_hit_4.play();
					}
				}
			}
		}
	}
}

void Mission::make_explosion(double X, double Y, double power, bool player_damage, bool monster_damage)
{
	//monster_damage = 1;
	double radius = 200.0;//радиус взрыва
	double exlposion_power = power;
	
	for (int t = 0; t < 3; ++t)
	{
		if (Rover.active_slots > t && Rover.slots[t] == 3 && !player_damage) //добавляем радиус и урон
		{
			radius *= 0.1;
			exlposion_power *= 0.2; //урон для монстров
		}
	}
	
	create_VFX(X, Y, 3, 4);
	sound_explode_1.play();

	//расчет попадания в игрока
	if (player_damage)
	{
		double distance = sqrt((Rover.player_coord_X - X) * (Rover.player_coord_X - X) + (Rover.player_coord_Y - Y) * (Rover.player_coord_Y - Y));
		if (distance < radius)
		{
			//рассчитываем ущерб игроку
			Rover.Health -= power * (1.0 - distance / radius / 2); //пропорционально расстоянию
			//printf("expl damage = %0.0f\n", power * (1.0 - distance / radius));
		}
	}

	//расчет попадания в монстров
	if (monster_damage)
	{
		for (int i = 0; i < 1000; ++i)
		{
			if (monster_array[i].isAlive == 1)
			{
				//считаем дистанцию до монстра	
				double distance_from_expl = sqrt((monster_array[i].X - X) * (monster_array[i].X - X) + (monster_array[i].Y - Y) * (monster_array[i].Y - Y));
				if (distance_from_expl < radius)
				{
					//рассчитываем ущерб монстру
					monster_array[i].health -= exlposion_power * (1.0 - distance_from_expl / radius / 2); //пропорционально расстоянию
					//printf("expl damage = %0.0f\n", power * (1.0 - distance / radius));
					if (monster_array[i].health <= 0)
					{
						monster_array[i].isAlive = 0; //фиксируем смерть
						monster_count--;
						//создаем спецэффект на месте монстра
						create_VFX(monster_array[i].X, monster_array[i].Y, 0, 1); // тип 0 - взрыв монстра
						Rover.collected_money += monster_array[i].power;
						
						if (monster_array[i].isJugger)
						{
							//выбрасываем ресурсы, если монстр мощный
							drop_flying_loot((planet_level + 1) * 2, (planet_level + 1) * 1, (int)floor((planet_level + 1) * 0.5), monster_array[i].X, monster_array[i].Y);
							//издаем жуткий звук
							sound_death_1.setVolume(game_options->sound_volume);
							sound_death_1.play();
						}



					}
				}
			}
		}
	}
}

void Mission::generate_stationary_obj()
{
	//создаем базу игрока

	obj_array[0].isValid = 1;
	obj_array[0].type = 0; //тип база
	obj_array[0].X = 0.0;
	obj_array[0].Y = 0.0;
	obj_array[0].size_X = 800;
	obj_array[0].size_Y = 600;

	//создаем точки выхода 1-8
	for (int i = 1; i < 9; ++i)
	{
		obj_array[i].isValid = 1;
		obj_array[i].type = 1; //тип корабль / точка выхода
		obj_array[i].X = cos(i * 2 * 3.1415 / 8) * 15000.0;
		obj_array[i].Y = sin(i * 2 * 3.1415 / 8) * 15000.0;
		obj_array[i].size_X = 318;
		obj_array[i].size_Y = 582;
	}

	//создаем гнезда выхода 9-16
	for (int i = 9; i < 17; ++i)
	{
		obj_array[i].isValid = 1;
		obj_array[i].type = 2; //тип гнездо
		obj_array[i].X = cos(i * 2 * 3.1415 / 8 + 20.0) * 10000.0;
		obj_array[i].Y = sin(i * 2 * 3.1415 / 8 + 20.0) * 10000.0;
		obj_array[i].size_X = 200;
		obj_array[i].size_Y = 200;
		obj_array[i].Health = 100;
		obj_array[i].Health_MAX = 100;
	}

	//создаем гнезда выхода 17-30
	for (int i = 18; i < 31; ++i)
	{
		obj_array[i].isValid = 1;
		obj_array[i].type = 2; //тип гнездо
		obj_array[i].X = cos(i * 2 * 3.1415 / 14 + 20.0) * 25000.0;
		obj_array[i].Y = sin(i * 2 * 3.1415 / 14 + 20.0) * 25000.0;
		obj_array[i].size_X = 200;
		obj_array[i].size_Y = 200;
		obj_array[i].Health = 200;
		obj_array[i].Health_MAX = 200;
	}




}

void Mission::draw_stationary_obj()
{
	//рисуем стационарные объекты

	for (int i = 0; i < 100; ++i)
	{
		if (obj_array[i].isValid == 1)
		{
			if (obj_array[i].type == 0)  //база
			{
				base_Sprite.setOrigin(obj_array[i].size_X / 2.0, obj_array[i].size_Y / 2.0);
				base_Sprite.setPosition(render_res_X / 2 + Rover.player_offset_X - Rover.player_coord_X / GAME_RENDER_SCALE + obj_array[i].X / GAME_RENDER_SCALE, render_res_Y / 2 + Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y - obj_array[i].Y / GAME_RENDER_SCALE);
				//base_Sprite.setColor(sf::Color(255, 255, 255, 100));
				base_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
				game_window->draw(base_Sprite);
			}

			if (obj_array[i].type == 1)  //корабль
			{
				ship_Sprite.setOrigin(obj_array[i].size_X / 2.0, obj_array[i].size_Y / 2.0);
				ship_Sprite.setPosition(render_res_X / 2 + Rover.player_offset_X - Rover.player_coord_X / GAME_RENDER_SCALE + obj_array[i].X / GAME_RENDER_SCALE, render_res_Y / 2 + Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y - obj_array[i].Y / GAME_RENDER_SCALE);
				ship_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
				game_window->draw(ship_Sprite);
			}
			
			if (obj_array[i].type == 2) //гнезда
			{
				nest_Sprite.setOrigin(obj_array[i].size_X / 2.0, obj_array[i].size_Y / 2.0);
				nest_Sprite.setPosition(render_res_X / 2 + Rover.player_offset_X - Rover.player_coord_X / GAME_RENDER_SCALE + obj_array[i].X / GAME_RENDER_SCALE, render_res_Y / 2 + Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y - obj_array[i].Y / GAME_RENDER_SCALE);
				nest_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
				game_window->draw(nest_Sprite);

				//рисуем здоровье гнезда
				double bar_Width = 200 * render_res_X / 1920.0;
				double bar_Height = 10 * render_res_Y / 1080.0;

				sf::RectangleShape health_rectangle(sf::Vector2f(bar_Width * (obj_array[i].Health/ obj_array[i].Health_MAX), bar_Height));
				health_rectangle.setFillColor(sf::Color::Red);
				health_rectangle.setPosition(render_res_X / 2 + Rover.player_offset_X - Rover.player_coord_X / GAME_RENDER_SCALE + obj_array[i].X / GAME_RENDER_SCALE - bar_Width / 2.0,
											 render_res_Y / 2 + Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y - obj_array[i].Y / GAME_RENDER_SCALE - obj_array[i].size_Y / GAME_RENDER_SCALE - bar_Height);
				game_window->draw(health_rectangle);
				health_rectangle.setSize(sf::Vector2f(bar_Width , bar_Height));
				health_rectangle.setOutlineColor(sf::Color::White);
				health_rectangle.setFillColor(sf::Color(0,0,0,0));
				health_rectangle.setOutlineThickness(2);
				game_window->draw(health_rectangle);

				//генерация монстров при приближении игрока
				obj_array[i].spec_data -= elapsed_ms;
				if (obj_array[i].spec_data < 0) obj_array[i].spec_data = 0;

				//определяем, попадает ли гнездо в зону видимости + 200 и "остыл" ли таймер
				if (obj_array[i].spec_data == 0)
				{
					//границы экрана на "местности"
					double left = - (render_res_X / 2 + Rover.player_offset_X - Rover.player_coord_X / GAME_RENDER_SCALE) * GAME_RENDER_SCALE;
					double right = left + render_res_X * GAME_RENDER_SCALE;
					double top = (render_res_Y / 2 + Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y) * GAME_RENDER_SCALE;
					double bottom = top - render_res_Y * GAME_RENDER_SCALE;
					
					if (obj_array[i].X > (left - 200) && obj_array[i].X < (right + 200))
					{
						if (obj_array[i].Y > (bottom - 200) && obj_array[i].Y < (top + 200))
						{
							//выпускаем монстра и запускаем таймаут
							for (int j = 0; j < 1000; ++j)  //ищем место в массиве монстров
							{
								if (monster_array[j].isAlive == 0)
								{
									//создаем случайную точку
									double direction = atan2(Rover.player_coord_Y - obj_array[i].Y, Rover.player_coord_X - obj_array[i].X);
									spawn_monster(j, obj_array[i].X + cos(direction) * 300, obj_array[i].Y + sin(direction) * 300);
									obj_array[i].spec_data = 2000; //таймер
									//printf("spawn monster (nest)\n");
									break;
								}
							}
						}
					}
				}
			}
		}
	}
}

void Mission::move_player(int acceleration)
{
	double direction = 0.0;
	double PI = 3.14159265358979323846;
	if (Rover.body_direction == 0) return;
	if (Rover.body_direction == 1) direction = PI * 0.5;
	if (Rover.body_direction == 2) direction = PI * 0.25;
	if (Rover.body_direction == 3) direction = 0.0;
	if (Rover.body_direction == 4) direction = -PI * 0.25;
	if (Rover.body_direction == 5) direction = -PI * 0.5;
	if (Rover.body_direction == 6) direction = -PI * 0.75;
	if (Rover.body_direction == 7) direction = PI;
	if (Rover.body_direction == 8) direction = PI * 0.75;

	sf::Vector2f moving_vector; //вектор движения
	moving_vector.x = Rover.Speed * cos(Rover.Speed_direction);
	moving_vector.y = Rover.Speed * sin(Rover.Speed_direction);

	
	bool Turn_Left = false, Turn_Right = false;
	if (acceleration == 1)
	{
		//ускорение путем сложения векторов
		sf::Vector2f accel_vector; //вектор ускорения
		accel_vector.x = Rover.Speed_MAX * cos(direction) * 0.05f;
		accel_vector.y = Rover.Speed_MAX * sin(direction) * 0.05f;
		moving_vector += accel_vector; 
		
		double R_turn = Rover.Speed_direction - direction;
		if (R_turn < 0) R_turn += PI * 2.0;
		double L_turn = direction - Rover.Speed_direction;
		if (L_turn < 0) L_turn += PI * 2.0;
		if (R_turn >= L_turn && L_turn >= PI / 2.0)	Turn_Right = true;
		if (R_turn < L_turn && R_turn >= PI / 2.0)	Turn_Left = true;

	}
	else
	{
		//замедление
		moving_vector.x *= 0.9f;
		moving_vector.y *= 0.9f;
	}

	//обновляем данные игрока
	Rover.Speed_direction = atan2(moving_vector.y, moving_vector.x);
	Rover.Speed = sqrt(moving_vector.y * moving_vector.y + moving_vector.x * moving_vector.x);
	if (Rover.Speed > Rover.Speed_MAX) Rover.Speed = Rover.Speed_MAX;
	//дополнительный подворот корпуса при разворотах
	if (Turn_Right) Rover.Speed_direction += PI / 180.0 * 5.0;
	if (Turn_Left) Rover.Speed_direction -= PI / 180.0 * 5.0;


	//Проверка на коллизии
	int pendulum = 1; //маятник
	for (double D_angle = 0.0; D_angle < PI; D_angle += PI / 90.0)
	{
		//просчитываем начальное направление + отклонения в обе стороны если игрок попадает в объект
		Rover.Speed_direction += D_angle * pendulum;
		double new_X = Rover.player_coord_X + cos(Rover.Speed_direction) * Rover.Speed / 1000 * elapsed_ms;
		double new_Y = Rover.player_coord_Y + sin(Rover.Speed_direction) * Rover.Speed / 1000 * elapsed_ms;

		//определяем наличие коллизии
		if (check_obj_collision(new_X, new_Y, 80.0f) == 0)
		{
			Rover.player_coord_Y = new_Y;
			Rover.player_coord_X = new_X;
			break;
		}
		else (Rover.Speed *= 0.99f); //замедляемся при столкновении
		pendulum *= -1;
	}

	if (visited_max_Y < Rover.player_coord_Y) visited_max_Y = Rover.player_coord_Y; //расширяем границы исследованной области
	if (visited_max_X < Rover.player_coord_X) visited_max_X = Rover.player_coord_X; //расширяем границы исследованной области
	if (visited_min_Y > Rover.player_coord_Y) visited_min_Y = Rover.player_coord_Y; //расширяем границы исследованной области
	if (visited_min_X > Rover.player_coord_X) visited_min_X = Rover.player_coord_X; //расширяем границы исследованной области

	Rover.player_offset_Y = Rover.player_offset_Y + sin(Rover.Speed_direction) * Rover.Speed / 1000 * elapsed_ms / GAME_RENDER_SCALE;
	Rover.player_offset_X = Rover.player_offset_X + cos(Rover.Speed_direction) * Rover.Speed / 1000 * elapsed_ms / GAME_RENDER_SCALE;
	if (Rover.player_offset_Y > player_max_offset_Y) Rover.player_offset_Y = player_max_offset_Y;
	if (Rover.player_offset_Y < -player_max_offset_Y) Rover.player_offset_Y = -player_max_offset_Y;
	if (Rover.player_offset_X > player_max_offset_X) Rover.player_offset_X = player_max_offset_X;
	if (Rover.player_offset_X < -player_max_offset_X) Rover.player_offset_X = -player_max_offset_X;

}

int Mission::check_obj_collision(double X, double Y, double offset)
{
	for (int i = 0; i < 100; ++i)
	{
		if (obj_array[i].isValid == 1)
		{
			if (X > (obj_array[i].X - obj_array[i].size_X / 2.0 - offset) &&
				X < (obj_array[i].X + obj_array[i].size_X / 2.0 + offset) &&
				Y >(obj_array[i].Y - obj_array[i].size_Y / 2.0 - offset) &&
				Y < (obj_array[i].Y + obj_array[i].size_Y / 2.0 + offset)) return i + 1;
		}
	}
	return 0;
}


int Mission::player_death()  //DEL
{
	double int_scale_x = render_res_X / 1920.0;
	double int_scale_y = render_res_Y / 1080.0;
	sf::Sprite screenshot_Sprite;

	screenshot_Sprite.setTexture(screenshot_Texture);
	screenshot_Sprite.setColor(sf::Color(255, 255, 255, 50));
	game_window->clear();
	game_window->draw(screenshot_Sprite);

	//диалоговое окно
	sf::Vector2f dialog_center(render_res_X / 2.0, render_res_Y / 2.0); //позиция окна  // удалить
	death_dialog_Sprite.setOrigin(death_dialog_Sprite.getLocalBounds().width / 2.0, death_dialog_Sprite.getLocalBounds().height / 2.0);
	death_dialog_Sprite.setPosition(dialog_center);
	death_dialog_Sprite.setScale(int_scale_x, int_scale_y);
	game_window->draw(death_dialog_Sprite);

	//рисуем курсор
	sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);
	mouse_pointer_Sprite.setPosition((float)localPosition.x, (float)localPosition.y);
	game_window->draw(mouse_pointer_Sprite);
	game_window->display();

	//проверяем нажатие кнопок
	int Return_area[4] = { render_res_Y / 2 + 256 * int_scale_y, render_res_Y / 2 + 321 * int_scale_y, render_res_X / 2 - 556 * int_scale_x, render_res_X / 2 + 56 * int_scale_x };//top,bottom,left,right
	int Runaway_area[4] = { render_res_Y / 2 + 256 * int_scale_y, render_res_Y / 2 + 321 * int_scale_y, render_res_X / 2 + 51 * int_scale_x, render_res_X / 2 + 551 * int_scale_x };//top,bottom,left,right

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//printf("%i  %i \n", localPosition.x, localPosition.y);

		// проверяем нажатие на кнопки
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Return_area[0]))
		{
			printf("Return \n");
			//обнуляем персонажа
			reset_Rover();
			return (1);//возврат на базу
		}


		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Runaway_area[0]))
		{
			printf("RUN!!! \n");
			return (2);//выход с уровня
		}


	}
	//sf::sleep(sf::milliseconds(1000));
	return (0);//продолжаем выполнение подпрограммы
}

void Mission::reset_Rover()
{
	death_delay = 3000;
	
	//обнуление параметров персонажа
	Rover.Health = Rover.Health_MAX;
//  Rover.Health_MAX = 100.0;
	Rover.Energy = 50; // Rover.Energy_MAX;
//	Rover.Energy_MAX = 100.0;
	Rover.Speed = 0.0;
//	Rover.Speed_MAX = 400.0;
	Rover.Speed_direction = 0.0;
	Rover.gun_damage = 1.0;
//	Rover.fire_rate = 4.0;
// 	Rover.fire_rate_boost = pow(1.0185, rover_upgrades[4]); //не меняется
//	Rover.active_slots = 0;
	for (int i = 0; i < 3; ++i) Rover.slots[i] = -1;//обнуляем все слоты (-1)
	//printf("slots empty\n");
	Rover.active_slots = 3;
	Rover.slots[0] = 2;
	Rover.slots[1] = 1;
	Rover.slots[2] = 4;
	Rover.player_coord_X = 0.0;
	Rover.player_coord_Y = 400.0;
	Rover.player_offset_X = 0.0;
	Rover.player_offset_Y = 0.0;
	Rover.body_direction = 3;
	Rover.body_direction_rad = 0.0;
	Rover.weapon_direction = 3;
	Rover.weapon_direction_rad = 0.0;
	Rover.animation_time = 0;		//тайминги анимации в микросекундах
	Rover.animation_time_MAX = 1000;
	Rover.weapon_animation_time = 0;		//тайминги анимации в микросекундах (оружие)
	Rover.weapon_animation_time_MAX = 1000;
	Rover.collected_money = 0;
	Rover.collected_metal = 0;
	Rover.collected_crystal = 0;
	Rover.collected_uran = 0;
	Rover.weapon.sprite_N = 0;
	Rover.weapon.type = 0; //Type
	Rover.weapon.color = 0; //Colour
	Rover.weapon.fire_rate = 2.0; //fire_rate
	Rover.weapon.power = 1.0; //power
	Rover.weapon.damageArea = 0.0; //area_damage
	Rover.weapon.hasSpecialEffect = false; //spec_effect
	Rover.weapon.level = 0;
	Rover.tires_XY[0] = Rover.player_coord_X; Rover.tires_XY[1] = Rover.player_coord_Y;

	//массив пуль тоже обнуляем
	for (int i = 0; i < 100; ++i)
	{
		player_bullet_array[i].isAlive = 0;
		player_bullet_array[i].type = 0;
		player_bullet_array[i].power = 0;
		player_bullet_array[i].X = 0.0;
		player_bullet_array[i].Y = 0.0;
		player_bullet_array[i].speed = 0.0;
		player_bullet_array[i].direction = 0.0;
		player_bullet_array[i].ttl = 0.0;
		player_bullet_array[i].lives = 0;
		player_bullet_array[i].monters_hit[0] = -1;
		player_bullet_array[i].monters_hit[1] = -1;
		player_bullet_array[i].monters_hit[2] = -1;
	}
	player_bullet_count = 0;

}

void Mission::reset_world()
{
	//перезапуск мира

	player_weapon_cooldown = 0.0;
	player_weapon_cooldown_G = 0.0;


	//обнуляем лут
	for (int i = 0; i < 1000; ++i)
	{
		loot_array[i].isValid = 0;
		loot_array[i].type = 0;
		loot_array[i].power = 0;
		loot_array[i].X = 0.0;
		loot_array[i].Y = 0.0;
	}
	loot_count = 0;

	//разбрасываемый лут
	for (int i = 0; i < 100; ++i)
	{
		flying_loot_array[i].isValid = 0;
		flying_loot_array[i].type = 0;
		flying_loot_array[i].power = 0;
		flying_loot_array[i].X = 0.0;
		flying_loot_array[i].Y = 0.0;
		flying_loot_array[i].dX = 0.0;
		flying_loot_array[i].dY = 0.0;
		flying_loot_array[i].dV = 0.0;
		flying_loot_array[i].V_Velocity = 0.0;
		flying_loot_array[i].ttl = 0.0;
	}

	//обнуляем стационарные объекты
	for (int i = 0; i < 100; ++i)
	{
		obj_array[i].isValid = 0;
		obj_array[i].type = 0;
		obj_array[i].size_X = 0.0;
		obj_array[i].size_Y = 0.0;
		obj_array[i].X = 0.0;
		obj_array[i].Y = 0.0;
		obj_array[i].spec_data = 0;
		obj_array[i].Health = 0.0;
		obj_array[i].Health_MAX = 0.0;
	}

	generate_stationary_obj();//создаем ключевые объекты

	//обнуляем оружие и апгрейды  DEL
	for (int i = 0; i < 1000; ++i)
	{
		equipment_array[i].isValid = 0;
		equipment_array[i].type = 0;
		equipment_array[i].power = 0;
		equipment_array[i].X = 0.0;
		equipment_array[i].Y = 0.0;
	}
	equipment_count = 0;
	

	//обнуляем монстров
	for (int i = 0; i < 1000; ++i)
	{
		monster_array[i].isAlive = 0;
		monster_array[i].isJugger = 0;
		monster_array[i].scale = 1;
		monster_array[i].level = 0;
		monster_array[i].sprite_N = 0;
		monster_array[i].anim_time_MAX = 0;
		monster_array[i].anim_time = 0;
		monster_array[i].power = 0;
		monster_array[i].moving_speed = 0.0;
		monster_array[i].direction = 0.0;
		monster_array[i].attack_type = 0.0;
		monster_array[i].attack_time = 0.0;
		monster_array[i].attack_cooldown = 0.0;
		monster_array[i].attack_distance = 0.0;
		monster_array[i].health = 0.0;
		monster_array[i].X = 0.0;
		monster_array[i].Y = 0.0;
		monster_array[i].chase_distance = 0.0;
	}
	monster_count = 0;

	//обнуляем массив пуль игрока
	for (int i = 0; i < 100; ++i)
	{
		player_bullet_array[i].isAlive = 0;
		player_bullet_array[i].type = 0;
		player_bullet_array[i].power = 0;
		player_bullet_array[i].X = 0.0;
		player_bullet_array[i].Y = 0.0;
		player_bullet_array[i].speed = 0.0;
		player_bullet_array[i].direction = 0.0;
		player_bullet_array[i].ttl = 0.0;
		player_bullet_array[i].lives = 0;
		player_bullet_array[i].monters_hit[0] = -1;
		player_bullet_array[i].monters_hit[1] = -1;
		player_bullet_array[i].monters_hit[2] = -1;
	}
	player_bullet_count = 0;


	//обнуляем массив пуль монстров
	for (int i = 0; i < 100; ++i)
	{
		monster_bullet_array[i].isAlive = 0;
		monster_bullet_array[i].type = 0;
		monster_bullet_array[i].sprite_N = 0;
		monster_bullet_array[i].power = 0.0;
		monster_bullet_array[i].speed = 0.0;
		monster_bullet_array[i].direction = 0.0;
		monster_bullet_array[i].ttl = 0.0;
		monster_bullet_array[i].X = 0.0;
		monster_bullet_array[i].Y = 0.0;
	}
	monster_bullet_count = 0;

	//массив эффектов
	for (int i = 0; i < 100; ++i)
	{
		VFX_array[i].isAlive = 0;
		VFX_array[i].number = 0;
		VFX_array[i].last_frame = 0;
		VFX_array[i].keep_alive = 0;
		VFX_array[i].total_time = 0.0;
		VFX_array[i].remaining_time = 0.0;
		VFX_array[i].remains_num = 0;
		VFX_array[i].scale = 1.0;
		VFX_array[i].X = 0.0;
		VFX_array[i].Y = 0.0;
	}
	VFX_effect_count = 0;

	//массив пятен крови и прочих останков
	for (int i = 0; i < 1000; ++i)
	{
		VFX_remains_array[i].isAlive = 0; // 0 - свободный слот, 1 - занято
		VFX_remains_array[i].number = 0;//номер в атласе. размер у всех 100*100
		VFX_remains_array[i].X = 0.0;
		VFX_remains_array[i].Y = 0.0;
		VFX_remains_array[i].rotation = 0.0;
	}
	VFX_remains_pointer = 0;

	//массив гранат
	for (int i = 0; i < 10; ++i)
	{
		grenades_array[i].isAlive = 0; // 0 - свободный слот, 1 - занято
		grenades_array[i].X = 0.0;
		grenades_array[i].Y = 0.0;
		grenades_array[i].V = 0.0;
		grenades_array[i].ttl = 0.0;
		grenades_array[i].dX = 0.0;
		grenades_array[i].dY = 0.0;
		grenades_array[i].V_Velocity = 0.0;
		grenades_array[i].power = 0.0;
	}



}

int Mission::get_collected_money() { return(Rover.collected_money + base_money); }
int Mission::get_rover_money() { return(Rover.collected_money); }
int Mission::get_collected_metal() { return(Rover.collected_metal + base_metal); }
int Mission::get_collected_crystal() { return(Rover.collected_crystal + base_crystal); }
int Mission::get_collected_uran() { return(Rover.collected_uran + base_uran); }

void Mission::set_options_object(Options* game_options_from_main, unsigned int planet_type_p, unsigned int planet_level_p, unsigned int* suit_upgrades, unsigned int* rover_upgrades)
{
	game_options = game_options_from_main;
	music_action.setVolume(game_options->music_volume);
	game_options->ptr_music_game = &music_action;

	//устанавливаем параметры планеты
	planet_type = planet_type_p;
	planet_level = planet_level_p;

	//инициализируем параметры ровера
	Rover.Health_MAX = floor(100.0 + 20.0 * rover_upgrades[0]);
	//Rover.Health = Rover.Health_MAX;
	Rover.Energy_MAX = floor(100.0 + 20.0 * rover_upgrades[1]);
	//Rover.Energy = Rover.Energy_MAX;
	Rover.Generate_speed = 1.0 + 0.5 * rover_upgrades[2];
	//базовая скорость 
	Rover.Speed_MAX = 350.0 * pow(1.0185f, rover_upgrades[3]);
	//Rover.gun_damage = 1.0;
	Rover.fire_rate_boost = pow(1.0185, rover_upgrades[4]);
	Rover.active_slots = rover_upgrades[5];
	for (int i = 0; i < 3; ++i) Rover.slots[i] = -1;//обнуляем все слоты (-1)
	//Rover.player_coord_X = 0.0;
	//Rover.player_coord_Y = 400.0;
	//Rover.player_offset_X = 0.0;
	//Rover.player_offset_Y = 0.0;
	//Rover.body_direction = 3;
	//Rover.weapon_direction = 3;
	//Rover.weapon_direction_rad = 0;
	//Rover.animation_time = 0;		//тайминги анимации в микросекундах
	//Rover.animation_time_MAX = 1000;
	//Rover.weapon_animation_time = 0;		//тайминги анимации в микросекундах (оружие)
	//Rover.weapon_animation_time_MAX = 1000;
	//Rover.collected_money = 0;
	//Rover.collected_metal = 0;
	//Rover.collected_crystal = 0;
	//Rover.collected_uran = 0;
	//printf("params = %i %i %0.0f %0.0f %0.0f %0.0f %0.0f %i \n", planet_type, planet_level, Rover.Health_MAX,Rover.Energy_MAX,Rover.Generate_speed,Rover.Speed_MAX,Rover.fire_rate,Rover.slots);
	
	//сбрасываем настройки оружия
	Rover.weapon.sprite_N = 0;
	Rover.weapon.type = 0; //printf("new type %i\n", weapon_data[0]);
	Rover.weapon.color = 0; //printf("new color %i\n", weapon_data[1]);
	Rover.weapon.power = 1.0;
	Rover.weapon.fire_rate = 2.0;
	Rover.weapon.damageArea = 0.0;
	Rover.weapon.hasSpecialEffect = false; //printf("new area_dam %i\n", Rover.weapon_data[5]);
	Rover.weapon.level = 0;  //уровень обнуляется
}

sf::Texture* Mission::getScreenshot() { return(&screenshot_Texture); }

int Mission::get_player_health_prc() { return (floor(100.0 * Rover.Health / Rover.Health_MAX)); }

int Mission::get_player_energy_prc() { return (floor(100.0 * Rover.Energy / Rover.Energy_MAX)); }

void Mission::transfer_to_base()
{
	base_money += Rover.collected_money;
	base_metal += Rover.collected_metal;
	base_crystal += Rover.collected_crystal;
	base_uran += Rover.collected_uran;
	Rover.collected_money = 0;
	Rover.collected_metal = 0;
	Rover.collected_crystal = 0;
	Rover.collected_uran = 0;
}

void Mission::transfer_res_to_orbit(unsigned int *res_arr)
{
	res_arr[0] = base_money;
	res_arr[1] = base_metal;
	res_arr[2] = base_crystal;
	res_arr[3] = base_uran;

	base_money = 0;
	base_metal = 0;
	base_crystal = 0;
	base_uran = 0;
}

void Mission::get_resources(int *res_arr)
{
	//получаем ресурсы
	// деньги, металл, кристалл, уран
	Rover.collected_money += res_arr[0];
	Rover.collected_metal += res_arr[1];
	Rover.collected_crystal += res_arr[2];
	Rover.collected_uran += res_arr[3];
	//printf("Got res (%i %i %i %i )\n", res_arr[0], res_arr[1], res_arr[2], res_arr[3]);
}

int Mission::get_weapon_to_take() {	return weapon_to_take; }

int Mission::get_upgrade_to_take() { return upgrage_to_take; }

WeaponData Mission::get_weapon_data() { return Rover.weapon; }

void Mission::set_weapon(WeaponData weapon_data)
{
	Rover.weapon.sprite_N = weapon_data.sprite_N; //printf("new sprite %i\n", weapon_data[0]);
	Rover.weapon.type = weapon_data.type; //printf("new type %i\n", weapon_data[0]);
	Rover.weapon.color = weapon_data.color; //printf("new color %i\n", weapon_data[1]);
	Rover.weapon.power = weapon_data.power;
	Rover.weapon.fire_rate = weapon_data.fire_rate;
	Rover.weapon.damageArea = weapon_data.damageArea; 
	Rover.weapon.hasSpecialEffect = weapon_data.hasSpecialEffect; //printf("new area_dam %i\n", Rover.weapon_data[5]);
	Rover.weapon.level = weapon_data.level;  //уровень обнуляется
}

int Mission::get_slots_number()
{
	return Rover.active_slots;
}

void Mission::get_slots_array(int* slots_arr)
{
	for (int i = 0;i<3;++i)
	slots_arr[i] = Rover.slots[i];

}

void Mission::install_upgrade(int slot_num, int upgrade_type)
{
	Rover.slots[slot_num - 1] = upgrade_type;
}

void Mission::draw_flying_loot()
{
	for (int i = 0;i < 100;++i)
	{
		//проходим по массиву лута
		if (flying_loot_array[i].isValid == 1)
		{
			//рисуем лут
			int tile_offset = flying_loot_array[i].type * 100;
			loot_Sprite.setTextureRect(sf::IntRect(tile_offset, 0, 100, 100));
			double temp_y = flying_loot_array[i].Y + flying_loot_array[i].dV / GAME_RENDER_SCALE;
			loot_Sprite.setPosition(flying_loot_array[i].X / GAME_RENDER_SCALE - (Rover.player_coord_X / GAME_RENDER_SCALE - Rover.player_offset_X - render_res_X / 2), Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y + render_res_Y / 2 - temp_y / GAME_RENDER_SCALE);
			loot_Sprite.setOrigin(50, 50);
			loot_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
			if (flying_loot_array[i].type == 7)
				{
					//дорисовываем цвет оружия
					int color = flying_loot_array[i].power >> 8;
					sf::CircleShape shape(60.f);
					shape.setOrigin(60.0, 60.0);
					shape.setPosition(flying_loot_array[i].X / GAME_RENDER_SCALE - (Rover.player_coord_X / GAME_RENDER_SCALE - Rover.player_offset_X - render_res_X / 2), Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y + render_res_Y / 2 - flying_loot_array[i].Y / GAME_RENDER_SCALE);
					shape.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
					if (color == 0) shape.setFillColor(sf::Color(128, 128, 128, 70));
					if (color == 1) shape.setFillColor(sf::Color(0, 255, 0, 70));
					if (color == 2) shape.setFillColor(sf::Color(0, 255, 255, 70));
					if (color == 3) shape.setFillColor(sf::Color(255, 0, 255, 70));
					if (color == 4) shape.setFillColor(sf::Color(255, 255, 0, 70));
					game_window->draw(shape);
				}
			game_window->draw(loot_Sprite);

			//пересчитываем координаты
			
			flying_loot_array[i].X += flying_loot_array[i].dX * elapsed_ms / 1000;
			flying_loot_array[i].Y += flying_loot_array[i].dY * elapsed_ms / 1000;
			flying_loot_array[i].V_Velocity -= 3200.0 / 1000 * elapsed_ms;
			flying_loot_array[i].dV += flying_loot_array[i].V_Velocity / 1000 * elapsed_ms;
			flying_loot_array[i].ttl -= elapsed_ms;

			if (flying_loot_array[i].ttl <= 0)
			{
				//переносим в основную таблицу
				int t = 0;
				
				for (t = 0; t < 1000; ++t)
				{
					if (loot_array[t].isValid == 0) break;
				}
				
				if (t < 1000)
				{
					//найдена пустая ячейка
					loot_array[t].isValid = 1; 
					loot_array[t].power = flying_loot_array[i].power;
					loot_array[t].type = flying_loot_array[i].type;
					loot_array[t].X = flying_loot_array[i].X;
					loot_array[t].Y = flying_loot_array[i].Y + flying_loot_array[i].dV;
					loot_count++;
					//printf("loot move\n");
					//break;
				}
				//printf("loot stop V_vel = %0.0f  dV=%0.0f\n", flying_loot_array[i].V_Velocity, flying_loot_array[i].dV);
				flying_loot_array[i].isValid = false;
			}
			/*
				int isValid;
				int type;// 0 - health, 1 - energy, 2 - health+energy, 3 - metal, 4 - crystal, 5 - uran, 6 - upgrade, 7 - weapon 
				int power; //сколько восстановится 50 - 250, кол-во ресурсов, тип оружия  и т.п.
				double X;
				double Y;
				double dX;
				double dY;
				double dV;
				double V_Velocity;
				double ttl;
			*/
		}
	}
}

void Mission::drop_flying_loot(int metal_drop, int crystal_drop, int uran_drop, double X, double Y)
{

	for (int i = 0; i < metal_drop; ++i)
	{
		add_flying_loot(3, X, Y);
	}
	for (int i = 0; i < crystal_drop; ++i)
	{
		add_flying_loot(4, X, Y);
	}
	for (int i = 0; i < uran_drop; ++i)
	{
		add_flying_loot(5, X, Y);
	}
}
	
void Mission::add_flying_loot(int type, int X, int Y)
	{
		for (int i = 0; i < 100; ++i)
		{
			if (!flying_loot_array[i].isValid)
			{
				flying_loot_array[i].type = type;
				flying_loot_array[i].isValid = true;
				flying_loot_array[i].power = 1;
				flying_loot_array[i].X = X;
				flying_loot_array[i].Y = Y;
				flying_loot_array[i].ttl = 500;
				double angle = 2 * 3.1415926 * rand() / (RAND_MAX) - 3.1415926;
				//double angle = 1.5;
				flying_loot_array[i].dX = cos(angle) * 400.0;
				flying_loot_array[i].dY = sin(angle) * 400.0;
				flying_loot_array[i].dV = 0.0;
				flying_loot_array[i].V_Velocity = 800.0;
				break;
			}
		}
}

void Mission::spawn_monster(int i, double X, double Y)
{
	//добавляем данные в массив монстров
	monster_array[i].isAlive = 1;
	monster_array[i].isJugger = ((rand() * 1.0 / (RAND_MAX + 1.0)) > 0.97) ? 1 : 0; //джагернаут с вероятностью 10%
	monster_array[i].level = floor(11.0 * rand() / (RAND_MAX + 1.0));
	monster_array[i].sprite_N = floor(33.0 * rand() / (RAND_MAX + 1.0));
	if (monster_array[i].sprite_N == 32) { monster_array[i].scale = 1.5; }
	else monster_array[i].scale = 1;
	if (monster_array[i].isJugger == 1) monster_array[i].scale *= 2;
	monster_array[i].power = monster_array[i].level * (monster_array[i].isJugger + 1);
	monster_array[i].moving_speed = 200 + rand() * 100.0 / (double)RAND_MAX;
	monster_array[i].anim_time_MAX = monster_array[i].moving_speed * 4.0;
	monster_array[i].direction = atan2(Rover.player_coord_Y - Y, Rover.player_coord_X - X);
	monster_array[i].chase_distance = sqrt(pow((render_res_X / 2 + Rover.player_offset_X), 2) + pow((render_res_Y / 2 + Rover.player_offset_Y), 2)) * GAME_RENDER_SCALE;
	int weapon;
	//подбираем оружие монстрам
	if (monster_array[i].sprite_N <= 7) weapon = 0;
	if (monster_array[i].sprite_N > 7 && monster_array[i].sprite_N <= 15) weapon = 2;
	if (monster_array[i].sprite_N > 15 && monster_array[i].sprite_N <= 31) weapon = 0;
	if (monster_array[i].sprite_N > 31) weapon = 1;

	//printf("%i\n", weapon);
	if (weapon == 0)
	{
		//рукопашка
		monster_array[i].attack_type = 0; // 0 - рукопашка, 1 - дальняя огнем, 2 - дальняя электричеством
		monster_array[i].attack_time = 1000.0;
		monster_array[i].attack_cooldown = 0.0;
		monster_array[i].attack_distance = 120.0;
	}
	if (weapon == 1)
	{
		//атака на дистанции
		monster_array[i].attack_type = 1; // 0 - рукопашка, 1 - дальняя огнем, 2 - дальняя электричеством
		monster_array[i].attack_time = 1500.0;
		monster_array[i].attack_cooldown = 0.0;
		monster_array[i].attack_distance = 800.0;
	}

	if (weapon == 2)
	{
		//атака на дистанции
		monster_array[i].attack_type = 2; // 0 - рукопашка, 1 - дальняя огнем, 2 - дальняя электричеством
		monster_array[i].attack_time = 2000.0;
		monster_array[i].attack_cooldown = 0.0;
		monster_array[i].attack_distance = 1200.0;
	}

	monster_array[i].health = (planet_level + (int)floor(rand() * 3.0 / (RAND_MAX + 1.0))) * (monster_array[i].isJugger * (planet_level) + 1);
	monster_array[i].X = X;
	monster_array[i].Y = Y;
	monster_count++;
	
}

void Mission::draw_flying_grenades()
{
	for (int i = 0; i < 10; ++i)
	{
		//проходим по массиву лута
		if (grenades_array[i].isAlive == 1)
		{
			player_bullet_Sprite.setTextureRect(sf::IntRect(0, 600, 100, 100));
			double temp_y = grenades_array[i].Y + grenades_array[i].V / GAME_RENDER_SCALE;
			player_bullet_Sprite.setPosition(grenades_array[i].X / GAME_RENDER_SCALE - (Rover.player_coord_X / GAME_RENDER_SCALE - Rover.player_offset_X - render_res_X / 2), Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y + render_res_Y / 2 - temp_y / GAME_RENDER_SCALE);
			player_bullet_Sprite.setOrigin(50, 50);
			player_bullet_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
			game_window->draw(player_bullet_Sprite);

			//пересчитываем координаты

			grenades_array[i].X += grenades_array[i].dX * elapsed_ms / 1000;
			grenades_array[i].Y += grenades_array[i].dY * elapsed_ms / 1000;
			grenades_array[i].V_Velocity -= 1600.0 / 1000 * elapsed_ms;
			grenades_array[i].V += grenades_array[i].V_Velocity / 1000 * elapsed_ms;
			grenades_array[i].ttl -= elapsed_ms;

			if (grenades_array[i].ttl <= 0)
			{
				grenades_array[i].isAlive = 0;
				make_explosion(grenades_array[i].X, grenades_array[i].Y, grenades_array[i].power, 0, 1);
			}
		}
	}
}

void Mission::draw_shield()
{
	//рисуем щит
	int shield_power = 0;
	for (int t = 0; t < 3; ++t)
	{
		if (Rover.active_slots > t && Rover.slots[t] == 4) //добавляем радиус и урон
		{
			shield_power++;
		}
	}
	//printf("%i\n", shield_power);
	//printf("%i %i %i\n", Rover.slots[0], Rover.slots[1], Rover.slots[2]);
	if (shield_power > 0)
	{
		double kx = render_res_X / 1920.0;
		double ky = render_res_Y / 1080.0;
		Rover.Energy -= 0.5 / 1000 * elapsed_ms * shield_power; //уменьшаем заряд батареи
		if (Rover.Energy < 0) Rover.Energy = 0;
		double trans = 128; //при уменьшении поля снижаем прозрачность
		if (Rover.Energy < 40.0) trans = Rover.Energy * 2.5;
		shield_Sprite.setScale(kx / GAME_RENDER_SCALE, ky / GAME_RENDER_SCALE);
		shield_Sprite.setPosition(render_res_X / 2 + Rover.player_offset_X, render_res_Y / 2 - Rover.player_offset_Y);
		shield_Sprite.setColor(sf::Color(255, 255, 255, trans));
		shield_Sprite.setOrigin(250, 250);
		game_window->draw(shield_Sprite);
	}
}
void Mission::repair(int price)
{
	if (price <= base_money)
	{
		base_money -= price;
		Rover.Health = Rover.Health_MAX;
	}
}
void Mission::recharge(int price)
{
	if (price <= base_money)
	{
		base_money -= price;
		Rover.Energy = Rover.Energy_MAX;
	}
}

sf::Vector3f Mission::find_nearest_enemy()
{
	sf::Vector3f target;
	struct Enemy
	{
		int number = -1;
		double dist = 0.0;

	};

	Enemy nearest_enemy;
	//printf("============\n");
	for (int i = 0; i < 1000; ++i)
	{
		if (monster_array[i].isAlive == 1) // проверяем жив ли монстр
		{
			
			//printf("#%i - ", i);
			//рассчитываем расстояние до игрока
			double dist_to_monster = sqrt((Rover.player_coord_X - monster_array[i].X) * (Rover.player_coord_X - monster_array[i].X) + (Rover.player_coord_Y - monster_array[i].Y) * (Rover.player_coord_Y - monster_array[i].Y));

			if (nearest_enemy.number != -1 && (dist_to_monster - monster_array[i].attack_distance) >= nearest_enemy.dist) 
				{ 
					//printf("skip1\n");  
					continue; //пропускаем если более ближний враг уже найден
				} 

			if (nearest_enemy.number == -1)
			{
				double angle_to_enemy = atan2(monster_array[i].Y - Rover.player_coord_Y, monster_array[i].X - Rover.player_coord_X);
				if (angle_to_enemy < 0.0) angle_to_enemy += 3.1415926;
				sf::Vector3f r = find_intersection(angle_to_enemy, Rover.player_coord_X, Rover.player_coord_Y, 3000.0);
				
				if (r.z != -1)
				{
					//printf("point 1 - ");
					double dist_to_obj = sqrt((Rover.player_coord_X - r.x)* (Rover.player_coord_X - r.x) + (Rover.player_coord_Y - r.y)* (Rover.player_coord_Y - r.y));
					if (dist_to_obj > dist_to_monster)
					{
						//printf("point 2 - ");
						//препятсвие дальше монстра
						nearest_enemy.number = i;
						nearest_enemy.dist = dist_to_monster - monster_array[i].attack_distance;
					}
					else { 
						//printf("point 3\n"); 
						continue; } //препятствие ближе
				}
				else
				{
					//нет препятствий
					//printf("point 4 \n");
					nearest_enemy.number = i;
					nearest_enemy.dist = dist_to_monster - monster_array[i].attack_distance;
					continue; 
				}
			}
			//printf("point 5 \n");
			if ((dist_to_monster - monster_array[i].attack_distance) < nearest_enemy.dist)
			{
				double angle_to_enemy = atan2(monster_array[i].Y - Rover.player_coord_Y, monster_array[i].X - Rover.player_coord_X);
				if (angle_to_enemy < 0.0) angle_to_enemy += 2.0 * 3.1415926;
				sf::Vector3f r = find_intersection(angle_to_enemy, Rover.player_coord_X, Rover.player_coord_Y, 3000.0);
				//printf("find i=%i x=%0.0f y=%0.0f z=%0.0f angle=%0.1f \n", i, r.x, r.y, r.z, angle_to_enemy);
				if (r.z != -1)
				{
					//printf("point 6 - ");
					double dist_to_obj = sqrt((Rover.player_coord_X - r.x) * (Rover.player_coord_X - r.x) + (Rover.player_coord_Y - r.y) * (Rover.player_coord_Y - r.y));
					if (dist_to_obj > dist_to_monster)
					{
						//препятсвие дальше монстра
						nearest_enemy.number = i;
						nearest_enemy.dist = dist_to_monster - monster_array[i].attack_distance;
						//printf("point 7\n");
						continue;
					}
				}
				else
				{
					nearest_enemy.number = i; 
					nearest_enemy.dist = dist_to_monster - monster_array[i].attack_distance;
					//printf("point 8 - ");
				}
			}
		}
	}
	//printf("find %i %0.0f %0.0f\n", nearest_enemy.number, monster_array[nearest_enemy.number].X, monster_array[nearest_enemy.number].Y);
	if (nearest_enemy.number != -1)
	{
		target.x = monster_array[nearest_enemy.number].X;
		target.y = monster_array[nearest_enemy.number].Y;
		target.z = (float)nearest_enemy.number;
		
	}
	else
	{
		target.x = 0;
		target.y = 0;
		target.z = -1;
		
	}
	//printf("\n");
	//printf("num=%i X=%0.0f Y=%0.0f Z(num)=%i dist_to_en=%0.0f\n", nearest_enemy.number, target.x, target.y, (int)target.z, nearest_enemy.dist);
	//sf:sleep(sf::milliseconds(100));
	//printf("=====END=============\n");
	
	return target;
}

sf::Vector3f Mission::find_intersection(double angle_rad, double X, double Y, double dist)
{
	//находим точку пересечения с ближайшим объектом и возвращаем ее координаты и тип объекта

	//angle_rad = acos(0) * 3.0;
	//X = 400;
	//Y = 500;
	//dist = 1000;
	

	sf::Vector3f result{ 0.0,0.0,-1.0 };
	int type = 0;
	
	for (int i = 0; i < 100; ++i)
	{
		if (obj_array[i].isValid == 1)
		{
			sf::Vector3f cross_dot{ 0.0,0.0,-1.0 }; // z - расстояние от точки (X1,Y1) до пересечения, если z=-1 пересечения нет
			//находим точку пересечения с каждой стороной	(double X11, double Y11, double X12, double Y12, double X21, double Y21, double X22, double Y22)

			// левая вертикальная сторона
			//printf("left (%0.0f %0.0f %0.0f %0.0f) (%0.0f %0.0f %0.0f %0.0f)\n", X, Y, X + dist * cos(angle_rad), Y + dist * sin(angle_rad), obj_array[i].X - obj_array[i].size_X / 2.0, obj_array[i].Y - obj_array[i].size_Y / 2.0, obj_array[i].X - obj_array[i].size_X / 2.0, obj_array[i].Y + obj_array[i].size_Y / 2.0);
			sf::Vector3f result1 = find_line_cross(X, Y, X + dist * cos(angle_rad), Y + dist * sin(angle_rad), obj_array[i].X - obj_array[i].size_X / 2.0, obj_array[i].Y - obj_array[i].size_Y / 2.0, obj_array[i].X - obj_array[i].size_X / 2.0, obj_array[i].Y + obj_array[i].size_Y / 2.0);
			
			cross_dot = result1;

			// верхняя сторона
			//printf("top (%0.0f %0.0f %0.0f %0.0f) (%0.0f %0.0f %0.0f %0.0f)\n", X, Y, X + dist * cos(angle_rad), Y + dist * sin(angle_rad), obj_array[i].X - obj_array[i].size_X / 2.0, obj_array[i].Y + obj_array[i].size_Y / 2.0, obj_array[i].X + obj_array[i].size_X / 2.0, obj_array[i].Y + obj_array[i].size_Y / 2.0);
			sf::Vector3f result2 = find_line_cross(X, Y, X + dist * cos(angle_rad), Y + dist * sin(angle_rad), obj_array[i].X - obj_array[i].size_X / 2.0, obj_array[i].Y + obj_array[i].size_Y / 2.0, obj_array[i].X + obj_array[i].size_X / 2.0, obj_array[i].Y + obj_array[i].size_Y / 2.0);

			if(result2.z != -1 && (result2.z < cross_dot.z || cross_dot.z == -1)) cross_dot = result2;

			// правая вертикальная сторона
			//printf("right (%0.0f %0.0f %0.0f %0.0f) (%0.0f %0.0f %0.0f %0.0f)\n", X, Y, X + dist * cos(angle_rad), Y + dist * sin(angle_rad), obj_array[i].X + obj_array[i].size_X / 2.0, obj_array[i].Y - obj_array[i].size_Y / 2.0, obj_array[i].X + obj_array[i].size_X / 2.0, obj_array[i].Y + obj_array[i].size_Y / 2.0);
			sf::Vector3f result3 = find_line_cross(X, Y, X + dist * cos(angle_rad), Y + dist * sin(angle_rad), obj_array[i].X + obj_array[i].size_X / 2.0, obj_array[i].Y - obj_array[i].size_Y / 2.0, obj_array[i].X + obj_array[i].size_X / 2.0, obj_array[i].Y + obj_array[i].size_Y / 2.0);

			if (result3.z != -1 && (result3.z < cross_dot.z || cross_dot.z == -1)) cross_dot = result3;

			// нижняя сторона
			//printf("bottom (%0.0f %0.0f %0.0f %0.0f) (%0.0f %0.0f %0.0f %0.0f)\n", X, Y, X + dist * cos(angle_rad), Y + dist * sin(angle_rad), obj_array[i].X - obj_array[i].size_X / 2.0, obj_array[i].Y - obj_array[i].size_Y / 2.0, obj_array[i].X + obj_array[i].size_X / 2.0, obj_array[i].Y - obj_array[i].size_Y / 2.0);
			sf::Vector3f result4 = find_line_cross(X, Y, X + dist * cos(angle_rad), Y + dist * sin(angle_rad), obj_array[i].X - obj_array[i].size_X / 2.0, obj_array[i].Y - obj_array[i].size_Y / 2.0, obj_array[i].X + obj_array[i].size_X / 2.0, obj_array[i].Y - obj_array[i].size_Y / 2.0);

			if (result4.z != -1 && (result4.z < cross_dot.z || cross_dot.z == -1)) cross_dot = result4;

			
			if ((cross_dot.z != -1 && cross_dot.z < result.z) || result.z == -1)
			{
				result = cross_dot; 
				type = obj_array[i].type;
			}
			
			/*
			obj_array[i].type = 1; //тип корабль / точка выхода
			obj_array[i].X = cos(i * 2 * 3.1415 / 8) * 15000.0;
			obj_array[i].Y = sin(i * 2 * 3.1415 / 8) * 15000.0;
			obj_array[i].size_X = 318;
			obj_array[i].size_Y = 582;
			*/
		}
		
		//result.z = (float)obj_array[i].type;
	}
	if (result.z != -1) result.z = (float)type;
	
	return result;
}


sf::Vector3f Mission::find_line_cross(double X11, double Y11, double X12, double Y12, double X21, double Y21, double X22, double Y22)
{
	
	//X12 = X11;
	sf::Vector3f result{ 0.0,0.0,-1.0 };

	//X21 = X11; X22 = X11;
	//X12 = X11;
	
	if (X11 == X12)
	{
		//линия 1 вертикальная
		if (X21 == X22)
		{
			//обе линии вертикальные

			if (X11 == X21)
			{
				//проверяем на пересечение отрезков
				if ((Y11 > fmax(Y21, Y22) && Y12 > fmax(Y21, Y22)) || (Y11 < fmin(Y21, Y22) && Y12 < fmin(Y21, Y22)))
				{
					//пересечений нет
					//printf("1&2 vert: X = %0.0f  Y = %0.0f Z = %0.0f\n", result.x, result.y, result.z);
					return result;
				}
				else
				{
					//находим пересечение, ближайшее к Y11 (первой точке)

					//если отрезок 1 входит в 2, то точка пересечения в начале X11,Y11, а расстояние = 0
					if (fmin(Y11, Y12) > fmin(Y21, Y22) && fmax(Y11, Y12) < fmax(Y21, Y22))
					{
						result.x = X11;
						result.y = Y11;
						result.z = 0.0;
						//printf("1&2 vert: X = %0.0f  Y = %0.0f Z = %0.0f\n", result.x, result.y, result.z);
						return result;
					}
					else
					{
						//если пересечение частичное, то ищем ближайшую из точек Y21, Y22
						if (Y11 > fmin(Y21, Y22) && Y11 < fmax(Y21, Y22))
						{
							// если отрезок 1 начинается внутри отрезка 2, то точка пересечения в начале X11,Y11, а расстояние = 0
							result.x = X11;
							result.y = Y11;
							result.z = 0.0;
							//printf("1&2 vert: X = %0.0f  Y = %0.0f Z = %0.0f\n", result.x, result.y, result.z);
							return result;
						}
						else
						{
							// считаем расстояние до ближайшей точки отрезка 2
							if (abs(Y11 - Y21) > abs(Y11 - Y22))
							{
								//ближайшая точка Y22
								result.x = X11;
								result.y = Y22;
								result.z = abs(Y11 - Y22);
								//printf("1&2 vert: X = %0.0f  Y = %0.0f Z = %0.0f\n", result.x, result.y, result.z);
								return result;
							}
							else
							{
								//ближайшая точка Y21
								result.x = X11;
								result.y = Y21;
								result.z = abs(Y11 - Y21);
								//printf("1&2 vert: X = %0.0f  Y = %0.0f Z = %0.0f\n", result.x, result.y, result.z);
								return result;
							}
						}
					}
				}
			}
			else
			{
				//printf("1&2 vert: X = %0.0f  Y = %0.0f Z = %0.0f\n", result.x, result.y, result.z);
				return result; // линии параллельны, пересечеий нет
			}
			
		}
		else
		{
			double k2 = (Y22 - Y21) / (X22 - X21);
			double d2 = (X22 * Y21 - X21 * Y22) / (X22 - X21);

			//в уравнение линии 2 подставляем значение X1 (точка пересечений линией 1 оси Х)
			result.x = X11;
			result.y = k2 * X11 + d2;
			
			//рассчет попадания в отрезок
			if (result.y >= fmin(Y11, Y12) && result.y <= fmax(Y11, Y12) && result.y >= fmin(Y21, Y22) && result.y <= fmax(Y21, Y22))
			{
				result.z = sqrt((result.x - X11) * (result.x - X11) + (result.y - Y11) * (result.y - Y11));
			}
			else
			{
				result.z = -1;
			}
			//printf("weapon vert: X = %0.0f  Y = %0.0f Z = %0.0f\n", result.x, result.y, result.z);
			return result;
		}

	}
	else
	{
		double k1 = (Y12 - Y11) / (X12 - X11); //кроме вертикальных линий
		double d1 = (X12 * Y11 - X11 * Y12) / (X12 - X11);

		if (X21 == X22)
		{
			//вторая линия вертикальная

			//в уравнение линии 2 подставляем значение X1 (точка пересечений линией 1 оси Х)
			result.x = X21;
			result.y = k1 * X21 + d1;
			
			//рассчет попадания в отрезок
			if (result.y >= fmin(Y11, Y12) && result.y <= fmax(Y11, Y12) && result.y >= fmin(Y21, Y22) && result.y <= fmax(Y21, Y22))
			{
				result.z = sqrt((result.x - X11) * (result.x - X11) + (result.y - Y11) * (result.y - Y11));
			}
			else
			{
				result.z = -1;
			}
			
			//printf("obj vert: X = %0.0f  Y = %0.0f Z = %0.0f\n", result.x, result.y, result.z);
			return result;

		}
		else
		{
			double k2 = (Y22 - Y21) / (X22 - X21);
			double d2 = (X22 * Y21 - X21 * Y22) / (X22 - X21);

			//точка Х на прямых
			result.x = (d2 - d1) / (k1 - k2);
			result.y = k1 * (d2 - d1) / (k1 - k2) + d1;

			//рассчет попадания в отрезок
			if (result.y >= fmin(Y11, Y12) && result.y <= fmax(Y11, Y12) && result.x >= fmin(X11, X12) && result.x <= fmax(X11, X12) && result.y >= fmin(Y21, Y22) && result.y <= fmax(Y21, Y22) && result.x >= fmin(X21, X22) && result.x <= fmax(X21, X22))
			{
				result.z = sqrt((result.x - X11) * (result.x - X11) + (result.y - Y11) * (result.y - Y11));
			}
			else
			{
				result.z = -1;
			}
			//printf("norm: X = %0.0f  Y = %0.0f Z = %0.0f\n", result.x, result.y, result.z);
		}
	}
	return result;
}