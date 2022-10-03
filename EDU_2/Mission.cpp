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
	GAME_RENDER_SCALE = 2.0;

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

	bg_player_Texture.loadFromFile(game_path + "Assets\\Textures\\Player_tiles.png");
	bg_player_Sprite.setTexture(bg_player_Texture);

	bg_tile_Texture.loadFromFile(game_path + "Assets\\Textures\\Ground_tiles.png");
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

	loot_Texture.loadFromFile(game_path + "Assets\\Textures\\Loot.png");
	loot_Sprite.setTexture(loot_Texture);

	interface_Texture.loadFromFile(game_path + "Assets\\Pictures\\Interface_2.png");
	interface_Sprite.setTexture(interface_Texture);

	interface_equip_Texture.loadFromFile(game_path + "Assets\\Pictures\\Dialog_equipment.png");
	interface_equip_Sprite.setTexture(interface_equip_Texture);

	player_bullet_Texture.loadFromFile(game_path + "Assets\\Textures\\Bullets.png");
	player_bullet_Sprite.setTexture(player_bullet_Texture);

	monster_Texture_1.loadFromFile(game_path + "Assets\\Textures\\Monsters_1.png");
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


	//загружаем шрифт
	if (!font.loadFromFile(game_path + "Assets\\Fonts\\arialnb.ttf")) printf("font load error\n");
	text.setFont(font); // font is a sf::Font

	//создаем создаем структуры данных
	loot_array = (loot*)malloc(1000 * sizeof(loot));
	if (loot_array == NULL) printf("malloc error loot array");

	obj_array = (stationary_object*)malloc(100 * sizeof(stationary_object));
	if (obj_array == NULL) printf("malloc error obj array");

	equipment_array = (equipment*)malloc(1000 * sizeof(equipment));
	if (equipment_array == NULL) printf("malloc error equipment array");

	equipment_to_take = -1;

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

	//открываем файл с музыкой
	if (!music_action.openFromFile(game_path + "Assets\\Music\\Mission music.ogg")) printf("Error load music\n");
	music_action.setLoop(true);
	music_action.setVolume(20);

	//открываем звуковые файлы
	if (!buffer_gun_1.loadFromFile(game_path + "Assets\\Sound\\gun_1.wav"))  printf("Error load gun_1.wav\n");
	sound_gun_1.setBuffer(buffer_gun_1);//выстрел из пушки
	sound_gun_1.setVolume(50);

	if (!buffer_bite_1.loadFromFile(game_path + "Assets\\Sound\\bite_1.wav"))  printf("Error load bite_1.wav\n");
	sound_bite_1.setBuffer(buffer_bite_1);//укус монстра
	sound_bite_1.setVolume(50);

	if (!buffer_hit_1.loadFromFile(game_path + "Assets\\Sound\\hit_1.wav"))  printf("Error load hit_1.wav\n");
	sound_hit_1.setBuffer(buffer_hit_1);//удар снаряда в металл
	sound_hit_1.setVolume(50);

	if (!buffer_hit_2.loadFromFile(game_path + "Assets\\Sound\\hit_2.wav"))  printf("Error load hit_2.wav\n");
	sound_hit_2.setBuffer(buffer_hit_2);//удар снаряда в плоть
	sound_hit_2.setVolume(50);

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

};

int Mission::run(sf::Time elapsed)
{
	//основной цикл игры

	//запускаем музыку если она не играет
	if (music_action.getStatus() != sf::SoundSource::Status::Playing) music_action.play();

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
	draw_stationary_obj();

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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && pause_delay_timer <= 0)
	{
		//делаем скриншот
		sf::Vector2u windowSize = game_window->getSize();
		screnshot_Texture.create(windowSize.x, windowSize.y);
		screnshot_Texture.update(*game_window);
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

	//обработка нажатия кнопки Е

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		//активация объекта нажатием на Е
		int obj_near = check_obj_collision(Rover.player_coord_X, Rover.player_coord_Y, 300.0f);
		if (obj_near != 0)
			if (obj_array[obj_near - 1].type == 0)
			{
				//вход в базу
				//printf("[E] Enter Base");
				sf::Vector2u windowSize = game_window->getSize();
				screnshot_Texture.create(windowSize.x, windowSize.y);
				screnshot_Texture.update(*game_window);
				return (4);
			}

		if (obj_array[obj_near - 1].type == 1)
		{
			//рядом выход
			//printf("[E] Evacuate");
			sf::Vector2u windowSize = game_window->getSize();
			screnshot_Texture.create(windowSize.x, windowSize.y);
			screnshot_Texture.update(*game_window);
			//music_action.stop();//вырубаем музон
			return (5);
		}
	}

	//проверка здоровья
	if (Rover.Health <= 0) //смерть
	{
		sf::Vector2u windowSize = game_window->getSize();
		screnshot_Texture.create(windowSize.x, windowSize.y);
		screnshot_Texture.update(*game_window);
		return (5);
	}

	return(0);//просто продолжаем цикл
};

int Mission::start()
{
	//инициализация игры
	game_window->clear(sf::Color::Black);
	game_window->draw(bg_game_Sprite);
	game_window->display();
	generate_loot();
	sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);
	srand(1);//перезапускаем генератор RND
	reset_Rover();
	reset_world(); //перезапускаем мир
	return(1); //в случае успеха возвращаем 1, иначе 0
}

int Mission::pause(sf::Time elapsed)
{
	//запрашиваем размеры окна
	sf::Vector2u new_win_size = game_window->getSize();
	double kx = new_win_size.x / 1920.0;
	double ky = new_win_size.y / 1080.0;

	music_action.pause(); //ставим музон на паузу
	if (pause_delay_timer > 0) pause_delay_timer -= elapsed.asMilliseconds();//охлаждаем таймер
	//printf("%i", pause_delay_timer);
	sf::Sprite screenshot_Sprite;

	screenshot_Sprite.setTexture(screnshot_Texture);
	screenshot_Sprite.setColor(sf::Color(255, 255, 255, 50));
	game_window->clear(sf::Color::Black);
	game_window->draw(screenshot_Sprite);
	pause_menu_Sprite.setScale(new_win_size.x / 1920.0, new_win_size.y / 1080.0);
	game_window->draw(pause_menu_Sprite);

	//задаем области кнопок
	int Continue_area[4] = { 200 * ky, 300 * ky, 500 * kx, 1420 * kx };
	int Exit_area[4] = { 600 * ky, 700 * ky, 500 * kx, 1420 * kx };

	//проверяем координаты мыши относительно экрана
	sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);

	//рисуем курсор
	mouse_pointer_Sprite.setPosition((float)localPosition.x, (float)localPosition.y);
	game_window->draw(mouse_pointer_Sprite);

	//проверяем попадание по пунктам меню
	if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Continue_area[0]))
	{
		sf::RectangleShape rectangle(sf::Vector2f(Continue_area[3] - Continue_area[2], Continue_area[1] - Continue_area[0]));
		rectangle.setFillColor(sf::Color(0, 0, 0, 128));
		rectangle.setPosition(Continue_area[2], Continue_area[0]);
		game_window->draw(rectangle);
	}

	if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0]))
	{
		sf::RectangleShape rectangle(sf::Vector2f(Exit_area[3] - Exit_area[2], Exit_area[1] - Exit_area[0]));
		rectangle.setFillColor(sf::Color(0, 0, 0, 128));
		rectangle.setPosition(Exit_area[2], Exit_area[0]);
		game_window->draw(rectangle);
	}

	game_window->display();

	//кнопка ESC
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && pause_delay_timer <= 0)
	{
		//OutputDebugString(L"Пауза x\n");
		//printf("%i", pause_delay_timer);
		pause_delay_timer = 200; //включаем задержку заново
		return(1);				//снять паузу
	}

	//кнопка продолжить
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Continue_area[0]))
		{
			if (pause_delay_timer <= 0)
			{
				pause_delay_timer = 200; //включаем задержку заново
				return(1);				//снять паузу
			}
		}
	}

	//проверяем нажатие на кнопку выход
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &Exit_area[0]))
		{
			if (pause_delay_timer <= 0)
			{
				pause_delay_timer = 200; //включаем задержку заново
				return(2);				//выход в главное меню
			}
		}
	}

	return (0);//пауза продолжается
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

	int LOOT_ITEMS_TO_GEN = 5;
	int LOOT_ITEMS_ADDED = 0;

	int EQUIP_ITEMS_TO_GEN = 50;
	int EQUIP_ITEMS_ADDED = 0;

	//цикл генерации для лута
	for (int i = 0; i < LOOT_ITEMS_TO_GEN; ++i)
	{
		double NEW_X = (rand() * 4000.0 / RAND_MAX - 2000.0);// 4000  2000
		if (NEW_X < 0) NEW_X -= 1200 * GAME_RENDER_SCALE; else NEW_X += 1200 * GAME_RENDER_SCALE;  // 1200
		double NEW_Y = (rand() * 4000.0 / RAND_MAX - 2000.0);  //4000 2000
		if (NEW_Y < 0) NEW_Y -= 800 * GAME_RENDER_SCALE; else NEW_Y += 800 * GAME_RENDER_SCALE; //800
		NEW_X += (Rover.player_coord_X - Rover.player_offset_X);
		NEW_Y += (Rover.player_coord_Y - Rover.player_offset_Y);

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
		NEW_X += (Rover.player_coord_X - Rover.player_offset_X);
		NEW_Y += (Rover.player_coord_Y - Rover.player_offset_Y);

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
				equipment_array[new_item].power = (unsigned char)floor(dist / 5000.0);
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

void Mission::draw_interface(sf::Vector2i localPosition)
{
	interface_Sprite.setScale(render_res_X / 1920.0, render_res_Y / 1080.0);
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

	//рисуем курсор в виде прицела
	mouse_pointer_2_Sprite.setPosition((float)localPosition.x - 10, (float)localPosition.y - 10);
	game_window->draw(mouse_pointer_2_Sprite);

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
	sprintf_s(buffer, "%i RUB", Rover.collected_money);
	text.setCharacterSize(40); // in pixels, not points!
	text.setScale(render_res_X / 1920.0, render_res_Y / 1080.0);
	text.setFillColor(sf::Color::Yellow);
	text.setString(buffer);
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(bounds.width, 0);
	text.setPosition(430.0 * render_res_X / 1920.0, 2.0 * render_res_Y / 1080.0);
	game_window->draw(text);
	//printf("%0.0f %0.0f\n", bounds.width, bounds.height);

	//рисуем кнопку активации
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
			if (dist > 114) dist = 114;
			dist *= render_res_X / 1920.0;
			sf::CircleShape dot(4.f);
			dot.setOrigin(2.0, 2.0);
			dot.setPosition(1760.0 * render_res_X / 1920.0 + cos(angle) * dist, 156.0 * render_res_Y / 1080.0 - sin(angle) * dist);
			if (obj_array[i].type == 0)
			{
				dot.setFillColor(sf::Color(0, 255, 0));  //база
				//printf("a = %0.1f d = %0.1f\n", angle, dist);
				//printf("type 0\n");
			}
			if (obj_array[i].type == 1)
			{
				dot.setFillColor(sf::Color(255, 0, 0));  //корабль
				//printf("type 1\n");
			}
			if (obj_array[i].type == 2)
			{
				dot.setFillColor(sf::Color(255, 0, 255));  //гнездо
				//printf("type 1\n");
			}

			game_window->draw(dot);
		}
	}
	//sf::sleep(sf::seconds(1));
}

int Mission::take_equipment()
{
	double int_scale_x = render_res_X / 1920.0;
	double int_scale_y = render_res_Y / 1080.0;
	sf::Sprite screenshot_Sprite;

	screenshot_Sprite.setTexture(screnshot_Texture);
	screenshot_Sprite.setColor(sf::Color(255, 255, 255, 50));
	game_window->clear();
	game_window->draw(screenshot_Sprite);
	//диалоговое окно
	sf::Vector2f dialog_center(render_res_X / 2.0, render_res_Y / 2.0); //позиция окна  // удалить
	interface_equip_Sprite.setOrigin(interface_equip_Sprite.getLocalBounds().width / 2.0, interface_equip_Sprite.getLocalBounds().height / 2.0);
	interface_equip_Sprite.setPosition(dialog_center);
	interface_equip_Sprite.setScale(int_scale_x, int_scale_y);
	game_window->draw(interface_equip_Sprite);

	//рисуем диалоговое окно

	//ЗАГОЛОВОК ОКНА
	text.setCharacterSize(35); // in pixels, not points!
	text.setFillColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold);
	if (equipment_array[equipment_to_take].type == 0) text.setString("Upgrade found!");
	else text.setString("Weapon found!");
	text.setOrigin(text.getLocalBounds().width / 2.0, text.getLocalBounds().height / 2.0);
	text.setScale(int_scale_x, int_scale_y);
	text.setPosition(render_res_X / 2.0, render_res_Y / 2.0 - 212.0 * int_scale_y);
	game_window->draw(text);

	//картинка
	if (equipment_array[equipment_to_take].type == 0) loot_Sprite.setTextureRect(sf::IntRect(300, 0, 100, 100));
	else loot_Sprite.setTextureRect(sf::IntRect(400, 0, 100, 100));
	loot_Sprite.setOrigin(50.0, 50.0);
	loot_Sprite.setScale(1.75 * int_scale_x, 1.75 * int_scale_y);
	loot_Sprite.setPosition(render_res_X / 2.0 - 256.0 * int_scale_x, render_res_Y / 2.0 - 70 * int_scale_y);
	game_window->draw(loot_Sprite);

	//описание
	if (equipment_array[equipment_to_take].type == 0) text.setString("Upgrade for radar \n+15 distance");
	else text.setString("Usual weapon \n damage = 5");
	text.setCharacterSize(30); // in pixels, not points!
	text.setOrigin(text.getLocalBounds().width / 2.0, text.getLocalBounds().height / 2.0);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setScale(int_scale_x, int_scale_y);
	text.setPosition(render_res_X / 2.0 + 150 * int_scale_x, render_res_Y / 2.0 - 75 * int_scale_y);
	game_window->draw(text);

	//кнопка замены
	text.setCharacterSize(40); // in pixels, not points!
	text.setFillColor(sf::Color::Green);
	text.setStyle(sf::Text::Bold);
	text.setString("Get item");
	text.setOrigin(text.getLocalBounds().width / 2.0, text.getLocalBounds().height / 2.0);
	text.setPosition(render_res_X / 2.0 - 250 * int_scale_x, render_res_Y / 2.0 + 130 * int_scale_y);
	text.setScale(int_scale_x, int_scale_y);
	game_window->draw(text);


	//кнопка разборки
	text.setCharacterSize(40); // in pixels, not points!
	text.setFillColor(sf::Color::Red);
	text.setString("Delete item");
	text.setStyle(sf::Text::Bold);
	text.setOrigin(text.getLocalBounds().width / 2.0, text.getLocalBounds().height / 2.0);
	text.setPosition(render_res_X / 2.0 + 170 * int_scale_x, render_res_Y / 2.0 + 130 * int_scale_y);
	text.setScale(int_scale_x, int_scale_y);
	game_window->draw(text);

	//рисуем курсор
	sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);
	mouse_pointer_Sprite.setPosition((float)localPosition.x, (float)localPosition.y);
	game_window->draw(mouse_pointer_Sprite);

	game_window->display();

	//проверяем нажатие кнопок
	int take_area[4] = { (int)dialog_center.y + 75 * (int)int_scale_y, (int)dialog_center.y + 208 * (int)int_scale_y, (int)dialog_center.x - 386 * (int)int_scale_x, (int)dialog_center.x - 103 * (int)int_scale_x };//top,bottom,left,right
	int dismis_area[4] = { (int)dialog_center.y + 75 * (int)int_scale_y, (int)dialog_center.y + 208 * (int)int_scale_y, (int)dialog_center.x - 60 * (int)int_scale_x, (int)dialog_center.x + 384 * (int)int_scale_x };//top,bottom,left,right

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
			bg_tile_Sprite.setTextureRect(sf::IntRect(num * 100, 0, land_tile_size + 1, land_tile_size + 1));
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

					//printf("loot eaten\n");

				}

				if (loot_array[loot_index].isValid == 1)
				{
					//рисуем лут
					int tile_offset = loot_array[loot_index].type * 100;
					loot_Sprite.setTextureRect(sf::IntRect(tile_offset, 0, 100, 100));
					loot_Sprite.setPosition(loot_array[loot_index].X / GAME_RENDER_SCALE - 50.0 / GAME_RENDER_SCALE - (Rover.player_coord_X / GAME_RENDER_SCALE - Rover.player_offset_X - render_res_X / 2), Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y + render_res_Y / 2 - loot_array[loot_index].Y / GAME_RENDER_SCALE - 50.0 / GAME_RENDER_SCALE);
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

void Mission::draw_equipment()
{
	int equipment_index = 0;
	int equipment_processed = 0;
	while (equipment_processed < equipment_count)
	{
		//проходим по массиву лута
		if (equipment_array[equipment_index].isValid == 1)
		{
			if (abs(equipment_array[equipment_index].X - Rover.player_coord_X) < render_res_X * GAME_RENDER_SCALE && abs(equipment_array[equipment_index].Y - Rover.player_coord_Y) < render_res_Y * GAME_RENDER_SCALE)
			{
				//проверяем коллизию
				if (abs(equipment_array[equipment_index].X - Rover.player_coord_X) < 100 && abs(equipment_array[equipment_index].Y - Rover.player_coord_Y) < 100)
				{
					equipment_to_take = equipment_index;
					//printf("Take %i \n", equipment_to_take);
				}
				else
				{
					//рисуем оборудование
					int tile_offset = (equipment_array[equipment_index].type + 3) * 100;
					loot_Sprite.setTextureRect(sf::IntRect(tile_offset, 0, 100, 100));
					loot_Sprite.setPosition(equipment_array[equipment_index].X / GAME_RENDER_SCALE - 50.0 / GAME_RENDER_SCALE - (Rover.player_coord_X / GAME_RENDER_SCALE - Rover.player_offset_X - render_res_X / 2), Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y + render_res_Y / 2 - equipment_array[equipment_index].Y / GAME_RENDER_SCALE - 50.0 / GAME_RENDER_SCALE);
					loot_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
					game_window->draw(loot_Sprite);
				}
			}
			equipment_processed++;
		}
		equipment_index++;
	}



}

void Mission::draw_character()
{
	//привязываем скорость анимации к прошедшему времени
	Rover.animation_time = Rover.animation_time + elapsed_ms;
	if (Rover.animation_time > Rover.animation_time_MAX) Rover.animation_time -= Rover.animation_time_MAX;

	body_tile_Sprite.setPosition(render_res_X / 2 + Rover.player_offset_X, render_res_Y / 2 - Rover.player_offset_Y);
	body_tile_Sprite.setTextureRect(sf::IntRect(round(Rover.animation_time / Rover.animation_time_MAX) * 7 * 200, (Rover.body_direction - 1) * 200, 200, 200));
	body_tile_Sprite.setOrigin(100.0, 100.0);
	body_tile_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
	game_window->draw(body_tile_Sprite);

	//рисуем пушку

	Rover.weapon_animation_time += elapsed_ms;
	if (Rover.weapon_animation_time > Rover.weapon_animation_time_MAX) Rover.weapon_animation_time -= Rover.weapon_animation_time_MAX;

	weapon_tile_Sprite.setPosition(render_res_X / 2 + Rover.player_offset_X, render_res_Y / 2 - Rover.player_offset_Y);
	weapon_tile_Sprite.setTextureRect(sf::IntRect(round(Rover.weapon_animation_time / Rover.weapon_animation_time_MAX) * 7 * 200, (Rover.weapon_direction - 1) * 200, 200, 200));
	weapon_tile_Sprite.setOrigin(100.0, 100.0);
	weapon_tile_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
	game_window->draw(weapon_tile_Sprite);


}

void Mission::controls_processing(sf::Vector2i localPosition)
{
	int player_D_x = 0;
	int player_D_y = 0;
	player_weapon_cooldown -= elapsed_ms; //охлаждаем ствол
	if (player_weapon_cooldown < 0) player_weapon_cooldown = 0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) //движение вверх
	{
		player_D_y++;//для определения направленияц
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) //движение вниз
	{
		player_D_y--;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //движение влево
	{
		player_D_x--;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) //движение вправо
	{
		player_D_x++;
	}

	//определение направления движения игрока
	{
		if (player_D_x == 0 && player_D_y == 1) Rover.body_direction = 1;
		if (player_D_x == 1 && player_D_y == 1) Rover.body_direction = 2;
		if (player_D_x == 1 && player_D_y == 0) Rover.body_direction = 3;
		if (player_D_x == 1 && player_D_y == -1) Rover.body_direction = 4;
		if (player_D_x == 0 && player_D_y == -1) Rover.body_direction = 5;
		if (player_D_x == -1 && player_D_y == -1) Rover.body_direction = 6;
		if (player_D_x == -1 && player_D_y == 0) Rover.body_direction = 7;
		if (player_D_x == -1 && player_D_y == 1) Rover.body_direction = 8;
	}

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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) //масштаб 2
	{
		GAME_RENDER_SCALE = 2;
	}


	//считаем угол между игроком и курсором
	int D_x = localPosition.x - (render_res_X / 2 + Rover.player_offset_X);
	int D_y = (render_res_Y / 2 - Rover.player_offset_Y) - localPosition.y;
	Rover.weapon_direction_rad = std::atan2((double)D_y, (double)D_x);
	if (Rover.weapon_direction_rad < 0) Rover.weapon_direction_rad += 2 * 3.1415926;

	//устанавливаем направление оружия для анимации
	if ((Rover.weapon_direction_rad >= 45 * 7.5 / 180 * 3.1415) || (Rover.weapon_direction_rad < 45 * 0.5 / 180 * 3.1415)) Rover.weapon_direction = 3;
	if ((Rover.weapon_direction_rad >= 45 * 0.5 / 180 * 3.1415) && (Rover.weapon_direction_rad < 45 * 1.5 / 180 * 3.1415)) Rover.weapon_direction = 2;
	if ((Rover.weapon_direction_rad >= 45 * 1.5 / 180 * 3.1415) && (Rover.weapon_direction_rad < 45 * 2.5 / 180 * 3.1415)) Rover.weapon_direction = 1;
	if ((Rover.weapon_direction_rad >= 45 * 2.5 / 180 * 3.1415) && (Rover.weapon_direction_rad < 45 * 3.5 / 180 * 3.1415)) Rover.weapon_direction = 8;
	if ((Rover.weapon_direction_rad >= 45 * 3.5 / 180 * 3.1415) && (Rover.weapon_direction_rad < 45 * 4.5 / 180 * 3.1415)) Rover.weapon_direction = 7;
	if ((Rover.weapon_direction_rad >= 45 * 4.5 / 180 * 3.1415) && (Rover.weapon_direction_rad < 45 * 5.5 / 180 * 3.1415)) Rover.weapon_direction = 6;
	if ((Rover.weapon_direction_rad >= 45 * 5.5 / 180 * 3.1415) && (Rover.weapon_direction_rad < 45 * 6.5 / 180 * 3.1415)) Rover.weapon_direction = 5;
	if ((Rover.weapon_direction_rad >= 45 * 6.5 / 180 * 3.1415) && (Rover.weapon_direction_rad < 45 * 7.5 / 180 * 3.1415)) Rover.weapon_direction = 4;

	//проверяем нажатие на кнопку LMB и доступность оружия
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && player_weapon_cooldown == 0)
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
				player_bullet_array[new_item].type = 1;
				player_bullet_array[new_item].power = 5;
				player_bullet_array[new_item].speed = 2000;
				player_bullet_array[new_item].power = 5;
				if (Rover.weapon_direction_rad > 3.1415926) player_bullet_array[new_item].direction = Rover.weapon_direction_rad - 2 * 3.141926;
				else player_bullet_array[new_item].direction = Rover.weapon_direction_rad;
				player_bullet_array[new_item].power = 5;
				player_bullet_array[new_item].X = Rover.player_coord_X + cos(player_bullet_array[new_item].direction) * 100.0;
				player_bullet_array[new_item].Y = Rover.player_coord_Y + sin(player_bullet_array[new_item].direction) * 100.0;
				player_bullet_array[new_item].ttl = 4000;//time to live
				player_bullet_count++;
				//рассчитываем скорость пули по осям в секунду
				//printf("fire %0.0f %0.0f \n", player_bullet_array[new_item].X, player_bullet_array[new_item].Y);
				player_weapon_cooldown = 1000.0 / Rover.fire_rate;
				sound_gun_1.play();//звук выстрела
			}
		}
	}

	//тестовая информация
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Multiply))
	{
		//printf("Monster_count = %i\n", monster_count);
		/*
		int counter = 0;
		for (int i = 0; i < 1000; ++i)
		{
			if (monster_array[i].isAlive == 1)
			{
				counter++;
				printf("n=%4i  spr=%4i  att_type=%i\n", i, monster_array[i].sprite_N, monster_array[i].attack_type);
			}
		}
		sf::sleep(sf::milliseconds(300));
		//printf("monsters_in_array=%i \n", counter);
		*/
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
		printf("Rover.fire_rate %0.1f\n", Rover.fire_rate);
		printf("Rover.slots %i\n", Rover.slots);
		printf("Rover.weapon_direction_rad %0.1f\n", Rover.weapon_direction_rad);

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
							//фиксируем попадание
							monster_array[j].health -= player_bullet_array[i].power; //уменьшаем здоровье монстра
							sound_hit_2.play();
							if (monster_array[j].health <= 0)
							{
								monster_array[j].isAlive = 0; //фиксируем смерть
								monster_count--;
								monsters_procecced--;
								//создаем спецэффект на месте монстра
								create_VFX(monster_array[j].X, monster_array[j].Y, 0, 1); // тип 0 - взрыв монстра
								//sound_death_1.play();
								Rover.collected_money += monster_array[j].power;//гонорар
							}
							player_bullet_array[i].isAlive = 0;//пуля исчезает после попадания
							player_bullet_count--;
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
					if (obj_array[obj-1].type == 0 || obj_array[obj-1].type == 1) //попадание в базу или корабль
					{
						create_VFX(player_bullet_array[i].X, player_bullet_array[i].Y, 2, 1); // тип 2 - попадание пули
						sound_hit_1.play();
					}
					if (obj_array[obj-1].type == 2) //попадание в гнездо
					{
						create_VFX(player_bullet_array[i].X, player_bullet_array[i].Y, 0, 2); // тип 0 - взрыв монстра
						sound_hit_2.play();
						obj_array[obj-1].Health -= player_bullet_array[i].power;
						//printf("%0.0f\n", obj_array[obj - 1].Health);
						if (obj_array[obj-1].Health <= 0)
						{
							//убиваем гнездо
							obj_array[obj-1].isValid = 0;
						}
					}

				}
			}

			if (player_bullet_array[i].isAlive == 1) //повторная проверка т.к. пуля могла попасть еще куда-нибудь
			{
				//рисуем спрайт пули
				player_bullet_Sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
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

	int MONSTER_TO_GEN = 3 + floor(sqrt(Rover.player_coord_X * Rover.player_coord_X + Rover.player_coord_Y * Rover.player_coord_Y) / 5000.0); // задать количество в зависимости от расстояния
	int GENERATED_MONSTER = 0;
	int min_gen_dist, ring_width;
	min_gen_dist = render_res_X * GAME_RENDER_SCALE;
	ring_width = 2000 * GAME_RENDER_SCALE; //ширина области генерации
	//printf("%i  %i", min_gen_dist, ring_width);
	double angle = 0.0, dist = 0.0;
	//цикл генерации
	for (int i = 0; i < 1000 && monster_count < 1000 && GENERATED_MONSTER < MONSTER_TO_GEN; ++i)
	{
		if (monster_array[i].isAlive == 0)
		{
			angle = (rand() / (double)RAND_MAX) * 2 * 3.1415926 - 3.1415926; //угол
			dist = min_gen_dist + (rand() / (double)RAND_MAX) * ring_width; //расстояние до монстра
			double NEW_X = cos(angle) * dist + (Rover.player_coord_X - Rover.player_offset_X);
			double NEW_Y = sin(angle) * dist + (Rover.player_coord_Y - Rover.player_offset_Y);

			//добавляем данные в массив монстров
			monster_array[i].isAlive = 1;
			monster_array[i].isJugger = ((rand() * 1.0 / (RAND_MAX + 1.0)) > 0.97) ? 1 : 0; //джагернаут с вероятностью 10%
			monster_array[i].level = floor(11.0 * rand() / (RAND_MAX + 1.0));
			monster_array[i].sprite_N = floor(33.0 * rand() / (RAND_MAX + 1.0));
			if (monster_array[i].sprite_N == 32) { monster_array[i].scale = 1.5; }
			else monster_array[i].scale = 1;
			if (monster_array[i].isJugger == 1) monster_array[i].scale *= 2;
			monster_array[i].power = monster_array[i].level * (monster_array[i].isJugger + 1);
			monster_array[i].moving_speed = 100 + rand() * 100.0 / (double)RAND_MAX;
			monster_array[i].anim_time_MAX = monster_array[i].moving_speed * 4.0;
			monster_array[i].direction = atan2(Rover.player_coord_Y - NEW_Y, Rover.player_coord_X - NEW_X);
			int weapon;
			double weapon_choice = rand() * 1.0 / (double)RAND_MAX;//выбор оружия
			if (weapon_choice < 0.6) weapon = 0;
			if (weapon_choice >= 0.6 && weapon_choice < 0.9) weapon = 1;
			if (weapon_choice >= 0.9) weapon = 2;

			//printf("%i\n", weapon);
			if (weapon == 0)
			{
				//рукопашка
				monster_array[i].attack_type = 0; // 0 - рукопашка, 1 - дальняя, 2 - по площади
				monster_array[i].attack_time = 1000.0;
				monster_array[i].attack_cooldown = 0.0;
				monster_array[i].attack_distance = 120.0;
			}
			if (weapon == 1)
			{
				//атака на дистанции
				monster_array[i].attack_type = 1; // 0 - рукопашка, 1 - дальняя, 2 - по площади
				monster_array[i].attack_time = 1500.0;
				monster_array[i].attack_cooldown = 0.0;
				monster_array[i].attack_distance = 800.0;
			}

			if (weapon == 2)
			{
				//атака на дистанции
				monster_array[i].attack_type = 2; // 0 - рукопашка, 1 - дальняя, 2 - по площади
				monster_array[i].attack_time = 2000.0;
				monster_array[i].attack_cooldown = 0.0;
				monster_array[i].attack_distance = 1200.0;
			}

			monster_array[i].health = (3 + (int)floor(rand() * 3.0 / (RAND_MAX + 1.0))) * (monster_array[i].isJugger * 10 + 1);
			monster_array[i].X = NEW_X;
			monster_array[i].Y = NEW_Y;

			//printf("monster added #%5i = %5i   %5i   (%0f, %0f)   %1.2f\n", i, monster_array[i].sprite_N, monster_array[i].power, NEW_X, NEW_Y, monster_array[i].direction);
			//printf("X=%5.0f Y=%5.0f ATAN2(%5.0f,%5.0f) = %1.3f", NEW_X, NEW_Y, Rover.player_coord_X - NEW_X, Rover.player_coord_Y - NEW_Y, monster_array[i].direction);
			monster_count++;
			GENERATED_MONSTER++;
		}
		else //если монстр жив, проверяем минимальное расстояние до игрока по X Y
		{
			if (abs(monster_array[i].X - Rover.player_coord_X) > render_res_X * GAME_RENDER_SCALE * 3 || abs(monster_array[i].Y - Rover.player_coord_Y) > render_res_Y * GAME_RENDER_SCALE * 3)
			{
				monster_array[i].isAlive = 0; //убиваем монстра если он далеко
				monster_count--;
				//printf("monster died, distance = %0f\n", sqrt((monster_array[i].X - Rover.player_coord_X) * (monster_array[i].X - Rover.player_coord_X) + (monster_array[i].Y - Rover.player_coord_Y)* (monster_array[i].Y - Rover.player_coord_Y)));
			}
		}
	}
	//printf("monsters added = %i\n", GENERATED_MONSTER);
}

void Mission::draw_monsters()
{
	double PI = 3.14159265358979323846;
	//проходимся по массиву монстров и рисуем спрайты
	//также нужно проверить коллизии и отрисовать анимацию
	for (int i = 0; i < 1000; ++i)
	{

		if (monster_array[i].isAlive == 1) // проверяем жив ли монстр
		{
			//рассчитываем расстояние до игрока
			double dist_to_player = sqrt((Rover.player_coord_X - monster_array[i].X) * (Rover.player_coord_X - monster_array[i].X) + (Rover.player_coord_Y - monster_array[i].Y) * (Rover.player_coord_Y - monster_array[i].Y));

			//меняем направление к игроку
			monster_array[i].direction = atan2(Rover.player_coord_Y - monster_array[i].Y, Rover.player_coord_X - monster_array[i].X);

			//если монстр слишком близко - разворот
			if (dist_to_player < monster_array[i].attack_distance * 0.8f)
			{
				monster_array[i].direction += PI;
				monster_array[i].attack_cooldown = monster_array[i].attack_time;//запрет атаки при бегстве
			}

			//рассчитываем движение
			if ((dist_to_player < monster_array[i].attack_distance * 0.8f || dist_to_player > monster_array[i].attack_distance * 0.9f) && (dist_to_player < render_res_X * GAME_RENDER_SCALE)) //если монстр далеко - движения нет
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
					if (check_obj_collision(new_X, new_Y, 80.0f) == 0)
					{
						monster_array[i].Y = new_Y;
						monster_array[i].X = new_X;
						break;
					}
					else monster_array[i].moving_speed *= 1.00f; //замедляемся при столкновении

					pendulum *= -1;
				}
			}

			//если монстр вблизи - рисуем его
			if (abs(monster_array[i].X - Rover.player_coord_X) < render_res_X * GAME_RENDER_SCALE && abs(monster_array[i].Y - Rover.player_coord_Y) < render_res_Y * GAME_RENDER_SCALE)
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

				game_window->draw(monster_Sprite);

				monster_array[i].attack_cooldown -= elapsed_ms;

				double dist_to_player = sqrt((Rover.player_coord_X - monster_array[i].X) * (Rover.player_coord_X - monster_array[i].X) + (Rover.player_coord_Y - monster_array[i].Y) * (Rover.player_coord_Y - monster_array[i].Y));
				if (monster_array[i].attack_cooldown <= 0 && dist_to_player < monster_array[i].attack_distance)
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
							monster_bullet_array[j].type = monster_array[j].attack_type;
							monster_bullet_array[j].sprite_N = 0;//дефолтный уровень
							if (monster_array[i].attack_type == 1) monster_bullet_array[j].sprite_N = 1;
							if (monster_array[i].attack_type == 2) monster_bullet_array[j].sprite_N = 2;
							if (monster_array[i].attack_type == 3) monster_bullet_array[j].sprite_N = 3;
							monster_bullet_array[j].power = monster_array[i].power;
							//скорость и время по-умолчанию (рукопашка)
							monster_bullet_array[j].speed = 500.0;
							monster_bullet_array[j].ttl = 300; //оставшееся время жизни
							if (monster_array[j].attack_type == 1)
							{
								monster_bullet_array[j].speed = 800.0;
								monster_bullet_array[j].ttl = 5000; //оставшееся время жизни
							}
							if (monster_array[j].attack_type == 2) //граната
							{
								monster_bullet_array[j].speed = 400.0;
								monster_bullet_array[j].ttl = 3000; //оставшееся время жизни
							}

							if (monster_array[j].attack_type == 3) //граната
							{
								monster_bullet_array[j].speed = 300.0;
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
		}
	}
}

void Mission::create_VFX(double VFX_X, double VFX_Y, int VFX_type, double scale)
{
	// 0 - взрыв монстра, 1 - укус, 2 - взрыв снаряда, 3 - взрыв гранаты 
	//поиск свободного слота	
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
		VFX_array[i].remaining_time = 500;
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
		VFX_array[i].remaining_time = 500;
		VFX_array[i].remains_num = 0;//после анимации оставить останки
		VFX_array[i].scale = scale;
		VFX_array[i].X = VFX_X;
		VFX_array[i].Y = VFX_Y;
		//printf("create VFX = 1\n");
	}

	if (VFX_type == 2) //взрыв снаряда
	{
		//добавляем строку в массив
		VFX_array[i].isAlive = 1; // 0 - свободный слот, 1 - анимация
		VFX_array[i].number = 2;//номер в атласе. размер у всех 100*100
		VFX_array[i].last_frame = 16;
		VFX_array[i].keep_alive = 0; // 0 или 1 - убрать
		VFX_array[i].total_time = 500; //в микросекундах
		VFX_array[i].remaining_time = 500;
		VFX_array[i].remains_num = 0;//после анимации оставить останки
		VFX_array[i].scale = scale;
		VFX_array[i].X = VFX_X;
		VFX_array[i].Y = VFX_Y;
		//printf("create VFX = 2\n");
	}

	if (VFX_type == 3) //взрыв гранаты
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

void Mission::draw_VFX_remains()
{
	for (int i = 0; i < 1000; ++i)
	{
		if (VFX_remains_array[i].isAlive == 1)
		{
			//проверяем координаты
			double sprite_X = render_res_X / 2.0 + Rover.player_offset_X - ((Rover.player_coord_X - VFX_remains_array[i].X) / GAME_RENDER_SCALE);
			double sprite_Y = render_res_Y / 2.0 - Rover.player_offset_Y + ((Rover.player_coord_Y - VFX_remains_array[i].Y) / GAME_RENDER_SCALE);
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

void Mission::create_VFX_remains(double VFX_X, double VFX_Y, int VFX_type)
{
	VFX_remains_array[VFX_remains_pointer].isAlive = 1;
	VFX_remains_array[VFX_remains_pointer].number = VFX_type;
	VFX_remains_array[VFX_remains_pointer].X = VFX_X;
	VFX_remains_array[VFX_remains_pointer].Y = VFX_Y;
	VFX_remains_pointer++;
	if (VFX_remains_pointer == 1000) VFX_remains_pointer = 0;
}

void Mission::draw_monster_bullets()
{
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
			if (monster_bullet_array[i].ttl <= 0)
			{
				monster_bullet_array[i].isAlive = 0;//удаляем пулю из массива
				//если это граната, создаем взрыв
				if (monster_bullet_array[i].type == 2) make_explosion(monster_bullet_array[i].X, monster_bullet_array[i].Y, monster_bullet_array[i].power, 1, 0);
				continue;
			}

			//проверка попадания в объекты
			if (monster_bullet_array[i].isAlive == 1) //повторная проверка т.к. пуля могла попасть в монстра
			{
				if (check_obj_collision(monster_bullet_array[i].X, monster_bullet_array[i].Y, -20.0f))
				{
					//фиксируем попадание
					monster_bullet_array[i].isAlive = 0;//пуля исчезает после попадания
					monster_bullet_count--;
					create_VFX(monster_bullet_array[i].X, monster_bullet_array[i].Y, 2, 1); // тип 0 - взрыв монстра
					sound_hit_1.play();
				}

			}


			if (sprite_X > 0 && sprite_X < render_res_X && sprite_Y > 0 && sprite_Y < render_res_Y && monster_bullet_array[i].isAlive == 1)
			{
				//рисуем спрайт
				monster_bullet_Sprite.setTextureRect(sf::IntRect(0, (monster_bullet_array[i].type) * 100, 100, 100));
				monster_bullet_Sprite.setOrigin(50, 50);
				monster_bullet_Sprite.setPosition(sprite_X, sprite_Y);
				monster_bullet_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
				monster_bullet_Sprite.setColor(sf::Color(255, 255, 255, 255));
				monster_bullet_Sprite.setRotation(-monster_bullet_array[i].direction * 180.0 / 3.1415926);
				game_window->draw(monster_bullet_Sprite);

				//проверяем на попадание
				if (abs(monster_bullet_array[i].X - Rover.player_coord_X) < 50 && abs(monster_bullet_array[i].Y - Rover.player_coord_Y) < 50)
				{
					//фиксируем попадание
					Rover.Health -= monster_bullet_array[i].power; //уменьшаем здоровье игрока
					//printf("health=%0.0f", Rover.Health);
					if (Rover.Health <= 0)
					{
						//printf("Player died\n");
						//создаем спецэффект смерти на месте игрока
						create_VFX(Rover.player_coord_X, Rover.player_coord_Y, 3, 4); // тип 2 - взрыв
						sound_explode_1.play();
					}
					monster_bullet_array[i].isAlive = 0;//пуля исчезает после попадания
					monster_bullet_count--;
					//создаем спецэффект месте попадания
					if (monster_bullet_array[i].type == 0)
					{
						create_VFX(monster_bullet_array[i].X, monster_bullet_array[i].Y, 1, 1); // тип 1 - укус
						sound_bite_1.play();
					}
					if (monster_bullet_array[i].type == 1)
					{
						create_VFX(monster_bullet_array[i].X, monster_bullet_array[i].Y, 2, 1); // тип 2 - взрыв снаряда
						sound_hit_1.play();
					}
					if (monster_bullet_array[i].type == 2) make_explosion(monster_bullet_array[i].X, monster_bullet_array[i].Y, monster_bullet_array[i].power, 1, 0);

				}

			}
		}
	}

}

void Mission::make_explosion(double X, double Y, double power, bool player_damage, bool monster_damage)
{
	//monster_damage = 1;
	double radius = 200.0;//радиус взрыва
	create_VFX(X, Y, 3, 4);
	sound_explode_1.play();

	//расчет попадания в игрока
	if (player_damage)
	{
		double distance = sqrt((Rover.player_coord_X - X) * (Rover.player_coord_X - X) + (Rover.player_coord_Y - Y) * (Rover.player_coord_Y - Y));
		if (distance < radius)
		{
			//рассчитываем ущерб игроку
			Rover.Health -= power * (1.0 - distance / radius); //пропорционально расстоянию
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
				double distance = sqrt((monster_array[i].X - X) * (monster_array[i].X - X) + (monster_array[i].Y - Y) * (monster_array[i].Y - Y));
				if (distance < radius)
				{
					//рассчитываем ущерб монстру
					monster_array[i].health -= power * (1.0 - distance / radius); //пропорционально расстоянию
					//printf("expl damage = %0.0f\n", power * (1.0 - distance / radius));
					if (monster_array[i].health <= 0)
					{
						monster_array[i].isAlive = 0; //фиксируем смерть
						monster_count--;
						//создаем спецэффект на месте монстра
						create_VFX(monster_array[i].X, monster_array[i].Y, 0, 1); // тип 0 - взрыв монстра
						Rover.collected_money += monster_array[i].power;
					}
				}
			}
		}
	}



}

void Mission::make_stationary_obj()
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
		obj_array[i].X = cos(i * 2 * 3.1415 / 8) * 10000.0;
		obj_array[i].Y = sin(i * 2 * 3.1415 / 8) * 10000.0;
		obj_array[i].size_X = 318;
		obj_array[i].size_Y = 582;
	}

	//создаем гнезда выхода 1-8
	for (int i = 9; i < 17; ++i)
	{
		obj_array[i].isValid = 1;
		obj_array[i].type = 2; //тип гнездо
		obj_array[i].X = cos(i * 2 * 3.1415 / 8 + 0.1) * 6000.0;
		obj_array[i].Y = sin(i * 2 * 3.1415 / 8 + 0.1) * 6000.0;
		obj_array[i].size_X = 200;
		obj_array[i].size_Y = 200;
		obj_array[i].Health = 100;
	}


}

void Mission::draw_stationary_obj()
{
	//рисуем стационарные объекты

	for (int i = 0; i < 100; ++i)
	{
		if (obj_array[i].isValid == 1)
		{
			if (obj_array[i].type == 0)
			{
				base_Sprite.setOrigin(obj_array[i].size_X / 2.0, obj_array[i].size_Y / 2.0);
				base_Sprite.setPosition(render_res_X / 2 + Rover.player_offset_X - Rover.player_coord_X / GAME_RENDER_SCALE + obj_array[i].X / GAME_RENDER_SCALE, render_res_Y / 2 + Rover.player_coord_Y / GAME_RENDER_SCALE - Rover.player_offset_Y - obj_array[i].Y / GAME_RENDER_SCALE);
				base_Sprite.setScale(sf::Vector2f(1.0 / GAME_RENDER_SCALE, 1.0 / GAME_RENDER_SCALE));
				game_window->draw(base_Sprite);
			}

			if (obj_array[i].type == 1)
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


	if (acceleration == 1)
	{
		//ускорение путем сложения векторов
		sf::Vector2f accel_vector; //вектор ускорения
		accel_vector.x = Rover.Speed_MAX * cos(direction) * 0.05f;
		accel_vector.y = Rover.Speed_MAX * sin(direction) * 0.05f;
		moving_vector += accel_vector;
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

int Mission::evacuate()
{
	double int_scale_x = render_res_X / 1920.0;
	double int_scale_y = render_res_Y / 1080.0;
	sf::Sprite screenshot_Sprite;

	screenshot_Sprite.setTexture(screnshot_Texture);
	screenshot_Sprite.setColor(sf::Color(255, 255, 255, 50));
	game_window->clear();
	game_window->draw(screenshot_Sprite);

	//диалоговое окно
	sf::Vector2f dialog_center(render_res_X / 2.0, render_res_Y / 2.0); //позиция окна  // удалить
	evac_dialog_Sprite.setOrigin(evac_dialog_Sprite.getLocalBounds().width / 2.0, evac_dialog_Sprite.getLocalBounds().height / 2.0);
	evac_dialog_Sprite.setPosition(dialog_center);
	evac_dialog_Sprite.setScale(int_scale_x, int_scale_y);
	game_window->draw(evac_dialog_Sprite);

	//рисуем курсор
	sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);
	mouse_pointer_Sprite.setPosition((float)localPosition.x, (float)localPosition.y);
	game_window->draw(mouse_pointer_Sprite);
	game_window->display();

	//проверяем нажатие кнопок
	int take_weapon_area[4] = { render_res_Y / 2 + 255 * int_scale_y, render_res_Y / 2 + 297 * int_scale_y, render_res_X / 2 - 406 * int_scale_x, render_res_X / 2 - 111 * int_scale_x };//top,bottom,left,right
	int take_res_area[4] = { render_res_Y / 2 + 255 * int_scale_y, render_res_Y / 2 + 297 * int_scale_y, render_res_X / 2 + 95 * int_scale_x, render_res_X / 2 + 391 * int_scale_x };//top,bottom,left,right


	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//printf("%i  %i \n", localPosition.x, localPosition.y);

		// проверяем нажатие на кнопки
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &take_weapon_area[0]))
		{
			printf("weapon_taken_OK \n");
			music_action.stop();
			return (1);//убираем диалог и продолжаем игру дальше
		}

		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &take_res_area[0]))
		{
			printf("resourse_taken_OK \n");
			music_action.stop();
			return (1);//убираем диалог и продолжаем игру дальше
		}
	}

	//sf::sleep(sf::milliseconds(1000));
	return (0);//продолжаем выполнение подпрограммы

}

int Mission::in_base()
{
	double int_scale_x = render_res_X / 1920.0;
	double int_scale_y = render_res_Y / 1080.0;
	sf::Sprite screenshot_Sprite;

	screenshot_Sprite.setTexture(screnshot_Texture);
	screenshot_Sprite.setColor(sf::Color(255, 255, 255, 50));
	game_window->clear();
	game_window->draw(screenshot_Sprite);

	//диалоговое окно
	sf::Vector2f dialog_center(render_res_X / 2.0, render_res_Y / 2.0); //позиция окна  // удалить
	base_dialog_Sprite.setOrigin(base_dialog_Sprite.getLocalBounds().width / 2.0, base_dialog_Sprite.getLocalBounds().height / 2.0);
	base_dialog_Sprite.setPosition(dialog_center);
	base_dialog_Sprite.setScale(int_scale_x, int_scale_y);
	game_window->draw(base_dialog_Sprite);

	//рисуем курсор
	sf::Vector2i localPosition = sf::Mouse::getPosition(*game_window);
	mouse_pointer_Sprite.setPosition((float)localPosition.x, (float)localPosition.y);
	game_window->draw(mouse_pointer_Sprite);
	game_window->display();

	//проверяем нажатие кнопок
	int RUN_area[4] = { render_res_Y / 2 + 333 * int_scale_y, render_res_Y / 2 + 381 * int_scale_y, render_res_X / 2 - 180 * int_scale_x, render_res_X / 2 + 141 * int_scale_x };//top,bottom,left,right
	//int take_res_area[4] = { render_res_Y / 2 + 255 * int_scale_y, render_res_Y / 2 + 297 * int_scale_y, render_res_X / 2 + 95 * int_scale_x, render_res_X / 2 + 391 * int_scale_x };//top,bottom,left,right

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		//printf("%i  %i \n", localPosition.x, localPosition.y);

		// проверяем нажатие на кнопки
		if (check_mouse_hover((int)localPosition.x, (int)localPosition.y, &RUN_area[0]))
		{
			printf("RUN \n");
			return (1);//убираем диалог и продолжаем игру дальше
		}
	}
	//sf::sleep(sf::milliseconds(1000));
	return (0);//продолжаем выполнение подпрограммы
}

int Mission::player_death()
{
	double int_scale_x = render_res_X / 1920.0;
	double int_scale_y = render_res_Y / 1080.0;
	sf::Sprite screenshot_Sprite;

	screenshot_Sprite.setTexture(screnshot_Texture);
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
	//обнуление параметров персонажа
	Rover.Health = Rover.Health_MAX;
//  Rover.Health_MAX = 100.0;
	Rover.Energy = Rover.Energy_MAX;
//	Rover.Energy_MAX = 100.0;
	Rover.Speed = 0.0;
//	Rover.Speed_MAX = 400.0;
	Rover.Speed_direction = 0.0;
	Rover.gun_damage = 1.0;
//	Rover.fire_rate = 4.0;
//	Rover.slots = 0;
	Rover.player_coord_X = 0.0;
	Rover.player_coord_Y = 400.0;
	Rover.player_offset_X = 0.0;
	Rover.player_offset_Y = 0.0;
	Rover.body_direction = 3;
	Rover.weapon_direction = 3;
	Rover.weapon_direction_rad = 0;
	Rover.animation_time = 0;		//тайминги анимации в микросекундах
	Rover.animation_time_MAX = 1000;
	Rover.weapon_animation_time = 0;		//тайминги анимации в микросекундах (оружие)
	Rover.weapon_animation_time_MAX = 1000;
	Rover.collected_money = 0;
	Rover.collected_metal = 0;
	Rover.collected_crystal = 0;
	Rover.collected_uran = 0;
}

void Mission::reset_world()
{
	//перезапуск мира

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

	//обнуляем стационарные объекты
	for (int i = 0; i < 100; ++i)
	{
		obj_array[i].isValid = 0;
		obj_array[i].type = 0;
		obj_array[i].size_X = 0.0;
		obj_array[i].size_Y = 0.0;
		obj_array[i].X = 0.0;
		obj_array[i].Y = 0.0;
	}

	make_stationary_obj();//создаем ключевые объекты

	//обнуляем оружие и апгрейды
	for (int i = 0; i < 1000; ++i)
	{
		equipment_array[i].isValid = 0;
		equipment_array[i].type = 0;
		equipment_array[i].power = 0;
		equipment_array[i].X = 0.0;
		equipment_array[i].Y = 0.0;
	}
	equipment_count = 0;
	equipment_to_take = -1;

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
	}
	VFX_remains_pointer = 0;

}

int Mission::get_collected_money() { return(Rover.collected_money + base_money); }
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
	Rover.Speed_MAX = 300.0 * pow(1.0185f, rover_upgrades[3]);
	//Rover.gun_damage = 1.0;
	Rover.fire_rate = 4.0 * pow(1.0185, rover_upgrades[4]);
	Rover.slots = rover_upgrades[5];
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
	printf("params = %i %i %0.0f %0.0f %0.0f %0.0f %0.0f %i \n", planet_type, planet_level, Rover.Health_MAX,Rover.Energy_MAX,Rover.Generate_speed,Rover.Speed_MAX,Rover.fire_rate,Rover.slots);
}

sf::Texture* Mission::getScreenshot() { return(&screnshot_Texture); }

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