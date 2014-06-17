/*Christopher Webb
2D Games Programming
Final Project
Hero.h*/

#pragma once
#include <iostream>
#include <string>
#include "Creature.h"

using namespace std;

class Hero : public Creature
{
private:
	int m_energy;			//hero will obviously need energy to dig holes and leave the level when collected enough
	int digwait;			//this variable will help slow the digging function, so the player does not waste energy digging holes they did not need to
	int m_score;			//added a scoring mechanic after testing, not in initial design.
public:
	Hero(int xpos, int ypos, string name, int energy, string filename, Arena* arena, int speed); //can start player off with energy if need be
	int getEnergy();
	void heroeaten();	
	void gainEnergy();
	//void respawnhero(); redundant method, changed in Appendix C - Testing
	bool leavemap();
	void digHole();			//other component to digging holes along with the Arena method: createHole(int x, int y)
	void addScore();		//having a score added some extra functionality not in initial design.
	int getScore();
};
