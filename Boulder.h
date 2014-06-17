/*Christopher Webb
2D Games Programming
Final Project
Boulder.h*/

#pragma once
#include <iostream>
#include "Creature.h"
#include "Hero.h"
using namespace std;

class Boulder : public Creature
{
private:
	bool bouldermoved;
public:
	Boulder(int xpos, int ypos, string name, string filename, Arena* arena, int speed);
	void pushright(Hero* hero2);		//can call these depending what side of the level they are created on
	void pushleft(Hero* hero2);
	void boulderrollright();
	void boulderrollleft();
	bool boulderstate();
	bool collidesWith(Hero* herop);		
};
