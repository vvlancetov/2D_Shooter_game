#pragma warning( disable : 4244 )

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Game_engine.h"
#include <windows.h>
#include <debugapi.h>
#include <stdio.h>
#include <cstdlib>
#include <cmath>

Game::Game(sf::RenderWindow *window)
{
	//конструктор класса
	//OutputDebugString(L"Запущен конструктор игры\n");
	game_window = window;
	//printf("%f - width", window->getView().getSize().x);
	//printf("%f - height", window->getView().getSize().y);
	//набор данных для рендеринга окна
	render_res_X = floor(game_window->getView().getSize().x);
	render_res_Y = floor(game_window->getView().getSize().y);
	player_max_offset_Y = render_res_Y / 2 - 400;
	if (player_max_offset_Y < 0) player_max_offset_Y = 0;
	player_max_offset_X = render_res_X / 2 - 800;
	if (player_max_offset_X < 0) player_max_offset_X = 0;
	GAME_RENDER_SCALE = 1;

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

	//enum class Game_stage { PLAYER_INIT, FREE_RUN, INTERACTION, DYING };
	Current_stage = Game_stage::PLAYER_INIT;

	//player Player_instance;
	//установка значений	
	Player_instance.Health = 100.0;
	Player_instance.Health_MAX = 100.0;
	Player_instance.Armor = 0.0;
	Player_instance.Armor_MAX = 0.0;
	Player_instance.Energy = 0.0;
	Player_instance.Energy_MAX = 0.0;
	Player_instance.Speed = 1000.0;
	Player_instance.Speed_MAX = 1000.0;
	Player_instance.gun_damage = 1.0;
	Player_instance.fire_rate = 2.0;
	Player_instance.player_coord_X = 0.0;
	Player_instance.player_coord_Y = 0.0;
	Player_instance.player_offset_X = 0.0;
	Player_instance.player_offset_Y = 0.0;
	Player_instance.body_direction = 3;
	Player_instance.weapon_direction = 3;
	Player_instance.weapon_direction_rad = 0;
	Player_instance.animation_time = 0;		//тайминги анимации в микросекундах
	Player_instance.animation_time_MAX = 1000;
	Player_instance.weapon_animation_time = 0;		//тайминги анимации в микросекундах (оружие)
	Player_instance.weapon_animation_time_MAX = 1000; 

	//загружаем текстуры
	
	bg_player_Texture.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Textures\\Player_tiles.png");
	bg_player_Sprite.setTexture(bg_player_Texture);

	bg_tile_Texture.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Textures\\Ground_tiles.png");
	bg_tile_Sprite.setTexture(bg_tile_Texture);

	bg_game_Texture.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Pictures\\Starting_Background.png");
	bg_game_Sprite.setTexture(bg_game_Texture);

	pause_menu_Texture.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Pictures\\Pause_Background.png");
	pause_menu_Sprite.setTexture(pause_menu_Texture);

	mouse_pointer_Texture.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Pointers\\Mouse_pointer_1.png");
	mouse_pointer_Sprite.setTexture(mouse_pointer_Texture);

	body_tile_Texture.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Textures\\Player_tiles.png");
	body_tile_Sprite.setTexture(body_tile_Texture);
	
	weapon_tile_Texture.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Textures\\Weapon_tiles.png");
	weapon_tile_Sprite.setTexture(weapon_tile_Texture);

	mouse_pointer_2_Texture.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Pointers\\Mouse_pointer_2.png");
	mouse_pointer_2_Sprite.setTexture(mouse_pointer_2_Texture);

	loot_Texture.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Textures\\Loot.png");
	loot_Sprite.setTexture(loot_Texture);

	interface_Texture.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Pictures\\Interface_2.png");
	interface_Sprite.setTexture(interface_Texture);

	interface_equip_Texture.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Pictures\\Dialog_equipment.png");
	interface_equip_Sprite.setTexture(interface_equip_Texture);

	player_bullet_Texture.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Textures\\Bullets.png");
	player_bullet_Sprite.setTexture(player_bullet_Texture);

	monster_Texture_array[0].loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Textures\\Monsters_1.png");
	monster_Sprite_array[0].setTexture(monster_Texture_array[0]);
	
	monster_Texture_array[1].loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Textures\\Monsters_2.png");
	monster_Sprite_array[1].setTexture(monster_Texture_array[1]);

	VFX_Texture.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Textures\\VFX.png");
	VFX_Sprite.setTexture(VFX_Texture);

	VFX_remains_Texture.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Textures\\Remains.png");
	VFX_remains_Sprite.setTexture(VFX_remains_Texture);

	monster_bullet_Texture.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Textures\\Monster_bullets.png");
	monster_bullet_Sprite.setTexture(monster_bullet_Texture);

	//загружаем шрифт
	if (!font.loadFromFile("F:\\CPP Projects\\EDU_2\\Assets\\Fonts\\arialnb.ttf")) printf("font load error\n");
	text.setFont(font); // font is a sf::Font

	//создаем структуры данных

	loot_array = (loot*)malloc(1000 * sizeof(loot));
	if (loot_array == NULL) printf("malloc error loot array");
	loot_count = 0;

	//инициализация массива нулями
	for (int i = 0; i < 1000; ++i)
	{
		loot_array[i].isValid = 0;
		loot_array[i].type = 0;
		loot_array[i].power = 0;
		loot_array[i].X = 0.0;
		loot_array[i].Y = 0.0;
	}
	
	equipment_array = (equipment*)malloc(1000 * sizeof(equipment));
	if (equipment_array == NULL) printf("malloc error equipment array");
	equipment_count = 0;
	equipment_to_take = -1;

	//инициализация массива нулями
	for (int i = 0; i < 1000; ++i)
	{
		equipment_array[i].isValid = 0;
		equipment_array[i].type = 0;
		equipment_array[i].power = 0;
		equipment_array[i].X = 0.0;
		equipment_array[i].Y = 0.0;
	}

	monster_array = (monster*)malloc(1000 * sizeof(monster));
	if (monster_array == NULL) printf("malloc error monster array");
	monster_count = 0;
	
	//инициализация массива нулями
	for (int i = 0; i < 1000; ++i)
	{
		monster_array[i].isAlive = 0;
		monster_array[i].sprite_N = 0;
		monster_array[i].power = 0;
		monster_array[i].moving_speed = 0.0;
		monster_array[i].direction = 0.0;
		monster_array[i].attack_type = 0.0;
		monster_array[i].attack_time = 0.0;
		monster_array[i].attack_cooldown = 0.0;
		monster_array[i].health = 0.0;
		monster_array[i].X = 0.0;
		monster_array[i].Y = 0.0;
	}


	player_bullet_array = (player_bullet*)malloc(100 * sizeof(player_bullet));
	if (player_bullet_array == NULL) printf("malloc error p_bullet array");
	player_bullet_count = 0;

	//инициализация массива нулями
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
	}
	player_weapon_cooldown = 200;

	monster_bullet_array = (monster_bullet*)malloc(100 * sizeof(monster_bullet));
	if (monster_bullet_array == NULL) printf("malloc error m_bullet array");
	monster_bullet_count = 0;

	//инициализация массива нулями
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

	VFX_array = (VFX_effect*)malloc(100 * sizeof(VFX_effect));
	if (VFX_array == NULL) printf("malloc error VFX_effect array");
	VFX_effect_count = 0;
	//инициализация массива нулями
	for (int i = 0; i < 100; ++i)
	{
		VFX_array[i].isAlive = 0;
		VFX_array[i].number = 0;
		VFX_array[i].last_frame = 0;
		VFX_array[i].keep_alive = 0;
		VFX_array[i].total_time = 0.0;
		VFX_array[i].remaining_time = 0.0;
		VFX_array[i].remains_num = 0;
		VFX_array[i].X = 0.0;
		VFX_array[i].Y = 0.0;
	}

	VFX_remains_array = (VFX_remains*)malloc(1000 * sizeof(VFX_remains));
	if (VFX_remains_array == NULL) printf("malloc error VFX_remains array");
	//инициализация массива нулями
	for (int i = 0; i < 1000; ++i)
	{
		VFX_remains_array[i].isAlive = 0; // 0 - свободный слот, 1 - занято
		VFX_remains_array[i].number = 0;//номер в атласе. размер у всех 100*100
		VFX_remains_array[i].X = 0.0;
		VFX_remains_array[i].Y = 0.0;
	}
	VFX_remains_pointer = 0;

};

int Game::run(sf::Time elapsed)
{
	//основной цикл игры
	// 
	//прошло времени с микросекундах
	elapsed_ms = elapsed.asMilliseconds();

	//уменьшаем значения счетчиков
	if (pause_delay_timer > 0) pause_delay_timer -= elapsed_ms;
	loot_gen_timer -= elapsed_ms;
	if (loot_gen_timer <= 0) generate_loot(); //генерация лута
	monster_gen_timer -= elapsed_ms;
	if (monster_gen_timer <= 0) generate_monsters();//генерация монстров

	//очистка окна
	game_window->clear(sf::Color::Black); //очистка окна

	//запрашиваем позицию курсора мыши
	sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);

	//рисуем территорию тайлами
	draw_terrain();

	//рисуем останки
	draw_VFX_remains();

	//рисуем гнезда


	//рисуем корабли


	//рисуем лут
	draw_loot();

	//рисуем оборудование
	draw_equipment();

	//рисуем персонажа
	draw_character();

	//рисуем мобов
	draw_monsters();

	//рисуем выстрелы и эффекты
	draw_bullets();
	draw_monster_bullets();
	draw_VFX();

	//рисуем интерфейс
	draw_interface(localPosition);
	
	game_window->display();

	//обрабатываем нажатия клавиш
	controls_processing(localPosition);

	//обработка нажатия ESC в самом конце
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && pause_delay_timer<=0)
		{
			pause_delay_timer = 200;//выставляем таймер 
			return(1);				//и ставим на паузу
		}
	if (equipment_to_take != -1)
		{
			//делаем скриншот экрана
			sf::Vector2u windowSize = game_window->getSize();
			screnshot_Texture.create(windowSize.x, windowSize.y);
			screnshot_Texture.update(*game_window);
			//printf("Screenshot!\n");
			return (2); //если берем оборудование - меняем состояние игры
		}
	return(0);//просто продолжаем цикл
};

int Game::start()
{
	//инициализация игры
	game_window->clear(sf::Color::Black);
	game_window->draw(bg_game_Sprite);
	game_window->display();
	generate_loot();

	//sf::sleep(sf::milliseconds(300));
	
	
	sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);
	srand(1);//перезапускаем генератор RND
	return(1); //в случае успеха возвращаем 1, иначе 0
}

int Game::pause(sf::Time elapsed)
{
	if (pause_delay_timer > 0) pause_delay_timer -= elapsed.asMilliseconds();//охлаждаем таймер
	//printf("%i", pause_delay_timer);

	//OutputDebugString(L"Пауза активна\n");
	game_window->clear(sf::Color::Black);
	game_window->draw(pause_menu_Sprite);

	//задаем области кнопок
	int Exit_area[4] = { 800, 1000, 300, 1620 };

	//проверяем координаты мыши относительно экрана
	sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);
	
	//рисуем курсор
	mouse_pointer_Sprite.setPosition((float)localPosition.x, (float)localPosition.y);
	game_window->draw(mouse_pointer_Sprite);

	//проверяем попадание по пунктам меню

	if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0]))
	{
		//OutputDebugString(L"Кнопка - Выход\n");
	}
	
	game_window->display();
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && pause_delay_timer <= 0)
		{
			//OutputDebugString(L"Пауза x\n");
			//printf("%i", pause_delay_timer);
			pause_delay_timer = 200; //включаем задержку заново
			return(1);				//снять паузу
		}

	//проверяем нажатие на кнопку выход
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0]))
			{
			if (pause_delay_timer<=0)
				{
					pause_delay_timer = 200; //включаем задержку заново
					return(2);				//выход в главное меню
				}
			}
		}
	
	return (0);//пауза продолжается
}

int Game::check_mouse_hover(int x, int y, int* area)
{
	if (y > area[0] && y < area[1] && x > area[2] && x < area[3]) return 1;
	return 0;
}

void Game::set_render_resolution(int x, int y)
{
	//удалить
	render_res_X = x;
	render_res_Y = y;
	player_max_offset_Y = render_res_Y / 4;
	player_max_offset_X = render_res_X / 4;
}

int Game::rnd_from_coord(int x, int y, int range)
{
	//пытаемся получить рандомное число их 2-мерных координат
	int code_1 = ((double)abs(x + 11) / 17 * 73);
	int code_2 = ((double)abs(y + 29) / 13 * 101);
	code_1 = code_1 ^ 42;  //227
	code_2 = code_2 ^ 21;  //51213
	
	//	% 13 + (x % 5) + (y % 7) + (abs(y) * 13) % 17; //you can use any prime number
	return ((code_1+code_2) % range); //Make sure it's in range 0-max

}

int Game::generate_loot()
{
	
	loot_gen_timer = 1000;//переделать под константы
	
	//генерируем лут и апгрейды вокруг игрока

	int LOOT_ITEMS_TO_GEN = 5;
	int LOOT_ITEMS_ADDED = 0;

	int EQUIP_ITEMS_TO_GEN = 5;
	int EQUIP_ITEMS_ADDED = 0;

	//цикл генерации для лута
	for (int i = 0; i < LOOT_ITEMS_TO_GEN; ++i)
	{
		double NEW_X = (rand() * 4000.0 / RAND_MAX - 2000.0);// 4000  2000
			if (NEW_X < 0) NEW_X -= 1200 * GAME_RENDER_SCALE; else NEW_X += 1200 * GAME_RENDER_SCALE;  // 1200
		double NEW_Y = (rand() * 4000.0 / RAND_MAX - 2000.0);  //4000 2000
			if (NEW_Y < 0) NEW_Y -= 800 * GAME_RENDER_SCALE; else NEW_Y += 800 * GAME_RENDER_SCALE; //800
			NEW_X += (Player_instance.player_coord_X - Player_instance.player_offset_X);
			NEW_Y += (Player_instance.player_coord_Y - Player_instance.player_offset_Y);
		
			//если координаты не попадают в область уже генерированного лута - добавляем в массив

		if (NEW_X < gen_loot_min_X || NEW_X > gen_loot_max_X || NEW_Y < gen_loot_min_Y || NEW_Y > gen_loot_max_Y)
			{
				char type = floor(2.999 * rand() / RAND_MAX);
				int power_rnd = floor(190.0 * rand() / RAND_MAX);
				unsigned char power = 0;
				if (power_rnd < 100) power = 50;
				else 
					{
						if (power_rnd < 150) power = 100;
						else
						{
							if (power_rnd < 175) power = 150;
							else
							{
								if (power_rnd < 185) power = 200;
								else power = 250;
							}
						}
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
					loot_array[new_item].X = NEW_X;
					loot_array[new_item].Y = NEW_Y;
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
	
	//printf("loot added = %i \n", LOOT_ITEMS_ADDED);
	
	//цикл генерации для апгрейдов и оружия
	for (int i = 0; i < EQUIP_ITEMS_TO_GEN; ++i)
	{
		double NEW_X = (rand() * 4000.0 / RAND_MAX - 2000.0);// 4000  2000
		if (NEW_X < 0) NEW_X -= 1200 * GAME_RENDER_SCALE; else NEW_X += 1200 * GAME_RENDER_SCALE;  // 1200
		double NEW_Y = (rand() * 4000.0 / RAND_MAX - 2000.0);  //4000 2000
		if (NEW_Y < 0) NEW_Y -= 800 * GAME_RENDER_SCALE; else NEW_Y += 800 * GAME_RENDER_SCALE; //800
		NEW_X += (Player_instance.player_coord_X - Player_instance.player_offset_X);
		NEW_Y += (Player_instance.player_coord_Y - Player_instance.player_offset_Y);

		//если координаты не попадают в область уже генерированного лута - добавляем в массив

		if (NEW_X < gen_loot_min_X || NEW_X > gen_loot_max_X || NEW_Y < gen_loot_min_Y || NEW_Y > gen_loot_max_Y)
		{
			unsigned char type = floor(1.999 * rand() / RAND_MAX);
			double dist = sqrt(NEW_X * NEW_X + NEW_Y * NEW_Y);
			//добавляем данные в массив оборудования

			if (equipment_count < 1000) //добавить вызов сборщика мусора если = 1000
			{
				//ищем пустую ячейку в массиве
				int new_item = 0;
				for (new_item = 0; new_item < 1000 && equipment_array[new_item].isValid == 1; ++new_item);
				equipment_array[new_item].isValid = 1;
				equipment_array[new_item].type = type;
				equipment_array[new_item].power = (unsigned char)floor(dist/5000.0);
				equipment_array[new_item].X = NEW_X;
				equipment_array[new_item].Y = NEW_Y;
				//printf("equip gen = %i, %i, (%0.0f, %0.0f) \n", type, equipment_array[equipment_count].power, NEW_X, NEW_Y);
				//обновляем информацию о границах области генерации оборудования
				if (NEW_X < gen_loot_min_X) gen_loot_min_X = NEW_X;
				if (NEW_X > gen_loot_max_X) gen_loot_max_X = NEW_X;
				if (NEW_Y < gen_loot_min_Y) gen_loot_min_Y = NEW_Y;
				if (NEW_Y > gen_loot_max_Y) gen_loot_max_Y = NEW_Y;
				//printf("min_X (%0f), max_X (%0f), min_Y (%0f), max_Y (%0f)) \n", gen_loot_min_X, gen_loot_max_X, gen_loot_min_Y, gen_loot_max_Y);
				equipment_count++;
				//printf("[add] item = %5i equipment_count = %5i \n", new_item, equipment_count);
			}
			else { 
					//printf("equipment_count=1000\n"); //сделать цикл очистки
				}
			EQUIP_ITEMS_ADDED++;
		}

	}

	//printf("equipment added = %i \n", EQUIP_ITEMS_ADDED);

	return(LOOT_ITEMS_ADDED + EQUIP_ITEMS_ADDED);
}

void Game::draw_interface(sf::Vector2i localPosition)
{
	interface_Sprite.setScale(render_res_X / 1920.0, render_res_Y / 1080.0);
	game_window->draw(interface_Sprite);
	
	//вывод текста в окно
	text.setCharacterSize(24); // in pixels, not points!
	text.setFillColor(sf::Color::Red);
	//text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	char buffer[50];
	sprintf_s(buffer, "%0.0f %0.0f monster=%i bullet=%i loot=%i", Player_instance.player_coord_X, Player_instance.player_coord_Y, monster_count, player_bullet_count, loot_count);
	text.setPosition(50.0, 50.0);
	text.setString(buffer);
	game_window->draw(text);

	//рисуем курсор в виде прицела
	mouse_pointer_2_Sprite.setPosition((float)localPosition.x - 10, (float)localPosition.y - 10);
	game_window->draw(mouse_pointer_2_Sprite);

}

int Game::take_equipment()
{
	//printf("equipment_to_take = %i \n", equipment_to_take);
	sf::Sprite screenshot_Sprite;
	sf::Vector2f dialog_pos(530, 300); //позиция окна
	screenshot_Sprite.setTexture(screnshot_Texture);
	screenshot_Sprite.setColor(sf::Color(255, 255, 255, 50));
	game_window->clear();
	game_window->draw(screenshot_Sprite);
	//диалоговое окно
	interface_equip_Sprite.setPosition(dialog_pos);
	game_window->draw(interface_equip_Sprite);
	
	//рисуем диалоговое окно

	//ЗАГОЛОВОК ОКНА
	text.setCharacterSize(35); // in pixels, not points!
	text.setFillColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold);
	//text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	if (equipment_array[equipment_to_take].type==0) text.setString("Upgrade found!");
	else text.setString("Weapon found!");
	text.setPosition(45 + dialog_pos.x, 11 + dialog_pos.y);
	game_window->draw(text);
	
	//char buffer[50];
	//sprintf_s(buffer, "%0.0f   %0.0f", Player_instance.player_coord_X, Player_instance.player_coord_Y);
	
	//картинка
	if (equipment_array[equipment_to_take].type == 0) loot_Sprite.setTextureRect(sf::IntRect(300, 0, 100, 100));
	else loot_Sprite.setTextureRect(sf::IntRect(400, 0, 100, 100));
	loot_Sprite.setScale(1.75, 1.75);
	loot_Sprite.setPosition (87 + dialog_pos.x, 84 + dialog_pos.y);
	game_window->draw(loot_Sprite);

	//описание
	if (equipment_array[equipment_to_take].type == 0) text.setString("Upgrade for radar \n+15 distance");
	else text.setString("Usual weapon \n damage = 5");
	text.setCharacterSize(30); // in pixels, not points!
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setPosition(376 + dialog_pos.x, 73 + dialog_pos.y);
	game_window->draw(text);

	//кнопка замены
	text.setCharacterSize(40); // in pixels, not points!
	text.setFillColor(sf::Color::Green);
	text.setStyle(sf::Text::Bold);
	text.setPosition(43 + dialog_pos.x, 314 + dialog_pos.y);
	text.setString("Get item");
	game_window->draw(text);

	
	//кнопка разборки
	text.setCharacterSize(40); // in pixels, not points!
	text.setFillColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold);
	text.setPosition(369 + dialog_pos.x, 314 + dialog_pos.y);
	text.setString("Delete item");
	game_window->draw(text);

	/*
	266,260 (175) - картинка
	376, 73 - описание
	43,314 - 327,447 - замена
		369,314,-- 814,446 - слив
		*/

	//рисуем курсор
	sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);
	mouse_pointer_Sprite.setPosition((float)localPosition.x, (float)localPosition.y);
	game_window->draw(mouse_pointer_Sprite);


	game_window->display();

	//проверяем нажатие кнопок
	int take_area[4] = { 314 + (int)dialog_pos.y, 447 + (int)dialog_pos.y, 43 + (int)dialog_pos.x, 327 + (int)dialog_pos.x };//top,bottom,left,right
	int dismis_area[4] = { 314 + (int)dialog_pos.y, 447 + (int)dialog_pos.y, 369 + (int)dialog_pos.x, 814 + (int)dialog_pos.x };
	

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//printf("%i  %i \n", localPosition.x, localPosition.y);
		
		// проверяем нажатие на кнопки
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &take_area[0]))
		{
			equipment_array[equipment_to_take].isValid = 0;
			equipment_count--; 
			//printf("equipment_OK \n");
			equipment_to_take = -1;
			return (1);//убираем диалог и продолжаем игру дальше
		}
			
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &dismis_area[0]))
		{
			equipment_array[equipment_to_take].isValid = 0;
			equipment_count--;
			//printf("equipment_DEL \n");
			equipment_to_take = -1;
			return (1);//убираем диалог и продолжаем игру дальше
		}
	}
	
	//sf::sleep(sf::milliseconds(1000));
	return (0);//продолжаем выполнение подпрограммы
}

void Game::set_render_scale(double scale)
{
	GAME_RENDER_SCALE = scale;	
	
}

void Game::draw_terrain()
{
	int left_tile = floor((Player_instance.player_coord_X / GAME_RENDER_SCALE - Player_instance.player_offset_X - render_res_X / 2.0) / ((double)land_tile_size / GAME_RENDER_SCALE));
	int right_tile = left_tile + ceil((double)render_res_X / ((double)land_tile_size / GAME_RENDER_SCALE));
	int top_tile = floor((round(Player_instance.player_coord_Y / GAME_RENDER_SCALE) - round(Player_instance.player_offset_Y) + (double)render_res_Y / 2.0) / ((double)land_tile_size / GAME_RENDER_SCALE));
	int bottom_tile = top_tile - ceil((double)render_res_Y / ((double)land_tile_size / GAME_RENDER_SCALE));

	for (int i = left_tile; i <= right_tile; ++i)
	{
		for (int j = top_tile; j >= bottom_tile; --j)
		{
			int num = rnd_from_coord(i, j, 19);
			bg_tile_Sprite.setTextureRect(sf::IntRect(num * 100, 0, land_tile_size + 1, land_tile_size + 1));
			bg_tile_Sprite.setPosition(i * ((double)land_tile_size / GAME_RENDER_SCALE) + render_res_X / 2 - (Player_instance.player_coord_X / GAME_RENDER_SCALE - Player_instance.player_offset_X), -(j + 1) * ((double)land_tile_size / GAME_RENDER_SCALE) + (Player_instance.player_coord_Y / GAME_RENDER_SCALE - Player_instance.player_offset_Y) + render_res_Y / 2);
			bg_tile_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
			game_window->draw(bg_tile_Sprite);
			//дорисовываем кусты поверх
			//num = rnd_from_coord(i + 349, j + 829, 19);
			//bg_tile_Sprite.setTextureRect(sf::IntRect(num * 100, 100, land_tile_size, land_tile_size));
			//game_window->draw(bg_tile_Sprite);
		}
	}

}

void Game::draw_loot()
	{
	int loot_index = 0;
	int loot_processed = 0;
	while (loot_processed < loot_count)
	{
		//проходим по массиву лута
		if (loot_array[loot_index].isValid == 1)
		{
			if (abs(loot_array[loot_index].X - Player_instance.player_coord_X) < render_res_X * GAME_RENDER_SCALE && abs(loot_array[loot_index].Y - Player_instance.player_coord_Y) < render_res_Y * GAME_RENDER_SCALE)
			{
				//проверяем коллизию
				if (abs(loot_array[loot_index].X - Player_instance.player_coord_X) < 100 && abs(loot_array[loot_index].Y - Player_instance.player_coord_Y) < 100)
				{
					//"потребляем" лут

					if (loot_array[loot_index].type == 0)
					{
						//пробуем съесть аптечку
						Player_instance.Health += loot_array[loot_index].power;
						//printf("health = %0f\n", Player_instance.Health);
					}

					if (loot_array[loot_index].type == 1)
					{
						//пробуем съесть энергетик
						Player_instance.Energy += loot_array[loot_index].power;
						//printf("energy = %0f\n", Player_instance.Energy);
					}

					if (loot_array[loot_index].type == 2)
					{
						//пробуем съесть комбо
						Player_instance.Health += loot_array[loot_index].power;
						Player_instance.Energy += loot_array[loot_index].power;
						//printf("health = %0f, energy = %0f\n", Player_instance.Health, Player_instance.Energy);
					}

					//printf("loot eaten\n");
					loot_array[loot_index].isValid = 0;
					loot_count--;

				}
				else
				{
					//рисуем лут
					int tile_offset = loot_array[loot_index].type * 100;
					loot_Sprite.setTextureRect(sf::IntRect(tile_offset, 0, 100, 100));
					loot_Sprite.setPosition(loot_array[loot_index].X / GAME_RENDER_SCALE - 50.0 / GAME_RENDER_SCALE - (Player_instance.player_coord_X / GAME_RENDER_SCALE - Player_instance.player_offset_X - render_res_X / 2), Player_instance.player_coord_Y / GAME_RENDER_SCALE - Player_instance.player_offset_Y + render_res_Y / 2 - loot_array[loot_index].Y / GAME_RENDER_SCALE - 50.0 / GAME_RENDER_SCALE);
					loot_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
					game_window->draw(loot_Sprite);
				}


			}

			loot_processed++;
		}
		loot_index++;
	}
	//printf("loot procc = %i \n", loot_processed);
	}

void Game::draw_equipment()
{
	int equipment_index = 0;
	int equipment_processed = 0;
	while (equipment_processed < equipment_count)
	{
		//проходим по массиву лута
		if (equipment_array[equipment_index].isValid == 1)
		{
			if (abs(equipment_array[equipment_index].X - Player_instance.player_coord_X) < render_res_X * GAME_RENDER_SCALE && abs(equipment_array[equipment_index].Y - Player_instance.player_coord_Y) < render_res_Y * GAME_RENDER_SCALE)
			{
				//проверяем коллизию
				if (abs(equipment_array[equipment_index].X - Player_instance.player_coord_X) < 100 && abs(equipment_array[equipment_index].Y - Player_instance.player_coord_Y) < 100)
				{
					equipment_to_take = equipment_index;
					//printf("Take %i \n", equipment_to_take);
				}
				else
				{
					//рисуем оборудование
					int tile_offset = (equipment_array[equipment_index].type + 3) * 100;
					loot_Sprite.setTextureRect(sf::IntRect(tile_offset, 0, 100, 100));
					loot_Sprite.setPosition(equipment_array[equipment_index].X / GAME_RENDER_SCALE - 50.0 / GAME_RENDER_SCALE - (Player_instance.player_coord_X / GAME_RENDER_SCALE - Player_instance.player_offset_X - render_res_X / 2), Player_instance.player_coord_Y / GAME_RENDER_SCALE - Player_instance.player_offset_Y + render_res_Y / 2 - equipment_array[equipment_index].Y / GAME_RENDER_SCALE - 50.0 / GAME_RENDER_SCALE);
					loot_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
					game_window->draw(loot_Sprite);
				}
			}
			equipment_processed++;
		}
		equipment_index++;
	}



}

void Game::draw_character()
{
	//привязываем скорость анимации к прошедшему времени
	Player_instance.animation_time = Player_instance.animation_time + elapsed_ms;
	if (Player_instance.animation_time > Player_instance.animation_time_MAX) Player_instance.animation_time -= Player_instance.animation_time_MAX;

	body_tile_Sprite.setPosition(render_res_X / 2 + Player_instance.player_offset_X, render_res_Y / 2 - Player_instance.player_offset_Y);
	body_tile_Sprite.setTextureRect(sf::IntRect(round(Player_instance.animation_time / Player_instance.animation_time_MAX) * 7 * 200, (Player_instance.body_direction - 1) * 200, 200, 200));
	body_tile_Sprite.setOrigin(100.0, 100.0);
	body_tile_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
	game_window->draw(body_tile_Sprite);

	//рисуем пушку

	Player_instance.weapon_animation_time += elapsed_ms;
	if (Player_instance.weapon_animation_time > Player_instance.weapon_animation_time_MAX) Player_instance.weapon_animation_time -= Player_instance.weapon_animation_time_MAX;

	weapon_tile_Sprite.setPosition(render_res_X / 2 + Player_instance.player_offset_X, render_res_Y / 2 - Player_instance.player_offset_Y);
	weapon_tile_Sprite.setTextureRect(sf::IntRect(round(Player_instance.weapon_animation_time / Player_instance.weapon_animation_time_MAX) * 7 * 200, (Player_instance.weapon_direction - 1) * 200, 200, 200));
	weapon_tile_Sprite.setOrigin(100.0, 100.0);
	weapon_tile_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
	game_window->draw(weapon_tile_Sprite);


}

void Game::controls_processing(sf::Vector2i localPosition)
{
	int player_D_x = 0;
	int player_D_y = 0;
	player_weapon_cooldown -= elapsed_ms; //охлаждаем ствол
	if (player_weapon_cooldown < 0) player_weapon_cooldown = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) //движение вверх
	{
		Player_instance.player_coord_Y = Player_instance.player_coord_Y + Player_instance.Speed / 1000 * elapsed_ms;
		if (visited_max_Y < Player_instance.player_coord_Y) visited_max_Y = Player_instance.player_coord_Y; //расширяем границы исследованной области
		Player_instance.player_offset_Y = Player_instance.player_offset_Y + Player_instance.Speed / 1000 * elapsed_ms / GAME_RENDER_SCALE;
		if (Player_instance.player_offset_Y > player_max_offset_Y) Player_instance.player_offset_Y = player_max_offset_Y;
		player_D_y++;//для определения направленияц
		//printf("%0.0f %0.0f\n", Player_instance.player_offset_Y, player_max_offset_Y);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) //движение вниз
	{
		Player_instance.player_coord_Y = Player_instance.player_coord_Y - Player_instance.Speed / 1000 * elapsed_ms;
		if (visited_min_Y > Player_instance.player_coord_Y) visited_min_Y = Player_instance.player_coord_Y;//расширяем границы исследованной области
		Player_instance.player_offset_Y = Player_instance.player_offset_Y - Player_instance.Speed / 1000 * elapsed_ms / GAME_RENDER_SCALE;;
		if (Player_instance.player_offset_Y < -player_max_offset_Y) Player_instance.player_offset_Y = -player_max_offset_Y;
		player_D_y--;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //движение влево
	{
		Player_instance.player_coord_X = Player_instance.player_coord_X - Player_instance.Speed / 1000 * elapsed_ms;
		if (visited_min_X > Player_instance.player_coord_X) visited_min_X = Player_instance.player_coord_X;//расширяем границы исследованной области
		Player_instance.player_offset_X = Player_instance.player_offset_X - Player_instance.Speed / 1000 * elapsed_ms / GAME_RENDER_SCALE;;
		if (Player_instance.player_offset_X < -player_max_offset_X) Player_instance.player_offset_X = -player_max_offset_X;
		player_D_x--;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) //движение вправо
	{
		Player_instance.player_coord_X = Player_instance.player_coord_X + Player_instance.Speed / 1000 * elapsed_ms;
		if (visited_max_X < Player_instance.player_coord_X) visited_max_X = Player_instance.player_coord_X;//расширяем границы исследованной области
		Player_instance.player_offset_X = Player_instance.player_offset_X + Player_instance.Speed / 1000 * elapsed_ms / GAME_RENDER_SCALE;;
		if (Player_instance.player_offset_X > player_max_offset_X) Player_instance.player_offset_X = player_max_offset_X;
		player_D_x++;
	}

	//определение направления движения игрока
	{
		if (player_D_x == 0 && player_D_y == 1) Player_instance.body_direction = 1;
		if (player_D_x == 1 && player_D_y == 1) Player_instance.body_direction = 2;
		if (player_D_x == 1 && player_D_y == 0) Player_instance.body_direction = 3;
		if (player_D_x == 1 && player_D_y == -1) Player_instance.body_direction = 4;
		if (player_D_x == 0 && player_D_y == -1) Player_instance.body_direction = 5;
		if (player_D_x == -1 && player_D_y == -1) Player_instance.body_direction = 6;
		if (player_D_x == -1 && player_D_y == 0) Player_instance.body_direction = 7;
		if (player_D_x == -1 && player_D_y == 1) Player_instance.body_direction = 8;
	}
	
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) //масштаб 2
	{
		GAME_RENDER_SCALE = 2;
	}


	//считаем угол между игроком и курсором
	int D_x = localPosition.x - (render_res_X / 2 + Player_instance.player_offset_X);
	int D_y = (render_res_Y / 2 - Player_instance.player_offset_Y) - localPosition.y;
	Player_instance.weapon_direction_rad = std::atan2((double)D_y, (double)D_x);
	if (Player_instance.weapon_direction_rad < 0) Player_instance.weapon_direction_rad += 2 * 3.1415926; 

	//устанавливаем направление оружия для анимации
	if ((Player_instance.weapon_direction_rad >= 45 * 7.5 / 180 * 3.1415) || (Player_instance.weapon_direction_rad < 45 * 0.5 / 180 * 3.1415)) Player_instance.weapon_direction = 3;
	if ((Player_instance.weapon_direction_rad >= 45 * 0.5 / 180 * 3.1415) && (Player_instance.weapon_direction_rad < 45 * 1.5 / 180 * 3.1415)) Player_instance.weapon_direction = 2;
	if ((Player_instance.weapon_direction_rad >= 45 * 1.5 / 180 * 3.1415) && (Player_instance.weapon_direction_rad < 45 * 2.5 / 180 * 3.1415)) Player_instance.weapon_direction = 1;
	if ((Player_instance.weapon_direction_rad >= 45 * 2.5 / 180 * 3.1415) && (Player_instance.weapon_direction_rad < 45 * 3.5 / 180 * 3.1415)) Player_instance.weapon_direction = 8;
	if ((Player_instance.weapon_direction_rad >= 45 * 3.5 / 180 * 3.1415) && (Player_instance.weapon_direction_rad < 45 * 4.5 / 180 * 3.1415)) Player_instance.weapon_direction = 7;
	if ((Player_instance.weapon_direction_rad >= 45 * 4.5 / 180 * 3.1415) && (Player_instance.weapon_direction_rad < 45 * 5.5 / 180 * 3.1415)) Player_instance.weapon_direction = 6;
	if ((Player_instance.weapon_direction_rad >= 45 * 5.5 / 180 * 3.1415) && (Player_instance.weapon_direction_rad < 45 * 6.5 / 180 * 3.1415)) Player_instance.weapon_direction = 5;
	if ((Player_instance.weapon_direction_rad >= 45 * 6.5 / 180 * 3.1415) && (Player_instance.weapon_direction_rad < 45 * 7.5 / 180 * 3.1415)) Player_instance.weapon_direction = 4;

	//проверяем нажатие на кнопку LMB и доступность оружия
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && player_weapon_cooldown == 0)
	{
		//если места в массиве патронов достаточно, создаем еще один
		if (player_bullet_count < 100)
		{
			//ищем пустую ячейку в массиве
			int new_item = 0;
			for (new_item = 0; new_item < 100 && player_bullet_array[new_item].isAlive == 1; ++new_item);
			if (player_bullet_array[new_item].isAlive == 0 )//еще раз проверяем, что позиция не занята
			{
				player_bullet_array[new_item].isAlive = 1;
				player_bullet_array[new_item].type = 1;
				player_bullet_array[new_item].power = 5;
				player_bullet_array[new_item].speed = 2000;
				player_bullet_array[new_item].power = 5;
				if (Player_instance.weapon_direction_rad > 3.1415926) player_bullet_array[new_item].direction = Player_instance.weapon_direction_rad - 2 * 3.141926;
				else player_bullet_array[new_item].direction = Player_instance.weapon_direction_rad;
				player_bullet_array[new_item].power = 5;
				player_bullet_array[new_item].X = Player_instance.player_coord_X + cos(player_bullet_array[new_item].direction) * 100.0;
				player_bullet_array[new_item].Y = Player_instance.player_coord_Y + sin(player_bullet_array[new_item].direction) * 100.0;
				player_bullet_array[new_item].ttl = 4000;//time to live
				player_bullet_count++;
				//рассчитываем скорость пули по осям в секунду
				//printf("fire %0.0f %0.0f \n", player_bullet_array[new_item].X, player_bullet_array[new_item].Y);
				player_weapon_cooldown = 100;
			}
		}
	}

	//тестовая информация
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Multiply))
	{
		printf("Monster_Bullet_count = %i\n", monster_bullet_count);
		int counter = 0;
		for (int i=0; i < 100; ++i)
		{
			if (monster_bullet_array[i].isAlive == 1)
			{
				counter++;
				printf("%4i  %4i  %6.0f  %6.0f  %6.2f %6.2f\n", i, monster_bullet_array[i].isAlive, monster_bullet_array[i].X, monster_bullet_array[i].Y, monster_bullet_array[i].direction, monster_bullet_array[i].ttl);
			}
		}
		sf::sleep(sf::milliseconds(300));
		printf("bullets_in_array=%i \n", counter);
	}
}

void Game::draw_bullets()
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
						if (abs(player_bullet_array[i].X - monster_array[j].X) < 50 && abs(player_bullet_array[i].Y - monster_array[j].Y) < 50)
						{
							//фиксируем попадание
							monster_array[j].health -= player_bullet_array[i].power; //уменьшаем здоровье
							if (monster_array[j].health <= 0)
							{
								monster_array[j].isAlive = 0; //фиксируем смерть
								monster_count--;
								monsters_procecced--;
								//создаем спецэффект на месте монстра
								create_VFX(monster_array[j].X, monster_array[j].Y, 1); // тип 1 - взрыв
							}
							player_bullet_array[i].isAlive = 0;//пуля исчезает после попадания
							player_bullet_count--;
						}
						monsters_procecced++;
					}
				}
			}
			
			if (player_bullet_array[i].isAlive == 1) //повторная проверка т.к. пуля могла попасть в монстра
			{
				//рисуем спрайт пули
				player_bullet_Sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
				player_bullet_Sprite.setOrigin(50, 50);
				player_bullet_Sprite.setPosition(render_res_X / 2 + Player_instance.player_offset_X - Player_instance.player_coord_X / GAME_RENDER_SCALE + player_bullet_array[i].X / GAME_RENDER_SCALE, render_res_Y / 2 + Player_instance.player_coord_Y / GAME_RENDER_SCALE - Player_instance.player_offset_Y - player_bullet_array[i].Y / GAME_RENDER_SCALE);
				player_bullet_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
				player_bullet_Sprite.setRotation(-player_bullet_array[i].direction * 180.0 / 3.1415926);
				game_window->draw(player_bullet_Sprite);
				//printf("%4i  %4i  %4i  %6.0f  %6.0f  %6.0f %6.6f\n", i, player_bullet_array[i].isAlive, player_bullet_array[i].type, player_bullet_array[i].power, player_bullet_array[i].X, player_bullet_array[i].Y, player_bullet_array[i].direction);
			}
		}
	}
	
}

void Game::generate_monsters()
{
	monster_gen_timer = 1000;//переделать под константы

	//генерируем монстров вокруг игрока

	int MONSTER_TO_GEN = 3 + floor((abs(Player_instance.player_coord_X) + abs(Player_instance.player_coord_Y))/5000.0); // задать количество в зависимости от расстояния
	int GENERATED_MONSTER = 0;
	int min_gen_dist, ring_width;
	min_gen_dist = (render_res_X / 2 + abs(Player_instance.player_offset_X)) * 1.5 * GAME_RENDER_SCALE;
	ring_width = 1000; //ширина области генерации
	//printf("%i  %i", min_gen_dist, ring_width);
	double angle = 0.0, dist = 0.0;
	//цикл генерации
	for (int i = 0; i < 1000 && monster_count < 1000 && GENERATED_MONSTER < MONSTER_TO_GEN; ++i)
	{
		if (monster_array[i].isAlive == 0)
		{
			angle = (rand() / (double)RAND_MAX) * 2 * 3.1415926 - 3.1415926; //угол
			dist = min_gen_dist + (rand() / (double)RAND_MAX) * ring_width; //расстояние до монстра
			double NEW_X = cos(angle) * dist + (Player_instance.player_coord_X - Player_instance.player_offset_X);
			double NEW_Y = sin(angle) * dist + (Player_instance.player_coord_Y - Player_instance.player_offset_Y);

			//добавляем данные в массив монстров
			monster_array[i].isAlive = 1;
			monster_array[i].sprite_N = floor(15.999 * rand() / (double)RAND_MAX);;
			monster_array[i].power = floor(10.0 * rand() / (double)RAND_MAX);
			monster_array[i].moving_speed = 100 + (rand() / (double)RAND_MAX) * 100;
			monster_array[i].direction = atan2(Player_instance.player_coord_Y - NEW_Y, Player_instance.player_coord_X - NEW_X);
			monster_array[i].attack_type = 1; // 0 - рукопашка, 1 - дальняя, 2 - по площади
			monster_array[i].attack_time = 1000;
			monster_array[i].attack_cooldown = 1000;
			monster_array[i].health = 3;
			monster_array[i].X = NEW_X;
			monster_array[i].Y = NEW_Y;

			//printf("monster added #%5i = %5i   %5i   (%0f, %0f)   %1.2f\n", i, monster_array[i].sprite_N, monster_array[i].power, NEW_X, NEW_Y, monster_array[i].direction);
			//printf("X=%5.0f Y=%5.0f ATAN2(%5.0f,%5.0f) = %1.3f", NEW_X, NEW_Y, Player_instance.player_coord_X - NEW_X, Player_instance.player_coord_Y - NEW_Y, monster_array[i].direction);
			monster_count++;
			GENERATED_MONSTER++;
		}
		else //если монстр жив, проверяем минимальное расстояние до игрока по X Y
		{
			if ((monster_array[i].X - Player_instance.player_coord_X) > 6000.0 || (monster_array[i].Y - Player_instance.player_coord_Y) > 6000.0)
			{
				monster_array[i].isAlive = 0; //убиваем монстра
				monster_count--;
				//printf("monter died\n");
			}
		}
		
	}
	//printf("monsters added = %i\n", GENERATED_MONSTER);
}

void Game::draw_monsters()
{
	//проходимся по массиву монстров и рисуем спрайты
	//также нужно проверить коллизии и отрисовать анимацию
	for (int i = 0; i < monster_count; ++i)
	{
		//меняем направление
		monster_array[i].direction = atan2(Player_instance.player_coord_Y - monster_array[i].Y, Player_instance.player_coord_X - monster_array[i].X);

		//рассчитываем движение
		monster_array[i].X += monster_array[i].moving_speed * cos(monster_array[i].direction) * elapsed_ms / 1000;
		monster_array[i].Y += monster_array[i].moving_speed * sin(monster_array[i].direction) * elapsed_ms / 1000;

		if (monster_array[i].isAlive == 1) // проверяем жив ли монстр
		{
			
			//если монстр вблизи - рисуем его
			if (abs(monster_array[i].X - Player_instance.player_coord_X) < render_res_X * GAME_RENDER_SCALE && abs(monster_array[i].Y - Player_instance.player_coord_Y) < render_res_Y * GAME_RENDER_SCALE)
			{

				int sprite_array = monster_array[i].sprite_N / 8;

				monster_Sprite_array[sprite_array].setTextureRect(sf::IntRect(0, 0, 96, 96));;
				monster_Sprite_array[sprite_array].setOrigin(48, 48);
				monster_Sprite_array[sprite_array].setPosition(render_res_X / 2 + Player_instance.player_offset_X - Player_instance.player_coord_X / GAME_RENDER_SCALE + monster_array[i].X / GAME_RENDER_SCALE, render_res_Y / 2 + Player_instance.player_coord_Y / GAME_RENDER_SCALE - Player_instance.player_offset_Y - monster_array[i].Y / GAME_RENDER_SCALE);
				monster_Sprite_array[sprite_array].setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
				game_window->draw(monster_Sprite_array[sprite_array]);
				
				monster_array[i].attack_cooldown -= elapsed_ms;
				if (monster_array[i].attack_cooldown <= 0)
				{
					//производим атаку
					monster_array[i].attack_cooldown = monster_array[i].attack_time; //перезарядка таймера
					//ищем место в массиве пуль монстров
					for (int j = 0; j < 10; ++j)
					{
						if (monster_bullet_array[j].isAlive == 0)
						{
							//заполняем массив данными пули
							monster_bullet_array[j].isAlive = 1;
							monster_bullet_array[j].type = monster_array[j].attack_type;
							monster_bullet_array[j].sprite_N = 0;//дефолтный уровень
							if (monster_array[i].attack_type == 1) monster_bullet_array[j].sprite_N = 1;
							if (monster_array[i].attack_type == 2) monster_bullet_array[j].sprite_N = 2;
							monster_bullet_array[j].power = monster_array[i].power;
							monster_bullet_array[j].speed = 100.0;
							monster_bullet_array[j].direction = monster_array[i].direction;
							monster_bullet_array[j].ttl = 0; //оставшееся время жизни по-умолчанию (укус)
							if (monster_array[i].attack_type == 1) monster_bullet_array[j].ttl = 5000; //пуля
							if (monster_array[i].attack_type == 2) monster_bullet_array[j].ttl = 1000; //граната
							monster_bullet_array[j].X = monster_array[i].X;
							monster_bullet_array[j].Y = monster_array[i].Y;
							break;
						}
					}
				}
			}
		}
	}
}

void Game::create_VFX(double VFX_X, double VFX_Y, int VFX_type)
{
	//поиск свободного слота	
	int i;
	for (i = 0; i < 100 && VFX_array[i].isAlive > 0; ++i);
	if (VFX_array[i].isAlive > 0) return; //если все слоты заняты - возврат
	
	if (VFX_type == 1)
	{
		//добавляем строку в массив
		VFX_array[i].isAlive = 2; // 0 - свободный слот, 1 - анимация, 2 - статичный спрайт (после проигрывания анимации)
		VFX_array[i].number = 1;//номер в атласе. размер у всех 100*100
		VFX_array[i].last_frame = 16;
		VFX_array[i].keep_alive = 0; // 0 или 1
		VFX_array[i].total_time = 500; //в микросекундах
		VFX_array[i].remaining_time = 500;
		VFX_array[i].remains_num = 1;
		VFX_array[i].X = VFX_X;
		VFX_array[i].Y = VFX_Y;
	}
}

void Game::draw_VFX()
{
	//return;
	for (int i = 0; i < 100; ++i)
	{
		if (VFX_array[i].isAlive == 0) continue; //если это поле = 0, переход к следующему элементу
		int pos = floor((VFX_array[i].total_time - VFX_array[i].remaining_time) / VFX_array[i].total_time * (VFX_array[i].last_frame - 1)) * 100;
		VFX_Sprite.setTextureRect(sf::IntRect(pos, 0, 100, 100));
		VFX_Sprite.setOrigin(50, 50);
		VFX_Sprite.setPosition(render_res_X / 2 + Player_instance.player_offset_X - Player_instance.player_coord_X / GAME_RENDER_SCALE + VFX_array[i].X / GAME_RENDER_SCALE, render_res_Y / 2 + Player_instance.player_coord_Y / GAME_RENDER_SCALE - Player_instance.player_offset_Y - VFX_array[i].Y / GAME_RENDER_SCALE);
		VFX_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
		game_window->draw(VFX_Sprite);
		//увеличиваем счетчик времени
		VFX_array[i].remaining_time -= elapsed_ms;
		if (VFX_array[i].remaining_time <= 0)
		{
			VFX_array[i].isAlive = 0; //убиваем спрайт
			if (VFX_array[i].remains_num != 0)
			{
				//рисуем останки
				create_VFX_remains(VFX_array[i].X, VFX_array[i].Y, VFX_array[i].remains_num);
			}
		}
		
	}
}

void Game::draw_VFX_remains()
{
	for (int i = 0; i < 1000; ++i)
	{
		if (VFX_remains_array[i].isAlive == 1)
		{
			//проверяем координаты
			double sprite_X = render_res_X / 2.0 + Player_instance.player_offset_X - ((Player_instance.player_coord_X - VFX_remains_array[i].X) / GAME_RENDER_SCALE);
			double sprite_Y = render_res_Y / 2.0 - Player_instance.player_offset_Y + ((Player_instance.player_coord_Y - VFX_remains_array[i].Y) / GAME_RENDER_SCALE);
			if (sprite_X > 0 && sprite_X < render_res_X && sprite_Y > 0 && sprite_Y < render_res_Y)
			{
				//рисуем спрайт
				VFX_remains_Sprite.setTextureRect(sf::IntRect((VFX_remains_array[i].number - 1) * 100, 0, 100, 100));
				VFX_remains_Sprite.setOrigin(50, 50);
				VFX_remains_Sprite.setPosition(sprite_X, sprite_Y);
				VFX_remains_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
				VFX_remains_Sprite.setColor(sf::Color(255, 255, 255, 128));
				game_window->draw(VFX_remains_Sprite);
			}
		}
	}
}

void Game::create_VFX_remains(double VFX_X, double VFX_Y, int VFX_type)
{
	VFX_remains_array[VFX_remains_pointer].isAlive = 1;
	VFX_remains_array[VFX_remains_pointer].number = VFX_type;
	VFX_remains_array[VFX_remains_pointer].X = VFX_X;
	VFX_remains_array[VFX_remains_pointer].Y = VFX_Y;
	VFX_remains_pointer++;
	if (VFX_remains_pointer == 1000) VFX_remains_pointer = 0;
}

void Game::draw_monster_bullets()
{
	for (int i = 0; i < 100; ++i)
	{
		if (monster_bullet_array[i].isAlive == 1)
		{
			//проверяем координаты
			double sprite_X = render_res_X / 2.0 + Player_instance.player_offset_X - ((Player_instance.player_coord_X - monster_bullet_array[i].X) / GAME_RENDER_SCALE);
			double sprite_Y = render_res_Y / 2.0 - Player_instance.player_offset_Y + ((Player_instance.player_coord_Y - monster_bullet_array[i].Y) / GAME_RENDER_SCALE);
			monster_bullet_array[i].ttl -= elapsed_ms;
			if (monster_bullet_array[i].ttl <= 0) monster_bullet_array[i].isAlive = 0;
			if (sprite_X > 0 && sprite_X < render_res_X && sprite_Y > 0 && sprite_Y < render_res_Y && monster_bullet_array[i].isAlive == 1)
			{
				//рисуем спрайт
				
				monster_bullet_Sprite.setTextureRect(sf::IntRect((monster_bullet_array[i].type) * 100, 0, 100, 100));
				monster_bullet_Sprite.setOrigin(50, 50);
				monster_bullet_Sprite.setPosition(sprite_X, sprite_Y);
				monster_bullet_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
				monster_bullet_Sprite.setColor(sf::Color(255, 255, 255, 128));
				game_window->draw(monster_bullet_Sprite);
				/*
				asa
				unsigned char type; // 0 - рукопашка/укус, 1 - обычная пуля, 2 - взрывная граната/ограниченный радиус
				unsigned char sprite_N;
				double power;
				double speed;
				double direction;
				double ttl; //оставшееся время жизни
				double X;
				double Y;*/
			}
		}
	}

}