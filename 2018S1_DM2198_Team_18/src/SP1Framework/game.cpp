// This is the main file for the game logic and function
//
//
//#pragma comment(lib,"irrKlang.lib")
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <windows.h>
#include <mmsystem.h>
//#include <irrKlang.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <time.h>
#pragma comment(lib,"winmm.lib")
//using namespace irrklang;
using namespace std;

string names[5] = { "Enos", "Enzo", "Ilya", "Shin", "Ella" };
bool charArray[5] = { false };
bool levels[5] = { false };
bool spawns[5] = { false };
bool gems[4] = { false };
bool equipWeapons[5] = { false };
bool boughtWeapons[5] = { false };
bool lost = false;
bool inven = true;
bool shop = false;
int lives = 0;
int coins = 0;
int a = 0, aaa = 0;

int shootdirection[128] = { 0, };
int playerdirection[64] = { 0, };

int b = 0, i = 0, n = 0, g = 0, h = 0, p = 0, ps = 0, bhugger[8] = { 0, }, bgunner[4] = { 0, }, bbullet[128] = { 0, }, bplayer = 0, bplayershoot[64] = { 0, }; // variables starting with b is used for buffer, others are used for array and stuff
int x[8] = { 0, }, h_hit[8] = { 0, };
bool fooeyhappened1, fooeyhappened2, fooeyhappened3, playershot;

double  g_dElapsedTime;
double  huggerbouncetime = g_dElapsedTime;
double  gunnerbouncetime = g_dElapsedTime;
double  bulletbouncetime = g_dElapsedTime;
double	playerbulletshot = g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT];

// Game specific variables here
SGameChar   g_sChar;
SGameChar   g_sHugger[8];
SGameChar	g_sGunner[4];
SGameChar	g_sBullets[128]; // consider enemy bullets as characters in the code
SGameChar	g_sPlayershots[64]; // consider player bullets as characters as well

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
void init( void )
{
    // Set precision for floating point output
    g_dElapsedTime = 0.0;
    g_dBounceTime = 0.0;

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;
	
    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");

	//Player Initial Location
	g_sChar.m_cLocation.X = 46;
	g_sChar.m_cLocation.Y = 9;

	//Set headquarters to default starting level and pistol as default weapon
	levels[0] = true;
	equipWeapons[0] = true;
	boughtWeapons[0] = true;

	for (int i = 0; i < g_Console.getConsoleSize().Y; i++)
	{
		map[i] = new char[g_Console.getConsoleSize().X];
	}
	for (int i = 0; i < 8; i++) {
		g_sHugger[i].m_cLocation.X = 0;
		g_sHugger[i].m_cLocation.Y = 0;
	}
	for (int i = 0; i < 4; i++) {
		g_sGunner[i].m_cLocation.X = 0;
		g_sGunner[i].m_cLocation.Y = 0;
	}
	for (int ps = 0; ps < 64; ps++) {
		g_sPlayershots[ps].m_cLocation.X = 1;
		g_sPlayershots[ps].m_cLocation.Y = 0;
	}
	for (int i = 0; i < 128; i++) {
		g_sBullets[i].m_cLocation.X = 0;
		g_sBullets[i].m_cLocation.Y = 0;
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
	g_abKeyPressed[K_RETURN] = isKeyPressed(VK_RETURN);
	g_abKeyPressed[K_1]	     = isKeyPressed(0x31);
	g_abKeyPressed[K_2]	     = isKeyPressed(0X32);
	g_abKeyPressed[K_3]	     = isKeyPressed(0x33);
	g_abKeyPressed[K_4]	     = isKeyPressed(0x34);
	g_abKeyPressed[K_5]      = isKeyPressed(0x35);
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
		case S_STARTMENU: processUserInput();
			break;
		case S_INSTRUCTIONS: processUserInput();
			break;
		case S_CHARACTERCREATION: processUserInput();
			break;
        case S_GAME: gameplay(); // gameplay logic when we are in the game
            break;
		case S_WINSCREEN: processUserInput();
			break;
		case S_LOSESCREEN: processUserInput();
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
		case S_CHARACTERCREATION: renderCharacterCreation();
			break;
        case S_GAME: renderGame();
            break;
		case S_WINSCREEN: renderWin();
			break;
		case S_LOSESCREEN: renderLose();
			break;
    }
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 2) // wait for 2 seconds to switch to game mode, else do nothing
        g_eGameState = S_STARTMENU;
}

void gameplay()            // gameplay logic
{
	processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
	moveCharacter();    // moves the character, collision detection, physics, etc
						// sound can be played here too.
	huggerdata();
	gunnerdata();
	enemybullet();
	playershoot();
	respawn();
}
void moveCharacter()
{
	bool bSomethingHappened = false;
	if (g_dBounceTime > g_dElapsedTime)
		return;

	// Updating the location of the character based on the key press
	if (g_abKeyPressed[K_UP] && map[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == ' ' && bplayer >= 2){
		g_sChar.m_cLocation.Y--;
		bplayer = 0;
	}
	if (g_abKeyPressed[K_LEFT] && map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == ' '){
		g_sChar.m_cLocation.X--;
	}
	if (g_abKeyPressed[K_DOWN] && map[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == ' ' && bplayer >= 2){
		g_sChar.m_cLocation.Y++; 
		bplayer = 0;
	}
	if (g_abKeyPressed[K_RIGHT] && map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == ' '){
		g_sChar.m_cLocation.X++;
	}
	bplayer++;
	bSomethingHappened = true;

	if (bSomethingHappened){
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime = g_dElapsedTime + 0.111; // 111ms should be enough
	}

	if (collision('S') || collision('H') || collision('O') || collision('P')){
		shop = true;
		
	} else {
		shop = false;
	}

	if (collision('Q') || collision('U') || collision('I') || collision('T')){
		saveProgression();
		g_bQuitGame = true;
	}

	if (levels[0] == true)
	{
		spawns[1] = false;
		spawns[2] = false;
		spawns[3] = false;
		spawns[4] = false;

		for (aaa = 0; aaa < 8; aaa++) {
			g_sHugger[aaa].m_cLocation.X = 0;
			g_sHugger[aaa].m_cLocation.Y = 0;
		}
		for (aaa = 0; aaa < 4; aaa++) {
			g_sGunner[aaa].m_cLocation.X = 0;
			g_sGunner[aaa].m_cLocation.Y = 0;
		}
		for (aaa = 0; aaa < 128; aaa++) {
			g_sBullets[aaa].m_cLocation.X = 0;
			g_sBullets[aaa].m_cLocation.Y = 0;
		}
		aaa = 0;

		if (collision('a')){
			levels[0] = false;
			levels[1] = true;
			spawns[0] = true;
		}
		else if (collision('b')){
			levels[0] = false;
			levels[2] = true;
			spawns[0] = true;
		}
		else if (collision('c')){
			levels[0] = false;
			levels[3] = true;
			spawns[0] = true;
		}
		else if (collision('d')){
			levels[0] = false;
			levels[4] = true;
			spawns[0] = true;
		}
		else if (collision('%')){
			levels[0] = false;
			inven = false;
			g_eGameState = S_WINSCREEN;
		}
	}

	else if (levels[1] == true){
		spawns[0] = false;

		while (aaa <= 0) {
			for (h = 0; h < 8; h++) { h_hit[h] = 0; }
			g_sHugger[0].m_cLocation.X = 6; g_sHugger[0].m_cLocation.Y = 16;
			g_sHugger[1].m_cLocation.X = 94; g_sHugger[1].m_cLocation.Y = 4;
			g_sHugger[2].m_cLocation.X = 84; g_sHugger[2].m_cLocation.Y = 17;
			g_sHugger[3].m_cLocation.X = 78; g_sHugger[3].m_cLocation.Y = 11;
			g_sHugger[4].m_cLocation.X = 35; g_sHugger[4].m_cLocation.Y = 12;
			g_sHugger[5].m_cLocation.X = 29; g_sHugger[5].m_cLocation.Y = 16;
			g_sGunner[0].m_cLocation.X = 54; g_sGunner[0].m_cLocation.Y = 4;
			g_sGunner[1].m_cLocation.X = 54; g_sGunner[1].m_cLocation.Y = 16;
			g_sGunner[2].m_cLocation.X = 40; g_sGunner[2].m_cLocation.Y = 8;
			aaa++;
		}
		if (collision('%')){
			levels[1] = false;
			levels[0] = true;
			spawns[1] = true;
		}
		if (collision('*')){
			gems[0] = true;
		}
	}
	else if (levels[2] == true){
		spawns[0] = false;

		while (aaa <= 0) {
			for (h = 0; h < 8; h++) { h_hit[h] = 0; }
			g_sHugger[0].m_cLocation.X = 6; g_sHugger[0].m_cLocation.Y = 10;
			g_sHugger[1].m_cLocation.X = 19; g_sHugger[1].m_cLocation.Y = 4;
			g_sHugger[2].m_cLocation.X = 34; g_sHugger[2].m_cLocation.Y = 11;
			g_sHugger[3].m_cLocation.X = 54; g_sHugger[3].m_cLocation.Y = 14;
			g_sHugger[4].m_cLocation.X = 39; g_sHugger[4].m_cLocation.Y = 13;
			g_sGunner[0].m_cLocation.X = 18; g_sGunner[0].m_cLocation.Y = 14;
			aaa++;
		}
		if (collision('%')){
			levels[2] = false;
			levels[0] = true;
			spawns[2] = true;
		}
		if (collision('&')){
			g_sChar.m_cLocation.X = 64;
			g_sChar.m_cLocation.Y = 2;
			for (h = 0; h < 8; h++) { h_hit[h] = 0; }
			g_sHugger[0].m_cLocation.X = 79; g_sHugger[0].m_cLocation.Y = 6;
			g_sHugger[1].m_cLocation.X = 69; g_sHugger[1].m_cLocation.Y = 12;
			g_sHugger[2].m_cLocation.X = 90; g_sHugger[2].m_cLocation.Y = 16;
			g_sHugger[3].m_cLocation.X = 90; g_sHugger[3].m_cLocation.Y = 19;
			g_sHugger[4].m_cLocation.X = 0; g_sHugger[4].m_cLocation.Y = 0;
			g_sGunner[0].m_cLocation.X = 81; g_sGunner[0].m_cLocation.Y = 4;
			g_sGunner[1].m_cLocation.X = 71; g_sGunner[1].m_cLocation.Y = 15;
			g_sGunner[2].m_cLocation.X = 86; g_sGunner[2].m_cLocation.Y = 16;
		}
		if (collision('*')){
			gems[1] = true;
		}
	}
	else if (levels[3] == true) {
		spawns[0] = false;

		while (aaa <= 0) {
			for (h = 0; h < 8; h++) { h_hit[h] = 0; }
			g_sHugger[0].m_cLocation.X = 24; g_sHugger[0].m_cLocation.Y = 7;
			g_sHugger[1].m_cLocation.X = 44; g_sHugger[1].m_cLocation.Y = 16;
			g_sHugger[2].m_cLocation.X = 6; g_sHugger[2].m_cLocation.Y = 16;
			g_sHugger[3].m_cLocation.X = 41; g_sHugger[3].m_cLocation.Y = 8;
			g_sGunner[0].m_cLocation.X = 10; g_sGunner[0].m_cLocation.Y = 4;
			g_sGunner[1].m_cLocation.X = 12; g_sGunner[1].m_cLocation.Y = 15;
			g_sGunner[2].m_cLocation.X = 33; g_sGunner[2].m_cLocation.Y = 4;
			g_sGunner[3].m_cLocation.X = 20; g_sGunner[3].m_cLocation.Y = 13;
			aaa++;
		}
		if (collision('%')){
			levels[3] = false;
			levels[0] = true;
			spawns[3] = true;
		}
		if (collision('&')){
			g_sChar.m_cLocation.X = 59;
			g_sChar.m_cLocation.Y = 2;
			for (h = 0; h < 8; h++) { h_hit[h] = 0; }
			g_sGunner[0].m_cLocation.X = 0; g_sGunner[0].m_cLocation.Y = 0;
			g_sGunner[1].m_cLocation.X = 0; g_sGunner[1].m_cLocation.Y = 0;
			g_sGunner[2].m_cLocation.X = 0; g_sGunner[2].m_cLocation.Y = 0;
			g_sGunner[3].m_cLocation.X = 0; g_sGunner[3].m_cLocation.Y = 0;
			g_sHugger[0].m_cLocation.X = 65; g_sHugger[0].m_cLocation.Y = 17;
			g_sHugger[1].m_cLocation.X = 80; g_sHugger[1].m_cLocation.Y = 17;
			g_sHugger[2].m_cLocation.X = 96; g_sHugger[2].m_cLocation.Y = 17;
			g_sHugger[3].m_cLocation.X = 82; g_sHugger[3].m_cLocation.Y = 23;
			g_sHugger[4].m_cLocation.X = 78; g_sHugger[4].m_cLocation.Y = 23;
			g_sHugger[5].m_cLocation.X = 80; g_sHugger[5].m_cLocation.Y = 11;
			g_sHugger[6].m_cLocation.X = 81; g_sHugger[6].m_cLocation.Y = 11;
			g_sHugger[7].m_cLocation.X = 82; g_sHugger[7].m_cLocation.Y = 11;
		}
		if (collision('*')){
			gems[2] = true;
		}
	}
	else if (levels[4] == true){
		spawns[0] = false;

		while (aaa <= 0) {
			for (h = 0; h < 8; h++) { h_hit[h] = 0; }
			g_sHugger[0].m_cLocation.X = 8; g_sHugger[0].m_cLocation.Y = 12;
			g_sHugger[1].m_cLocation.X = 8; g_sHugger[1].m_cLocation.Y = 13;
			g_sHugger[2].m_cLocation.X = 25; g_sHugger[2].m_cLocation.Y = 12;
			g_sHugger[3].m_cLocation.X = 26; g_sHugger[3].m_cLocation.Y = 12;
			g_sHugger[4].m_cLocation.X = 39; g_sHugger[4].m_cLocation.Y = 14;
			g_sHugger[5].m_cLocation.X = 39; g_sHugger[5].m_cLocation.Y = 15;
			g_sHugger[6].m_cLocation.X = 39; g_sHugger[6].m_cLocation.Y = 6;
			g_sHugger[7].m_cLocation.X = 40; g_sHugger[7].m_cLocation.Y = 6;
			g_sGunner[0].m_cLocation.X = 7; g_sGunner[0].m_cLocation.Y = 14;
			g_sGunner[1].m_cLocation.X = 27; g_sGunner[1].m_cLocation.Y = 12;
			g_sGunner[2].m_cLocation.X = 39; g_sGunner[2].m_cLocation.Y = 16;
			g_sGunner[3].m_cLocation.X = 41; g_sGunner[3].m_cLocation.Y = 6;
			aaa++;
		}
		if (collision('%')){
			levels[4] = false;
			levels[0] = true;
			spawns[4] = true;
		}
		if (collision('&')){
			g_sChar.m_cLocation.X = 60;
			g_sChar.m_cLocation.Y = 2;
			for (h = 0; h < 8; h++) { h_hit[h] = 0; }
			g_sHugger[0].m_cLocation.X = 60; g_sHugger[0].m_cLocation.Y = 12;
			g_sHugger[1].m_cLocation.X = 61; g_sHugger[1].m_cLocation.Y = 12;
			g_sHugger[2].m_cLocation.X = 84; g_sHugger[2].m_cLocation.Y = 11;
			g_sHugger[3].m_cLocation.X = 77; g_sHugger[3].m_cLocation.Y = 4;
			g_sHugger[4].m_cLocation.X = 60; g_sHugger[4].m_cLocation.Y = 15;
			g_sHugger[5].m_cLocation.X = 70; g_sHugger[5].m_cLocation.Y = 17;
			g_sHugger[6].m_cLocation.X = 89; g_sHugger[6].m_cLocation.Y = 19;
			g_sHugger[7].m_cLocation.X = 84; g_sHugger[7].m_cLocation.Y = 21;
			g_sGunner[0].m_cLocation.X = 62; g_sGunner[0].m_cLocation.Y = 12;
			g_sGunner[1].m_cLocation.X = 83; g_sGunner[1].m_cLocation.Y = 9;
			g_sGunner[2].m_cLocation.X = 60; g_sGunner[2].m_cLocation.Y = 18;
			g_sGunner[3].m_cLocation.X = 45; g_sGunner[3].m_cLocation.Y = 22;
			aaa++;
		}
		if (collision('*')){
			gems[3] = true;
		}
	}
}
void processUserInput()
{
	COORD c;
	// quits the game if player hits the escape key
	if (g_abKeyPressed[K_ESCAPE]) {
		saveProgression();
		g_bQuitGame = true;
	}
	//Start menu key presses
	if (g_eGameState == S_STARTMENU) {
		if (g_abKeyPressed[K_1]) {
			g_eGameState = S_CHARACTERCREATION;
		}
		if (g_abKeyPressed[K_2]) {
				loadProgression();
				g_eGameState = S_GAME;
		}
		if (g_abKeyPressed[K_3]) {
			g_eGameState = S_INSTRUCTIONS;
		}
	}
	//Instructions key presses
	if (g_eGameState == S_INSTRUCTIONS){
		if (g_abKeyPressed[K_RETURN]){
			g_eGameState = S_CHARACTERCREATION;
		}
	}
	//Character creation key presses
	if (g_eGameState == S_CHARACTERCREATION){
		if (g_abKeyPressed[K_1]){
			charArray[0] = true;
			charArray[1] = false;
			charArray[2] = false;
			charArray[3] = false;
			charArray[4] = false;
			lives = 7;
			coins = 0;
		}
		if (g_abKeyPressed[K_2]){
			charArray[1] = true;
			charArray[0] = false;
			charArray[2] = false;
			charArray[3] = false;
			charArray[4] = false;
			lives = 5;
			coins = 40;
		}
		if (g_abKeyPressed[K_3]){
			charArray[2] = true;
			charArray[0] = false;
			charArray[1] = false;
			charArray[3] = false;
			charArray[4] = false;
			lives = 5;
			coins = 40;
		}
		if (g_abKeyPressed[K_4]){
			charArray[3] = true;
			charArray[0] = false;
			charArray[1] = false;
			charArray[2] = false;
			charArray[4] = false;
			lives = 4;
			coins = 70;
		}
		if (g_abKeyPressed[K_5]){
			charArray[4] = true;
			charArray[0] = false;
			charArray[1] = false;
			charArray[2] = false;
			charArray[3] = false;
			lives = 3;
			coins = 100;
		}
		if (g_abKeyPressed[K_SPACE] && (charArray[0] == true || charArray[1] == true || charArray[2] == true || charArray[3] == true || charArray[4] == true)){
			g_eGameState = S_GAME;
		}
	}
	//Win and Lose screen key presses
	if (g_eGameState == S_WINSCREEN) {
		if (g_abKeyPressed[K_RETURN]){
			g_eGameState = S_STARTMENU;
		}
	}
	if (g_eGameState == S_LOSESCREEN) {
			/*DWORD now = timeGetTime();
			DWORD nextEndGame = now + 5000;
			while (g_eGameState==S_LOSESCREEN)
			{
				now = timeGetTime();
				if (now >= nextEndGame)
				{
					g_eGameState = S_CHARACTERCREATION;
					nextEndGame += 5000;
				}
			}*/
		if (g_abKeyPressed[K_RETURN]) {
			g_eGameState = S_STARTMENU;
		}
	}
}
bool collision(char collider)
{
	bool collided = false;

	if (map[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == collider || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == collider || map[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == collider || map[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == collider) {
		collided = true;
	}
	return collided;
}

int PlayAudio(int argc, char* argv[])
{
	PlaySound(TEXT("game_muse.wav"), NULL, SND_LOOP | SND_ASYNC | SND_FILENAME);
	return 0;
}

void huggerdata() {
	double up, left, down, right, min_double;

	fooeyhappened1 = false;

	if (huggerbouncetime > g_dElapsedTime)
		return;

	for (h = 0; h < 8; h++) { // x[h] in this case is used for a "no reverse rule". Example, if one enemy is moving up, he's not allowed to move down immediately after moving up
		if (g_sHugger[h].m_cLocation.X == 0 && g_sHugger[h].m_cLocation.Y == 0) {
			continue;
		}
		up = 99.0; left = 99.0; down = 99.0; right = 99.0;
		if (map[g_sHugger[h].m_cLocation.Y - 1][g_sHugger[h].m_cLocation.X] == ' ' && x[h] != 3) {
			up = sqrt(pow(g_sChar.m_cLocation.X - (g_sHugger[h].m_cLocation.X), 2) + pow(g_sChar.m_cLocation.Y - (g_sHugger[h].m_cLocation.Y - 1), 2));
		}
		if (map[g_sHugger[h].m_cLocation.Y][g_sHugger[h].m_cLocation.X - 1] == ' ' && x[h] != 4) {
			left = sqrt(pow(g_sChar.m_cLocation.X - (g_sHugger[h].m_cLocation.X - 1), 2) + pow(g_sChar.m_cLocation.Y - (g_sHugger[h].m_cLocation.Y), 2));
		}
		if (map[g_sHugger[h].m_cLocation.Y + 1][g_sHugger[h].m_cLocation.X] == ' ' && x[h] != 1) {
			down = sqrt(pow(g_sChar.m_cLocation.X - (g_sHugger[h].m_cLocation.X), 2) + pow(g_sChar.m_cLocation.Y - (g_sHugger[h].m_cLocation.Y + 1), 2));
		}
		if (map[g_sHugger[h].m_cLocation.Y][g_sHugger[h].m_cLocation.X + 1] == ' ' && x[h] != 2) {
			right = sqrt(pow(g_sChar.m_cLocation.X - (g_sHugger[h].m_cLocation.X + 1), 2) + pow(g_sChar.m_cLocation.Y - (g_sHugger[h].m_cLocation.Y), 2));
		}
		min_double = min(min(up, down), min(left, right));
		if (min_double == 99.0) { // don't move if not possible, and reset no reverse rule
			x[h] = 0;
			bhugger[h]++;
			continue;
		}
		else if (min_double == up && x[h] != 3 && bhugger[h] >= 2) { // move up
			g_sHugger[h].m_cLocation.Y--;
			x[h] = 1;
			bhugger[h] = 0;
		}
		else if (min_double == left && x[h] != 4) { // move left
			g_sHugger[h].m_cLocation.X--;
			x[h] = 2;
		}
		else if (min_double == down && x[h] != 1 && bhugger[h] >= 2) { // move down
			g_sHugger[h].m_cLocation.Y++;
			x[h] = 3;
			bhugger[h] = 0;
		}
		else if (min_double == right && x[h] != 2) { // move right
			g_sHugger[h].m_cLocation.X++;
			x[h] = 4;
		}
		//Hugger collision with player and player bullets
		if (levels[1] == true || levels[2] == true || levels[3] == true || levels[4] == true)
		{
			if ((g_sHugger[h].m_cLocation.Y == g_sChar.m_cLocation.Y) && (g_sHugger[h].m_cLocation.X == g_sChar.m_cLocation.X))
			{
				spawns[0] = true;
				lives--;
			}
		}
		bhugger[h]++;
	}

	fooeyhappened1 = true;

	if (fooeyhappened1) {
		huggerbouncetime = g_dElapsedTime + 0.17; // huggers act around six times per second
	}
}
void gunnerdata() {
	fooeyhappened2 = false;

	if (gunnerbouncetime > g_dElapsedTime)
		return;

	for (g = 0; g < 4; g++) {
		if (g_sGunner[g].m_cLocation.X == 0 && g_sGunner[g].m_cLocation.Y == 0) {
			continue;
		}
		if (sqrt(pow((g_sGunner[g].m_cLocation.X - g_sChar.m_cLocation.X), 2)) <= 10 && sqrt(pow((g_sGunner[g].m_cLocation.Y - g_sChar.m_cLocation.Y), 2)) <= 5) {
			if (g_sGunner[g].m_cLocation.Y < g_sChar.m_cLocation.Y && map[g_sGunner[g].m_cLocation.Y - 1][g_sGunner[g].m_cLocation.X] == ' ' && bhugger[g] >= 2) {
				g_sGunner[g].m_cLocation.Y--;
				bhugger[g] = 0;
			}
			if (g_sGunner[g].m_cLocation.Y > g_sChar.m_cLocation.Y && map[g_sGunner[g].m_cLocation.Y + 1][g_sGunner[g].m_cLocation.X] == ' ' && bhugger[g] >= 2) {
				g_sGunner[g].m_cLocation.Y++;
				bhugger[g] = 0;
			}
			if (g_sGunner[g].m_cLocation.X < g_sChar.m_cLocation.X && map[g_sGunner[g].m_cLocation.Y][g_sGunner[g].m_cLocation.X - 1] == ' ') {
				g_sGunner[g].m_cLocation.X--;
			}
			if (g_sGunner[g].m_cLocation.X > g_sChar.m_cLocation.X && map[g_sGunner[g].m_cLocation.Y][g_sGunner[g].m_cLocation.X + 1] == ' ') {
				g_sGunner[g].m_cLocation.X++;
			}
		}
		else {
			if (g_sGunner[g].m_cLocation.Y < g_sChar.m_cLocation.Y && map[g_sGunner[g].m_cLocation.Y + 1][g_sGunner[g].m_cLocation.X] == ' ' && bhugger[g] >= 2) {
				g_sGunner[g].m_cLocation.Y++;
				bhugger[g] = 0;
			}
			if (g_sGunner[g].m_cLocation.Y > g_sChar.m_cLocation.Y && map[g_sGunner[g].m_cLocation.Y - 1][g_sGunner[g].m_cLocation.X] == ' ' && bhugger[g] >= 2) {
				g_sGunner[g].m_cLocation.Y--;
				bhugger[g] = 0;
			}
			if (g_sGunner[g].m_cLocation.X < g_sChar.m_cLocation.X && map[g_sGunner[g].m_cLocation.Y][g_sGunner[g].m_cLocation.X + 1] == ' ') {
				g_sGunner[g].m_cLocation.X++;
			}
			if (g_sGunner[g].m_cLocation.X > g_sChar.m_cLocation.X && map[g_sGunner[g].m_cLocation.Y][g_sGunner[g].m_cLocation.X - 1] == ' ') {
				g_sGunner[g].m_cLocation.X--;
			}
		}
		//Gunner body collision with player
		if (levels[1] == true || levels[2] == true || levels[3] == true || levels[4] == true)
		{
			if ((g_sGunner[g].m_cLocation.Y == g_sChar.m_cLocation.Y) && (g_sGunner[g].m_cLocation.X == g_sChar.m_cLocation.X))
			{
				spawns[0] = true;
				lives--;
			}
		}
		if (g_sGunner[g].m_cLocation.X == g_sChar.m_cLocation.X) {
			i++;
			g_sBullets[i].m_cLocation.X = g_sGunner[g].m_cLocation.X;
			g_sBullets[i].m_cLocation.Y = g_sGunner[g].m_cLocation.Y;
			if (g_sGunner[g].m_cLocation.Y < g_sChar.m_cLocation.Y) {
				shootdirection[i] = 3; // shoot down
			}
			else shootdirection[i] = 1; //shoot up
		}
		if (g_sGunner[g].m_cLocation.Y == g_sChar.m_cLocation.Y) {
			i++;
			g_sBullets[i].m_cLocation.X = g_sGunner[g].m_cLocation.X;
			g_sBullets[i].m_cLocation.Y = g_sGunner[g].m_cLocation.Y;
			if (g_sGunner[g].m_cLocation.X < g_sChar.m_cLocation.X) {
				shootdirection[i] = 4;// shoot right
			}
			else shootdirection[i] = 2; //shoot left
		}
		bhugger[g]++;
	}

	if (i >= 123) {
		i = 0;
	}

	fooeyhappened2 = true;

	if (fooeyhappened2) {
		gunnerbouncetime = g_dElapsedTime + 0.5; // gunners act around twice per second
	}
}
void enemybullet() {
	if (levels[1] == true || levels[2] == true || levels[3] == true || levels[4] == true) {
		fooeyhappened3 = false;

		if (bulletbouncetime > g_dElapsedTime)
			return;

		n = i;
		for (i = 0; i < 128; i++) {
			if (shootdirection[i] == 1 && bbullet[i] >= 2) { // move up
				g_sBullets[i].m_cLocation.Y--;
				bbullet[i] = 0;
			}
			if (shootdirection[i] == 2) { // move left
				g_sBullets[i].m_cLocation.X--;
			}
			if (shootdirection[i] == 3 && bbullet[i] >= 2) { // move down
				g_sBullets[i].m_cLocation.Y++;
				bbullet[i] = 0;
			}
			if (shootdirection[i] == 4) { // move right
				g_sBullets[i].m_cLocation.X++;
			}
			if ((g_sBullets[i].m_cLocation.Y == g_sChar.m_cLocation.Y) && (g_sBullets[i].m_cLocation.X == g_sChar.m_cLocation.X)) { // enemy bullet collision with player
				spawns[0] = true;
				lives--;
				g_sBullets[i].m_cLocation.X = 0;
				g_sBullets[i].m_cLocation.Y = 0;
				shootdirection[i] = 0;
			}
			if (g_sBullets[i].m_cLocation.X >= 108 || g_sBullets[i].m_cLocation.X <= 1 || g_sBullets[i].m_cLocation.Y >= 28 || g_sBullets[i].m_cLocation.Y <= 1 || map[g_sBullets[i].m_cLocation.Y][g_sBullets[i].m_cLocation.X] != ' ') {
				g_sBullets[i].m_cLocation.X = 0;
				g_sBullets[i].m_cLocation.Y = 0;
				shootdirection[i] = 0;
			}
			bbullet[i]++;
		}
		i = n;

		fooeyhappened3 = true;

		if (fooeyhappened3) {
			bulletbouncetime = g_dElapsedTime + 0.05; // enemy bullets move around 20 tiles per second
		}
	}
}

void playershoot()
{
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
		else playerdirection[ps] = playerdirection[63];
	}

	if (g_abKeyPressed[K_SPACE] && playerdirection[ps] != 0) {
		if (equipWeapons[0] && b >= 8 || equipWeapons[1] && b >= 4 || equipWeapons[2] && b >= 6 || equipWeapons[3] && b >= 70 || equipWeapons[4] && b >= 2) { // Pistol fires around 2 times per second, Smg fires around 7 times per second, Assault rifle fires around 4 times per second, Sniper fires around once every 1.5 seconds, Minigun fires around 20 times per second
			g_sPlayershots[ps].m_cLocation.X = g_sChar.m_cLocation.X;
			g_sPlayershots[ps].m_cLocation.Y = g_sChar.m_cLocation.Y;
			ps++;
			if (ps >= 64) { ps = 0; }
			g_sPlayershots[ps].m_cLocation.X = 1;
			g_sPlayershots[ps].m_cLocation.Y = 0; // this fixes a bug
			playerdirection[ps] = 0;
			b = 0;
		}
	}
	// Equipping weapons
	if (g_abKeyPressed[K_1] && boughtWeapons[0] == true) {
		equipWeapons[0] = true;
		equipWeapons[1] = false;
		equipWeapons[2] = false;
		equipWeapons[3] = false;
		equipWeapons[4] = false;
		p = ps;
		for (ps = 0; ps < 64; ps++) {
			g_sPlayershots[ps].m_cLocation.X = 1;
			g_sPlayershots[ps].m_cLocation.Y = 0;
		}
		ps = p;
	}
	if (g_abKeyPressed[K_2] && boughtWeapons[1] == true) {
		equipWeapons[0] = false;
		equipWeapons[1] = true;
		equipWeapons[2] = false;
		equipWeapons[3] = false;
		equipWeapons[4] = false;
		p = ps;
		for (ps = 0; ps < 64; ps++) {
			g_sPlayershots[ps].m_cLocation.X = 1;
			g_sPlayershots[ps].m_cLocation.Y = 0;
		}
		ps = p;
	}
	if (g_abKeyPressed[K_3] && boughtWeapons[2] == true) {
		equipWeapons[0] = false;
		equipWeapons[1] = false;
		equipWeapons[2] = true;
		equipWeapons[3] = false;
		equipWeapons[4] = false;
		p = ps;
		for (ps = 0; ps < 64; ps++) {
			g_sPlayershots[ps].m_cLocation.X = 1;
			g_sPlayershots[ps].m_cLocation.Y = 0;
		}
		ps = p;
	}
	if (g_abKeyPressed[K_4] && boughtWeapons[3] == true) {
		equipWeapons[0] = false;
		equipWeapons[1] = false;
		equipWeapons[2] = false;
		equipWeapons[3] = true;
		equipWeapons[4] = false;
		p = ps;
		for (ps = 0; ps < 64; ps++) {
			g_sPlayershots[ps].m_cLocation.X = 1;
			g_sPlayershots[ps].m_cLocation.Y = 0;
		}
		ps = p;
	}
	if (g_abKeyPressed[K_5] && boughtWeapons[4] == true) {
		equipWeapons[0] = false;
		equipWeapons[1] = false;
		equipWeapons[2] = false;
		equipWeapons[3] = false;
		equipWeapons[4] = true;
		p = ps;
		for (ps = 0; ps < 64; ps++) {
			g_sPlayershots[ps].m_cLocation.X = 1;
			g_sPlayershots[ps].m_cLocation.Y = 0;
		}
		ps = p;
	}

	playershot = false;

	if (playerbulletshot > g_dElapsedTime)
		return;

	p = ps;
	for (ps = 0; ps < 64; ps++) {
		//Player's bullet collision with enemies before moving player bullet
		if (levels[1] == true || levels[2] == true || levels[3] == true || levels[4] == true)
		{
			for (h = 0; h < 8; h++) {
				if ((g_sPlayershots[ps].m_cLocation.Y == g_sHugger[h].m_cLocation.Y) && (g_sPlayershots[ps].m_cLocation.X == g_sHugger[h].m_cLocation.X)) {
					g_sPlayershots[ps].m_cLocation.X = 1;
					g_sPlayershots[ps].m_cLocation.Y = 0;
					if (equipWeapons[0] || equipWeapons[1] || equipWeapons[4]) { // Pisto, Smg and Minigun deals 1 damage per shot
						h_hit[h]++;
					}
					if (equipWeapons[2]) { // Assault rifle deal 2 damage per shot
						h_hit[h] += 2;
					}
					if (equipWeapons[3]) { // Sniper deals 5 damage
						h_hit[h] += 5;
					}
					if (h_hit[h] >= 3) { // representing that huggers have 3 HP
						coins += 5;
						g_sHugger[h].m_cLocation.X = 0;
						g_sHugger[h].m_cLocation.Y = 0;
					}
					continue;
				}
			}
			for (g = 0; g < 4; g++) {
				if ((g_sPlayershots[ps].m_cLocation.Y == g_sGunner[g].m_cLocation.Y) && (g_sPlayershots[ps].m_cLocation.X == g_sGunner[g].m_cLocation.X)) {
					g_sPlayershots[ps].m_cLocation.X = 1;
					g_sPlayershots[ps].m_cLocation.Y = 0;
					coins += 5;
					g_sGunner[g].m_cLocation.X = 0;
					g_sGunner[g].m_cLocation.Y = 0;
					continue;
				}
			}
		}
		if (g_sPlayershots[ps].m_cLocation.X != 1 || g_sPlayershots[ps].m_cLocation.Y != 0) {
			if (playerdirection[ps] == 1 && bplayershoot[ps] >= 2) { // move up
				g_sPlayershots[ps].m_cLocation.Y--;
				bplayershoot[ps] = 0;
			}
			if (playerdirection[ps] == 2 && bplayershoot[ps] >= 2) { // move down
				g_sPlayershots[ps].m_cLocation.Y++;
				bplayershoot[ps] = 0;
			}
			if (playerdirection[ps] == 3) { // move left
				g_sPlayershots[ps].m_cLocation.X--;
			}
			if (playerdirection[ps] == 4) { // move right
				g_sPlayershots[ps].m_cLocation.X++;
			}
		}
		//Player's bullet collision with enemies after moving player bullet
		//Checking for collision before and afer moving player bullets ensure that enemies don't move through player bullets anymore
		if (levels[1] == true || levels[2] == true || levels[3] == true || levels[4] == true)
		{
			for (h = 0; h < 8; h++) {
				if ((g_sPlayershots[ps].m_cLocation.Y == g_sHugger[h].m_cLocation.Y) && (g_sPlayershots[ps].m_cLocation.X == g_sHugger[h].m_cLocation.X)) {
					g_sPlayershots[ps].m_cLocation.X = 1;
					g_sPlayershots[ps].m_cLocation.Y = 0;
					if (equipWeapons[0] || equipWeapons[1]) { // Pistol and Smg deal 1 damage
						h_hit[h]++;
					}
					if (equipWeapons[2] || equipWeapons[4]) { // Assault rifle and Minigun deal 2 damage
						h_hit[h] += 2;
					}
					if (equipWeapons[3]) { // Sniper deals 5 damage
						h_hit[h] += 5;
					}
					if (h_hit[h] >= 3) { // representing that huggers have 3 HP
						h_hit[h] = 0;
						coins += 5;
						g_sHugger[h].m_cLocation.X = 0;
						g_sHugger[h].m_cLocation.Y = 0;
					}
				}
			}
			for (g = 0; g < 4; g++) {
				if ((g_sPlayershots[ps].m_cLocation.Y == g_sGunner[g].m_cLocation.Y) && (g_sPlayershots[ps].m_cLocation.X == g_sGunner[g].m_cLocation.X)) {
					g_sPlayershots[ps].m_cLocation.X = 1;
					g_sPlayershots[ps].m_cLocation.Y = 0;
					coins += 5;
					g_sGunner[g].m_cLocation.X = 0;
					g_sGunner[g].m_cLocation.Y = 0;
				}
			}
		}
		if (g_sPlayershots[ps].m_cLocation.X >= 108 || g_sPlayershots[ps].m_cLocation.X <= 1 || g_sPlayershots[ps].m_cLocation.Y >= 28 || g_sPlayershots[ps].m_cLocation.Y <= 1 || map[g_sPlayershots[ps].m_cLocation.Y][g_sPlayershots[ps].m_cLocation.X] != ' ') { // player bullets near leaving the console window or player bullets' collision with walls
			g_sPlayershots[ps].m_cLocation.X = 1;
			g_sPlayershots[ps].m_cLocation.Y = 0;
		}
		bplayershoot[ps]++;
	}
	ps = p;

	playershot = true;

	if (equipWeapons[3]) {
		b++; // b stands for gun buffers, used for gun firing speeds
		playerbulletshot = g_dElapsedTime + 0.02; // Sniper bullets fly around 50 tiles per second
	}
	else if (playershot) {
		b++;
		playerbulletshot = g_dElapsedTime + 0.05; // player bullets fly as fast as enemy bullets for now
	}
}

void respawn()
{
	if (lives <= 0) {
		g_eGameState = S_LOSESCREEN;
	}
	if (spawns[0] == true) {
		g_sChar.m_cLocation.X = 5;
		g_sChar.m_cLocation.Y = 2;
	}
	if (spawns[1] == true) {
		g_sChar.m_cLocation.X = 12;
		g_sChar.m_cLocation.Y = 6;
	}
	if (spawns[2] == true) {
		g_sChar.m_cLocation.X = 29;
		g_sChar.m_cLocation.Y = 6;
	}
	if (spawns[3] == true) {
		g_sChar.m_cLocation.X = 46;
		g_sChar.m_cLocation.Y = 6;
	}
	if (spawns[4] == true) {
		g_sChar.m_cLocation.X = 63;
		g_sChar.m_cLocation.Y = 6;
	}
}

void saveProgression()
{
	ofstream saveGame("Textfiles/SaveGame.txt");
	saveGame << coins << endl;
	saveGame << lives << endl;
	for (int a = 0; a < 5; a++)
	{
		saveGame << charArray[a] ? '1' : '0';
	}
	saveGame << endl;
	for (int a = 0; a < 4; a++)
	{
		saveGame << gems[a] ? '1' : '0';
	}
	saveGame << endl;
	for (int a = 0; a < 5; a++)
	{
		saveGame << boughtWeapons[a] ? '1' : '0';
	}
	saveGame << endl;
}
void loadProgression()
{
	string loadGameFile = "";
	ifstream loadGame("Textfiles/SaveGame.txt");

	getline(loadGame, loadGameFile);
	coins = stoi(loadGameFile);
	getline(loadGame, loadGameFile);
	lives = stoi(loadGameFile);

	getline(loadGame, loadGameFile);
	for (int a = 0; a < loadGameFile.length(); a++)
	{
		charArray[a] = loadGameFile[a] == '1' ? true : false;
		if (charArray[a]) break;
	}

	getline(loadGame, loadGameFile);
	for (int a = 0; a < loadGameFile.length(); a++)
	{
		spawns[1] = false;
		spawns[2] = false;
		spawns[3] = false;
		spawns[4] = false;

		for (aaa = 0; aaa < 8; aaa++) {
			g_sHugger[aaa].m_cLocation.X = 0;
			g_sHugger[aaa].m_cLocation.Y = 0;
		}
		for (aaa = 0; aaa < 4; aaa++) {
			g_sGunner[aaa].m_cLocation.X = 0;
			g_sGunner[aaa].m_cLocation.Y = 0;
		}
		for (aaa = 0; aaa < 128; aaa++) {
			g_sBullets[aaa].m_cLocation.X = 0;
			g_sBullets[aaa].m_cLocation.Y = 0;
		}
		aaa = 0;

		if (collision('a')){
			levels[0] = false;
			levels[1] = true;
			spawns[0] = true;
		}
		else if (collision('b')){
			levels[0] = false;
			levels[2] = true;
			spawns[0] = true;
		}
		else if (collision('c')){
			levels[0] = false;
			levels[3] = true;
			spawns[0] = true;
		}
		else if (collision('d')){
			levels[0] = false;
			levels[4] = true;
			spawns[0] = true;
		}
		else if (collision('%')){
			levels[0] = false;
			inven = false;
			g_eGameState = S_WINSCREEN;
		}
	}

	else if (levels[1] == true){
		spawns[0] = false;

		while (aaa <= 0) {
			for (h = 0; h < 8; h++) { h_hit[h] = 0; }
			g_sHugger[0].m_cLocation.X = 6; g_sHugger[0].m_cLocation.Y = 16;
			g_sHugger[1].m_cLocation.X = 94; g_sHugger[1].m_cLocation.Y = 4;
			g_sHugger[2].m_cLocation.X = 84; g_sHugger[2].m_cLocation.Y = 17;
			g_sHugger[3].m_cLocation.X = 78; g_sHugger[3].m_cLocation.Y = 11;
			g_sHugger[4].m_cLocation.X = 35; g_sHugger[4].m_cLocation.Y = 12;
			g_sHugger[5].m_cLocation.X = 29; g_sHugger[5].m_cLocation.Y = 16;
			g_sGunner[0].m_cLocation.X = 54; g_sGunner[0].m_cLocation.Y = 4;
			g_sGunner[1].m_cLocation.X = 54; g_sGunner[1].m_cLocation.Y = 16;
			g_sGunner[2].m_cLocation.X = 40; g_sGunner[2].m_cLocation.Y = 8;
			aaa++;
		}
		if (collision('%')){
			levels[1] = false;
			levels[0] = true;
			spawns[1] = true;
		}
		if (collision('*')){
			gems[0] = true;
		}
	}
	else if (levels[2] == true){
		spawns[0] = false;

		while (aaa <= 0) {
			for (h = 0; h < 8; h++) { h_hit[h] = 0; }
			g_sHugger[0].m_cLocation.X = 6; g_sHugger[0].m_cLocation.Y = 10;
			g_sHugger[1].m_cLocation.X = 19; g_sHugger[1].m_cLocation.Y = 4;
			g_sHugger[2].m_cLocation.X = 34; g_sHugger[2].m_cLocation.Y = 11;
			g_sHugger[3].m_cLocation.X = 54; g_sHugger[3].m_cLocation.Y = 14;
			g_sHugger[4].m_cLocation.X = 39; g_sHugger[4].m_cLocation.Y = 13;
			g_sGunner[0].m_cLocation.X = 18; g_sGunner[0].m_cLocation.Y = 14;
			aaa++;
		}
		if (collision('%')){
			levels[2] = false;
			levels[0] = true;
			spawns[2] = true;
		}
		if (collision('&')){
			g_sChar.m_cLocation.X = 64;
			g_sChar.m_cLocation.Y = 2;
			for (h = 0; h < 8; h++) { h_hit[h] = 0; }
			g_sHugger[0].m_cLocation.X = 79; g_sHugger[0].m_cLocation.Y = 6;
			g_sHugger[1].m_cLocation.X = 69; g_sHugger[1].m_cLocation.Y = 12;
			g_sHugger[2].m_cLocation.X = 90; g_sHugger[2].m_cLocation.Y = 16;
			g_sHugger[3].m_cLocation.X = 90; g_sHugger[3].m_cLocation.Y = 19;
			g_sHugger[4].m_cLocation.X = 0; g_sHugger[4].m_cLocation.Y = 0;
			g_sGunner[0].m_cLocation.X = 81; g_sGunner[0].m_cLocation.Y = 4;
			g_sGunner[1].m_cLocation.X = 71; g_sGunner[1].m_cLocation.Y = 15;
			g_sGunner[2].m_cLocation.X = 86; g_sGunner[2].m_cLocation.Y = 16;
		}
		if (collision('*')){
			gems[1] = true;
		}
	}
	else if (levels[3] == true) {
		spawns[0] = false;

		while (aaa <= 0) {
			for (h = 0; h < 8; h++) { h_hit[h] = 0; }
			g_sHugger[0].m_cLocation.X = 24; g_sHugger[0].m_cLocation.Y = 7;
			g_sHugger[1].m_cLocation.X = 44; g_sHugger[1].m_cLocation.Y = 16;
			g_sHugger[2].m_cLocation.X = 6; g_sHugger[2].m_cLocation.Y = 16;
			g_sHugger[3].m_cLocation.X = 41; g_sHugger[3].m_cLocation.Y = 8;
			g_sGunner[0].m_cLocation.X = 10; g_sGunner[0].m_cLocation.Y = 4;
			g_sGunner[1].m_cLocation.X = 12; g_sGunner[1].m_cLocation.Y = 15;
			g_sGunner[2].m_cLocation.X = 33; g_sGunner[2].m_cLocation.Y = 4;
			g_sGunner[3].m_cLocation.X = 20; g_sGunner[3].m_cLocation.Y = 13;
			aaa++;
		}
		if (collision('%')){
			levels[3] = false;
			levels[0] = true;
			spawns[3] = true;
		}
		if (collision('&')){
			g_sChar.m_cLocation.X = 59;
			g_sChar.m_cLocation.Y = 2;
			for (h = 0; h < 8; h++) { h_hit[h] = 0; }
			g_sGunner[0].m_cLocation.X = 0; g_sGunner[0].m_cLocation.Y = 0;
			g_sGunner[1].m_cLocation.X = 0; g_sGunner[1].m_cLocation.Y = 0;
			g_sGunner[2].m_cLocation.X = 0; g_sGunner[2].m_cLocation.Y = 0;
			g_sGunner[3].m_cLocation.X = 0; g_sGunner[3].m_cLocation.Y = 0;
			g_sHugger[0].m_cLocation.X = 65; g_sHugger[0].m_cLocation.Y = 17;
			g_sHugger[1].m_cLocation.X = 80; g_sHugger[1].m_cLocation.Y = 17;
			g_sHugger[2].m_cLocation.X = 96; g_sHugger[2].m_cLocation.Y = 17;
			g_sHugger[3].m_cLocation.X = 82; g_sHugger[3].m_cLocation.Y = 23;
			g_sHugger[4].m_cLocation.X = 78; g_sHugger[4].m_cLocation.Y = 23;
			g_sHugger[5].m_cLocation.X = 80; g_sHugger[5].m_cLocation.Y = 11;
			g_sHugger[6].m_cLocation.X = 81; g_sHugger[6].m_cLocation.Y = 11;
			g_sHugger[7].m_cLocation.X = 82; g_sHugger[7].m_cLocation.Y = 11;
		}
		if (collision('*')){
			gems[2] = true;
		}
	}
	else if (levels[4] == true){
		spawns[0] = false;

		while (aaa <= 0) {
			for (h = 0; h < 8; h++) { h_hit[h] = 0; }
			g_sHugger[0].m_cLocation.X = 8; g_sHugger[0].m_cLocation.Y = 12;
			g_sHugger[1].m_cLocation.X = 8; g_sHugger[1].m_cLocation.Y = 13;
			g_sHugger[2].m_cLocation.X = 25; g_sHugger[2].m_cLocation.Y = 12;
			g_sHugger[3].m_cLocation.X = 26; g_sHugger[3].m_cLocation.Y = 12;
			g_sHugger[4].m_cLocation.X = 39; g_sHugger[4].m_cLocation.Y = 14;
			g_sHugger[5].m_cLocation.X = 39; g_sHugger[5].m_cLocation.Y = 15;
			g_sHugger[6].m_cLocation.X = 39; g_sHugger[6].m_cLocation.Y = 6;
			g_sHugger[7].m_cLocation.X = 40; g_sHugger[7].m_cLocation.Y = 6;
			g_sGunner[0].m_cLocation.X = 7; g_sGunner[0].m_cLocation.Y = 14;
			g_sGunner[1].m_cLocation.X = 27; g_sGunner[1].m_cLocation.Y = 12;
			g_sGunner[2].m_cLocation.X = 39; g_sGunner[2].m_cLocation.Y = 16;
			g_sGunner[3].m_cLocation.X = 41; g_sGunner[3].m_cLocation.Y = 6;
			aaa++;
		}
		if (collision('%')){
			levels[4] = false;
			levels[0] = true;
			spawns[4] = true;
		}
		if (collision('&')){
			g_sChar.m_cLocation.X = 60;
			g_sChar.m_cLocation.Y = 2;
			for (h = 0; h < 8; h++) { h_hit[h] = 0; }
			g_sHugger[0].m_cLocation.X = 60; g_sHugger[0].m_cLocation.Y = 12;
			g_sHugger[1].m_cLocation.X = 61; g_sHugger[1].m_cLocation.Y = 12;
			g_sHugger[2].m_cLocation.X = 84; g_sHugger[2].m_cLocation.Y = 11;
			g_sHugger[3].m_cLocation.X = 77; g_sHugger[3].m_cLocation.Y = 4;
			g_sHugger[4].m_cLocation.X = 60; g_sHugger[4].m_cLocation.Y = 15;
			g_sHugger[5].m_cLocation.X = 70; g_sHugger[5].m_cLocation.Y = 17;
			g_sHugger[6].m_cLocation.X = 89; g_sHugger[6].m_cLocation.Y = 19;
			g_sHugger[7].m_cLocation.X = 84; g_sHugger[7].m_cLocation.Y = 21;
			g_sGunner[0].m_cLocation.X = 62; g_sGunner[0].m_cLocation.Y = 12;
			g_sGunner[1].m_cLocation.X = 83; g_sGunner[1].m_cLocation.Y = 9;
			g_sGunner[2].m_cLocation.X = 60; g_sGunner[2].m_cLocation.Y = 18;
			g_sGunner[3].m_cLocation.X = 45; g_sGunner[3].m_cLocation.Y = 22;
			aaa++;
		}
		if (collision('*')){
			gems[3] = true;
		}
	}
}
void processUserInput()
{
	COORD c;
	// quits the game if player hits the escape key
	if (g_abKeyPressed[K_ESCAPE]) {
		saveProgression();
		g_bQuitGame = true;
	}
	//Start menu key presses
	if (g_eGameState == S_STARTMENU) {
		if (g_abKeyPressed[K_1]) {
			g_eGameState = S_CHARACTERCREATION;
		}
		if (g_abKeyPressed[K_2]) {
				loadProgression();
				g_eGameState = S_GAME;
		}
		if (g_abKeyPressed[K_3]) {
			g_eGameState = S_INSTRUCTIONS;
		}
	}
	//Instructions key presses
	if (g_eGameState == S_INSTRUCTIONS){
		if (g_abKeyPressed[K_RETURN]){
			g_eGameState = S_CHARACTERCREATION;
		}
	}
	//Character creation key presses
	if (g_eGameState == S_CHARACTERCREATION){
		if (g_abKeyPressed[K_1]){
			charArray[0] = true;
			charArray[1] = false;
			charArray[2] = false;
			charArray[3] = false;
			charArray[4] = false;
			lives = 7;
			coins = 0;
		}
		if (g_abKeyPressed[K_2]){
			charArray[1] = true;
			charArray[0] = false;
			charArray[2] = false;
			charArray[3] = false;
			charArray[4] = false;
			lives = 5;
			coins = 40;
		}
		if (g_abKeyPressed[K_3]){
			charArray[2] = true;
			charArray[0] = false;
			charArray[1] = false;
			charArray[3] = false;
			charArray[4] = false;
			lives = 5;
			coins = 40;
		}
		if (g_abKeyPressed[K_4]){
			charArray[3] = true;
			charArray[0] = false;
			charArray[1] = false;
			charArray[2] = false;
			charArray[4] = false;
			lives = 4;
			coins = 70;
		}
		if (g_abKeyPressed[K_5]){
			charArray[4] = true;
			charArray[0] = false;
			charArray[1] = false;
			charArray[2] = false;
			charArray[3] = false;
			lives = 3;
			coins = 100;
		}
		if (g_abKeyPressed[K_SPACE] && (charArray[0] == true || charArray[1] == true || charArray[2] == true || charArray[3] == true || charArray[4] == true)){
			g_eGameState = S_GAME;
		}
	}
	//Win and Lose screen key presses
	if (g_eGameState == S_WINSCREEN) {
		if (g_abKeyPressed[K_RETURN]){
			g_eGameState = S_STARTMENU;
		}
	}
	if (g_eGameState == S_LOSESCREEN) {
			/*DWORD now = timeGetTime();
			DWORD nextEndGame = now + 5000;
			while (g_eGameState==S_LOSESCREEN)
			{
				now = timeGetTime();
				if (now >= nextEndGame)
				{
					g_eGameState = S_CHARACTERCREATION;
					nextEndGame += 5000;
				}
			}*/
		if (g_abKeyPressed[K_RETURN]) {
			g_eGameState = S_STARTMENU;
		}
	}
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x0F);
}

void renderSplashScreen()
{
	//Render Splash Screen
	COORD c = g_Console.getConsoleSize();
	string splashscreen;
	ifstream splashscreenFile;
	int i = 0;

	splashscreenFile.open("TextFiles/SplashScreen.txt");
	if (splashscreenFile.is_open()){
		while (getline(splashscreenFile, splashscreen)){
			for (a = 0; a < splashscreen.length(); a++){
				if (splashscreen[a] == 'F'){
					splashscreen[a] = 178;
				}
				map[i][a] = splashscreen[a];
			}
			c.X = 0;
			c.Y = i;
			i++;
			g_Console.writeToBuffer(c, splashscreen, 0x0B);

			if (g_dElapsedTime > 0.5){
				c.X++;
				g_Console.writeToBuffer(c, splashscreen, 0x0C);
			}
			if (g_dElapsedTime > 1){
				c.X++;
				g_Console.writeToBuffer(c, splashscreen, 0x0A);
			}
			if (g_dElapsedTime > 1.5){
				c.X++;
				g_Console.writeToBuffer(c, splashscreen, 0x09);
			}
		}
	}
	splashscreenFile.close();
}

void renderStartMenu()
{
	//Render Start Menu
	COORD c;
	string menu;
	ifstream menuFile;
	int i = 0;
	
	PlaySound(TEXT("game_music.wav"), NULL, SND_LOOP | SND_ASYNC | SND_FILENAME);
	menuFile.open("TextFiles/StartMenu.txt");
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
	//Render Instructions
	COORD c;	
	string instructions;
	ifstream instructionFile;
	int i = 0;

	instructionFile.open("Textfiles/Instructions.txt");
	if (instructionFile.is_open()){
		while (getline(instructionFile, instructions)){
			c.X = 0;
			c.Y = i;
			i++;
			g_Console.writeToBuffer(c, instructions, 0x0B);
		}
	}
	instructionFile.close();
}

void renderCharacterCreation()
{
	//Render Character Creation
	COORD c;
	string creation;
	ifstream creationFile;
	int i = 0;
	
	creationFile.open("Textfiles/CharacterCreation.txt");
	if (creationFile.is_open()){
		while (getline(creationFile, creation)){
			for (a = 0; a < creation.length(); a++){
				switch (creation[a]) {
				case '6':
					creation[a] = (char)2;
					break;
				case '7':
					creation[a] = (char)3;
					break;
				case '8':
					creation[a] = (char)4;
					break;
				case '9':
					creation[a] = (char)5;
					break;
				case '0':
					creation[a] = (char)6;
					break;
				}
			}
			c.X = 0;
			c.Y = i;
			i++;
			g_Console.writeToBuffer(c, creation, 0x0B);
		}
	}
	creationFile.close();

	if (charArray[0] == true){
		c.X = 54;
		c.Y = 11;
		g_Console.writeToBuffer(c, names[0], 0x0B);
		c.X = 49;
		c.Y = 19;
		g_Console.writeToBuffer(c, (char)2, 0x0F);
	}
	if (charArray[1] == true){
		c.X = 54;
		c.Y = 11;
		g_Console.writeToBuffer(c, names[1], 0x0B);
		c.X = 49;
		c.Y = 19;
		g_Console.writeToBuffer(c, (char)3, 0x0C);
	}
	if (charArray[2] == true){
		c.X = 54;
		c.Y = 11;
		g_Console.writeToBuffer(c, names[2], 0x0B);
		c.X = 49;
		c.Y = 19;
		g_Console.writeToBuffer(c, (char)4, 0x09);
	}
	if (charArray[3] == true){
		c.X = 54;
		c.Y = 11;
		g_Console.writeToBuffer(c, names[3], 0x0B);
		c.X = 49;
		c.Y = 19;
		g_Console.writeToBuffer(c, (char)5, 0x0A);
	}
	if (charArray[4] == true){
		c.X = 54;
		c.Y = 11;
		g_Console.writeToBuffer(c, names[4], 0x0B);
		c.X = 49;
		c.Y = 19;
		g_Console.writeToBuffer(c, (char)6, 0x0E);
	}
}

void renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
	renderEntities();   // renders the hugger, gunner, player and enemy bullet into the buffer
	playershoot();
}
void renderWin()
{
	//Render Win Screen
	COORD c;
	string winScreen;
	ifstream winFile;
	int i = 0;
	PlaySound(NULL, NULL, 0);
	PlaySound(TEXT("miiWin.wav"), NULL, SND_LOOP | SND_ASYNC | SND_FILENAME);
	winFile.open("Textfiles/WinScreen.txt");
	if (winFile.is_open())
	{
		while (getline(winFile, winScreen))
		{
			for (a = 0; a < winScreen.length(); a++)
			{
				if (winScreen[a] == 'w')
				{
					winScreen[a] = 223;
				}
				map[i][a] = winScreen[a];
			}
			c.X = 0;
			c.Y = i;
			i++;
			g_Console.writeToBuffer(c, winScreen, 0x0A);
		}
	}
	winFile.close();

	c.X = 20;
	c.Y = 24;
	g_Console.writeToBuffer(c, "Press return to proceed, Press escape to quit the game", 0x08);
}

void renderLose()
{
	COORD c;
	string loseScreen;
	ifstream loseFile;
	int i = 0;

	PlaySound(TEXT("lose_Audio.wav"), NULL, SND_ASYNC | SND_FILENAME);
	loseFile.open("Textfiles/LoseScreen.txt");
	if (loseFile.is_open())
	{
		while (getline(loseFile, loseScreen))
		{
			for (a = 0; a < loseScreen.length(); a++)
			{
				if (loseScreen[a] == 'D')
				{
					loseScreen[a] = 219;
				}
				map[i][a] = loseScreen[a];
			}
			c.X = 0;
			c.Y = i;
			i++;
			g_Console.writeToBuffer(c, loseScreen, 0x0C);
		}
	}
	loseFile.close();

	c.X = 7;
	c.Y = 24;
	g_Console.writeToBuffer(c, "This is so sad, Alexa play Despacinno", 0x0C);
}
void renderMap()
{
	COORD c;
	int i = 0;

	//Render Headquarters
	if (levels[0] == true){
			string headquarters;
			ifstream headquartersFile;

			headquartersFile.open("Textfiles/Headquarters.txt");
			if (headquartersFile.is_open()){
				while (getline(headquartersFile, headquarters)){
					for (a = 0; a < headquarters.length(); a++){
						switch (headquarters[a]) {
						case '#':
							headquarters[a] = 223;
							break;
						case '@':
							headquarters[a] = 219;
							break;
						case 'u':
							if (gems[0] == true){
								headquarters[a] = '*';
							} else {
								headquarters[a] = 255;
							}
							break;
						case 'i':
							if (gems[1] == true){
								headquarters[a] = '*';
							} else {
								headquarters[a] = 255;
							}
							break;
						case 'o':
							if (gems[2] == true){
								headquarters[a] = '*';
							} else {
								headquarters[a] = 255;
							}
							break;
						case 'p':
							if (gems[3] == true){
								headquarters[a] = '*';
							} else {
								headquarters[a] = 255;
							}
							break;
						case '%':
							if (gems[0] == true && gems[1] == true && gems[2] == true && gems[3] == true){
								headquarters[a] = '%';
							} else {
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
	else if (levels[1] == true){
		string level1;
		ifstream level1File;
		i = 0;

		level1File.open("Textfiles/LevelA.txt");
		if (level1File.is_open()){
			while (getline(level1File, level1)){
				for (a = 0; a < level1.length(); a++){
					switch (level1[a]) {
					case '#':
						level1[a] = 223;
						break;
					case '@':
						level1[a] = 219;
						break;
					case '*':
						if (gems[0] == true){
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
	else if (levels[2] == true){
		string level2;
		ifstream level2File;
		i = 0;

		level2File.open("Textfiles/LevelB.txt");
		if (level2File.is_open()){
			while (getline(level2File, level2)){
				for (a = 0; a < level2.length(); a++){
					switch (level2[a]) {
					case '#':
						level2[a] = 223;
						break;
					case '@':
						level2[a] = 219;
						break;
					case '*':
						if (gems[1] == true){
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
	else if (levels[3] == true){
		string level3;
		ifstream level3File;
		i = 0;

		level3File.open("Textfiles/LevelC.txt");
		if (level3File.is_open()){
			while (getline(level3File, level3)){
				for (a = 0; a < level3.length(); a++){
					switch (level3[a]) {
					case '#':
						level3[a] = 223;
						break;
					case '@':
						level3[a] = 219;
						break;
					case '*':
						if (gems[2] == true){
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
	else if (levels[4] == true){
		string level4;
		ifstream level4File;
		i = 0;

		level4File.open("Textfiles/LevelD.txt");
		if (level4File.is_open()){
			while (getline(level4File, level4)){
				for (a = 0; a < level4.length(); a++){
					switch (level4[a]) {
					case '#':
						level4[a] = 223;
						break;
					case '@':
						level4[a] = 219;
						break;
					case '*':
						if (gems[3] == true){
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
	if (shop == true){
		string shop;
		ifstream shopFile;
		i = 0;

		shopFile.open("Textfiles/Shop.txt");
		if (shopFile.is_open()){
			while (getline(shopFile, shop)){
				for (a = 0; a < shop.length(); a++){
					switch (shop[a]) {
					case '#':
						shop[a] = 223;
						break;
					case '@':
						shop[a] = 219;
						break;
					}
				}
				c.X = 39;
				c.Y = 17 + i;
				i++;
				g_Console.writeToBuffer(c, shop, 0x00 + i);
			}
		}
		shopFile.close();

		//Buying Smg
		if (g_abKeyPressed[K_2] && boughtWeapons[1] == false && (coins >= 40)){
			coins -= 40;
			boughtWeapons[1] = true;
		}
		//Buying Rifle
		if (g_abKeyPressed[K_3] && boughtWeapons[2] == false && (coins >= 70)){
			 coins -= 70;
			 boughtWeapons[2] = true;
		}
		//Buying Sniper
		if (g_abKeyPressed[K_4] && boughtWeapons[3] == false && (coins >= 85)){
			coins -= 85;
			boughtWeapons[3] = true;
		}
		//Buying Minigun
		if (g_abKeyPressed[K_5] && boughtWeapons[4] == false && (coins >= 200)){
			coins -= 200;
			boughtWeapons[4] = true;
		}
	}
	//Render Inventory
	if (inven == true){
		string inventory;
		ifstream inventoryFile;
		i = 0;

		inventoryFile.open("Textfiles/Inventory.txt");
		if (inventoryFile.is_open()){
			while (getline(inventoryFile, inventory)){
				for (a = 0; a < inventory.length(); a++){
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
						if (boughtWeapons[0] == true){
							inventory[a] = '1';
						} else {
							inventory[a] = 'x';
						}
						break;
					case '2':
						if (boughtWeapons[1] == true){
							if (shop == true){
								c.X = 52;
								c.Y = 22;
								g_Console.writeToBuffer(c, "Sold Out ", 0x06);
							}
							inventory[a] = '2';
						} else {
							inventory[a] = 'x';
						}
						break;
					case '3':
						if (boughtWeapons[2] == true){
							if (shop == true){
								c.X = 52;
								c.Y = 23;
								g_Console.writeToBuffer(c, "Sold Out ", 0x07);
							}
							inventory[a] = '3';
						} else {
							inventory[a] = 'x';
						}
						break;
					case '4':
						if (boughtWeapons[3] == true){
							if (shop == true){
								c.X = 52;
								c.Y = 24;
								g_Console.writeToBuffer(c, "Sold Out ", 0x08);
							}
							inventory[a] = '4';
						} else {
							inventory[a] = 'x';
						}
						break;
					case '5':
						if (boughtWeapons[4] == true){
							if (shop == true){
								c.X = 52;
								c.Y = 25;
								g_Console.writeToBuffer(c, "Sold Out ", 0x09);
							}
							inventory[a] = '5';
						} else {
							inventory[a] = 'x';
						}
						break;
					case '6':
						if (gems[0] == true){
							inventory[a] = '*';
						} else {
							inventory[a] = 'x';
						}
						break;
					case '7':
						if (gems[1] == true){
							inventory[a] = '*';
						} else {
							inventory[a] = 'x';
						}
						break;
					case '8':
						if (gems[2] == true){
							inventory[a] = '*';
						} else {
							inventory[a] = 'x';
						}
						break;
					case '9':
						if (gems[3] == true){
							inventory[a] = '*';
						} else {
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

		//Render Lives
		c.X = 9;
		c.Y = 21;
		g_Console.writeToBuffer(c, to_string(lives), 0x0F);
		//Render Coins
		c.X = 21;
		c.Y = 21;
		g_Console.writeToBuffer(c, to_string(coins), 0x0F);
		//Render Current Gun
		if (equipWeapons[0] == true){
			c.X = 4;
			c.Y = 24;
			g_Console.writeToBuffer(c, "Pistol", 0x08);
		}
		if (equipWeapons[1] == true){
			c.X = 4;
			c.Y = 24;
			g_Console.writeToBuffer(c, "Smg", 0x08);
		}
		if (equipWeapons[2] == true){
			c.X = 4;
			c.Y = 24;
			g_Console.writeToBuffer(c, "Rifle", 0x08);
		}
		if (equipWeapons[3] == true){
			c.X = 4;
			c.Y = 24;
			g_Console.writeToBuffer(c, "Sniper", 0x08);
		}
		if (equipWeapons[4] == true){
			c.X = 4;
			c.Y = 24;
			g_Console.writeToBuffer(c, "Minigun", 0x08);
		}
	}
}
void renderCharacter()
{
	COORD c;

	if (collision('q')){
		charArray[0] = true;
		charArray[1] = false;
		charArray[2] = false;
		charArray[3] = false;
		charArray[4] = false;
	}
	if (collision('w')){
		charArray[1] = true;
		charArray[0] = false;
		charArray[2] = false;
		charArray[3] = false;
		charArray[4] = false;
	}
	if (collision('e')){
		charArray[2] = true;
		charArray[0] = false;
		charArray[1] = false;
		charArray[3] = false;
		charArray[4] = false;
	}
	if (collision('r')){
		charArray[3] = true;
		charArray[0] = false;
		charArray[1] = false;
		charArray[2] = false;
		charArray[4] = false;
	}
	if (collision('t')){
		charArray[4] = true;
		charArray[0] = false;
		charArray[1] = false;
		charArray[2] = false;
		charArray[3] = false;
	}
	//Characters' rendering
	if (charArray[0] == true){
		if (inven == true){
			c.X = 8;
			c.Y = 20;
			g_Console.writeToBuffer(c, names[0], 0x0F);
		}
		g_Console.writeToBuffer(g_sChar.m_cLocation, (char)2, 0x0F);
	}
	else if (charArray[1] == true){
		if (inven == true){
			c.X = 8;
			c.Y = 20;
			g_Console.writeToBuffer(c, names[1], 0x0F);
		}
		g_Console.writeToBuffer(g_sChar.m_cLocation, (char)3, 0x0C);
	}
	else if (charArray[2] == true){
		if (inven == true){
			c.X = 8;
			c.Y = 20;
			g_Console.writeToBuffer(c, names[2], 0x0F);
		}
		g_Console.writeToBuffer(g_sChar.m_cLocation, (char)4, 0x09);
	}
	else if (charArray[3] == true){
		if (inven == true){
			c.X = 8;
			c.Y = 20;
			g_Console.writeToBuffer(c, names[3], 0x0F);
		}
		g_Console.writeToBuffer(g_sChar.m_cLocation, (char)5, 0x0A);
	}
	else if (charArray[4] == true){
		if (inven == true){
			c.X = 8;
			c.Y = 20;
			g_Console.writeToBuffer(c, names[4], 0x0F);
		}
		g_Console.writeToBuffer(g_sChar.m_cLocation, (char)6, 0x0E);
	}
}

void renderEntities()
{
	// Draw the location of the enemies
	WORD charE_Color = 0x0C;

	// For player bullets
	for (int ps = 0; ps < 64; ps++) {
		g_Console.writeToBuffer(g_sPlayershots[ps].m_cLocation, (char)249, 0x06);
	}
	if (levels[0] == false)
	{
		for (int h = 0; h < 8; h++) {
			g_Console.writeToBuffer(g_sHugger[h].m_cLocation, (char)72, charE_Color);
		}
		for (int g = 0; g < 4; g++) {
			g_Console.writeToBuffer(g_sGunner[g].m_cLocation, (char)83, charE_Color);
		}
		// For enemy bullets
		for (int i = 0; i < 128; i++) {
			g_Console.writeToBuffer(g_sBullets[i].m_cLocation, (char)7, charE_Color);
		}
	}
}
void renderWin()
{
	//Render Win Screen
	COORD c;
	string winScreen;
	ifstream winFile;
	int i = 0;
	PlaySound(NULL, NULL, 0);
	PlaySound(TEXT("miiWin.wav"), NULL, SND_LOOP | SND_ASYNC | SND_FILENAME);
	winFile.open("Textfiles/WinScreen.txt");
	if (winFile.is_open())
	{
		while (getline(winFile, winScreen))
		{
			for (a = 0; a < winScreen.length(); a++)
			{
				if (winScreen[a] == 'w')
				{
					winScreen[a] = 223;
				}
				map[i][a] = winScreen[a];
			}
			c.X = 0;
			c.Y = i;
			i++;
			g_Console.writeToBuffer(c, winScreen, 0x0A);
		}
	}
	winFile.close();

	c.X = 20;
	c.Y = 24;
	g_Console.writeToBuffer(c, "Press return to proceed, Press escape to quit the game", 0x08);
}
void renderLose()
{
	COORD c;
	string loseScreen;
	ifstream loseFile;
	int i = 0;

	PlaySound(TEXT("lose_Audio.wav"), NULL, SND_ASYNC | SND_FILENAME);
	loseFile.open("Textfiles/LoseScreen.txt");
	if (loseFile.is_open())
	{
		while (getline(loseFile, loseScreen))
		{
			for (a = 0; a < loseScreen.length(); a++)
			{
				if (loseScreen[a] == 'D')
				{
					loseScreen[a] = 219;
				}
				map[i][a] = loseScreen[a];
			}
			c.X = 0;
			c.Y = i;
			i++;
			g_Console.writeToBuffer(c, loseScreen, 0x0C);
		}
	}
	loseFile.close();

	c.X = 7;
	c.Y = 24;
	g_Console.writeToBuffer(c, "This is so sad, Alexa play Despacinno", 0x0C);
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