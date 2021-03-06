#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"
#include <vector>
using namespace std;

extern CStopWatch g_swTimer;
extern bool g_bQuitGame;

// Enumeration to store the control keys that your game will have
enum EKEYS
{
	K_UP,
	K_DOWN,
	K_LEFT,
	K_RIGHT,
	K_ESCAPE,
	K_SPACE,
	K_RETURN,
	K_1,
	K_2,
	K_3,
	K_4,
	K_5,
	K_A,
	K_COUNT
};

// Enumeration for the different screen states
enum EGAMESTATES
{
    S_SPLASHSCREEN,
	S_STARTMENU,
	S_INSTRUCTIONS,
	S_CHARACTERCREATION,
    S_GAME,
	S_WINSCREEN,
	S_LOSESCREEN,
    S_COUNT
};

// struct for the game character
struct SGameChar
{
    COORD m_cLocation;
};

void init        ( void );      // initialize your variables, allocate memory, etc
void getInput    ( void );      // get input from player
void update      ( double dt ); // update the game and the state of the game
void render      ( void );      // renders the current state of the game to the console
void shutdown    ( void );      // do clean up, free memory

void splashScreenWait();    // waits for time to pass in splash screen
void gameplay();            // gameplay logic
void moveCharacter();       // moves the character, collision detection, physics, etc
void processUserInput();    // checks if you should change states or do something else with the game, e.g. pause, exit
bool collision(char collider); // use for collision detection
void huggerdata();          // movement and collision for hugger enemy
void gunnerdata();          // movement and collision for gunner enemy
void enemybullet();         // enemy bullets and collision
void playershoot();         // player bullets and collision
void respawn();             // respawn coordinates
void saveProgression();     // save game progression
void loadProgression();     // load game progression
void clearScreen();         // clears the current screen and draw from scratch 
void renderSplashScreen();  // renders the splash screen
void renderStartMenu();     // renders the start menu
void renderInstructions();  // renders the instructions
void renderCharacterCreation(); // renders the character creation
void renderGame();          // renders the game stuff
void renderMap();           // renders the map to the buffer first
void renderCharacter();     // renders the character into the buffer
void renderEntities();      // renders the hugger, gunner, enemy and player bullets
void renderWin();           // renders the win screen
void renderLose();          // renders the lose screen
void renderFramerate();     // renders debug information, frame rate, elapsed time, etc
void renderToScreen();      // dump the contents of the buffer to the screen, one frame worth of game

void loseAuto(int argc, char** argv);
int PlayAudio(int argc, char* argv[]);

// define variables

struct melee {
	int hp = 3;
};

struct ranged {
	int hp = 1;
};
#endif // _GAME_H