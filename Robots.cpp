// Robots.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <Windows.h>
#include <iostream>
#include "Robots.h"
#include <fstream>
#include <string>
#include "optionparser.h"
#include <conio.h>
#include <queue>

#define WIDTH  20
//#define HEIGHT 10

#define EMPTY '0'
#define ROCK  '1'
#define BOMB  '2'
#define APPLE '3'
#define ROBOTS '4'

#define UP        0
#define DOWN      1
#define LEFT      2
#define RIGHT     3
#define SCAN      4
#define GRAB      5
#define GRAB_ALL  6

#define MANUAL_MODE 0
#define SCAN_MODE   1
#define AUTO_MODE   2



/*struct Sapper
{
	int x;
	int y;
	char ceil;
};*/
 

int sTOi(std::string str) {
	int res = 0;

	for (unsigned int i = 0; i < str.length(); i++) {
		if (str[i] == '\n')
			break;
		else
			res = res * 10 + (int)(str[i] - '0');

	}
	return res;
}
Game::Game()
{
	run = true;
	mode = MANUAL_MODE;
	
}

int Game::Execute(std::string cheme)
{
//инициализация класса карты, робота
	//gamemap gmap;
	//Robot scout;
	run = gmap.fillmap(cheme);
	scout.scan(gmap);
	showMap(gmap);
	screen.pos_str(0, 20, "Robots,  WELCOME:  ");
	screen.pos_str(0, 23, "(u/d/l/r/grab/scan/set_mode scanN/auto/manual) ");
	screen.pos_str(0, 22, "MODE MANUAL");
	while (run) {
		int step = 0;
		std::string input;
		int comand = 100;
		screen.pos(19, 20);
		screen.cursor_show(true);
		std::getline(std::cin, input);
		screen.cursor_show(false);
		screen.pos(19, 20);
		if (mode == MANUAL_MODE) {
			if (input == "u" || input == "U")
				comand = UP;
			else if (input == "d" || input == "D")
				comand = DOWN;
			else if (input == "l" || input == "L")
				comand = LEFT;
			else if (input == "r" || input == "R")
				comand = RIGHT;
			else if (input == "scan" || input == "Scan" || input == "SCAN" || input == "s")
				comand = SCAN;
			else if (input == "grab" || input == "Grab" || input == "GRAB" || input == "g")
				comand = GRAB;
			else if (input.substr(0, 8) == "set_mode" || input.substr(0, 8) == "Set_mode" || input.substr(0, 8) == "SET_MODE") {
				if (input.substr(9, 4) == "scan" || input.substr(9, 4) == "Scan" || input.substr(9, 4) == "SCAN") {
					mode = SCAN_MODE;
					step = sTOi(input.substr(13));
					screen.pos_str(0, 22, "MODE SCAN  ");
				}
				if (input.substr(9, 4) == "auto" || input.substr(9, 4) == "Auto" || input.substr(9, 4) == "AUTO") {
					comand = GRAB_ALL;
					mode = AUTO_MODE;
					screen.pos_str(0, 22, "MODE AUTO  ");
				}

			}
			else if (input == "SAPPER ON" || input == "sapper on" || input == "on") {

				//==поместим сапера на случайную клетку клетку
				bool findplace = false;

				Sapper rbt;
				while (!findplace) {
					int x = rand() % scout.cursize;
					int y = rand() % scout.cursize;

					int k = 0;
					std::pair<int, int> coord = { x, y };

					if ((scout.openmap[x][y] == 1) && (gmap.mp[x][y] != ROCK)
						&& (gmap.mp[x][y] != ROBOTS)) { //если на клетке стоит разведчик
						if (scout.cur == coord) { //сдвинем разведчика
							while (k == 0) {
								std::vector<std::pair<int, int>> moves{ //возможное движение по клеткам
											{-1, 0}, {0, -1}, {+1, 0}, {0, +1},

								};

								for (auto move : moves) {
									int dx = move.first, dy = move.second;
									k++;
									if (scout.can_visited(gmap, { scout.cur.first + dx, scout.cur.second + dy })) {
										scout.cur = { scout.cur.first + dx, scout.cur.second + dy };
										break;
									}
								}
							}

						}
						findplace = true;
						//==если попали на клетку с бомбой-разминируем
						if (gmap.mp[coord.first][coord.second] == BOMB) {
							gmap.mp[coord.first][coord.second] = EMPTY;
							scout.bombs.erase(coord);
							rbt.y = coord.second;
							rbt.x = coord.first;


						}
						rbt.ceil = gmap.mp[coord.first][coord.second];
						gmap.mp[coord.first][coord.second] = ROBOTS;
						rbt.y = coord.second;
						rbt.x = coord.first;
						showMap(gmap);

					}

				}
				marswalker.push_back(rbt);



			}
			else if (input == "SAPPER OFF" || input == "sapper off" || input == "off") {

				if (!marswalker.empty()) {
					auto pt = marswalker.back();
					std::pair<int, int> curpos = { pt.x, pt.y };
					gmap.mp[curpos.first][curpos.second] = pt.ceil;
					marswalker.pop_back();
					showMap(gmap);

				}

			}
			else if (input == "q") run = false;
		}
		else if (mode == SCAN_MODE) {
			if (input.substr(0, 4) == "scan" || input.substr(0, 4) == "Scan" || input.substr(0, 4) == "SCAN") {
				step = sTOi(input.substr(4));
				comand = SCAN;
			}
			if (input.substr(0, 8) == "set_mode" || input.substr(0, 8) == "Set_mode" || input.substr(0, 8) == "SET_MODE") {
				if (input.substr(9, 6) == "manual" || input.substr(9, 6) == "Manual" || input.substr(9, 6) == "MANUAL") {
					mode = MANUAL_MODE;
					screen.pos_str(0, 22, "MODE MANUAL");
				}
				if (input.substr(9, 4) == "auto" || input.substr(9, 4) == "Auto" || input.substr(9, 4) == "AUTO") {
					comand = GRAB_ALL;
					mode = AUTO_MODE;
					screen.pos_str(0, 22, "MODE AUTO  ");
				}
			}
			if (input == "q") run = false;

		}
		else if (mode == AUTO_MODE) {
			if (input.substr(0, 4) == "auto" || input.substr(0, 4) == "Auto" || input.substr(0, 4) == "AUTO") {
				comand = GRAB_ALL;
				screen.pos_str(0, 22, "MODE AUTO  ");
			}
			if (input.substr(0, 8) == "set_mode" || input.substr(0, 8) == "Set_mode" || input.substr(0, 8) == "SET_MODE") {
				if (input.substr(9, 6) == "manual" || input.substr(9, 6) == "Manual" || input.substr(9, 6) == "MANUAL") {
					mode = MANUAL_MODE;
					screen.pos_str(0, 22, "MODE MANUAL");
				}
				if (input.substr(9, 4) == "scan" || input.substr(9, 4) == "Scan" || input.substr(9, 4) == "SCAN") {
					mode = SCAN_MODE;
					screen.pos_str(0, 22, "MODE SCAN  ");
					comand = SCAN;
					step = sTOi(input.substr(13));

				}

			}
			if (input == "q") run = false;
		}


		if (mode == MANUAL_MODE) {


			if (comand == LEFT || comand == RIGHT || comand == UP || comand == DOWN) {
				std::pair<int, int> coord1 = scout.cur;
				int res = scout.makestep(comand, gmap);
				if (res == 2) {
					run = false;  //подорвался
					//нарисуем вспышку 
					screen.pos(scout.cur.second, scout.cur.first, '*');
					screen.pos_str(0, 19, "You are lost Robot-scout                                         ");
					scout.iamLive = false;
					scout.cur = { 0,0 };
				}

				//отрисовка 2 позиций- прежней и текущей
				showCeil(coord1, scout.cur);
			}
			else if (comand == SCAN) {				
				scout.scan(gmap);
				showMap(gmap);
			}
			else if (comand == GRAB) {
				scout.grab(gmap);
				screen.pos_str(0, 21, "                                                        ");
				screen.pos(0, 21);
				printf("Grabed Apples: %d/%d ", scout.appleGrab, scout.appleCount);
			}

			
		}

		if (mode == SCAN_MODE) {
			bool doscan = true;
			while (step > 0 && doscan) {
				if (scout.newMap > 0) {
					scout.updateMap();
					scout.newMap = 0;
				}
				doscan = scout.choosepoint(scout.cur);
				int res = pathfind(step, scout);
				if (res == 2) {
					run = false;  //подорвался
					scout.iamLive = false;
					//нарисуем вспышку
					screen.pos(scout.cur.second, scout.cur.first, '*');
					screen.pos_str(0, 19, "You are lost Robot-scout                                         ");
					step = 0;
					scout.cur = { 0,0 };
				}
				if (res == 1) { //тупик
					while (step > 0 && res == 1) {
						doscan = scout.choosepoint(scout.stop);
						if (scout.stop.first == 0 && scout.stop.second == 0) { //нет шагов
							step = 0;
							break;
						}
						else
							res = pathfind(step, scout);

					}
					if (res == 2) {
						run = false;  //подорвался
						//нарисуем вспышку
						screen.pos(scout.cur.second, scout.cur.first, '*');
						screen.pos_str(0, 19, "You are lost Robot-scout                                         ");
						step = 0;
						scout.cur = { 0,0 };
						scout.iamLive = false;
					}
				}
			}

		}

		if (mode == AUTO_MODE) { //сбор доступных ресурсов
			//пока есть яблоки для сбора
			//обнулим вектор координат до конечной точки
			while (!scout.apples.empty() || !scout.bombs.empty()) { //пока есть что собирать
				//==============очистим путь обхода
				if (scout.apples.empty()) {
					if ( marswalker.empty())
						break;
				}

				if (!scout.path.empty()) {
					scout.path.clear();
				}
				unsigned int len = 0;
				Robot r[5];
				
				//возьмем конечную точку
				while (scout.apples.size() > 0) {
					//auto it = scout.apples.lower_bound(scout.cur);
					auto it = scout.apples.begin();
					
					scout.stop = *it;//позиция яблока
					int res = scout.getpath(gmap);
					if (res == 1) {  //нашли яблоко
						scout.grab(gmap);
						
					}
					else break;
				}
				//позиции бомб смотрим у глав.робота.
				while ((scout.bombs.size() > 0) and !marswalker.empty()) {
					if (len >= marswalker.size()) break;
					for (auto sapIt = marswalker.begin(); sapIt != marswalker.end(); sapIt++) {
						//===ОБНОВИТЬ КАРТУ И ДАННЫЕ САППЕРОВ
						if (len < 5) {


							r[len].iamScout = false;
							r[len].cur.first = (*sapIt).x;
							r[len].cur.second = (*sapIt).y;
							r[len].getmap(scout);
							r[len].hideCeil = (*sapIt).ceil;
							//если попал на бомбу
							if (gmap.mp[r[len].cur.first][r[len].cur.second] == BOMB) {
								scout.openmap[r[len].cur.first][r[len].cur.second] = 1;
								marswalker[len].ceil = EMPTY;
								r[len].hideCeil = EMPTY;
								scout.bombs.erase(r[len].cur);
								scout.newMap = 1;
							}

							auto it = scout.bombs.begin();
							if ((len < scout.bombs.size()) && len > 0 )
							for (unsigned int i = 1; i <=len; i++)
								it++;

							r[len].stop = *it; //позиция бомбы
							int res = r[len].getpath(gmap);

							len++;

						}

						if (scout.bombs.size() == 0) //кончились бомбы
							break;

					}
					if (len > 4) break;

				}
				//=========Делаем шаг просто шаг

				bool scChange = true, spChange = true;
				bool doExecute = (!scout.apples.empty() || !scout.bombs.empty());
				int res = 0, res1 = 0;
				while (doExecute) {
					if (scout.cur == scout.stop) { //яблоки
						scout.grab(gmap);
						scChange = true;
					}
					else {
						res = scout.go2Neibor(gmap);
						scChange = false;
					}
					//==бомбы
					unsigned int i;

					for ( i = 0; i < len; i++) {
						
						if (r[i].cur == r[i].stop) {
							//if (gmap.mp[r[i].stop.first][r[i].stop.second] == BOMB) {
								r[i].demining(gmap);
								scout.openmap[r[i].stop.first][r[i].stop.second] = 3;
								marswalker[i].x = r[i].stop.first;
								marswalker[i].y = r[i].stop.second;
								marswalker[i].ceil = EMPTY;
								scout.bombs.erase(r[i].cur);
								scout.newMap = 1;
							//}
							spChange = true;
							
						}
						else if (gmap.mp[r[i].cur.first][r[i].cur.second] == BOMB) {
							scout.openmap[r[i].cur.first][r[i].cur.second] = 3;
							marswalker[i].x = r[i].cur.first;
							marswalker[i].y = r[i].cur.second;
							marswalker[i].ceil = EMPTY;
							scout.bombs.erase(r[i].cur);
							spChange = true;
							scout.newMap = 1;
						}
						else {
							res1 = r[i].go2Neibor(gmap);
							marswalker[i].x = r[i].cur.first;
							marswalker[i].y = r[i].cur.second;
							marswalker[i].ceil = r[i].hideCeil;

							spChange = false;
						}
					}
					if (scout.apples.empty() && scout.bombs.empty())
						doExecute = false;
					else if (scChange || spChange) {
						
						doExecute = false;

					}

					showMap(gmap);
				}
			}
		}

		screen.pos_str(0, 21, "                                                        ");
		screen.pos(0, 21);
		printf("Grabed Apples: %d/%d ", scout.appleGrab, scout.appleCount);
		screen.pos_str(0, 20, "Robots,  WELCOME:                                                ");
		

	}

	


	return 0;
}

void Game::Exit()
{
	run = false;
}


void Game::showMap(gamemap gmap) {
	std::pair<int, int> scf;
	for (int i = 0; i < scout.cursize; i ++)
		for (int j = 0; j < scout.cursize; j++) {
			if ((scout.openmap[i][j] == 1) || (scout.openmap[i][j] == 3)) {
				char symb = gmap.mp[i][j];
				if (symb == APPLE)
					screen.pos(j, i, 'A');
				if (symb == ROCK)
					screen.pos(j, i, 'R');
				if (symb == EMPTY)
					screen.pos(j, i, '_');
				if (symb == BOMB)
					screen.pos(j, i, 'B');
				if (symb == ROBOTS)
					screen.pos(j, i, 15);  //робот саппер
				scf.first = i;
				scf.second = j;
				if (scout.iamLive == true) {
					if ((scout.cur == scf) && scout.iamLive)
						screen.pos(scf.second, scf.first, 1); //сам робот-развдчик					
				}
			}
		}
	
}

void Game::showCeil(std::pair<int, int> coord1, std::pair<int, int> coord2) {
	auto x    = coord1.first;
	auto y    = coord1.second;
	char symb = gmap.mp[x][y];

	if (symb == APPLE)
		screen.pos(y, x, 'A');
	if (symb == ROCK)
		screen.pos(y, x, 'R');
	if (symb == EMPTY)
		screen.pos(y, x, '_');
	if (symb == BOMB)
		screen.pos(y, x, 'B');
	if (symb == ROBOTS)
		screen.pos(y, x, 15);
	
	x    = coord2.first;
	y    = coord2.second;
	symb = gmap.mp[x][y];

	if (symb == APPLE)
		screen.pos(y, x, 'A');
	if (symb == ROCK)
		screen.pos(y, x, 'R');
	if (symb == EMPTY)
		screen.pos(y, x, '_');
	if (symb == BOMB)
		screen.pos(y, x, 'B');
	if (symb == ROBOTS)
		screen.pos(y, x, 15);
	
	
	if ((scout.cur == coord2 ) &&(scout.iamLive))
		screen.pos(y, x, 1); //сам робот
}


gamemap::gamemap() {
	
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < WIDTH; j++) {
			mp[i][j] = EMPTY;

		}
	}
	
}
gamemap::~gamemap()
{
	
}

void Robot::updateMap() {
	for (int i = 0; i < cursize; i++)
		for (int j = 0; j < cursize; j++)
			if (openmap[i][j] >= 2) 
				openmap[i][j] = 0;


}

Robot::Robot() {
	// стоит на поле 0,0
	cursize = 3; // 10;
	cur = { 0, 0 };
	stop = { 1, 1 };
	openmap = new int*[cursize];
	for (int i = 0; i < cursize; i++) {
		openmap[i] = new int[cursize];
		for (int j = 0; j < cursize; j++)
			openmap[i][j] = 0;
	}
	openmap[cur.first][cur.second] = 1;
}

Robot::Robot(const Robot &rbt) {
	// стоит на поле 0,0
	cursize = rbt.cursize;
	cur = { 0, 0 };
	stop = { 0, 0 };
	openmap = new int* [cursize];
	for (int i = 0; i < cursize; i++) {
		openmap[i] = new int[cursize];
		for (int j = 0; j < cursize; j++)
			openmap[i][j] = rbt.openmap[i][j];
	}
	bombs = rbt.bombs;
	iamScout = false;
}


Robot::~Robot()
{
	for (int i = 0; i < cursize; i++) {
		delete[] openmap[i];
	}
	
	delete[] openmap;
	cursize = 0;
}

int Robot::moreMemory(int newsize) {
	if ((newsize > WIDTH) && (cursize < WIDTH))
		newsize = WIDTH;
	else if (newsize > WIDTH || newsize < cursize) return 0;
	else if (newsize == cursize) return 0;
	
	int** pt;
	pt = new int* [newsize];
	if (!pt)
		return 2;
	for (int i = 0; i < newsize; i++) {
		pt[i] = new int[newsize];	
	}
	for (int i = 0; i < newsize; i++) {
		for (int j = 0; j < newsize; j++) {
			if ((i < cursize) && (j < cursize))
				pt[i][j] = openmap[i][j];
			else
				pt[i][j] = 0;

		}
	}
	
	
	for (int i = 0; i < cursize; i++) {
		delete[] openmap[i];
	}
	delete[] openmap;

	openmap = pt;
	cursize = newsize;
	return 0;
}

void Robot::scan(gamemap& map ) {
  //с текущ позиции отсканировать 4 клетки, если возможно
	std::pair<int, int> scf= cur;
	int openPl = 0;
	int news = 0;
	if (!iamLive || !iamScout)
		return;

	if (scf.first+5 >= cursize || scf.second + 5 >= cursize)
		news = moreMemory(cursize + 10);
	if (news == 2) {
		//error open
		return;
	}
	else {
		if (--scf.first >= 0)
			if (openmap[scf.first][scf.second] !=1){
				openmap[scf.first][scf.second] = 1;
			if (map.mp[scf.first][scf.second] == APPLE) {
				appleCount++;
				apples.insert(scf);
			}
			if (map.mp[scf.first][scf.second] == BOMB) {
				bombs.insert(scf);
			}
			openPl++;
		}

		scf = cur;
		if (++scf.first < WIDTH) {
			if (openmap[scf.first][scf.second] != 1) {
				openmap[scf.first][scf.second] = 1;
				if (map.mp[scf.first][scf.second] == APPLE) {
					appleCount++;
					apples.insert(scf);
				}
				if (map.mp[scf.first][scf.second] == BOMB) {
					bombs.insert(scf);
				}
				openPl++;
			}
		}
		scf = cur;
		if (--scf.second >= 0) {
			if (openmap[scf.first][scf.second] != 1) {
				openmap[scf.first][scf.second] = 1;
				if (map.mp[scf.first][scf.second] == APPLE) {
					appleCount++;
					apples.insert(scf);
				}
				if (map.mp[scf.first][scf.second] == BOMB) {
					bombs.insert(scf);
				}
				openPl++;
			}
		}
		scf = cur;
		if (++scf.second < WIDTH) {
			if (openmap[scf.first][scf.second] != 1) {
				openmap[scf.first][scf.second] = 1;
				if (map.mp[scf.first][scf.second] == APPLE) {
					appleCount++;
					apples.insert(scf);
				}
				if (map.mp[scf.first][scf.second] == BOMB) {
					bombs.insert(scf);
				}
				openPl++;
			}
		}
	}
	if (openPl > 0)  //флаг на обновление
		newMap++;


}

void Robot::grab(gamemap &map) {
	if ((map.mp[cur.first][cur.second] == APPLE)||
		(map.mp[cur.first][cur.second] == ROBOTS)){
		//очистить от яблока
		map.mp[cur.first][cur.second] = EMPTY;
		appleGrab++;
		apples.erase(cur);
		
	}
	
}
void Robot::demining(gamemap& map) {
	if (map.mp[cur.first][cur.second] == BOMB) {
		//очистить от бомбы
		map.mp[cur.first][cur.second] = EMPTY;
		bombs.erase(cur);
	}
	if (!iamScout && hideCeil == BOMB) {
		//map.mp[cur.first][cur.second] = EMPTY;
		bombs.erase(cur);
	}

	

}

int Robot::makestep(int mode, gamemap& map) {  //0-OK, 1-нельзя сделать шаг, 2-bomb
	std::pair<int, int> newpos = cur;

	switch (mode) {
	case LEFT:

		--newpos.second;
		break;
	case RIGHT:
		//--перерисовка робота later
		++newpos.second;
		break;
	case UP:
		//--перерисовка робота later
		--newpos.first;
		break;
	case DOWN:
		//--перерисовка робота later
		++newpos.first;
		break;
	}
	//==проверка на входимость в поле

	if (newpos.first < 0 || newpos.first >= WIDTH ||
		newpos.second < 0 || newpos.second >= WIDTH)
		return 1;
	if (openmap[newpos.first][newpos.second] == 0)
		return 1;
	

	if (map.mp[newpos.first][newpos.second] == BOMB) {
		map.mp[newpos.first][newpos.second] = EMPTY;
		bombs.erase(newpos);
		cur = newpos;
		return 2;
	}


	if (map.mp[newpos.first][newpos.second] == ROCK)
		return 1;
	//--перерисовка робота later
	cur = newpos;
	return 0;

}

bool Robot::choosepoint(std::pair<int, int> newpos) {
	bool notfind = true;
	int i, j;
	int skip = 0;
	//1/ найти неисследованную точку  

	while (skip < cursize) {
		for (i = newpos.first - skip; i <= newpos.first + skip; i++) {
			if (i < 0 || i >= cursize) continue;
			for (j = newpos.second - skip; j < newpos.second + skip; j++) {
				if (j < 0 || j >= cursize) continue;
				if ((i == newpos.first) && (j == newpos.second))continue;
				else if (openmap[i][j] == 0) { //нашли неисследованную точку
					stop.first = i;
					stop.second = j;
					notfind = false;
					return true;
				}
			}

		}
		skip++;
	}


	

	if (notfind )  {
		if (iamScout)
		  stop = { 0, 0 };
		return false;  //тупик
	}
	return true;
}

void Robot::getmap(Robot rbt) {
	
	if (cursize < rbt.cursize) {
		//delete openmap; // = rbt.openmap;
		openmap = new int* [rbt.cursize];
		for (int i = 0; i < rbt.cursize; i++) {
			openmap[i] = new int[rbt.cursize];
		}
		//moreMemory(rbt.cursize);
		cursize = rbt.cursize;
	}


	
	if (cursize == rbt.cursize) {
		for (int i = 0; i < cursize; i++) {
			for (int j = 0; j < cursize; j++) {
				openmap[i][j] = rbt.openmap[i][j];
			}
		}

	}
	

}

int Robot::getpath(gamemap& gmap){ //установить вектор координат до конечной точки (от конеч до текущей)

	if (cur == stop) //мы уже стоим
		return 1;

	

	if (path.empty()) {
		std::queue<std::pair<int, int>> q;    //храним  координаты клеток.
		int m2 = cursize * cursize;
		bool** used;  //возможно ли посещение данной точки t/f
		int** dst;    //массив весов прохода от текущ точки
		int i, j;
		//std::vector<std::pair<int, int> > prev;
		//prev = (TPointI*)malloc(sizeof(TPointI) * m2);


		dst = new int* [cursize];  //массив весов
		used = new bool* [cursize];  //массив обработки точек

		if (!dst || !used || !path.empty() )
			return 1;
		for (i = 0; i < cursize; i++) {
			dst[i] = new int[cursize];
		}
		for (i = 0; i < cursize; i++) {
			used[i] = new bool[cursize];
		}

		for (i = 0; i < cursize; i++) {
			for (j = 0; j < cursize; j++) {
				dst[i][j] = -1;
				used[i][j] = false;

			}
		}
		//  координаты предыд.ячейки- из какой прошли в эту
		std::pair<int, int> pr = { -1, -1 };
		for (i = 0; i < m2; i++)
		{			
			path.push_back(pr);
		}
		// найдем  путь до этой точки и пройдем до нее

		q.push(cur);
		used[cur.first][cur.second] = true;
		dst[cur.first][cur.second] = 0;

		while (!q.empty()) {
			std::pair<int, int> curq = q.front();
			q.pop();

			int cx = curq.first, cy = curq.second;

			std::vector<std::pair<int, int>> moves{ //возможное движение по клеткам
				{-1, 0}, {0, -1}, {+1, 0}, {0, +1},

			};

			for (auto move : moves) {
				int dx = move.first, dy = move.second;
				if (visited(gmap, { cx + dx, cy + dy }) &&
					!used[cx + dx][cy + dy]) {
					q.push({ cx + dx, cy + dy });
					used[cx + dx][cy + dy] = true;
					dst[cx + dx][cy + dy] = dst[cx][cy] + 1;
					path[(cx + dx) * cursize + cy + dy].first = cx;
					path[(cx + dx) * cursize + cy + dy].second = cy;

				}
			}
		}

		bool canStep = used[stop.first][stop.second];

		if (!canStep) { //перевернем массив
		/*	int index = stop.first * cursize + stop.second;
			for (auto it = index; it >=0; it--)
				path.push_back(path.at(it));*/
			path.clear();
			return 1;
		}
			
		else
			return 0;
	}
	return 0;
}
int Robot::go2Neibor(gamemap& gmap) {
	if (path.size() <= 0)
		return 1;
	int index = stop.first * cursize + stop.second;

	std::pair<int, int> newpos, nextpos;

	newpos = stop;
	while (true) {
		index = newpos.first * cursize + newpos.second;
		nextpos = path[index];

		if (nextpos == cur) {
			
			
			if (!iamScout) { //если в клетке,с которой ухожу  есть другой робот
				/*if(gmap.mp[cur.first][cur.second] == ROBOTS)
				  gmap.mp[cur.first][cur.second] = EMPTY;
				else */
				  gmap.mp[cur.first][cur.second] = hideCeil;
			}
			cur = newpos;
			if (!iamScout) {

				hideCeil = gmap.mp[cur.first][cur.second];
				if (hideCeil == ROBOTS)
					hideCeil = EMPTY;
				gmap.mp[cur.first][cur.second] = ROBOTS;
			}

			break;
		}
		newpos = nextpos;

	}
	return 0;

}

int Game::pathfind(int &step, Robot &scout) {
	std::pair<int, int> newpos = scout.cur, oldpos = scout.cur;
	std::queue<std::pair<int, int>> q;    //храним  координаты клеток.	
	struct TPointI {
		int x;
		int y;
	}; 
	int m2 = scout.cursize * scout.cursize;
	TPointI* prev = (TPointI*) malloc(sizeof(TPointI)*m2);//  координаты предыд.ячейки- из какой прошли в эту
	
	
	int i, j;
	int res = 0;
	int** dst;    //массив весов прохода от текущ точки
	int skip = 1;   //шаг поиска
	int k = 0;
	bool** used;  //возможно ли посещение данной точки t/f
	bool notfind = true;
	bool doStep = true;		
	int oldSize = scout.cursize;
	
	//поиск пути- обход в ширину
	dst = new int* [scout.cursize];  //массив весов
	used = new bool* [scout.cursize];  //массив обработки точек
	
	if (!dst || !used || !prev)
		return 1;
	for ( i = 0; i < scout.cursize; i++) {
		dst[i] = new int[scout.cursize];
	}
	for (i = 0; i < scout.cursize; i++) {
		used[i] = new bool[scout.cursize];
	}
	
	for ( i = 0; i < scout.cursize; i++) {
		for ( j = 0; j < scout.cursize; j++) {
			dst[i][j] = -1;
			used[i][j] = false;

		}
	}
	//  координаты предыд.ячейки- из какой прошли в эту
	for (i = 0; i < m2; i++)
	{
		prev->x = -1;
		prev->y = -1;
	}
	// найдем  путь до этой точки и пройдем до нее
	
	q.push(scout.cur);
	used[scout.cur.first][scout.cur.second] = true;
	dst[scout.cur.first][scout.cur.second] = 0;

	while (!q.empty()) {
		std::pair<int, int> curq = q.front();
		q.pop();

		int cx = curq.first, cy = curq.second;

		std::vector<std::pair<int, int>> moves{ //возможное движение по клеткам
			{-1, 0}, {0, -1}, {+1, 0}, {0, +1},

		};

		for (auto move : moves) {
			int dx = move.first, dy = move.second;
			if (scout.can_visited(gmap, { cx + dx, cy + dy }) &&
				!used[cx + dx][cy + dy]) {
				q.push({ cx + dx, cy + dy });
				used[cx + dx][cy + dy] = true;
				dst[cx + dx][cy + dy] = dst[cx][cy] + 1;
				prev[(cx + dx)*oldSize + cy + dy].x = cx;
				prev[(cx + dx)*oldSize + cy + dy].y = cy;

			}
		}
	}
	doStep = (used[scout.stop.first][scout.stop.second] == true);
	//обратный ход
	
	if (!doStep) {

		for (j = 0; j < oldSize; j++) {
			delete[] dst[j];
			delete[] used[j];

		}
		delete[] dst;
		delete[] used;
		free(prev);
		

		if ((scout.stop.first < scout.cursize) && (scout.stop.second < scout.cursize))
			scout.openmap[scout.stop.first][scout.stop.second] = 2;
		return 1;
	}
		
		
		//не хватает шагов до точки-  дойдем до ближайшей по шагам
	if (step < dst[scout.stop.first][scout.stop.second])
	{
		while (true) {
			i = scout.stop.first * oldSize;
			j = scout.stop.second;
			scout.stop.first = prev[i + j].x;
			scout.stop.second = prev[i + j].y;
			if (step <= dst[scout.stop.first][scout.stop.second])
				break;
		}
	}
		
		
		
	std::set<std::pair<int, int> > scanIt; //массив для обновления области сканирования с этих точек
    
	
	scout.cur = scout.stop;
	scanIt.insert(scout.stop);
	while ((step >= 0) && (scout.cur != oldpos) && (scout.iamScout == true)) {
		i = scout.cur.first * oldSize;
		j = scout.cur.second;
		scout.cur.first = prev[i + j].x;
		scout.cur.second = prev[i + j].y;
		scanIt.insert(scout.cur);
		step--;
	}
	if (scout.iamScout == true) {  //обновление только для робота-разведчика
		for (auto it = scanIt.begin(); it != scanIt.end(); it++) {
			scout.cur = *it;
			scout.scan(gmap);
		}
	}
	scout.cur = scout.stop;
	showMap(gmap);

	
	for (j = 0; j < oldSize; j++) {
		delete[] dst[j];
		delete[] used[j];

	}
	delete[] dst;
	delete[] used;
	free(prev);

	

	return 0;
}
	

bool Robot::can_visited(gamemap map, std::pair<int, int> pos){
	auto x = pos.first;
	auto y = pos.second;
	bool step = (map.mp[x][y] != BOMB);
		if (!iamScout) step = true;
	
		return  x >= 0 && x < cursize
		&& y >= 0 && y < cursize
		&& (map.mp[x][y] != ROCK)
		&& step 
		&& (map.mp[x][y] != ROBOTS);
	
	
}

bool Robot::visited(gamemap map, std::pair<int, int> pos) {
	auto x = pos.first;
	auto y = pos.second;
	bool step = (map.mp[x][y] != BOMB);
	if (!iamScout) step = true;

	return  x >= 0 && x < cursize
		&& y >= 0 && y < cursize
		&& ((openmap[x][y] == 1) || (openmap[x][y] ==3))
		&& (map.mp[x][y] != ROCK)
		&& step	
		&& (map.mp[x][y] != ROBOTS);


}

bool gamemap::fillmap(std::string cheme) {
	if (cheme.empty())
		cheme = "map.txt";
	std::ifstream file(cheme);
	std::string input;
	int len = WIDTH;
	
	std::getline(file, input);
	len = sTOi(input);
	
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			file >> mp[i][j];

		}
	}
	file.close();
	return true;
}
