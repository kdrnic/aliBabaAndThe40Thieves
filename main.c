#include <gb/gb.h>
#include <gb/sample.h>

#include "scimitar.h"
#include "title.h"
#include "font.h"
#include "stage.h"
#include "sprites.h"
#include "treasureRoom.h"

UBYTE scimitarNextFrame;
UBYTE time;
UWORD tempuw1, tempuw2;
UBYTE tempub1, tempub2, tempub3;
UBYTE tempub4, tempub5;
UBYTE scrollX, scrollY;
UBYTE numberOfDaggers;
UBYTE extraLives;
UBYTE oldJoypad;
UBYTE deathCounter;
UBYTE *map, minSolidTile;
UBYTE mapWidth;

#define AIM_AT_TILE(x,y) \
	tempuw1 = (UWORD) x; \
	tempuw2 = (UWORD) y; \
	tempuw1 = tempuw1 >> 3; \
	tempuw2 = tempuw2 >> 3; \
	tempuw1 &= 31; \
	tempuw2 &= 31;
#define GET_TILE() \
	map[(tempuw2 * mapWidth) + tempuw1]

void Collision(UBYTE x1, UBYTE y1, UBYTE w1, UBYTE h1, UBYTE x2, UBYTE y2, UBYTE w2, UBYTE h2)
{
	tempub2 = 1;
	tempub4 = x1 + w1;
	tempub5 = x2;
	if(tempub4 < tempub5)
	{
		tempub2 = 0;
		return;
	}
	tempub4 = x2 + w2;
	tempub5 = x1;
	if(tempub4 < tempub5)
	{
		tempub2 = 0;
		return;
	}
	tempub4 = y1 + h1;
	tempub5 = y2;
	if(tempub4 < tempub5)
	{
		tempub2 = 0;
		return;
	}
	tempub4 = y2 + h2;
	tempub5 = y1;
	if(tempub4 < tempub5)
	{
		tempub2 = 0;
		return;
	}
}

#define SPR_DEFLT 0
#define SPR_FLIPX S_FLIPX

UBYTE playerX, playerY;
#include "enemies.h"
#include "misc.h"
#include "player.h"

void ClearScreen()
{
	//Sets the entire background map to 0xFF
	for(tempuw1 = 0x9800; tempuw1 != 0x9C00; tempuw1++)
	{
		*((UBYTE *) tempuw1) = 0xFF;
	}
	//Sets the tile of number 0xFF to be a completely dark tile
	for(tempuw1 = 0x8FF0; tempuw1 != 0x9000; tempuw1++)
	{
		*((UBYTE *) tempuw1) = 0xFF;
	}
	//Moves all sprites to 0,0 to make them invisible
	for(tempub1 = 0; tempub1 != 40; tempub1++)
	{
		move_sprite(tempub1, 0, 0);
	}
}

void WaitStart()
{
	while(1)
	{
		tempub3 = joypad();
		if(tempub3 & J_START) break;
	}
	while(1)
	{
		tempub3 = joypad();
		if(tempub3 & J_START) continue;
		else break;
	}
}

void main()
{
	StartScreen:
	
	wait_vbl_done();
	disable_interrupts();
	DISPLAY_OFF;
	
	//& 0xDF is to disable the window
	LCDC_REG = 0x67 & 0xDF;
	
	scimitarNextFrame = 1;
	time = 0;
	
	//Clear screen
	ClearScreen();
	
	//Load first frame of the scimitar
	set_bkg_data(0, scimitarTileAmounts[0], scimitarTiles[0]);
	set_bkg_tiles(0, 10, 20, 3, scimitarMaps[0]);
	
	//Load the fancy text that says "Ali Baba And The 40 Thieves"
	set_bkg_data(titleOffset, titleTileAmount, titleTiles);
	set_bkg_tiles(2, 2, 16, 6, titleMap);
	
	//Load the "PRESS START BUTON ©2014 drnick" text
	set_bkg_data(titleOffset + titleTileAmount, fontTileAmount, fontTiles);
	set_bkg_tiles(1, 14, 18, 2, startScreenText);
	
	//Move the window away from the screen so that it remains unseen
	WX_REG = MAXWNDPOSX;
	WY_REG = MAXWNDPOSY;
	
	//Set palettes, 0 is lightest, 3 is darkest
	BGP_REG = OBP0_REG = OBP1_REG = 0xE4;
	
	DISPLAY_ON;
	enable_interrupts();
	
	while(1)
	{
		wait_vbl_done();
		if(!(time & 3))
		{
			//Load tiles to the next scimitar frame
			if(scimitarNextFrame & 1)
			{
				set_bkg_data(scimitarMaxTile, scimitarTileAmounts[(UWORD)scimitarNextFrame], scimitarTiles[(UWORD)scimitarNextFrame]);
			}
			else
			{
				set_bkg_data(0, scimitarTileAmounts[(UWORD)scimitarNextFrame], scimitarTiles[(UWORD)scimitarNextFrame]);
			}
			//Set the tile indices for the next scimitar frame
			set_bkg_tiles(0, 10, 20, 3, scimitarMaps[(UWORD)scimitarNextFrame]);
			scimitarNextFrame++;
			if(scimitarNextFrame == 10) scimitarNextFrame = 0;
		}
		time++;
		
		tempub1 = joypad();
		if(tempub1 & J_START) break;
	}
	
	StartGame:
	
	numberOfDaggers = 0;
	extraLives = 3;
	
	Stage:
	
	wait_vbl_done();
	DISPLAY_OFF;
	disable_interrupts();
	
	playerX = 0;
	playerY = 48;
	playerDied = 0;
	scrollX = scrollY = 0;
	playerNextFrame = 0;
	SetPlayerPosition();
	rockX = rockY = rockAlive = rockDirection = 0;
	SetPlayerTiles();
	SetWindowText();
	numberOfEnemies = 4;
	pickupIndex = 0;
	deathCounter = 0;
	mapWidth = 32;
	KillPickup(0);
	KillPickup(1);
	KillPickup(2);
	KillPickup(3);
	KillPickup(4);
	KillPickup(5);
	KillPickup(6);
	KillPickup(7);
	enemyAlive[0] = enemyAlive[1] = enemyAlive[2] = enemyAlive[3] = 0;
	minSolidTile = stageMinSolidTile;
	map = stageMap;
	
	LCDC_REG = 0x67;
	
	//Load the stage background and corresponding tiles
	set_bkg_data(0, stageTileAmount, stageTiles);
	set_bkg_tiles(0, 0, 32, 32, stageMap);
	
	//Load the font (used in the window)
	set_bkg_data(stageTileAmount, fontTileAmount, fontTiles);
	
	//Load the sprites
	set_sprite_data(0, spritesTileAmount, spritesTiles);
	
	//Set up the window
	WX_REG = 7;
	WY_REG = 136;
	SetWindowText();
	tempub2 = 91;
	tempub1 = 0;
	while(tempub1 != 20)
	{
		set_win_tiles(tempub1, 0, 1, 1, &tempub2);
		tempub1++;
	}
	set_win_tiles(0, 0, 8, 1, windowText);
	
	//Set up the rock sprite
	set_sprite_tile(4, 94);
	set_sprite_prop(4, SPR_DEFLT);
	
	//Set up the vase sprites
	set_sprite_tile(5, 84);
	set_sprite_tile(6, 86);
	set_sprite_tile(7, 88);
	set_sprite_tile(8, 90);
	set_sprite_prop(5, SPR_DEFLT);
	set_sprite_prop(6, SPR_DEFLT);
	set_sprite_prop(7, SPR_DEFLT);
	set_sprite_prop(8, SPR_DEFLT);
	
	//Set up the enemies' heads' sprites
	for(tempub1 = 0; tempub1 != 4; tempub1++)
	{
		set_sprite_prop(11 + (tempub1 << 2), SPR_DEFLT);
		set_sprite_prop(12 + (tempub1 << 2), SPR_DEFLT);
	}
	
	DISPLAY_ON;
	enable_interrupts();
	
	while(1)
	{
		time++;
		
		UpdatePlayer();
		if(snakeAlive) UpdateSnake();
		SpawnEnemies();
		UpdateEnemies();
		if(!vaseAlive) SpawnVase();
		
		if(windowText[6] == 82)
		{
			goto TreasureScreen;
		}
		
		wait_vbl_done();
		SetPlayerPosition();
		SetPlayerTiles();
		SetRockPosition();
		SetVasePosition();
		SetSnakePosition();
		SetSnakeTiles();
		SetEnemySprites();
		SetPickupPositions();
		set_win_tiles(0, 0, 8, 1, windowText);
		move_bkg(scrollX, scrollY);
		if(playerDied)
		{
			deathCounter++;
			if(deathCounter == 120)
			{
				break;
			}
		}
	}
	
	if(extraLives > 0)
	{
		extraLives--;
		goto Stage;
	}
	
	GameOver:
	
	wait_vbl_done();
	DISPLAY_OFF;
	disable_interrupts();
	
	//& 0xDF to disable window
	LCDC_REG = 0x67 & 0xDF;
	
	ClearScreen();
	
	//Set up some text in a black screen
	set_bkg_data(0, fontTileAmount, fontTiles);
	set_bkg_tiles(5, 9, 9, 1, gameOverText);
	move_bkg(0, 0);
	
	DISPLAY_ON;
	enable_interrupts();
	
	WaitStart();
	
	goto StartScreen;
	
	Congraturations:
	
	wait_vbl_done();
	DISPLAY_OFF;
	disable_interrupts();
	
	//& 0xDF to disable window
	LCDC_REG = 0x67 & 0xDF;
	
	ClearScreen();
	
	//Set up some text in a black screen
	set_bkg_data(0, fontTileAmount, fontTiles);
	set_bkg_tiles(2, 8, 15, 1, congraturationsText);
	set_bkg_tiles(1, 9, 17, 1, babaGotTreasureText);
	move_bkg(0, 0);
	
	DISPLAY_ON;
	enable_interrupts();
	
	WaitStart();
	
	goto StartScreen;
	
	TreasureScreen:
	
	wait_vbl_done();
	DISPLAY_OFF;
	disable_interrupts();
	
	playerX = 20;
	playerY = 144;
	playerDied = 0;
	scrollX = scrollY = 0;
	playerNextFrame = 0;
	rockX = rockY = rockAlive = rockDirection = 0;
	//Since UpdatePlayer is the same, enemies that are still alive but invisible and still could kill the player otherwise
	enemyAlive[0] = enemyAlive[1] = enemyAlive[2] = enemyAlive[3] = 0;
	ClearScreen();
	minSolidTile = treasureMinSolidTile;
	map = treasureMap;
	mapWidth = 27;
	
	LCDC_REG = 0x67 & 0xDF;
	
	//Load the stage background and corresponding tiles
	set_bkg_data(0, treasureTileAmount, treasureTiles);
	set_bkg_tiles(0, 0, 27, 27, treasureMap);
	
	//Load the sprites
	set_sprite_data(0, spritesTileAmount, spritesTiles);
	
	DISPLAY_ON;
	enable_interrupts();
	
	while(1)
	{
		time++;
		UpdatePlayer();
		
		wait_vbl_done();
		SetPlayerPosition();
		SetPlayerTiles();
		move_bkg(scrollX, scrollY);
		
		//Checks if the player is sufficiently near the treasure
		Collision(56, 112, 104, 32, playerX, playerY, 16, 32);
		if(tempub2)
		{
			goto Congraturations;
		}
	}
}