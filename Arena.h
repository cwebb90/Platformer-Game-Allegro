/*Christopher Webb
2D Games Programming
Final Project
Arena.h*/

#pragma once
#include <iostream>
#include <vector>
#include <allegro5/allegro_primitives.h>
using namespace std;

class Arena
{
private:
	vector<string> m_grid_layout;		//2d vector of strings, will create level from this
	int m_energy_needed;				// energy required to finish level, can increase this as levels progress
	ALLEGRO_BITMAP* levelbitmap;		//bitmaps for the various different block types
	ALLEGRO_BITMAP* platformbitmap;		
	ALLEGRO_BITMAP* ladderbitmap;
	ALLEGRO_BITMAP* exitbitmap;
	ALLEGRO_BITMAP* holebitmap;
	string m_filename;					//strings that can be taken in the constructor and then used in al_load_bitmap()
	string p_filename;
	string l_filename;
	string e_filename;
	string h_filename;
	string d_filename;

public:
	Arena(string filename, string filename2, string filename3, string filename4, string filename5, int energy_needed);		//arena constructor
	int getHeight();		
	int getWidth();
	int getEnergyNeeded();
	void render();
	bool isWall(int x, int y);			//collision checks
	bool isPlatform(int x, int y);
	bool isInSameRow(int x, int y);
	bool isLadder(int x, int y);
	bool isExit(int x, int y);
	bool isHole(int x, int y);
	void createHole(int x, int y);		//changes map coordinates to a hole
};
