/*Christopher Webb
2D Games Programming
Final Project
Monster.cpp*/

#include "Monster.h"
#include "Hero.h"

Monster::Monster(int xpos, int ypos, string name, string filename, Arena* arena, int speed, int limit1, int limit2) : Creature(xpos, ypos, name, filename, arena, speed)
{
	dead=false;					//all monsters should start as alive (dead=false)
	m_limit1 = limit1;			//passing this in through the parameters allows different movement limits for each monster created.
	m_limit2 = limit2;			
	direction=true;
}

void Monster::chase(Hero* herop)
{			

	if(m_xpos>herop->getX())	//this method checks each x and y position to see if it is greater than or less than the hero's passed in the parameters
	{							//it can then call the relevant movement method to effectively chase the player
		left();
	}

	if(m_xpos<herop->getX())
	{
		right();
	}

	if(m_ypos<herop->getY())
	{		
		down();
	}
	
	if(m_ypos>herop->getY())
	{
		up();
	}
}


bool Monster::collidesWith(Hero* herop)
{
	if	((herop->getX() > m_xpos + creature_width - 1) ||		//this method checks to see if the hero is colliding with the monster
		(herop->getY() > m_ypos + creature_height -1) ||		//i can then use the returned boolean to call other methods, such as heroeaten() from the Hero class
		(m_xpos > herop->getX() + herop->getWidth() -1) ||
		(m_ypos > herop->getY() + herop->getHeight() -1))
	{
		return false;
	}
	return true;

}

void Monster::mummieMovement()		//having movement specific to the two types allows me to keep the monster class as it is and not create two new mummie and evilspirit classes,
{									//the only difference between their behaviours is the way they move around the level
	if(m_xpos==m_limit1)			//this checks if the monsters x position is equal to the m_limit_1
	{
		direction=false;			//then sets direction to false
	}
	else if(m_xpos==m_limit2)		//then this statement does the opposite when m_limit2 is reached.
	{
		direction=true;
	}

	if(direction)					//then if direction is true or false, left and right can be called, respectively.
	{
		left();
	}
	else if(!direction)				//there is no need to worry about the monsters not reaching either limit as the direction is set to false in the constructor
	{								//this ensures movement immediately once the game starts.
		right();					//the only care must be taken in setting the limits, they must not be inside a wall or obstacle, or past one.
	}
}

void Monster::evilSpiritMovement()	//this method does essentially the same as the mummieMovement() method, except calling up and down instead of left and right
{
	if(m_ypos==m_limit1)
	{
		direction=false;
	}
	else if(m_ypos==m_limit2)
	{
		direction=true;
	}

	if(direction)
	{
		ghostUp();
	}
	else if(!direction)
	{
		ghostDown();				//these ghost methods were added after design - refer to Appendix C - Testing
	}

}

void Monster::falldown(Hero* herop)//passes in hero pointer so can increment player score. not in initial design.
{
	if(m_arena->isHole(m_xpos,m_ypos + creature_height +1)==true&&m_arena->isHole(m_xpos + creature_width -1, m_ypos + creature_height +1)==true)
	{								//this method checks to see if the monster is above a hole
		m_ypos = m_ypos+50;			//if so, its y position is added to so that it then falls to the bottom of the hole but not through it, due to the inSameRow check.	
		dead=true;					//it is also set to dead, so that i can only call methods for monsters in the main that are alive and not dead monsters.
		herop->addScore();			//added during testing, not in initial design, gives player one score point every time he kills a monster.		
	}								

}

bool Monster::isDead()
{
	return dead;
}

void Monster::ghostDown()		//these methods were added after design - refer to Appendix C - Testing
{
	bool wall_below = m_arena->isWall(m_xpos,m_ypos + creature_height +1)||m_arena->isWall(m_xpos + creature_width -1, m_ypos + creature_height +1)||m_arena->isHole(m_xpos,m_ypos + creature_height +1)||m_arena->isHole(m_xpos + creature_width -1, m_ypos + creature_height +1);
	bool platform_below = m_arena->isPlatform(m_xpos,m_ypos + creature_height +1)||m_arena->isPlatform(m_xpos + creature_width -1, m_ypos + creature_height +1);
	bool in_same_row = m_arena->isInSameRow(m_ypos + creature_height +1, m_ypos + creature_height);

	if(!wall_below && !(platform_below && !in_same_row))
	{
	m_ypos = (m_ypos+m_speed);

	cycle++;

		if(cycle==8)
		{
			spriteright();
			cycle = 0;
		}
	}
}




void Monster::ghostUp()		//these methods were added after design - refer to Appendix C - Testing
{
	m_ypos = (m_ypos-m_speed);

	if(m_arena->isWall(m_xpos,m_ypos)==true||m_arena->isWall(m_xpos + creature_width -1, m_ypos)==true)
	{
		m_ypos = (m_ypos+m_speed);
	}

	else
	cycle++;

	if(cycle==8)
	{
		spriteleft();
		cycle = 0;
	}

}
