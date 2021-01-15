#pragma once

#include <Windows.h>
#include <map>
#include <set>
#include <vector>
#include "CScreen.h"
#include <string>
#define WIDTH  20



struct Sapper
{
	int x;
	int y;
	char ceil;
};



class gamemap
{
	
public:
	gamemap();
	~gamemap();
	char mp[WIDTH] [WIDTH] ;
	bool fillmap(std::string cheme);
	friend class Robot;
private:

};






class Robot
{
public:
	
	std::vector<std::pair<int, int> > path;                   //Маршрут до точки stop

	std::pair<int, int> cur;                   //текущая позиция робота
	std::pair<int, int> stop;                  // позиция-цель  робота
	std::set<std::pair<int, int> > apples;     //яблоки
	std::set<std::pair<int, int> > bombs;      //бомбы
	int** openmap;                             //исследованная карта 
	int cursize;
	int appleCount = 0;                        // общий счетчик яблок
	int appleGrab = 0;                         // кол-во собранных яблок
	int newMap = 0;                            //флаг на обновление карты                           
	bool iamLive = true;
	bool iamScout = true;
	char hideCeil = ' ';
	Robot();
	Robot(const Robot &rbt);
	~Robot();
	int makestep(int move, gamemap& map);   //l/r/u/d/ возвращает 2 если бомба и 1 если не может пройти
	void scan(gamemap& map);  // 4 клетки вокруг в карту, флаг новой карты для передачи общей
	void grab(gamemap &map);   //взять ресурс, счетчик ресурсов увеличить
	void demining(gamemap& map); //разминирование
	bool choosepoint(std::pair<int, int> coord);    // false при отсутствии неисслед.точек
	int moreMemory(int newSize); //для открытой карты выделить больше размера
	bool can_visited(gamemap map, std::pair<int, int> pos);
	bool visited(gamemap map, std::pair<int, int> pos);
	void getmap(Robot rbt);
	int getpath(gamemap& map);   //прокладывает путь к конечной точке. 0- в путь, 1- стоим на точке
	int go2Neibor(gamemap& map); //идти к соседней точке по пути к конечной
	void updateMap();

private:

};



class Game
{
private:
	bool run;
	int mode;
	std::vector<Sapper> marswalker;  //роботы дополнительные к разведчику
	gamemap gmap;
	Robot scout;
	CScreen screen;

public:
	Game();
	int Execute(std::string cheme);
	void showMap(gamemap gmap);
	void showCeil(std::pair<int, int> coord1, std::pair<int, int> coord2);
	void Exit();
	int pathfind(int &step, Robot &r);   //исследование карты на step шагов возвращает 2 если подзорвался
};

/* openmap   */