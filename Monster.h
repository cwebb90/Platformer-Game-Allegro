/*Christopher Webb
2D Games Programming
Final Project
Monster.h*/

#pragma once
#include <iostream>
#include "Creature.h"
#include "Hero.h"
using namespace std;

class Monster : public Creature
{
private:
	bool dead;				//this will allow us to only call monster methods on monsters that are currently alive
	int m_limit1;			//these limit integers and direction boolean allow monsters to be able to move back and forth, or up and down
	int m_limit2;
	bool direction;
public:
	Monster(int xpos, int ypos, string name, string filename, Arena* arena, int speed, int limit1, int limit2);
	void chase(Hero* herop);
	bool collidesWith(Hero* herop);
	void mummieMovement();
	void evilSpiritMovement();
	void falldown(Hero* herop);
	bool isDead();
	void ghostUp();			//these ghost methods had to be added in after the design as explained in Appendix C - Testing
	void ghostDown();
	void gspriteUp();
	void gspriteDown();
};
