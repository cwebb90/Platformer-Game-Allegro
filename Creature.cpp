/*Christopher Webb
2D Games Programming
Final Project
Creature.cpp*/

#include "Creature.h"
#include <conio.h>
#include <iostream>
using namespace std;

Creature::Creature(int xpos, int ypos, string name, string filename, Arena* arena, int speed)
{
	m_xpos = xpos;						//allows for positioning creatures in the arena
	m_ypos = ypos;
	m_name = name;
	m_filename = filename;
	creaturebit = al_load_bitmap(m_filename.c_str());	//loads bitmap
	m_speed = speed;
	m_arena = arena;
	creature_width = 40;
	creature_height = 40;

	//jumping
	m_is_jumping = false;
	m_current_jump_height = 0;
	m_max_jump_height = 80;				//i thought i good jump height was double that of the character itself, with all the characters being 40 pixels tall

	//spritesheet/animation variables
	source_h = 40;
	source_w = 40;
	source_x = 0;
	source_y = 0;
	cycle = 0;

	al_convert_mask_to_alpha(creaturebit, al_map_rgb(255, 0, 255)); //clears the background of the bitmaps
	
	if (!creaturebit)												//checks to see if bitmap has loaded correctly, displays error if it has not
	{
		cout << "Creature bitmap creation failed" << endl;
		cout << "Press any key to exit" << endl;
		_getch();
		al_destroy_bitmap(creaturebit);		
	}
}


void Creature::left()
{
	m_xpos = (m_xpos-m_speed);
	if(	m_arena->isWall(m_xpos, m_ypos)==true||m_arena->isWall(m_xpos, m_ypos + creature_height -1)==true				//this checks for each type of block on the left edge of the creature bitmap
		||	m_arena->isHole(m_xpos, m_ypos)==true||m_arena->isHole(m_xpos, m_ypos + creature_height -1)==true			//using the techniques discussed in the analysis and design section: Collision Detection
		||	m_arena->isPlatform(m_xpos, m_ypos)==true||m_arena->isPlatform(m_xpos, m_ypos + creature_height -1)==true	
		||  m_arena->isExit(m_xpos, m_ypos)==true||m_arena->isExit(m_xpos, m_ypos + creature_height -1)==true)
	{
		m_xpos = (m_xpos+m_speed);
	}
	else
	cycle++;			//having this placed here allows the animation not to play if the character is not moving i.e colliding with an object

	if(cycle==4)
	{
		spriteleft();	//only calling the sprite animation method everytime the cycle integer reaches 4 allows for a smoother animation,
		cycle = 0;		//it stops it from playing too quickly
	}

}

void Creature::right()	//this again does the same as the previous method, only it checks for the opposite side as the character is now moving right
{
	m_xpos = (m_xpos+m_speed);
	if(		m_arena->isWall(m_xpos + creature_width -1, m_ypos)==true||m_arena->isWall(m_xpos + creature_width -1, m_ypos +creature_height -1)==true
		||	m_arena->isHole(m_xpos + creature_width -1, m_ypos)==true||m_arena->isHole(m_xpos + creature_width -1, m_ypos +creature_height -1)==true
		||	m_arena->isPlatform(m_xpos + creature_width -1, m_ypos)==true||m_arena->isPlatform(m_xpos + creature_width -1, m_ypos +creature_height -1)==true
		||	m_arena->isExit(m_xpos + creature_width -1, m_ypos)==true||m_arena->isExit(m_xpos + creature_width -1, m_ypos +creature_height -1)==true)
	{
		m_xpos = (m_xpos-m_speed);
	}
	else
	cycle++;

	if(cycle==4)
	{
		spriteright();
		cycle = 0;
	}

}

void Creature::up()
{
	m_ypos = (m_ypos-m_speed);

	if(m_arena->isWall(m_xpos,m_ypos)==true||m_arena->isWall(m_xpos + creature_width -1, m_ypos)==true)
	{
		m_ypos = (m_ypos+m_speed);
	}

	else
	cycle++;

	if(cycle==4)
	{
		spriteup();
		cycle = 0;
	}

}

void Creature::jump()
{
	bool wall_below = m_arena->isWall(m_xpos,m_ypos + creature_height +1)||m_arena->isWall(m_xpos + creature_width -1, m_ypos + creature_height +1)||m_arena->isHole(m_xpos,m_ypos + creature_height +1)||m_arena->isHole(m_xpos + creature_width -1, m_ypos + creature_height +1);
	bool platform_below = m_arena->isPlatform(m_xpos,m_ypos + creature_height +1)||m_arena->isPlatform(m_xpos + creature_width -1, m_ypos + creature_height +1);
	bool in_same_row = m_arena->isInSameRow(m_ypos + creature_height +1, m_ypos + creature_height);
	//this method creates three booleans that check for platform and wall below the creature, and checks that it is not in the same row.
	
		if(wall_below || (platform_below && !in_same_row))
		{
			m_is_jumping = true;		//this allows the creature to only jump if they are on an able platfrom
			m_current_jump_height = 0;
		}
}

void Creature::down()
{
	bool wall_below = m_arena->isWall(m_xpos,m_ypos + creature_height +1)||m_arena->isWall(m_xpos + creature_width -1, m_ypos + creature_height +1)||m_arena->isHole(m_xpos,m_ypos + creature_height +1)||m_arena->isHole(m_xpos + creature_width -1, m_ypos + creature_height +1);
	bool platform_below = m_arena->isPlatform(m_xpos,m_ypos + creature_height +1)||m_arena->isPlatform(m_xpos + creature_width -1, m_ypos + creature_height +1);
	bool in_same_row = m_arena->isInSameRow(m_ypos + creature_height +1, m_ypos + creature_height);

	if(!wall_below && !(platform_below && !in_same_row))
	{
	m_ypos = (m_ypos+m_speed);		//similar to before, only this time we are only calling down if there are no walls or platforms below and not in the same row.

	cycle++;

		if(cycle==4)
		{
			spritedown();
			cycle = 0;
		}
	}
}


void Creature::fall()
{
	if(m_is_jumping==true) //if the jump method is called, we are no longer calling down() and instead calling up()
	{
		up();
		m_current_jump_height ++; //this integer is incremented so that we can eventually reach the max jump height

		if(m_current_jump_height >= m_max_jump_height || m_arena->isWall(m_xpos,m_ypos-m_speed)==true||m_arena->isWall(m_xpos + creature_width, m_ypos-m_speed)==true || m_arena->isHole(m_xpos,m_ypos-m_speed)==true||m_arena->isHole(m_xpos + creature_width, m_ypos-m_speed)==true)
		{
				m_is_jumping = false;	//if the current jump height is greater than or equal to the max jump height OR the top edge of the creature hits a wall, we reset jumping to false,
		}								
	}
	else if((m_arena->isLadder(m_xpos,m_ypos + creature_height -1)==false)||(m_arena->isLadder(m_xpos + creature_width -1, m_ypos)==false))
	{
		down();							//we do one extra check to see if the player is currently on a ladder and if they are not we can then call down.
	}									//this allows the creature free up and down movement on ladder blocks without interference from the fall() method and simulates the effect of them climbing.
}

void Creature::climbup()
{
	if((m_arena->isLadder(m_xpos,m_ypos + creature_height -1)==true)||(m_arena->isLadder(m_xpos + creature_width -1, m_ypos)==true))
	{
		m_ypos = (m_ypos-m_speed); //this method simply checks to see if the creature is on a ladder and if so can climb up
	}

	cycle++;

		if(cycle==4)
		{
			spriteup();
			cycle = 0;
		}
}

void Creature::climbdown()
{
	if((m_arena->isLadder(m_xpos,m_ypos + creature_height -1)==true)||(m_arena->isLadder(m_xpos + creature_width -1, m_ypos)==true))
	{
		m_ypos = (m_ypos+m_speed);	//the partner method to the one above, only this time the creature is climbing down
	}

cycle++;

		if(cycle==4)
		{
			spritedown();
			cycle = 0;
		}
}

void Creature::setX(int x)//good programming practice
{
	m_xpos=x;
}

void Creature::setY(int y)
{
	m_ypos=y;
}

int Creature::getX()
{
	return m_xpos;
}

int Creature::getY()
{
	return m_ypos;
}

int Creature::getHeight()
{
	return creature_height;
}

int Creature::getWidth()
{
	return creature_width;
}



Creature::~Creature()
{	
	al_destroy_bitmap(creaturebit);
}

void Creature::draw()
{																										//this method only draws one region of the bitmap, which allows us to create animation using only one bitmap
	al_draw_bitmap_region(creaturebit, source_x, source_y, source_h, source_w, m_xpos, m_ypos, 0);		//by creating each of these variables we can change which region of the bitmap we are drawing depending on what direction the character is moving
}																										//we can then call these sprite methods in the relevant movement methods

void Creature::spriteright()
{
	source_y=0;							//as our bitmaps are set up with the right animation on the top row, we set the source y to 0 - refer to the assets folder
	source_x = source_x + source_w;		//we can then add the width of the creature the source x position, effectively scrolling through each frame of the animation

	if(source_x==160)					//this is until we reach the end of the bitmap, i could have used allegro methods to retrieve this value, but as i knew the size of the bitmap anyway, i simply put the value itself there instead
	{
		source_x=0;
	}

}

void Creature::spriteleft()
{
	source_y=40;						//similar to above only this time the y position is set to 40 as the next animation frame is set underneath the last
	source_x = source_x + source_w;

	if(source_x==160)
	{
		source_x=0;
	}
}

void Creature::spriteup()				//again the same as before, with the new animation frames being below the last
{
	source_y=80;
	source_x=0;

	if(source_x==160)
	{
		source_x=0;
	}

}

void Creature::spritedown()
{
	source_y=120;
	source_x=0;

	if(source_x==160)
	{
		source_x=0;
	}

}

void Creature::setMaxJumpHeight(int height)
{
	m_max_jump_height = height;
}
