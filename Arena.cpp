/*Christopher Webb
2D Games Programming
Final Project
Arena.cpp*/

#include <conio.h>
#include <iostream>
#include "Arena.h"
using namespace std;

Arena::Arena(string filename, string filename2, string filename3, string filename4, string filename5, int energy_needed)//new
{
	m_filename = filename;
	p_filename = filename2;
	l_filename = filename3;
	e_filename = filename4;
	d_filename = filename5;
	levelbitmap = al_load_bitmap(m_filename.c_str());
	platformbitmap = al_load_bitmap(p_filename.c_str());
	ladderbitmap = al_load_bitmap(l_filename.c_str());
	exitbitmap = al_load_bitmap(e_filename.c_str());
	holebitmap = al_load_bitmap(d_filename.c_str());
	m_energy_needed = energy_needed;
	m_grid_layout.push_back("WWWWWWWWWWWWWWWW");
	m_grid_layout.push_back("W......WW......W");
	m_grid_layout.push_back("WW............LW");
	m_grid_layout.push_back("WL.WWWPPPPWWWWLW");
	m_grid_layout.push_back("WL............LW");
	m_grid_layout.push_back("WL..WWPWWPWW..LW");
	m_grid_layout.push_back("WLW.W......W.WLW");
	m_grid_layout.push_back("WLW...WWWW...W.W");
	m_grid_layout.push_back("WLW...WWWW...W.W");
	m_grid_layout.push_back("WLW.W......W.W.W");
	m_grid_layout.push_back("WL..WWPWWPWW...W");
	m_grid_layout.push_back("WL............LW");
	m_grid_layout.push_back("WLWWWWWPPWWWWWLW");
	m_grid_layout.push_back("WL............LW");
	m_grid_layout.push_back("W......WW......E");
	m_grid_layout.push_back("WWWWWWWWWWWWWWWW");

}

int Arena::getHeight()
{
	return m_grid_layout.size(); //vector.size() returns the amount of objects in the vector, in this case it will give us the height: 15 (vectors start at 0)
}

int Arena::getWidth()
{
	return m_grid_layout[0].length(); //string.length() returns the amount of characters in said string
}

int Arena::getEnergyNeeded()		//retrurns amount of energy needed to finish level, can check for this in main game loop to see if the player can finish
{
	return m_energy_needed;
}

void Arena::render()
{
	for(int h=0; h<getHeight(); h++)				//this method uses a nested loop to loop through element of the vector
	{												//at each coordinate a check is done to see what character is there
		for(int w=0; w<getWidth(); w++)				//dependant on what character is at those coordinates, a block will be drawn.
		{
			if(m_grid_layout[w][h]=='W')
			{
				al_draw_bitmap(levelbitmap, h*al_get_bitmap_height(levelbitmap), w*al_get_bitmap_height(levelbitmap), 0);	
			}
			if(m_grid_layout[w][h]=='P')			//the coordinates must be multiplied by the height/width of the block being drawn, otherwise the blocks would be drawn at the vector position (1,1) and not the pixel coordinates (60,60)
			{
				al_draw_bitmap(platformbitmap, h*al_get_bitmap_height(platformbitmap), w*al_get_bitmap_height(platformbitmap), 0);
			}
			if(m_grid_layout[w][h]=='L')
			{
				al_draw_bitmap(ladderbitmap, h*al_get_bitmap_height(ladderbitmap), w*al_get_bitmap_height(ladderbitmap), 0);
			}
			if(m_grid_layout[w][h]=='E')
			{
				al_draw_bitmap(exitbitmap, h*al_get_bitmap_height(exitbitmap), w*al_get_bitmap_height(exitbitmap), 0);
			}
			if(m_grid_layout[w][h]=='D')
			{
				al_draw_bitmap(holebitmap, h*al_get_bitmap_height(holebitmap), w*al_get_bitmap_height(holebitmap), 0);
			}
		}
	}
		al_convert_mask_to_alpha(exitbitmap, al_map_rgb(255, 0, 255));		//this converts the background of the bitmap to clear,
		al_convert_mask_to_alpha(ladderbitmap, al_map_rgb(255, 0, 255));	//i used pink (RGB 255, 0, 255) as it was not a colour i planned to use in the game
		al_convert_mask_to_alpha(holebitmap, al_map_rgb(255, 0, 255));
}

bool Arena::isWall(int x, int y)																		//these collision checks essentially do the opposite of the drawing method above.
{																										//they divide the pixel position passed in by the block height/width to get the vector position
	if(m_grid_layout[y/al_get_bitmap_height(levelbitmap)][x/al_get_bitmap_height(levelbitmap)]=='W')	//then i can return true if there is a wall there and false if there is not
	{
		return true;
	}
	else
		return false;
}

bool Arena::isPlatform(int x, int y)
{
	if(m_grid_layout[y/al_get_bitmap_height(levelbitmap)][x/al_get_bitmap_height(levelbitmap)]=='P')
	{
		return true;
	}
	else
		return false;
}

bool Arena::isInSameRow(int y1, int y2)															//isInSameRow() is slightly different as it is checking whether 2 y positions are in the same row
{																								//this essentially will allow players to fall back through jump through platforms
	if(y1/al_get_bitmap_height(levelbitmap) == y2/al_get_bitmap_height(levelbitmap))
	{
		return true;
	}
	else
		return false;
}

bool Arena::isLadder(int x, int y)
{
	if(m_grid_layout[y/al_get_bitmap_height(levelbitmap)][x/al_get_bitmap_height(levelbitmap)]=='L')
	{
		return true;
	}
	else
		return false;
}

bool Arena::isExit(int x, int y)
{
	if(m_grid_layout[y/al_get_bitmap_height(levelbitmap)][x/al_get_bitmap_height(levelbitmap)]=='E')
	{
		return true;
	}
	else
		return false;
}

bool Arena::isHole(int x, int y)
{
	if(m_grid_layout[y/al_get_bitmap_height(holebitmap)][x/al_get_bitmap_height(holebitmap)]=='D')
	{
		return true;
	}
	else
		return false;
}

void Arena::createHole(int x, int y)				//this method will work in conjunction with the Hero class's method digHole()
{													//digHole will pass in the hero's bottom coordinates which can then be manipulated here
	int x1 = x/al_get_bitmap_height(holebitmap);	//again doing the opposite of the render methods calculations to get the vector position
	int y1 = y/al_get_bitmap_height(holebitmap);	//the y position must be incremented to get the block below
	y1++;

	m_grid_layout[y1][x1]='D';						//then its simply reassigning the block type to a hole 'D'
}
