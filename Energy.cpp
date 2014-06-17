/*Christopher Webb
2D Games Programming
Final Project
Energy.cpp*/

#include "Energy.h"


Energy::Energy(int xpos, int ypos, string name, string filename, Arena* arena, int speed) : Creature(xpos, ypos, name, filename, arena, speed)
{
	
}

bool Energy::collidesWith(Hero* herop)						//this checks to see if the energy bar is colliding with the player
{															//i can then use the boolean returned to call other relevant methods
	if	((herop->getX() > m_xpos + creature_width - 1) ||
		(herop->getY() > m_ypos + creature_height -1) ||
		(m_xpos > herop->getX() + herop->getWidth() -1) ||
		(m_ypos > herop->getY() + herop->getHeight() -1))
	{
		return false;
	}
	return true;

}

void Energy::draw()
{
	al_draw_bitmap(creaturebit, m_xpos, m_ypos, 0); //had to include another draw method as the energy bitmap is not a sprite sheet of animation rather a single bitmap
}

void Energy::movetoCount()
{
	m_xpos = 0;										//i can call this method once the energy bar has been collided with, removing it from the playing area
	m_ypos = -10;
}

