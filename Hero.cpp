/*Christopher Webb
2D Games Programming
Final Project
Hero.cpp*/

#include "Hero.h"
#include <conio.h>
#include <iostream>
using namespace std;

Hero::Hero(int xpos, int ypos, string name, int energy, string filename, Arena* arena, int speed) : Creature(xpos, ypos, name, filename, arena, speed)
{
	m_energy = energy;
	digwait = 0;
	m_score = 0;
}


int Hero::getEnergy()
{
	return m_energy;	//we can use this method to check against the total energy needed to complete the level
}

void Hero::gainEnergy()
{
	m_energy++;
}

void Hero::heroeaten()
{
	m_energy--;		//once the hero is 'eaten' i can decrement the amount of energy the player has, and redraw the position, effectively 'respawning' the player
	m_xpos = 63;
	m_ypos = 63;
}

//void Hero::respawnhero()		 this method was in the design but was redundant, i could simply do the same thing in heroeaten()
//{
//	m_xpos = 63;
//	m_ypos = 63;
//}

bool Hero::leavemap()
{
	if(getEnergy() >= m_arena->getEnergyNeeded() && m_arena->isExit(m_xpos + creature_width +m_speed, m_ypos)==true||m_arena->isExit(m_xpos + creature_width +m_speed, m_ypos +creature_height -1)==true 
	|| m_arena->isExit(m_xpos, m_ypos)==true||m_arena->isExit(m_xpos, m_ypos + creature_height -1)==true)
	{
		return true;		//this method checks both the energy the player has is sufficient and also that he is colliding with the exit
	}
	else
		return false;
}

void Hero::digHole()
{
	bool wall_below = m_arena->isWall(m_xpos,m_ypos + creature_height +1)||m_arena->isWall(m_xpos + creature_width -1, m_ypos + creature_height +1);
	bool platform_below = m_arena->isPlatform(m_xpos,m_ypos + creature_height +1)||m_arena->isPlatform(m_xpos + creature_width -1, m_ypos + creature_height +1);
	bool in_same_row = m_arena->isInSameRow(m_ypos + creature_height +1, m_ypos + creature_height);
	
		if(wall_below || (platform_below && !in_same_row))
		{
		
			digwait++;												//this method checks in a similar way to the jump method in the Creature class
																	//it checks the player is standing on a block capable of being dug
				if(digwait==4)										//it then increments the digwait integer, once this has reached 4
				{													//it decrements the players energy and creates two integer variables based on its position
					m_energy--;										//these can then be passed into the createHole method in the Arena class,
																	//which can then use this information to draw a hole in the level
					int x = m_xpos;									//then finally digwait is reset to 0
					int y = m_ypos + creature_height;				//the digwait cycle is used so that it slows how fast the player digs
																	//ensuring they dont dig holes too quickly or by accident
					m_arena->createHole(x, y);

					digwait = 0;
				}
		}
}

void Hero::addScore()
{
	m_score++;
}

int Hero::getScore()
{
	return m_score;
}
