/*Christopher Webb
2D Games Programming
Final Project
Creature.h*/

#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <string>
#include "Arena.h"
using namespace std;

class Creature
{
protected:
	int m_xpos;						//x and y positions of the creature
	int m_ypos;
	string m_name;					//allows naming of object
	string m_filename;				//allows bitmap to be loaded in using a string as a filename
	Arena* m_arena;					//allows access to collision methods and coordinates
	ALLEGRO_BITMAP* creaturebit;	//bitmap for the creature
	int creature_width;				//need to calculate width and height to get proper coordinates to check collision, 
	int creature_height;			//for example checking the top right corner of the bitmap would be x + width, y + height
	int m_speed;					//having a speed attribute allows for different speed for different creatures, for example the hero can move faster than the monsters

	//jumping attributes
	bool m_is_jumping;				//if this is set to true then the character can jump
	int m_max_jump_height;			//these two variables are essential for jumping
	int m_current_jump_height;		//can increment this variable until it is equal to m_max_jump_height

	//spritesheet attributes

	float source_x;					//these variables are used in the draw method al_draw_bitmap_region,
	float source_y;					//and are needed to create animation using a sprite sheet method
	float source_w;					
	float source_h;
	int cycle;						//this variable allows me to delay when the animation is played, so it does not animate too fast

public:

//constructor/destructor
	Creature(int xpos, int ypos, string name, string filename, Arena* arena, int speed);
	~Creature();

//movement
	void left();
	void right();
	void up();
	void down();
	void climbup();
	void climbdown();

//sets/gets
	void setX(int x);
	void setY(int y);
	int getX();
	int getY();
	int getWidth();
	int getHeight();
	
//draws/frames
	void draw();
	void addFrame(string f_filename);
	void nextFrame();

//jumping
	void setMaxJumpHeight(int height);
	void jump();
	void fall();
	
//spritesheet
	void spriteleft();
	void spriteright();
	void spriteup();
	void spritedown();
};
