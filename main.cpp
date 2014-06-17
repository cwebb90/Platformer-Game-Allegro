/*Chris Webb
Computer Games Programming
Tutorial 7 Exercise 1
main.cpp*/

#include "Creature.h"
#include "Hero.h"
#include "Monster.h"
#include "Arena.h"
#include "Energy.h"
#include "Boulder.h"
#include <iostream>
#include <conio.h>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h> 


using namespace std;

// Function declarations
int initialise_allegro();
void process_events();
void gameoverscreen();
 
// Global variables
enum KEYS_PRESSED_LIST
{
   KEY_ESCAPE, KEY_W, KEY_A, KEY_S, KEY_D, KEY_P, KEY_SPACE, KEY_E, SIZE_OF_KEYS_PRESSED_ENUM
};

int x=0;
int y=0;


bool keys_pressed[SIZE_OF_KEYS_PRESSED_ENUM];

ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_FONT* small_font = NULL;
ALLEGRO_FONT* big_font = NULL;
ALLEGRO_EVENT_QUEUE* event_queue = NULL;

//timer
ALLEGRO_TIMER *timer = NULL;
int frames_per_second = 60;

bool update_logic = true;
bool render_frame = true;
int consecutive_logic_updates = 0;
int max_consecutive_logic_updates = 10;

int main()
{
	al_init();
	initialise_allegro();

	//font stuff
	al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon();  // initialize the True Type Font addon

	small_font = al_load_ttf_font("assets/DejaVuSans.ttf", 15, 0); 
	if (!small_font) 
	{
		cout << "Small Font creation failed" << endl;
		cout << "Press any key to exit" << endl;
		_getch();
		al_destroy_display(display);
		return -1;
	}

	big_font = al_load_ttf_font("assets/DejaVuSans.ttf", 50 ,0); 
	if (!big_font) 
	{
		cout << "Big Font creation failed" << endl;
		cout << "Press any key to exit" << endl;
		_getch();
		al_destroy_font(small_font);
		al_destroy_display(display);
		return -1;
	 }

	//object creations

	Arena* level = new Arena("assets/wall60x60.bmp", "assets/walljumpthrough60x60.bmp", "assets/ladder60x60.bmp", "assets/exitbro.bmp", "assets/wallhole.bmp", 4);//previous arena block size was 63 --- new

	//monster stuff here later

	Hero indy(63, 63, "Indy", 0, "assets/indianajonesmaster40x40.bmp", level, 2);

	Energy* energy1 = new Energy(180, 120, "energy1", "assets/energybar.bmp", level, 0);
	Energy* energy2 = new Energy(780, 120, "energy2", "assets/energybar.bmp", level, 0);
	Energy* energy3 = new Energy(120, 660, "energy3", "assets/energybar.bmp", level, 0);
	Energy* energy4 = new Energy(780, 660, "energy4", "assets/energybar.bmp", level, 0);
	Energy* energy5 = new Energy(360, 840, "energy5", "assets/energybar.bmp", level, 0);
	Energy* energy6 = new Energy(540, 840, "energy6", "assets/energybar.bmp", level, 0);
	Energy* nonpickup = new Energy(0, -10, "dontpickup", "assets/energybar.bmp", level, 0);

	vector<Energy*> energyvector;
	energyvector.push_back(energy1);
	energyvector.push_back(energy2);
	energyvector.push_back(energy3);
	energyvector.push_back(energy4);
	energyvector.push_back(energy5);
	energyvector.push_back(energy6);
	energyvector.push_back(nonpickup);

	Monster* mummie1 = new Monster(320, 560, "brendanfraiser", "assets/mummie40x40.bmp", level, 1, 320, 620);
	Monster* mummie2 = new Monster(620, 560, "dwaynejohnson", "assets/mummie40x40.bmp", level, 1, 320, 620);
	Monster* mummie3 = new Monster(300, 140, "mummiesrdumb", "assets/mummie40x40.bmp", level, 1, 300, 740);

	vector<Monster*> mummievector;
	mummievector.push_back(mummie1);
	mummievector.push_back(mummie2);
	mummievector.push_back(mummie3);

	Monster* spirit1 = new Monster(190, 380, "casper", "assets/evilspiritsaha.bmp", level, 1, 380, 560);
	Monster* spirit2 = new Monster(730, 380, "caspersfriend", "assets/evilspiritsaha.bmp", level, 1, 380, 560);

	vector<Monster*> spiritvector;
	spiritvector.push_back(spirit1);
	spiritvector.push_back(spirit2);

	Monster* anti_indy = new Monster(860,860, "lolwhut", "assets/indianajonesevil.bmp", level, 1, 0, 0);

	Boulder boulder(860, 860, "raiders", "assets/boulder.bmp", level, 1);

	//gameover boolean
	bool gameover = false;

	al_clear_to_color(al_map_rgb(195, 195, 195));

	al_start_timer(timer);

	while(!keys_pressed[KEY_ESCAPE])//game loop
	{
		process_events(); // deal with events, such as mouse and keyboard input


////////////RENDERING/////////////////////////////////////////////////////////////////////////

	if(render_frame)
	{
		render_frame = false;

		level->render();

		indy.draw();//draws hero character

		anti_indy->draw();

		boulder.draw();

		for (int count= 0; count<energyvector.size(); count++)//loops through size of vector and draws each monster in the vector
		{
			energyvector[count]->draw();
		}
		
		for (int count= 0; count<spiritvector.size(); count++)//loops through size of vector and draws each monster in the vector
		{
			spiritvector[count]->draw();
		}

		for (int count= 0; count<mummievector.size(); count++)//loops through size of vector and draws each monster in the vector
		{
			mummievector[count]->draw();
		}

		

		al_draw_textf(big_font , al_map_rgb(255, 0, 0), 55, 0, 0, ": %d", indy.getEnergy());//displays amount of hero lives.
		al_flip_display();
		al_clear_to_color(al_map_rgb(51, 102, 0));

	}

////////////LOGIC/////////////////////////////////////////////////////////////////////////

	if(update_logic)
	{
		update_logic = false;

		indy.fall();

		anti_indy->chase(&indy);

		if(keys_pressed[KEY_SPACE])
		{
			indy.jump();
		}
		
		if(keys_pressed[KEY_W])
		{
			indy.climbup();
		}

		if(keys_pressed[KEY_A])
		{
			indy.left();					
		}

		if(keys_pressed[KEY_S])
		{
			indy.climbdown();
		}

		if(keys_pressed[KEY_D])
		{
			indy.right();
		}

		if(keys_pressed[KEY_E])
		{
			if(indy.getEnergy()>0)
			{
			indy.digHole();
			}
		}


		//monster movements

		
		for (int count= 0; count<mummievector.size(); count++)//loops through vector and calls chase on each monster
		{
			mummievector[count]->mummieMovement();
			mummievector[count]->falldown();
		}

		for (int count= 0; count<spiritvector.size(); count++)//loops through vector and calls chase on each monster
		{
			spiritvector[count]->evilSpiritMovement();
		}

		//monster eaten / respawn

		if(anti_indy->collidesWith(&indy))
		{
		indy.heroeaten();
		}

		for (int count= 0; count<mummievector.size(); count++)// same loop but checks if eaten is true on each
		{
			if(mummievector[count]->collidesWith(&indy)==true && mummievector[count]->isDead()==false)//if true, heroeaten is called as well as respawn, decrementing the lives and repositioning the hero
			{
				indy.heroeaten();

				indy.respawnhero();
			}
		}
		
		for (int count= 0; count<spiritvector.size(); count++)// same loop but checks if eaten is true on each
		{
			if(spiritvector[count]->collidesWith(&indy)==true)//if true, heroeaten is called as well as respawn, decrementing the lives and repositioning the hero
			{
				indy.heroeaten();

				indy.respawnhero();

			}
		}

		//energy bar collision checks

		for (int count= 0; count<energyvector.size(); count++)// same loop but checks if eaten is true on each
		{
			if(energyvector[count]->collidesWith(&indy)==true)//if true, heroeaten is called as well as respawn, decrementing the lives and repositioning the hero
			{
				indy.gainEnergy();

				energyvector[count]->movetoCount();//replacing them to top

			}
		}

		if(boulder.collidesWith(&indy))
		{
		boulder.pushright(&indy);
		}

		if(indy.getEnergy()>=0)//if the hero lives falls below zero, game ends.
		{
			gameover = false;		
		}
		if ((indy.leavemap()==true) && (boulder.boulderstate()==true))
		{
			gameover = true;
		}
		else if(indy.getEnergy()<0)
		{
			gameover = true;
		}



		if(gameover==true)
		{
			gameoverscreen();
			al_rest(3);
			break;
		}
	}
}




	//cleaning

	//monster delete loop

	delete level;
	al_destroy_display(display);
	al_destroy_font(small_font);
	al_destroy_font(big_font);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);


	return 0;
}


////////////////////////////////////////////////////////////////////////

void process_events()
{
	ALLEGRO_EVENT allegro_event;

	// get the next event from the event queue
	bool got_event = al_get_next_event(event_queue, &allegro_event);

	if(got_event == false) return; // no event waiting

	// test if a key is pressed down, update global keypress array
	if(allegro_event.type == ALLEGRO_EVENT_KEY_DOWN) 
	{
		switch(allegro_event.keyboard.keycode) 
		{
		case ALLEGRO_KEY_ESCAPE:
			keys_pressed[KEY_ESCAPE] = true;
			break;
		case ALLEGRO_KEY_W:
			keys_pressed[KEY_W] = true;
			break;
		case ALLEGRO_KEY_A:
			keys_pressed[KEY_A] = true;
			break;
		case ALLEGRO_KEY_S:
			keys_pressed[KEY_S] = true;
			break;
		case ALLEGRO_KEY_D:
			keys_pressed[KEY_D] = true;
			break;
		case ALLEGRO_KEY_P:
			keys_pressed[KEY_P] = true;
			break;
		case ALLEGRO_KEY_SPACE:
			keys_pressed[KEY_SPACE] = true;
			break;
		case ALLEGRO_KEY_E:
			keys_pressed[KEY_E] = true;
			break;
		}
	}	
	// Test if a key is let go, update global keypress array
	else if(allegro_event.type == ALLEGRO_EVENT_KEY_UP) 
	{
		switch(allegro_event.keyboard.keycode) 
		{
		case ALLEGRO_KEY_ESCAPE:
			keys_pressed[KEY_ESCAPE] = false;
			break;
		case ALLEGRO_KEY_W:
			keys_pressed[KEY_W] = false;
			break;
		case ALLEGRO_KEY_A:
			keys_pressed[KEY_A] = false;
			break;
		case ALLEGRO_KEY_S:
			keys_pressed[KEY_S] = false;
			break;
		case ALLEGRO_KEY_D:
			keys_pressed[KEY_D] = false;
			break;
		case ALLEGRO_KEY_P:
			keys_pressed[KEY_P] = false;
			break;
		case ALLEGRO_KEY_SPACE:
			keys_pressed[KEY_SPACE] = false;
			break;
		case ALLEGRO_KEY_E:
			keys_pressed[KEY_E] = false;
			break;
		}
	}

		else if(got_event && allegro_event.type == ALLEGRO_EVENT_TIMER)
	{

	// if logic taking too long ensures frame is rendered occasionally and input is responsive
	// this essentially ignores logic updates if there are a lot of consecutive ones

		if(++consecutive_logic_updates < max_consecutive_logic_updates)
		{
			update_logic = true;
		}

	// only render frame if no events left to process
		if(al_is_event_queue_empty(event_queue))
		{
			render_frame = true;
			consecutive_logic_updates=0;
		}
	}

}

////////////////////////////////////////////////////////////////////////

int initialise_allegro()
{
	if(!al_init()) 
	{
		cout << "Allegro initialisation failed" << endl;
		cout << "Press any key to exit" << endl;
		_getch();
		return -1;
	}

	// tell allegro program needs keyboard input
	if(!al_install_keyboard()) 
	{
		cout << "Keyboard initialisation failed" << endl;
		cout << "Press any key to exit" << endl;
		_getch();
		return -1;
	}
	

	// initialise keypress array
	for(int i=0; i < SIZE_OF_KEYS_PRESSED_ENUM; i++) 
	{
		keys_pressed[i] = false;
	}

	display = al_create_display(960, 960); // create a display window of size 640 by 480

	if(!display) 
	{
		cout << "Display initialisation failed" << endl;
		cout << "Press any key to exit" << endl;
		_getch();
		return -1;
	}

 	al_set_window_position(display, 10, 10); // position display window on screen

	if(!al_init_image_addon())//checks for allegro bitmap stuff
	{
		cout << "Bitmap initialisation failed" << endl;
		cout << "Press any key to exit" << endl;
		_getch();
		return -1;
	}



	// need to create an event queue so allegro can respond to events, such as keypresses
	event_queue = al_create_event_queue();

	if(!event_queue) 
	{
		cout << "Event queue creation failed" << endl;
		cout << "Press any key to exit" << endl;
		_getch();
		al_destroy_display(display);
		return -1;
	}

	//stuff an thingz
	al_init_primitives_addon(); 
	
	timer = al_create_timer(1.0 / frames_per_second);
 
	// register  display with the event queue, so display change events can be processed
	al_register_event_source(event_queue, al_get_display_event_source(display));

	// register keyboard with the event queue, so keypress events can be processed
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	return 0;
}

void gameoverscreen()
{
	al_clear_to_color(al_map_rgb(195, 195, 195));
	al_draw_textf(big_font , al_map_rgb(0, 0, 0), 160, 160, 0, "GAME OVER");
	al_flip_display();
}
