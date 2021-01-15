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
	
	std::vector<std::pair<int, int> > path;                   //������� �� ����� stop

	std::pair<int, int> cur;                   //������� ������� ������
	std::pair<int, int> stop;                  // �������-����  ������
	std::set<std::pair<int, int> > apples;     //������
	std::set<std::pair<int, int> > bombs;      //�����
	int** openmap;                             //������������� ����� 
	int cursize;
	int appleCount = 0;                        // ����� ������� �����
	int appleGrab = 0;                         // ���-�� ��������� �����
	int newMap = 0;                            //���� �� ���������� �����                           
	bool iamLive = true;
	bool iamScout = true;
	char hideCeil = ' ';
	Robot();
	Robot(const Robot &rbt);
	~Robot();
	int makestep(int move, gamemap& map);   //l/r/u/d/ ���������� 2 ���� ����� � 1 ���� �� ����� ������
	void scan(gamemap& map);  // 4 ������ ������ � �����, ���� ����� ����� ��� �������� �����
	void grab(gamemap &map);   //����� ������, ������� �������� ���������
	void demining(gamemap& map); //��������������
	bool choosepoint(std::pair<int, int> coord);    // false ��� ���������� ��������.�����
	int moreMemory(int newSize); //��� �������� ����� �������� ������ �������
	bool can_visited(gamemap map, std::pair<int, int> pos);
	bool visited(gamemap map, std::pair<int, int> pos);
	void getmap(Robot rbt);
	int getpath(gamemap& map);   //������������ ���� � �������� �����. 0- � ����, 1- ����� �� �����
	int go2Neibor(gamemap& map); //���� � �������� ����� �� ���� � ��������
	void updateMap();

private:

};



class Game
{
private:
	bool run;
	int mode;
	std::vector<Sapper> marswalker;  //������ �������������� � ����������
	gamemap gmap;
	Robot scout;
	CScreen screen;

public:
	Game();
	int Execute(std::string cheme);
	void showMap(gamemap gmap);
	void showCeil(std::pair<int, int> coord1, std::pair<int, int> coord2);
	void Exit();
	int pathfind(int &step, Robot &r);   //������������ ����� �� step ����� ���������� 2 ���� �����������
};

/* openmap   */