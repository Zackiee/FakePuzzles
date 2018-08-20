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

int money = 0, x = 0;
int shootdirection[128] = { 0, };
int playerdirection[32] = { 0, };

bool hq = true;
bool inven = true;
bool shop = false;
bool levelA = false;
bool levelB = false;
bool levelC = false;
bool levelD = false;
bool levelAgem = false;
bool levelBgem = false;
bool levelCgem = false;
bool levelDgem = false;
bool hqSpawn = false;
bool playerRespawn = false;

bool firstChar = true;
bool secondChar = false;
bool thirdChar = false;
bool fourthChar = false;
bool fifthChar = false;
bool sixthChar = false;

bool equipPistol = true;
bool equipSmg = false;
bool equipRifle = false;
bool equipSniper = false;
bool equipMinigun = false;

string playerName;

double  g_dElapsedTime;
double  huggerbouncetime = g_dElapsedTime;
double  gunnerbouncetime = g_dElapsedTime;
double  bulletbouncetime = g_dElapsedTime;
double	playerbulletshot = g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT];

// Game specific variables here
SGameChar   g_sChar;
SGameChar   g_sHugger;
SGameChar	g_sGunner;
SGameChar	g_sBullets[128]; // consider enemy bullets as characters in the code
SGameChar	g_sPlayershots[32]; // consider player bullets as characters as well
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime; // this is to prevent key bouncing, so we won't trigger keypresses more than once

// Console object
Console g_Console(110, 30, "Fake Puzzles");
char** map = new char*[30];

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
//void playerInput()
//{
//	cin >> playerName;
//}

void init( void )
{
    // Set precision for floating point output
    g_dElapsedTime = 0.0;
    g_dBounceTime = 0.0;

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

	g_sChar.m_cLocation.X = 46;
	g_sChar.m_cLocation.Y = 10;
	g_sHugger.m_cLocation.X = 5;
	g_sHugger.m_cLocation.Y = 16;
	g_sGunner.m_cLocation.X = 6;
	g_sGunner.m_cLocation.Y = 16;
	for (int i = 0; i < 128; i++) {
		g_sBullets[i].m_cLocation.X = 0;
		g_sBullets[i].m_cLocation.Y = 0;
	}
	for (int ps = 0; ps < 32; ps++) {
		g_sPlayershots[ps].m_cLocation.X = 0;
		g_sPlayershots[ps].m_cLocation.Y = 0;
	}
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
	g_abKeyPressed[K_ONE]	 = isKeyPressed(0x31);
	g_abKeyPressed[K_TWO]	 = isKeyPressed(0X32);
	g_abKeyPressed[K_THREE]	 = isKeyPressed(0x33);
	g_abKeyPressed[K_FOUR]	 = isKeyPressed(0x34);
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
		case S_STARTMENU: startMenu();
			break;
		case S_INSTRUCTIONS: instructions();
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
		case S_STARTMENU: renderStartMenu();
			break;
		case S_INSTRUCTIONS: renderInstructions();
			break;
        case S_GAME: renderGame();
            break;
    }
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 2) // wait for 3 seconds to switch to game mode, else do nothing
        g_eGameState = S_STARTMENU;
}

void startMenu()
{
	if (g_abKeyPressed[K_ONE])
	{
		g_eGameState = S_GAME;
	}

	if (g_abKeyPressed[K_TWO])
	{
		g_eGameState = S_INSTRUCTIONS;
	}

	if (g_abKeyPressed[K_THREE])
	{
		g_bQuitGame = true;
	}
}
void instructions()
{
	if (g_abKeyPressed[K_SPACE])
	{
		g_eGameState = S_GAME;
	}
}

melee hugger;
ranged gunner;
void renderEntities()
{
	// Draw the location of the enemies
	WORD charE_Color = 0x0C;
	
	g_Console.writeToBuffer(g_sHugger.m_cLocation, (char)128, charE_Color);
	g_Console.writeToBuffer(g_sGunner.m_cLocation, (char)83, charE_Color);
	for (int i = 0; i < 128; i++) {
		g_Console.writeToBuffer(g_sBullets[i].m_cLocation, (char)7, charE_Color);
	}
	// For player bullets
	for (int ps = 0; ps < 32; ps++) {
		g_Console.writeToBuffer(g_sPlayershots[ps].m_cLocation, (char)7, 0x06);
	}
}
int i = 0, n = 0,p = 0, ps = 0, shootbuffer = 0;
bool fooeyhappened1, fooeyhappened2, fooeyhappened3, playershot;
void enemydata() {
	double up, left, down, right, min_double;
	melee hugger;
	ranged gunner;

	fooeyhappened3 = false;

	if (bulletbouncetime > g_dElapsedTime)
		return;

	n = i;
	if (i >= 127) {
		i = 0;
	}

	for (i = 0; i < 128; i++) {
		if (shootdirection[i] == 1) { // shoot up
			g_sBullets[i].m_cLocation.Y--;
		}
		if (shootdirection[i] == 2) { // shoot left
			g_sBullets[i].m_cLocation.X--;
		}
		if (shootdirection[i] == 3) { // shoot down
			g_sBullets[i].m_cLocation.Y++;
		}
		if (shootdirection[i] == 4) { // shoot right
			g_sBullets[i].m_cLocation.X++;
		}
	}
	i = n;

	fooeyhappened3 = true;

	if (fooeyhappened3)
		bulletbouncetime = g_dElapsedTime + 0.05; // enemy bullets move around 20 tiles per second

	fooeyhappened1 = false;

	if (huggerbouncetime > g_dElapsedTime)
		return;

	up = 99.0; left = 99.0; down = 99.0; right = 99.0;

	if (map[g_sHugger.m_cLocation.Y - 1][g_sHugger.m_cLocation.X] == ' ' && x != 3) {
		up = sqrt(pow(g_sChar.m_cLocation.X - (g_sHugger.m_cLocation.X), 2) + pow(g_sChar.m_cLocation.Y - (g_sHugger.m_cLocation.Y - 1), 2));
	}
	if (map[g_sHugger.m_cLocation.Y][g_sHugger.m_cLocation.X - 1] == ' ' && x != 4) {
		left = sqrt(pow(g_sChar.m_cLocation.X - (g_sHugger.m_cLocation.X - 1), 2) + pow(g_sChar.m_cLocation.Y - (g_sHugger.m_cLocation.Y), 2));
	}
	if (map[g_sHugger.m_cLocation.Y + 1][g_sHugger.m_cLocation.X] == ' ' && x != 1) {
		down = sqrt(pow(g_sChar.m_cLocation.X - (g_sHugger.m_cLocation.X), 2) + pow(g_sChar.m_cLocation.Y - (g_sHugger.m_cLocation.Y + 1), 2));
	}
	if (map[g_sHugger.m_cLocation.Y][g_sHugger.m_cLocation.X + 1] == ' ' && x != 2) {
		right = sqrt(pow(g_sChar.m_cLocation.X - (g_sHugger.m_cLocation.X + 1), 2) + pow(g_sChar.m_cLocation.Y - (g_sHugger.m_cLocation.Y), 2));
	}
	min_double = min(min(up, down), min(left, right));
	if (min_double == up && x != 3) {
		g_sHugger.m_cLocation.Y--;
		x = 1;
	}
	else if (min_double == left && x != 4) {
		g_sHugger.m_cLocation.X--;
		x = 2;
	}
	else if (min_double == down && x != 1) {
		g_sHugger.m_cLocation.Y++;
		x = 3;
	}
	else if (min_double == right && x != 2) {
		g_sHugger.m_cLocation.X++;
		x = 4;
	}
	fooeyhappened1 = true;

	if (fooeyhappened1)
		huggerbouncetime = g_dElapsedTime + 0.2; // huggers act around five times per second


	fooeyhappened2 = false;

	if (gunnerbouncetime > g_dElapsedTime)
		return;

	if (sqrt(pow((g_sGunner.m_cLocation.X - g_sChar.m_cLocation.X), 2)) <= 8 && sqrt(pow((g_sGunner.m_cLocation.Y - g_sChar.m_cLocation.Y), 2)) <= 8) {
		if (g_sGunner.m_cLocation.Y < g_sChar.m_cLocation.Y && map[g_sGunner.m_cLocation.Y - 1][g_sGunner.m_cLocation.X] == ' ') {
			g_sGunner.m_cLocation.Y--;
		}
		if (g_sGunner.m_cLocation.Y > g_sChar.m_cLocation.Y && map[g_sGunner.m_cLocation.Y + 1][g_sGunner.m_cLocation.X] == ' ') {
			g_sGunner.m_cLocation.Y++;
		}
		if (g_sGunner.m_cLocation.X < g_sChar.m_cLocation.X && map[g_sGunner.m_cLocation.Y][g_sGunner.m_cLocation.X - 1] == ' ') {
			g_sGunner.m_cLocation.X--;
		}
		if (g_sGunner.m_cLocation.X > g_sChar.m_cLocation.X && map[g_sGunner.m_cLocation.Y][g_sGunner.m_cLocation.X + 1] == ' ') {
			g_sGunner.m_cLocation.X++;
		}
	}
	else {
		if (g_sGunner.m_cLocation.Y < g_sChar.m_cLocation.Y && map[g_sGunner.m_cLocation.Y + 1][g_sGunner.m_cLocation.X] == ' ') {
			g_sGunner.m_cLocation.Y++;
		}
		if (g_sGunner.m_cLocation.Y > g_sChar.m_cLocation.Y && map[g_sGunner.m_cLocation.Y - 1][g_sGunner.m_cLocation.X] == ' ') {
			g_sGunner.m_cLocation.Y--;
		}
		if (g_sGunner.m_cLocation.X < g_sChar.m_cLocation.X && map[g_sGunner.m_cLocation.Y][g_sGunner.m_cLocation.X + 1] == ' ') {
			g_sGunner.m_cLocation.X++;
		}
		if (g_sGunner.m_cLocation.X > g_sChar.m_cLocation.X && map[g_sGunner.m_cLocation.Y][g_sGunner.m_cLocation.X - 1] == ' ') {
			g_sGunner.m_cLocation.X--;
		}
	}

	if (g_sGunner.m_cLocation.X == g_sChar.m_cLocation.X) {
		i++;
		g_sBullets[i].m_cLocation.X = g_sGunner.m_cLocation.X;
		g_sBullets[i].m_cLocation.Y = g_sGunner.m_cLocation.Y;
		if (g_sGunner.m_cLocation.Y < g_sChar.m_cLocation.Y) {
			shootdirection[i] = 3; // shoot down
		}
		else shootdirection[i] = 1; //shoot up
	}
	if (g_sGunner.m_cLocation.Y == g_sChar.m_cLocation.Y) {
		i++;
		g_sBullets[i].m_cLocation.X = g_sGunner.m_cLocation.X;
		g_sBullets[i].m_cLocation.Y = g_sGunner.m_cLocation.Y;
		if (g_sGunner.m_cLocation.X < g_sChar.m_cLocation.X) {
			shootdirection[i] = 4;// shoot right
		}
		else shootdirection[i] = 2; //shoot left
	}

	fooeyhappened2 = true;

	if (fooeyhappened2)
		gunnerbouncetime = g_dElapsedTime + 0.6; // gunners act around twice per second
}

void playershoot()
{
	playershot = false;

	if (playerbulletshot > g_dElapsedTime)
		return;

	if (g_abKeyPressed[K_UP]) {
		playerdirection[ps] = 1; // shoot up
	}
	else if (g_abKeyPressed[K_DOWN]) {
		playerdirection[ps] = 2; // shoot down
	}
	else if (g_abKeyPressed[K_LEFT]) {
		playerdirection[ps] = 3; // shoot left
	}
	else if (g_abKeyPressed[K_RIGHT]) {
		playerdirection[ps] = 4; // shoot right
	}
	if (playerdirection[ps] == 0) {
		if (ps - 1 >= 0) {
			playerdirection[ps] = playerdirection[ps - 1];
		}
		else playerdirection[ps] = playerdirection[31];
	}

	if (g_abKeyPressed[K_SPACE] && playerdirection[ps] != 0) {
		shootbuffer++;
		if (shootbuffer == 3) {
			g_sPlayershots[ps].m_cLocation.X = g_sChar.m_cLocation.X;
			g_sPlayershots[ps].m_cLocation.Y = g_sChar.m_cLocation.Y;
			ps++;
			if (ps >= 32)
				ps = 0;
			playerdirection[ps] = 0;
			shootbuffer = 0;
		}
	}

	p = ps;
	for (ps = 0; ps < 32; ps++) {
		if (g_sPlayershots[ps].m_cLocation.X != 0 || g_sPlayershots[ps].m_cLocation.Y != 0) {
			if (playerdirection[ps] == 1) { // shoot up
				g_sPlayershots[ps].m_cLocation.Y--;
			}
			if (playerdirection[ps] == 2) { // shoot down
				g_sPlayershots[ps].m_cLocation.Y++;
			}
			if (playerdirection[ps] == 3) { // shoot left
				g_sPlayershots[ps].m_cLocation.X--;
			}
			if (playerdirection[ps] == 4) { // shoot right
				g_sPlayershots[ps].m_cLocation.X++;
			}
		}
		if (g_sPlayershots[ps].m_cLocation.X >= 110 || g_sPlayershots[ps].m_cLocation.X <= 0 || g_sPlayershots[ps].m_cLocation.Y >= 30 || g_sPlayershots[ps].m_cLocation.Y <= 0) {
			g_sPlayershots[ps].m_cLocation.X = 0;
			g_sPlayershots[ps].m_cLocation.Y = 0;
		}
	}
	ps = p;

	playershot = true;

	if (playershot)
		playerbulletshot = g_dElapsedTime + 0.05; // player bullets fly as fast as enemy bullets for now
}

void gameplay()            // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
                        // sound can be played here too.
	enemydata();
	playershoot();
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

	if (bSomethingHappened)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime = g_dElapsedTime + 0.125; // 125ms should be enough
	}

	if (collision('S') || collision('H') || collision('O') || collision('P'))
	{
		shop = true;
	}
	else
	{
		shop = false;
	}

	if (collision('Q') || collision('U') || collision('I') || collision('T'))
	{
		g_bQuitGame = true;
	}

	if (hq == true)
	{
		hqSpawn = false;

		if (collision('a'))
		{
			hq = false;
			levelA = true;
			playerRespawn = true;
		}
		else if (collision('b'))
		{
			hq = false;
			levelB = true;
			playerRespawn = true;
		}
		else if (collision('c'))
		{
			hq = false;
			levelC = true;
			playerRespawn = true;
		}
		else if (collision('d'))
		{
			hq = false;
			levelD = true;
			playerRespawn = true;
		}
		else if (collision('%'))
		{
			hq = false;
			inven = false;
		}
	}

	else if (levelA == true)
	{
		playerRespawn = false;

		if (collision('%'))
		{
			levelA = false;
			hq = true;
			hqSpawn = true;
		}
		if (collision('*'))
		{
			levelAgem = true;
		}
	}
	else if (levelB == true)
	{
		playerRespawn = false;

		if (collision('%'))
		{
			levelB = false;
			hq = true;
			hqSpawn = true;
		}
		if (collision('&'))
		{
			g_sChar.m_cLocation.X = 64;
			g_sChar.m_cLocation.Y = 2;
		}
		if (collision('*'))
		{
			levelBgem = true;
		}
	}
	else if (levelC == true)
	{
		playerRespawn = false;

		if (collision('%'))
		{
			levelC = false;
			hq = true;
			hqSpawn = true;
		}
		if (collision('&'))
		{
			g_sChar.m_cLocation.X = 59;
			g_sChar.m_cLocation.Y = 2;
		}
		if (collision('*'))
		{
			levelCgem = true;
		}
	}
	else if (levelD == true)
	{
		playerRespawn = false;

		if (collision('%'))
		{
			levelD = false;
			hq = true;
			hqSpawn = true;
		}
		if (collision('&'))
		{
			g_sChar.m_cLocation.X = 55;
			g_sChar.m_cLocation.Y = 2;
		}
		if (collision('*'))
		{
			levelDgem = true;
		}
	}
	if (hqSpawn == true)
	{
		g_sChar.m_cLocation.X = 46;
		g_sChar.m_cLocation.Y = 10;
	}

	if (playerRespawn == true)
	{
		g_sChar.m_cLocation.X = 5;
		g_sChar.m_cLocation.Y = 2;
	}
}
void processUserInput()
{
	/*playerInput();*/
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
	int i = 0;
	int a = 0;
	COORD c = g_Console.getConsoleSize();
	string splashscreen;
	ifstream splashscreenFile;

	splashscreenFile.open("Splashscreen.txt");
	if (splashscreenFile.is_open())
	{
		while (getline(splashscreenFile, splashscreen))
		{
			for (a = 0; a < splashscreen.length(); a++)
			{
				if (splashscreen[a] == 'F')
				{
					splashscreen[a] = 178;
				}
				map[i][a] = splashscreen[a];
			}
			c.X = 0;
			c.Y = i;
			i++;
			g_Console.writeToBuffer(c, splashscreen, 0x0B);

			if (g_dElapsedTime > 0.5)
			{
				c.X++;
				g_Console.writeToBuffer(c, splashscreen, 0x0C);
			}
			if (g_dElapsedTime > 1)
			{
				c.X++;
				g_Console.writeToBuffer(c, splashscreen, 0x0A);
			}
			if (g_dElapsedTime > 1.5)
			{
				c.X++;
				g_Console.writeToBuffer(c, splashscreen, 0x09);
			}
		}
	}
	splashscreenFile.close();
}

void renderStartMenu()
{
	COORD c;
	int i = 0;
	int a = 0;

	string menu;
	ifstream menuFile;

	menuFile.open("MainMenu.txt");
	if (menuFile.is_open())
	{
		while (getline(menuFile, menu))
		{
			for (a = 0; a < menu.length(); a++)
			{
				if (menu[a] == 'F')
				{
					menu[a] = 178;
				}
				map[i][a] = menu[a];
			}
			c.X = 0;
			c.Y = i;
			i++;
			g_Console.writeToBuffer(c, menu, 0x0B);
		}
	}
	menuFile.close();
}

void renderInstructions()
{
	COORD c;

	c.Y = 9;
	c.X = 45;
	g_Console.writeToBuffer(c, "Instructions", 0x0B);
	c.Y = 11;
	c.X = 25;
	g_Console.writeToBuffer(c, "- Use arrow keys to nagivate through the game.", 0x0B);
	c.Y = 12;
	c.X = 25;
	g_Console.writeToBuffer(c, "- Space bar to use weapon.", 0x0B);
	c.Y = 13;
	c.X = 25;
	g_Console.writeToBuffer(c, "- You will spawn at a headquarter, and enter the levels from there.", 0x0B);
	c.Y = 14;
	c.X = 25;
	g_Console.writeToBuffer(c, "- Buy weapons using coin earned in headquarters.", 0x0B);
	c.Y = 15;
	c.X = 25;
	g_Console.writeToBuffer(c, "- Collect the stars in all 4 levels to win the game.", 0x0B);
	c.Y = 17;
	c.X = 45;
	g_Console.writeToBuffer(c, "Press start!", 0x0B);
}

void renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
	renderEntities();
	playershoot();
}

void renderMap()
{
	COORD c;
	int i = 0;
	int a = 0;

	//Render Headquarters
	if (hq == true)
	{
		string headquarters;
		ifstream headquartersFile;

		headquartersFile.open("Headquarters.txt");
		if (headquartersFile.is_open())
		{
			while (getline(headquartersFile, headquarters))
			{
				for (a = 0; a < headquarters.length(); a++)
				{
					switch (headquarters[a]) {
					case '#':
						headquarters[a] = 223;
						break;
					case '@':
						headquarters[a] = 219;
						break;
					case '1':
						if (levelAgem == true)
						{
							headquarters[a] = 251;
						}
						else
						{
							headquarters[a] = 'x';
						}
						break;
					case '2':
						if (levelBgem == true)
						{
							headquarters[a] = 251;
						}
						else
						{
							headquarters[a] = 'x';
						}
						break;
					case '3':
						if (levelCgem == true)
						{
							headquarters[a] = 251;
						}
						else
						{
							headquarters[a] = 'x';
						}
						break;
					case '4':
						if (levelDgem == true)
						{
							headquarters[a] = 251;
						}
						else
						{
							headquarters[a] = 'x';
						}
						break;
					case 'u':
						if (levelAgem == true)
						{
							headquarters[a] = '*';
						}
						else
						{
							headquarters[a] = 255;
						}
						break;
					case 'i':
						if (levelBgem == true)
						{
							headquarters[a] = '*';
						}
						else
						{
							headquarters[a] = 255;
						}
						break;
					case 'o':
						if (levelCgem == true)
						{
							headquarters[a] = '*';
						}
						else
						{
							headquarters[a] = 255;
						}
						break;
					case 'p':
						if (levelDgem == true)
						{
							headquarters[a] = '*';
						}
						else
						{
							headquarters[a] = 255;
						}
						break;
					case '%':
						if (levelAgem == true && levelBgem == true && levelCgem == true && levelDgem == true)
						{
							headquarters[a] = '%';
						}
						else
						{
							headquarters[a] = 255;
						}
					}
					map[i][a] = headquarters[a];
				}
				c.X = 0;
				c.Y = i;
				i++;
				g_Console.writeToBuffer(c, headquarters, 0x09);
			}
		}
		headquartersFile.close();
	}
	//Render Level A
	else if (levelA == true)
	{
		string level1;
		ifstream level1File;
		i = 0;

		level1File.open("LevelA.txt");
		if (level1File.is_open())
		{
			while (getline(level1File, level1))
			{
				for (a = 0; a < level1.length(); a++)
				{
					switch (level1[a]) {
					case '#':
						level1[a] = 223;
						break;
					case '@':
						level1[a] = 219;
						break;
					case '*':
						if (levelAgem == true)
						{
							level1[a] = 255;
						}
					}
					map[i][a] = level1[a];
				}
				c.X = 0;
				c.Y = i;
				i++;
				g_Console.writeToBuffer(c, level1, 0x0F);
			}
		}
		level1File.close();
	}
	//Render Level B
	else if (levelB == true)
	{
		string level2;
		ifstream level2File;
		i = 0;
		level2File.open("LevelB.txt");
		if (level2File.is_open())
		{
			while (getline(level2File, level2))
			{
				for (a = 0; a < level2.length(); a++)
				{
					switch (level2[a]) {
					case '#':
						level2[a] = 223;
						break;
					case '@':
						level2[a] = 219;
						break;
					case '*':
						if (levelBgem == true)
						{
							level2[a] = 255;
						}
					}
					map[i][a] = level2[a];
				}
				c.X = 0;
				c.Y = i;
				i++;
				g_Console.writeToBuffer(c, level2, 0x0D);
			}
		}
		level2File.close();
	}
	//Render Level C
	else if (levelC == true)
	{
		string level3;
		ifstream level3File;
		i = 0;
		level3File.open("LevelC.txt");
		if (level3File.is_open())
		{
			while (getline(level3File, level3))
			{
				for (a = 0; a < level3.length(); a++)
				{
					switch (level3[a]) {
					case '#':
						level3[a] = 223;
						break;
					case '@':
						level3[a] = 219;
						break;
					case '*':
						if (levelCgem == true)
						{
							level3[a] = 255;
						}
					}
					map[i][a] = level3[a];
				}
				c.X = 0;
				c.Y = i;
				i++;
				g_Console.writeToBuffer(c, level3, 0x0B);
			}
		}
		level3File.close();
	}
	//Render Level D
	else if (levelD == true)
	{
		string level4;
		ifstream level4File;
		i = 0;
		level4File.open("LevelD.txt");
		if (level4File.is_open())
		{
			while (getline(level4File, level4))
			{
				for (a = 0; a < level4.length(); a++)
				{
					switch (level4[a]) {
					case '#':
						level4[a] = 223;
						break;
					case '@':
						level4[a] = 219;
						break;
					case '*':
						if (levelDgem == true)
						{
							level4[a] = 255;
						}
					}
					map[i][a] = level4[a];
				}
				c.X = 0;
				c.Y = i;
				i++;
				g_Console.writeToBuffer(c, level4, 0x0E);
			}
		}
		level4File.close();
	}

	//Render Shop
	if (shop == true)
	{
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
					switch (shop[a]) {
					case '#':
						shop[a] = 223;
						break;
					case '@':
						shop[a] = 219;
						break;
					}
				}
				c.X = 14;
				c.Y = 17 + i;
				i++;
				g_Console.writeToBuffer(c, shop, 0x00 + i);
			}
		}
		shopFile.close();

		if (g_abKeyPressed[K_ONE])
		{
			equipSmg = true;
		}
		 if (g_abKeyPressed[K_TWO])
		{
			equipRifle = true;
		}
		if (g_abKeyPressed[K_THREE])
		{
			equipSniper = true;
		}
		if (g_abKeyPressed[K_FOUR])
		{
			equipMinigun = true;
		}
	}

	//Render Inventory
	if (inven == true)
	{
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
					switch (inventory[a]) {
					case '#':
						inventory[a] = 223;
						break;
					case '@':
						inventory[a] = 219;
						break;
					case '$':
						inventory[a] = ' ';
						break;
					case '1':
						if (equipPistol == true)
						{
							inventory[a] = 251;
						}
						else
						{
							inventory[a] = 'x';
						}
						break;
					case '2':
						if (equipSmg == true)
						{
							inventory[a] = 251;
						}
						else
						{
							inventory[a] = 'x';
						}
						break;
					case '3':
						if (equipRifle == true)
						{
							inventory[a] = 251;
						}
						else
						{
							inventory[a] = 'x';
						}
						break;
					case '4':
						if (equipSniper == true)
						{
							inventory[a] = 251;
						}
						else
						{
							inventory[a] = 'x';
						}
						break;
					case '5':
						if (equipMinigun == true)
						{
							inventory[a] = 251;
						}
						else
						{
							inventory[a] = 'x';
						}
						break;
					}
				}
				c.X = 0;
				c.Y = 17 + i;
				i++;
				g_Console.writeToBuffer(c, inventory, 0x00 + i);
			}
		}
		inventoryFile.close();
	}
}

void renderCharacter()
{
    // Draw the location of the character
    //WORD charColor = 0x0F;
    /*if (g_sChar.m_bActive)
    {
        charColor = 0x0E;
    }*/

	if (collision('q'))
	{
		secondChar = true;
		firstChar = false;
		thirdChar = false;
		fourthChar = false;
		fifthChar = false;
		sixthChar = false;
	}
	if (collision('w'))
	{
		thirdChar = true;
		firstChar = false;
		secondChar = false;
		fourthChar = false;
		fifthChar = false;
		sixthChar = false;
	}
	if (collision('e'))
	{
		fourthChar = true;
		firstChar = false;
		secondChar = false;
		thirdChar = false;
		fifthChar = false;
		sixthChar = false;
	}
	if (collision('r'))
	{
		fifthChar = true;
		firstChar = false;
		secondChar = false;
		thirdChar = false;
		fourthChar = false;
		sixthChar = false;
	}
	if (collision('t'))
	{
		sixthChar = true;
		firstChar = false;
		secondChar = false;
		thirdChar = false;
		fourthChar = false;
		fifthChar = false;
	}

	//Characters' rendering
	if (firstChar == true)
	{
		g_Console.writeToBuffer(g_sChar.m_cLocation, (char)1, 0x0F);
	}
	else if (secondChar == true)
	{
		g_Console.writeToBuffer(g_sChar.m_cLocation, (char)2, 0x0F);
	}
	else if (thirdChar == true)
	{
		g_Console.writeToBuffer(g_sChar.m_cLocation, (char)3, 0x0C);
	}
	else if (fourthChar == true)
	{
		g_Console.writeToBuffer(g_sChar.m_cLocation, (char)4, 0x09);
	}
	else if (fifthChar == true)
	{
		g_Console.writeToBuffer(g_sChar.m_cLocation, (char)5, 0x0A);
	}
	else if (sixthChar == true)
	{
		g_Console.writeToBuffer(g_sChar.m_cLocation, (char)6, 0x0E);
	}
}
bool collision(char collider)
{
	bool collided = false;

	if (map[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == collider || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == collider || map[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == collider || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == collider)
	{
		collided = true;
	}
	return collided;
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
