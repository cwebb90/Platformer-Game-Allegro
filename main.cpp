/*Christopher Webb
2D Games Programming
Final Project
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
   KEY_ESCAPE, KEY_W, KEY_A, KEY_S, KEY_D, KEY_P, KEY_SPACE, KEY_E, KEY_C, SIZE_OF_KEYS_PRESSED_ENUM
};

int x=0;
int y=0;

bool keys_pressed[SIZE_OF_KEYS_PRESSED_ENUM];

ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_FONT* big_font = NULL;
ALLEGRO_EVENT_QUEUE* event_queue = NULL;

//allegro timer
ALLEGRO_TIMER *timer = NULL;		//using an allegro timer allows us to make the game run smoothly
int frames_per_second = 60;			//it allows us to create a stable framerate by associating it with event queues

bool update_logic = true;			//these variables allow us to update the game logic and the game rendering seperately 
bool render_frame = true;			
int consecutive_logic_updates = 0;
int max_consecutive_logic_updates = 10;

//main starts here
int main()
{
	al_init();				//i need to call these first as my entire game relies upon the allegro library
	initialise_allegro();

	//font stuff
	al_init_font_addon();	//these allow me to use text and fonts in the game
    al_init_ttf_addon();	

	big_font = al_load_ttf_font("assets/DejaVuSans.ttf", 50 ,0); //this checks that the fonts have been loaded and displays an error message if they have not been loaded correctly
	if (!big_font) 
	{
		cout << "Big Font creation failed" << endl;
		cout << "Press any key to exit" << endl;
		_getch();
		al_destroy_font(big_font);
		al_destroy_display(display);
		return -1;
	 }

	ALLEGRO_BITMAP* endscreen = al_load_bitmap("assets/LOSESCREEN.bmp"); // i created these two simple bitmaps to display at the end of the game, depending on whether you win or lose
	ALLEGRO_BITMAP* winscreen = al_load_bitmap("assets/WINSCREEN.bmp");
	ALLEGRO_BITMAP* background = al_load_bitmap("assets/background.bmp");// bitmap to display behind the level

	//Object creations start here

	Arena* level = new Arena("assets/wall60x60.bmp", "assets/walljumpthrough60x60.bmp", "assets/ladder60x60.bmp", "assets/exitbro.bmp", "assets/wallhole.bmp", 4);

	Hero indy(63, 63, "Indy", 0, "assets/indianajonesmaster40x40.bmp", level, 2);

	Energy* energy1 = new Energy(180, 120, "energy1", "assets/energybar.bmp", level, 0);
	Energy* energy2 = new Energy(780, 120, "energy2", "assets/energybar.bmp", level, 0);
	Energy* energy3 = new Energy(120, 660, "energy3", "assets/energybar.bmp", level, 0);
	Energy* energy4 = new Energy(780, 660, "energy4", "assets/energybar.bmp", level, 0);
	Energy* energy5 = new Energy(360, 840, "energy5", "assets/energybar.bmp", level, 0);
	Energy* energy6 = new Energy(540, 840, "energy6", "assets/energybar.bmp", level, 0);
	Energy* energy7 = new Energy(450, 540, "energy7", "assets/energybar.bmp", level, 0);
	Energy* energy8 = new Energy(240, 240, "energy8", "assets/energybar.bmp", level, 0);
	Energy* energy9 = new Energy(660, 240, "energy9", "assets/energybar.bmp", level, 0);
	Energy* energy10 = new Energy(460, 360, "energy10", "assets/energybar.bmp", level, 0);
	Energy* nonpickup = new Energy(0, -10, "dontpickup", "assets/energybar.bmp", level, 0);

	vector<Energy*> energyvector;
	energyvector.push_back(energy1);
	energyvector.push_back(energy2);
	energyvector.push_back(energy3);
	energyvector.push_back(energy4);
	energyvector.push_back(energy5);
	energyvector.push_back(energy6);
	energyvector.push_back(energy7);
	energyvector.push_back(energy8);
	energyvector.push_back(energy9);
	energyvector.push_back(energy10);
	energyvector.push_back(nonpickup);

	Monster* mummie1 = new Monster(320, 560, "brendanfraiser", "assets/mummie40x40.bmp", level, 1, 320, 620);
	Monster* mummie2 = new Monster(620, 560, "dwaynejohnson", "assets/mummie40x40.bmp", level, 1, 320, 620);
	Monster* mummie3 = new Monster(180, 140, "moviereferences", "assets/mummie40x40.bmp", level, 1, 180, 800);
	Monster* mummie4 = new Monster(800, 140, "run out", "assets/mummie40x40.bmp", level, 1, 180, 800);
	Monster* mummie5 = new Monster(60, 860, "of funny", "assets/mummie40x40.bmp", level, 1, 60, 380);
	Monster* mummie6 = new Monster(860, 860, "names for monsters", "assets/mummie40x40.bmp", level, 1, 540, 860);

	vector<Monster*> mummievector;
	mummievector.push_back(mummie1);
	mummievector.push_back(mummie2);
	mummievector.push_back(mummie3);
	mummievector.push_back(mummie4);
	mummievector.push_back(mummie5);
	mummievector.push_back(mummie6);

	Monster* spirit1 = new Monster(190, 380, "casper", "assets/evilspiritsaha.bmp", level, 1, 380, 560);
	Monster* spirit2 = new Monster(730, 380, "caspersfriend", "assets/evilspiritsaha.bmp", level, 1, 380, 560);

	vector<Monster*> spiritvector;
	spiritvector.push_back(spirit1);
	spiritvector.push_back(spirit2);

	Monster* anti_indy = new Monster(860,860, "ihavenoimagination", "assets/indianajonesevil.bmp", level, 1, 0, 0);

	Boulder boulder(860, 860, "raiders", "assets/boulder.bmp", level, 1);

	//gameover boolean, this allows me to end the game with a relevant message, once set to true
	bool gameover = false;

	//////ADDED AFTER DESIGN//////////this timer was added after testing the game, it did not have much challenge, thus adding a time limit gives the game something extra
	int counter = 60;				//it consists of two ints, counter and time, time is the number of seconds the player has to complete the game
	int time = 60;					//counter is the amount of frames per second the game has, so we can decrement time every 60 frames.
	/////////////////////////////////explained more in the main.

	al_start_timer(timer);

	while(!keys_pressed[KEY_ESCAPE])//game loop
	{
		process_events(); // deals with events, such as mouse and keyboard input


////////////RENDERING/////////////////////////////////////////////////////////////////////////

	if(render_frame)
	{
		render_frame = false;

		level->render();	//draws level

		indy.draw();		//draws hero character

		anti_indy->draw();	//draws evil chasing monster

		boulder.draw();		//draws boulder

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

		

		al_draw_textf(big_font , al_map_rgb(0, 0, 0), 55, 0, 0, ": %d", indy.getEnergy());//displays amount of hero energy.
		al_draw_textf(big_font , al_map_rgb(0, 0, 0), 200, 0, 0,"Score: %d", indy.getScore());//added after testing, not in initial design
		al_draw_textf(big_font , al_map_rgb(0, 0, 0), 500, 0, 0, "Time Left: %d", time);//added after testing, not in initial design
		al_flip_display();	//draws to screen
		al_draw_bitmap(background,0,0,0);//draws background bitmap
	}

////////////LOGIC/////////////////////////////////////////////////////////////////////////

	if(update_logic)
	{
		update_logic = false;

		indy.fall(); //constantly calls downforce on the player character

		anti_indy->chase(&indy); //calls chase for the monster that chases the character

		///timer related code, added after design///
		counter--;								 ///counter is decremented every frame, as they are 60 per second
		if (counter==0)							 ///once this reaches 0 one second has passed
		{										 ///then we can decrement the time counter by one
			time--;								 ///then  reset the frames per second so this can happen again and again every second
			counter=60;
		}
		///////////////////////////////////////////

//////////keyboard methods/////////////

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
			if(indy.getEnergy()>0)	//checks to see if the player has enough energy
			{
			indy.digHole();			//if so, player can dig a hole
			}
		}

		if(keys_pressed[KEY_C]) // this keypress was added after design purely for testing purposes
		{					
			indy.gainEnergy();
		}

////////monster checks////////////

		for (int count= 0; count<mummievector.size(); count++)//loops through vector and calls methods for each monster
		{
			if(mummievector[count]->isDead()==false)//only call methods if the mummie is alive
			{
			mummievector[count]->mummieMovement();
			mummievector[count]->falldown(&indy); //mummies can fall down holes dug by the player, so it is checked for here
			}
		}

		for (int count= 0; count<spiritvector.size(); count++)//loops through vector and calls methods for each monster
		{
			spiritvector[count]->evilSpiritMovement();
		}

		if(anti_indy->collidesWith(&indy)) //this character cannot be trapped or defeated and must simply be avoided
		{
			indy.heroeaten();
		}

		for (int count= 0; count<mummievector.size(); count++)
		{
			if(mummievector[count]->collidesWith(&indy)==true && mummievector[count]->isDead()==false)//if collision has occured and the monster is not dead
			{
				indy.heroeaten();	//call heroeaten method

				//indy.respawnhero(); redundant method
			}
		}
		
		for (int count= 0; count<spiritvector.size(); count++)
		{
			if(spiritvector[count]->collidesWith(&indy)==true)//if collision has occurred
			{
				indy.heroeaten();	//call heroeaten method

				//indy.respawnhero(); redundant method

			}
		}

////////energy checks////////////

		for (int count= 0; count<energyvector.size(); count++)
		{
			if(energyvector[count]->collidesWith(&indy)==true)//if collision has occurred
			{
				indy.gainEnergy();

				energyvector[count]->movetoCount();//replacing them to top

			}
		}

///////endgame checks//////////

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

		//timer check, added after design
		if(time==0)
		{
			gameover = true;
		}



		if(gameover==true)
		{
			if(indy.leavemap()==true) //if left map, player wins and win screen is shown
			{
				al_draw_bitmap(winscreen, 0,0,0);
				al_flip_display();
				al_rest(3);
				break;
			}
			else					//otherwise the player has lost the game and lose screen is shown
			al_draw_bitmap(endscreen, 0,0,0);
			al_flip_display();
			al_rest(3);
			break;
		}
	
	}
	}





	//cleaning, at the end of the game delete these objects

	for (int count= 0; count<mummievector.size(); count++)
	{
		delete mummievector[count];
	}
	for (int count= 0; count<spiritvector.size(); count++)
	{
		delete spiritvector[count];
	}
		for (int count= 0; count<energyvector.size(); count++)
	{
		delete energyvector[count];
	}

	delete level;
	al_destroy_display(display);
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
		case ALLEGRO_KEY_C:
			keys_pressed[KEY_C] = true;
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
		case ALLEGRO_KEY_C:
			keys_pressed[KEY_C] = false;
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

	display = al_create_display(960, 960); // create a display window of size 960 by 960

	if(!display) 
	{
		cout << "Display initialisation failed" << endl;
		cout << "Press any key to exit" << endl;
		_getch();
		return -1;
	}

 	al_set_window_position(display, 0, 0); // position display window on screen

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

	al_init_primitives_addon(); 
	
	timer = al_create_timer(1.0 / frames_per_second);
 
	// register  display with the event queue, so display change events can be processed
	al_register_event_source(event_queue, al_get_display_event_source(display));

	// register keyboard with the event queue, so keypress events can be processed
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	return 0;
}
