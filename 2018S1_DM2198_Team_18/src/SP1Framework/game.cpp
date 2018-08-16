// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

int money;

bool shop = false;
bool levelOne = true;
bool levelTwo = false;
bool levelThree = false;
bool levelFour = false;
bool playerRespawn = false;

double  g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT];

// Game specific variables here
SGameChar   g_sChar;
SGameChar   g_sEnemy;
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime; // this is to prevent key bouncing, so we won't trigger keypresses more than once

// Console object
Console g_Console(109, 30, "SP1 Framework");
char** map = new char*[30];

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
    // Set precision for floating point output
    g_dElapsedTime = 0.0;
    g_dBounceTime = 0.0;

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

    /*g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 2;
    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y / 2;*/
	g_sChar.m_cLocation.X = 5;
	g_sChar.m_cLocation.Y = 2;
	g_sEnemy.m_cLocation.X = 5;
	g_sEnemy.m_cLocation.Y = 16;
    g_sChar.m_bActive = true;
    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");

	for (int i = 0; i < g_Console.getConsoleSize().Y; i++)
	{
		map[i] = new char[g_Console.getConsoleSize().X];
	}
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown( void )
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();
}

//--------------------------------------------------------------
// Purpose  : Getting all the key press states
//            This function checks if any key had been pressed since the last time we checked
//            If a key is pressed, the value for that particular key will be true
//
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput( void )
{    
    g_abKeyPressed[K_UP]     = isKeyPressed(VK_UP);
    g_abKeyPressed[K_DOWN]   = isKeyPressed(VK_DOWN);
    g_abKeyPressed[K_LEFT]   = isKeyPressed(VK_LEFT);
    g_abKeyPressed[K_RIGHT]  = isKeyPressed(VK_RIGHT);
    g_abKeyPressed[K_SPACE]  = isKeyPressed(VK_SPACE);
    g_abKeyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{
    // get the delta time
    g_dElapsedTime += dt;
    g_dDeltaTime = dt;

    switch (g_eGameState)
    {
        case S_SPLASHSCREEN : splashScreenWait(); // game logic for the splash screen
            break;
        case S_GAME: gameplay(); // gameplay logic when we are in the game
            break;
    }
}
//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
        case S_SPLASHSCREEN: renderSplashScreen();
            break;
        case S_GAME: renderGame();
            break;
    }
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 3.0) // wait for 3 seconds to switch to game mode, else do nothing
        g_eGameState = S_GAME;
}

melee hugger;
ranged gunner;
void renderEnemies()
{
	// Draw the location of the enemies
	WORD charE_Color = 0x0C;
	
	g_Console.writeToBuffer(g_sEnemy.m_cLocation, (char)128, charE_Color);
}

void enemydata() {
	bool fooeyhappened;
	double enemybouncetime = g_dElapsedTime, up, left, down, right, min_double;
	melee hugger;
	ranged gunner;

		fooeyhappened = false;
		if (enemybouncetime > g_dElapsedTime)
			return;

		up = 99.0; left = 99.0; down = 99.0; right = 99.0;

		if (g_sEnemy.m_cLocation.Y - 1 == ' ') {
			up = sqrt(pow(g_sChar.m_cLocation.X - (g_sEnemy.m_cLocation.X), 2) + pow(g_sChar.m_cLocation.Y - (g_sEnemy.m_cLocation.Y - 1), 2));
		}
		if (g_sEnemy.m_cLocation.X - 1 == ' ') {
			left = sqrt(pow(g_sChar.m_cLocation.X - (g_sEnemy.m_cLocation.X - 1), 2) + pow(g_sChar.m_cLocation.Y - (g_sEnemy.m_cLocation.Y), 2));
		}
		if (g_sEnemy.m_cLocation.Y + 1 == ' ') {
			down = sqrt(pow(g_sChar.m_cLocation.X - (g_sEnemy.m_cLocation.X), 2) + pow(g_sChar.m_cLocation.Y - (g_sEnemy.m_cLocation.Y + 1), 2));
		}
		if (g_sEnemy.m_cLocation.X + 1 == ' ') {
			right = sqrt(pow(g_sChar.m_cLocation.X - (g_sEnemy.m_cLocation.X + 1), 2) + pow(g_sChar.m_cLocation.Y - (g_sEnemy.m_cLocation.Y), 2));
		}
		min_double = min(min(up, down), min(left, right));
		if (min_double == up && min_double == left) {
			if (sqrt(pow(g_sChar.m_cLocation.X - g_sEnemy.m_cLocation.X, 2)) > sqrt(pow(g_sChar.m_cLocation.Y - g_sEnemy.m_cLocation.Y, 2))) {
				g_sEnemy.m_cLocation.X--;
			}
			else g_sEnemy.m_cLocation.Y--;
		}
		if (min_double == up && min_double == right) {
			if (sqrt(pow(g_sChar.m_cLocation.X - g_sEnemy.m_cLocation.X, 2)) > sqrt(pow(g_sChar.m_cLocation.Y - g_sEnemy.m_cLocation.Y, 2))) {
				g_sEnemy.m_cLocation.X++;
			}
			else g_sEnemy.m_cLocation.Y--;
		}
		if (min_double == down && min_double == right) {
			if (sqrt(pow(g_sChar.m_cLocation.X - g_sEnemy.m_cLocation.X, 2)) > sqrt(pow(g_sChar.m_cLocation.Y - g_sEnemy.m_cLocation.Y, 2))) {
				g_sEnemy.m_cLocation.X++;
			}
			else g_sEnemy.m_cLocation.Y++;
		}
		if (min_double == down && min_double == left) {
			if (sqrt(pow(g_sChar.m_cLocation.X - g_sEnemy.m_cLocation.X, 2)) > sqrt(pow(g_sChar.m_cLocation.Y - g_sEnemy.m_cLocation.Y, 2))) {
				g_sEnemy.m_cLocation.X--;
			}
			else g_sEnemy.m_cLocation.Y++;
		}

		fooeyhappened = true;

		if (fooeyhappened)
			enemybouncetime = g_dElapsedTime + 0.25; // enemies act every 1/4 seconds
}

void gameplay()            // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
                        // sound can be played here too.
	enemydata();
}

void inventory()		// handles inventory, inventory[0] contains money, inventory[1] && inventory[2] contains the 2 weapons held
{
	vector<int> inventory;
	inventory.push_back(10);
	inventory.push_back(0);
	inventory.push_back(0);
	if (money >= 1) {
		inventory[0] += money;
		money = 0;
	}
}

void moveCharacter()
{
	bool bSomethingHappened = false;
	if (g_dBounceTime > g_dElapsedTime)
		return;

	// Updating the location of the character based on the key press
	// providing a beep sound whenver we shift the character
	if (g_abKeyPressed[K_UP] && map[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == ' ')
	{
		//Beep(1440, 30);
		g_sChar.m_cLocation.Y--;
		bSomethingHappened = true;

	}
	if (g_abKeyPressed[K_LEFT] && map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == ' ')
	{
		//Beep(1440, 30);
		g_sChar.m_cLocation.X--;
		bSomethingHappened = true;
	}
	if (g_abKeyPressed[K_DOWN] && map[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == ' ')
	{
		//Beep(1440, 30);
		g_sChar.m_cLocation.Y++;
		bSomethingHappened = true;
	}
	if (g_abKeyPressed[K_RIGHT] && map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == ' ')
	{
		//Beep(1440, 30);
		g_sChar.m_cLocation.X++;
		bSomethingHappened = true;
	}
	if (g_abKeyPressed[K_SPACE])
	{
		g_sChar.m_bActive = !g_sChar.m_bActive;
		bSomethingHappened = true;
	}

	if (bSomethingHappened)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime = g_dElapsedTime + 0.125; // 125ms should be enough
	}

	if (map[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == 'S' || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == 'S' || map[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == 'S' || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == 'S' ||
		map[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == 'H' || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == 'H' || map[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == 'H' || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == 'H' ||
		map[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == 'O' || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == 'O' || map[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == 'O' || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == 'O' ||
		map[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == 'P' || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == 'P' || map[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == 'P' || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == 'P')
	{
		shop = true;
	}
	else
	{
		shop = false;
	}
	if (levelOne == true)
	{
		if (map[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == '%' || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == '%' || map[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == '%' || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == '%')
		{
			levelOne = false;
			levelTwo = true;
			playerRespawn = true;
		}
	}
	else if (levelTwo == true)
	{
		playerRespawn = false;

		if (map[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == '%' || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == '%' || map[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == '%' || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == '%')
		{
			levelTwo = false;
			levelThree = true;
			playerRespawn = true;
		}
	}
	else if (levelThree == true)
	{
		playerRespawn = false;

		if (map[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == '%' || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == '%' || map[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == '%' || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == '%')
		{
			levelThree = false;
			levelFour = true;
			playerRespawn = true;
		}
	}
	else if (levelFour == true)
	{
		playerRespawn = false;

		if (map[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == '%' || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == '%' || map[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == '%' || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == '%')
		{
			g_bQuitGame = true;
		}

	}
	if (playerRespawn == true)
	{
		g_sChar.m_cLocation.X = 5;
		g_sChar.m_cLocation.Y = 2;
	}
}
void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_abKeyPressed[K_ESCAPE])
        g_bQuitGame = true;    
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x0F);
}

void renderSplashScreen()  // renders the splash screen
{
    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - 9;
    g_Console.writeToBuffer(c, "A game in 3 seconds", 0x03);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 20;
    g_Console.writeToBuffer(c, "Press <Space> to change character colour", 0x09);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 9;
    g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x09);
}

void renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
	renderEnemies();
}

void renderMap()
{
	//Render Map
	string level1;
	ifstream level1File;
	COORD c;
	int i = 0;
	int a = 0;

	if (levelOne == true)
	{
		level1File.open("Level1.txt");
		if (level1File.is_open())
		{
			while (getline(level1File, level1))
			{
				for (a = 0; a < level1.length(); a++)
				{
					if (level1[a] == '#')
					{
						level1[a] = 223;
					}
					else if (level1[a] == '@')
					{
						level1[a] = 219;
					}
					map[i][a] = level1[a];
				}
				c.X = 0;
				c.Y = i;
				i++;
				g_Console.writeToBuffer(c, level1, 0x09);
			}
		}
		level1File.close();
	}
	if (levelTwo == true)
	{
		string level2;
		ifstream level2File;
		i = 0;
		level2File.open("Level2.txt");
		if (level2File.is_open())
		{
			while (getline(level2File, level2))
			{
				for (a = 0; a < level2.length(); a++)
				{
					if (level2[a] == '#')
					{
						level2[a] = 223;
					}
					if (level2[a] == '@')
					{
						level2[a] = 219;
					}
					map[i][a] = level2[a];
				}
				c.X = 0;
				c.Y = i;
				i++;
				g_Console.writeToBuffer(c, level2, 0x09);
			}
		}
		level2File.close();
	}
	if (levelThree == true)
	{
		string level3;
		ifstream level3File;
		i = 0;
		level3File.open("Level3.txt");
		if (level3File.is_open())
		{
			while (getline(level3File, level3))
			{
				for (a = 0; a < level3.length(); a++)
				{
					if (level3[a] == '#')
					{
						level3[a] = 223;
					}
					if (level3[a] == '@')
					{
						level3[a] = 219;
					}
					map[i][a] = level3[a];
				}
				c.X = 0;
				c.Y = i;
				i++;
				g_Console.writeToBuffer(c, level3, 0x09);
			}
		}
		level3File.close();
	}
	if (levelFour == true)
	{
		string level4;
		ifstream level4File;
		i = 0;
		level4File.open("Level4.txt");
		if (level4File.is_open())
		{
			while (getline(level4File, level4))
			{
				for (a = 0; a < level4.length(); a++)
				{
					if (level4[a] == '#')
					{
						level4[a] = 223;
					}
					if (level4[a] == '@')
					{
						level4[a] = 219;
					}
					map[i][a] = level4[a];
				}
				c.X = 0;
				c.Y = i;
				i++;
				g_Console.writeToBuffer(c, level4, 0x09);
			}
		}
		level4File.close();
	}
	//Render Inventory
	string inventory;
	ifstream inventoryFile;
	i = 0;

	inventoryFile.open("Inventory.txt");
	if (inventoryFile.is_open())
	{
		while (getline(inventoryFile, inventory))
		{
			for (a = 0; a < inventory.length(); a++)
			{
				if (inventory[a] == '#')
				{
					inventory[a] = 223;
				}
				else if (inventory[a] == '@')
				{
					inventory[a] = 219;
				}
				else if (inventory[a] == '$')
				{
					inventory[a] = ' ';
				}
				else if (inventory[a] == '1')
				{
					inventory[a] = 251;
				}
				else if (inventory[a] == '2')
				{
					inventory[a] = 'x';
				}
				else if (inventory[a] == '3')
				{
					inventory[a] = 'x';
				}
				else if (inventory[a] == '4')
				{
					inventory[a] = 'x';
				}
				else if (inventory[a] == '5')
				{
					inventory[a] = 'x';
				}
				else if (inventory[a] == '6')
				{
					inventory[a] = 'x';
				}
			}
			c.X = 0;
			c.Y = 17 + i;
			i++;
			g_Console.writeToBuffer(c, inventory, 0x00 + i);
		}
	}
	inventoryFile.close();

	if (shop == true)
	{
		//Render Shop
		string shop;
		ifstream shopFile;
		i = 0;

		shopFile.open("Shop.txt");
		if (shopFile.is_open())
		{
			while (getline(shopFile, shop))
			{
				for (a = 0; a < shop.length(); a++)
				{
					if (shop[a] == '#')
					{
						shop[a] = 223;
					}
					else if (shop[a] == '@')
					{
						shop[a] = 219;
					}
				}
				c.X = 22;
				c.Y = 17 + i;
				i++;
				g_Console.writeToBuffer(c, shop, 0x00 + i);
			}
		}
		shopFile.close();
	}
}

void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x0F;
    if (g_sChar.m_bActive)
    {
        charColor = 0x0E;
    }
    g_Console.writeToBuffer(g_sChar.m_cLocation, (char)14, charColor);
}

void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = g_Console.getConsoleSize().X - 9;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x59);
}
void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}
