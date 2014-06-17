/*Christopher Webb
2D Games Programming
Final Project
Boulder.cpp*/

#include "Boulder.h"

Boulder::Boulder(int xpos, int ypos, string name, string filename, Arena* arena, int speed) : Creature(xpos, ypos, name, filename, arena, speed)
{
	bouldermoved = false;			//at creation, it is assumed that the player has not instantly got enough energy bars and reached the boulder, so it is set to false
}

bool Boulder::boulderstate()
{
	return bouldermoved;
}

void Boulder::pushright(Hero* hero2)						//depending on the level, boulders can either be pushed right or left
{
	if(hero2->getEnergy() >= m_arena->getEnergyNeeded())	//this checks to see if the player has enough energy to move the boulder	
	{
		boulderrollright();									//if so it can call the boulder animation method defined below,
		bouldermoved = true;								//and also set bouldermoved to true, allowing the player to exit the level
	}

}

void Boulder::pushleft(Hero* hero2)							//this is the same as the previous method, but for left instead of right
{
	if(hero2->getEnergy() >= m_arena->getEnergyNeeded())
	{
		boulderrollleft();
		bouldermoved = true;
	}
	
}

void Boulder::boulderrollright()						//to only get the animation to play once, instead of looping
{														//i had to create new animation methods that only allow one cycle of the animation to play
	if(source_x<160)
	{
	source_y=0;
	source_x = source_x + source_w;
	}

}

void Boulder::boulderrollleft()							//again this is the same as before but for left instead of right
{
	if(source_x<160)
	{
	source_y=40;
	source_x = source_x + source_w;
	}

}

bool Boulder::collidesWith(Hero* herop)					//this is essentially the same as the monster and energy bar collidesWith methods, only for a boulder instead
{
	if	((herop->getX() > m_xpos + creature_width - 1) ||
		(herop->getY() > m_ypos + creature_height -1) ||
		(m_xpos > herop->getX() + herop->getWidth() -1) ||
		(m_ypos > herop->getY() + herop->getHeight() -1))
	{
		return false;
	}
	return true;

}
