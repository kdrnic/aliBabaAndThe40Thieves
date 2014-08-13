UBYTE pickupX[8];
UBYTE pickupY[8];
UBYTE pickupType[8];
UBYTE pickupAlive[8];
UBYTE pickupIndex;

void SpawnPickup(UWORD x, UWORD y, UBYTE type)
{
	pickupType[(UWORD) pickupIndex] = type;
	pickupAlive[(UWORD) pickupIndex] = 1;
	pickupX[(UWORD) pickupIndex] = x;
	pickupY[(UWORD) pickupIndex] = y;
	//Dagger
	if(type == 0) set_sprite_tile(30 + pickupIndex, 92);
	//Crescent
	if(type == 1) set_sprite_tile(30 + pickupIndex, 96);
	//First in, last out
	pickupIndex++;
	if(pickupIndex == 8) pickupIndex = 0;
}

void KillPickup(UBYTE n)
{
	pickupAlive[(UWORD) n] = 0;
	move_sprite(30 + n, 0, 0);
}

void SetPickupPositions()
{
	for(tempub1 = 0; tempub1 != 8; tempub1++)
	{
		if(!pickupAlive[(UWORD) tempub1]) continue;
		pickupX[(UWORD) tempub1] += 8;
		pickupY[(UWORD) tempub1] += 16;
		pickupX[(UWORD) tempub1] -= scrollX;
		pickupY[(UWORD) tempub1] -= scrollY;
		move_sprite(30U + tempub1, pickupX[(UWORD) tempub1], pickupY[(UWORD) tempub1]);
		pickupX[(UWORD) tempub1] += scrollX;
		pickupY[(UWORD) tempub1] += scrollY;
		pickupX[(UWORD) tempub1] -= 8;
		pickupY[(UWORD) tempub1] -= 16;
	}
}

UBYTE rockX, rockY;
UBYTE rockDirection;
UBYTE rockAlive;
UBYTE rockAge;

void SetRockPosition()
{
	if(rockAlive)
	{
		rockX += 8;
		rockY += 16;
		rockX -= scrollX;
		rockY -= scrollY;
		move_sprite(4, rockX, rockY);
		rockX += scrollX;
		rockY += scrollY;
		rockX -= 8;
		rockY -= 16;
	}
	else
	{
		move_sprite(4, 0, 0);
	}
}

UBYTE vaseX, vaseY;
UBYTE vaseAlive;

void SetVasePosition()
{
	if(vaseAlive)
	{
		vaseX += 8;
		vaseY += 16;
		vaseX -= scrollX;
		vaseY -= scrollY;
		move_sprite(5, vaseX, vaseY);
		move_sprite(6, vaseX, vaseY + 16);
		move_sprite(7, vaseX + 8, vaseY);
		move_sprite(8, vaseX + 8, vaseY + 16);
		vaseX += scrollX;
		vaseY += scrollY;
		vaseX -= 8;
		vaseY -= 16;
	}
	else
	{
		move_sprite(5, 0, 0);
		move_sprite(6, 0, 0);
		move_sprite(7, 0, 0);
		move_sprite(8, 0, 0);
	}
}

void SpawnVase()
{
	//Shitty "RNG"
	vaseX = (time & 31);
	vaseY = (time & 31);
	vaseX ^= 17;
	vaseY ^= 6;
	if(vaseX >= 30) vaseX = 29;
	vaseX = vaseX << 3;
	vaseY = vaseY << 3;
	//Avoids spawning above a cactus or wall
	AIM_AT_TILE(vaseX, vaseY);
	if(GET_TILE() >= minSolidTile) return;
	AIM_AT_TILE(vaseX, vaseY + 8);
	if(GET_TILE() >= minSolidTile) return;
	AIM_AT_TILE(vaseX, vaseY + 16);
	if(GET_TILE() >= minSolidTile) return;
	AIM_AT_TILE(vaseX + 8, vaseY);
	if(GET_TILE() >= minSolidTile) return;
	AIM_AT_TILE(vaseX + 8, vaseY + 8);
	if(GET_TILE() >= minSolidTile) return;
	AIM_AT_TILE(vaseX + 8, vaseY + 16);
	if(GET_TILE() >= minSolidTile) return;
	Collision(0, 0, 255, 255, vaseX, vaseY, 16, 24);
	if(!tempub2) return;
	vaseAlive = 1;
}

UBYTE snakeX, snakeY;
UBYTE snakeDx, snakeDy;
UBYTE snakeAlive;
UBYTE snakeFrame;
UBYTE snakeAge;

void SetSnakeTiles()
{
	if(snakeDx)
	{
		set_sprite_tile(9, 76 + (snakeFrame << 2));
		set_sprite_tile(10, 78 + (snakeFrame << 2));
		set_sprite_prop(9, SPR_DEFLT);
		set_sprite_prop(10, SPR_DEFLT);
	}
	else
	{
		set_sprite_tile(9, 78 + (snakeFrame << 2));
		set_sprite_tile(10, 76 + (snakeFrame << 2));
		set_sprite_prop(9, SPR_FLIPX);
		set_sprite_prop(10, SPR_FLIPX);
	}
}

void SetSnakePosition()
{
	if(snakeAlive)
	{
		snakeX += 8;
		snakeY += 16;
		snakeX -= scrollX;
		snakeY -= scrollY;
		move_sprite(9, snakeX, snakeY);
		move_sprite(10, snakeX + 8, snakeY);
		snakeX += scrollX;
		snakeY += scrollY;
		snakeX -= 8;
		snakeY -= 16;
	}
	else
	{
		move_sprite(9, 0, 0);
		move_sprite(10, 0, 0);
	}
}

void UpdateSnake()
{
	//The snake walks in 45º angles and bounces against the stage's solid tiles
	snakeFrame = (time & 8) >> 3;
	if(snakeDx)
	{
		AIM_AT_TILE(snakeX + 16, snakeY);
		tempub1 = (GET_TILE() >= minSolidTile);
		AIM_AT_TILE(snakeX + 16, snakeY + 15);
		tempub1 |= (GET_TILE() >= minSolidTile);
		if(!tempub1) snakeX++;
		else snakeDx = 0;
	}
	else
	{
		AIM_AT_TILE(snakeX - 1, snakeY);
		tempub1 = (GET_TILE() >= minSolidTile);
		AIM_AT_TILE(snakeX - 1, snakeY + 15);
		tempub1 |= (GET_TILE() >= minSolidTile);
		if(!tempub1) snakeX--;
		else snakeDx = 1;
	}
	if(snakeDy)
	{
		AIM_AT_TILE(snakeX, snakeY + 16);
		tempub1 = (GET_TILE() >= minSolidTile);
		AIM_AT_TILE(snakeX + 15, snakeY + 16);
		tempub1 |= (GET_TILE() >= minSolidTile);
		if(!tempub1) snakeY++;
		else snakeDy = 0;
	}
	else
	{
		AIM_AT_TILE(snakeX, snakeY - 1);
		tempub1 = (GET_TILE() >= minSolidTile);
		AIM_AT_TILE(snakeX + 15, snakeY - 1);
		tempub1 |= (GET_TILE() >= minSolidTile);
		if(!tempub1) snakeY--;
		else snakeDy = 1;
	}
	
	//snakeAge increases twice as slow as time
	snakeAge += (time >> 1) & 1;
	if(snakeAge > 120) snakeAlive = 0;
	
	//This is probably the source of the snake's slowdowns
	for(tempub1 = 0; tempub1 != numberOfEnemies; tempub1++)
	{
		if(enemyAlive[(UWORD) tempub1])
		{
			Collision(enemyX[(UWORD) tempub1], enemyY[(UWORD) tempub1], 16, 32, snakeX, snakeY, 16, 16);
			if(tempub2)
			{
				//Will spawn a dagger or a crescent at the feet of the fallen enemy
				//Dagger is 7X more probable than crescent
				SpawnPickup(enemyX[(UWORD) tempub1] + 4, enemyY[(UWORD) tempub1] + 20, ((time & 7) == 0));
				enemyAlive[(UWORD) tempub1] = 0;
			}
		}
	}
}

/*
Javascript to translate text to my font:
function Translate(s,o){if(typeof(o)=="undefined")o=0;s=s.toUpperCase();a = function(c){return "ABCDEGIKNOPRSTUX0123456789©†☪ MV".indexOf(c);};s2="";for(var i=0;i<s.length;i++){if(a(s[i])!=255)s2+=(a(s[i])+o);else s2+="255";if(i!=s.length - 1)s2+=",";}return s2;}
s: string
o: offset (position of the font in tile memory)
*/
const UBYTE startScreenText[] = {168,169,162,170,170,255,170,171,158,169,171,255,159,172,171,171,167,166,255,255,255,184,176,174,175,178,255,161,169,166,164,160,165,255,255,255};
UBYTE windowText[8] = {90,90,90,91,89,77,78,78};
UBYTE gameOverText[] = {5,0,30,4,29,9,31,4,11};
UBYTE congraturationsText[] = {2,9,8,5,11,0,13,14,11,0,13,6,9,8,12};
UBYTE babaGotTreasureText[] = {1,0,1,0,29,5,9,13,29,13,11,4,0,12,14,11,4};

void SetWindowText()
{
	if(extraLives != 0) windowText[0] = 90;
	else windowText[0] = 91;
	if(extraLives > 1) windowText[1] = 90;
	else windowText[1] = 91;
	if(extraLives > 2) windowText[2] = 90;
	else windowText[2] = 91;
	windowText[6] = 78 + (numberOfDaggers / 10);
	windowText[7] = 78 + (numberOfDaggers % 10);
}