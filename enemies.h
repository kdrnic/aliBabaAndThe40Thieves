#define MAX_ENEMIES 4
#define MAX_ENEMIES_MINUS_ONE 3

UBYTE enemyX[MAX_ENEMIES], enemyY[MAX_ENEMIES];
UBYTE enemyAlive[MAX_ENEMIES];
UBYTE enemyFrame[MAX_ENEMIES];
UBYTE numberOfEnemies;

void UpdateEnemies()
{
	for(tempub1 = 0; tempub1 != numberOfEnemies; tempub1++)
	{
		if(enemyAlive[(UWORD) tempub1])
		{
			enemyFrame[(UWORD) tempub1] &= 12;
			//Basically enemies walk in a "down, right, up, left" pattern, turning whenever they meet a solid tile
			switch(enemyFrame[(UWORD) tempub1])
			{
				case 0:
					if(time & 1) break;
					AIM_AT_TILE(enemyX[(UWORD) tempub1], enemyY[(UWORD) tempub1] + 32);
					tempub2 = (GET_TILE() >= minSolidTile);
					AIM_AT_TILE(enemyX[(UWORD) tempub1] + 15, enemyY[(UWORD) tempub1] + 32);
					tempub2 |= (GET_TILE() >= minSolidTile);
					if(tempub2) enemyFrame[(UWORD) tempub1] = 12;
					else enemyY[(UWORD) tempub1]++;
					break;
				case 4:
					if(time & 1) break;
					AIM_AT_TILE(enemyX[(UWORD) tempub1], enemyY[(UWORD) tempub1] + 15);
					tempub2 = (GET_TILE() >= minSolidTile);
					AIM_AT_TILE(enemyX[(UWORD) tempub1] + 15, enemyY[(UWORD) tempub1] + 15);
					tempub2 |= (GET_TILE() >= minSolidTile);
					if(tempub2) enemyFrame[(UWORD) tempub1] = 8;
					else enemyY[(UWORD) tempub1]--;
					break;
				case 8:
					if(time & 1) break;
					AIM_AT_TILE(enemyX[(UWORD) tempub1] - 1, enemyY[(UWORD) tempub1] + 16);
					tempub2 = (GET_TILE() >= minSolidTile);
					AIM_AT_TILE(enemyX[(UWORD) tempub1] - 1, enemyY[(UWORD) tempub1] + 31);
					tempub2 |= (GET_TILE() >= minSolidTile);
					if(tempub2) enemyFrame[(UWORD) tempub1] = 0;
					else enemyX[(UWORD) tempub1]--;
					break;
				case 12:
					if(time & 1) break;
					AIM_AT_TILE(enemyX[(UWORD) tempub1] + 16, enemyY[(UWORD) tempub1] + 16);
					tempub2 = (GET_TILE() >= minSolidTile);
					AIM_AT_TILE(enemyX[(UWORD) tempub1] + 16, enemyY[(UWORD) tempub1] + 31);
					tempub2 |= (GET_TILE() >= minSolidTile);
					if(tempub2) enemyFrame[(UWORD) tempub1] = 4;
					else enemyX[(UWORD) tempub1]++;
					break;
			}
			enemyFrame[(UWORD) tempub1] += ((time >> 3) & 3);
		}
	}
}

void SetEnemySprites()
{
	for(tempub1 = 0; tempub1 != numberOfEnemies; tempub1++)
	{
		if(enemyAlive[(UWORD) tempub1])
		{
			//Facing left or right
			if(enemyFrame[(UWORD) tempub1] & 8)
			{
				//Head
				//Facing right
				if(enemyFrame[(UWORD) tempub1] & 4)
				{
					set_sprite_tile(11 + (tempub1 << 2), 60);
					set_sprite_tile(12 + (tempub1 << 2), 62);
				}
				//Facing left
				else
				{
					set_sprite_tile(11 + (tempub1 << 2), 44);
					set_sprite_tile(12 + (tempub1 << 2), 48);
				}
				//Legs
				switch(enemyFrame[(UWORD) tempub1])
				{
					//Arms aligned, facing left
					case 8:
					case 10:
						set_sprite_tile(13 + (tempub1 << 2), 46);
						set_sprite_tile(14 + (tempub1 << 2), 50);
						set_sprite_prop(13 + (tempub1 << 2), SPR_DEFLT);
						set_sprite_prop(14 + (tempub1 << 2), SPR_DEFLT);
						break;
					//Arms aligned, facing right
					case 12:
					case 14:
						set_sprite_tile(13 + (tempub1 << 2), 50);
						set_sprite_tile(14 + (tempub1 << 2), 46);
						set_sprite_prop(13 + (tempub1 << 2), SPR_FLIPX);
						set_sprite_prop(14 + (tempub1 << 2), SPR_FLIPX);
						break;
					//Frontmost arm ahead, facing left
					case 9:
						set_sprite_tile(13 + (tempub1 << 2), 52);
						set_sprite_tile(14 + (tempub1 << 2), 54);
						set_sprite_prop(13 + (tempub1 << 2), SPR_DEFLT);
						set_sprite_prop(14 + (tempub1 << 2), SPR_DEFLT);
						break;
					//Frontmost arm behind, facing left
					case 11:
						set_sprite_tile(13 + (tempub1 << 2), 56);
						set_sprite_tile(14 + (tempub1 << 2), 58);
						set_sprite_prop(13 + (tempub1 << 2), SPR_DEFLT);
						set_sprite_prop(14 + (tempub1 << 2), SPR_DEFLT);
						break;
					//Frontmost arm ahead, facing right
					case 13:
						set_sprite_tile(13 + (tempub1 << 2), 54);
						set_sprite_tile(14 + (tempub1 << 2), 52);
						set_sprite_prop(13 + (tempub1 << 2), SPR_FLIPX);
						set_sprite_prop(14 + (tempub1 << 2), SPR_FLIPX);
						break;
					//Frontmost arm behind, facing right
					case 15:
						set_sprite_tile(13 + (tempub1 << 2), 58);
						set_sprite_tile(14 + (tempub1 << 2), 56);
						set_sprite_prop(13 + (tempub1 << 2), SPR_FLIPX);
						set_sprite_prop(14 + (tempub1 << 2), SPR_FLIPX);
						break;
				}
			}
			//Facing up or down
			else
			{
				//Head
				//Facing up
				if(enemyFrame[(UWORD) tempub1] & 4)
				{
					set_sprite_tile(11 + (tempub1 << 2), 64);
					set_sprite_tile(12 + (tempub1 << 2), 68);
				}
				//Facing down
				else
				{
					set_sprite_tile(11 + (tempub1 << 2), 28);
					set_sprite_tile(12 + (tempub1 << 2), 32);
				}
				//Legs
				switch(enemyFrame[(UWORD) tempub1])
				{
					//Legs aligned, player facing down
					case 0:
					case 2:
						set_sprite_tile(13 + (tempub1 << 2), 30);
						set_sprite_tile(14 + (tempub1 << 2), 34);
						set_sprite_prop(13 + (tempub1 << 2), SPR_DEFLT);
						set_sprite_prop(14 + (tempub1 << 2), SPR_DEFLT);
						break;
					//Legs aligned, player facing up
					case 4:
					case 6:
						set_sprite_tile(13 + (tempub1 << 2), 66);
						set_sprite_tile(14 + (tempub1 << 2), 70);
						set_sprite_prop(13 + (tempub1 << 2), SPR_DEFLT);
						set_sprite_prop(14 + (tempub1 << 2), SPR_DEFLT);
						break;
					//Leftmost leg ahead, facing up
					case 5:
						set_sprite_tile(13 + (tempub1 << 2), 74);
						set_sprite_tile(14 + (tempub1 << 2), 72);
						set_sprite_prop(13 + (tempub1 << 2), SPR_FLIPX);
						set_sprite_prop(14 + (tempub1 << 2), SPR_FLIPX);
						break;
					//Rightmost leg ahead, facing up
					case 7:
						set_sprite_tile(13 + (tempub1 << 2), 72);
						set_sprite_tile(14 + (tempub1 << 2), 74);
						set_sprite_prop(13 + (tempub1 << 2), SPR_DEFLT);
						set_sprite_prop(14 + (tempub1 << 2), SPR_DEFLT);
						break;
					//Leftmost leg ahead, facing down
					case 1:
						set_sprite_tile(13 + (tempub1 << 2), 40);
						set_sprite_tile(14 + (tempub1 << 2), 42);
						set_sprite_prop(13 + (tempub1 << 2), SPR_DEFLT);
						set_sprite_prop(14 + (tempub1 << 2), SPR_DEFLT);
						break;
					//Rightmost leg ahead, facing down
					case 3:
						set_sprite_tile(13 + (tempub1 << 2), 36);
						set_sprite_tile(14 + (tempub1 << 2), 38);
						set_sprite_prop(13 + (tempub1 << 2), SPR_DEFLT);
						set_sprite_prop(14 + (tempub1 << 2), SPR_DEFLT);
						break;
				}
			}
			move_sprite(11 + (tempub1 << 2), enemyX[(UWORD) tempub1] + 8 - scrollX, enemyY[(UWORD) tempub1] + 16 - scrollY);
			move_sprite(13 + (tempub1 << 2), enemyX[(UWORD) tempub1] + 8 - scrollX, enemyY[(UWORD) tempub1] + 32 - scrollY);
			move_sprite(12 + (tempub1 << 2), enemyX[(UWORD) tempub1] + 16 - scrollX, enemyY[(UWORD) tempub1] + 16 - scrollY);
			move_sprite(14 + (tempub1 << 2), enemyX[(UWORD) tempub1] + 16 - scrollX, enemyY[(UWORD) tempub1] + 32 - scrollY);
		}
		else
		{
			move_sprite(11 + (tempub1 << 2), 0, 0);
			move_sprite(13 + (tempub1 << 2), 0, 0);
			move_sprite(12 + (tempub1 << 2), 0, 0);
			move_sprite(14 + (tempub1 << 2), 0, 0);
		}
	}
}

void SpawnEnemies()
{
	tempub3 = time;
	for(tempub1 = 0; tempub1 != numberOfEnemies; tempub1++)
	{
		if(enemyAlive[(UWORD) tempub1]) continue;
		//Again, a shitty "RNG"
		enemyX[(UWORD) tempub1] = (tempub3 ^ 23) & 31;
		enemyY[(UWORD) tempub1] = (tempub3 ^ 15) & 31;
		enemyX[(UWORD) tempub1] = enemyX[(UWORD) tempub1] << 3;
		enemyY[(UWORD) tempub1] = enemyY[(UWORD) tempub1] << 3;
		//Avoids spawning above solid tiles
		AIM_AT_TILE(enemyX[(UWORD) tempub1], enemyY[(UWORD) tempub1]);
		if(GET_TILE() >= minSolidTile) continue;
		AIM_AT_TILE(enemyX[(UWORD) tempub1], enemyY[(UWORD) tempub1] + 8);
		if(GET_TILE() >= minSolidTile) continue;
		AIM_AT_TILE(enemyX[(UWORD) tempub1], enemyY[(UWORD) tempub1] + 16);
		if(GET_TILE() >= minSolidTile) continue;
		AIM_AT_TILE(enemyX[(UWORD) tempub1], enemyY[(UWORD) tempub1] + 24);
		if(GET_TILE() >= minSolidTile) continue;
		AIM_AT_TILE(enemyX[(UWORD) tempub1] + 8, enemyY[(UWORD) tempub1]);
		if(GET_TILE() >= minSolidTile) continue;
		AIM_AT_TILE(enemyX[(UWORD) tempub1] + 8, enemyY[(UWORD) tempub1] + 8);
		if(GET_TILE() >= minSolidTile) continue;
		AIM_AT_TILE(enemyX[(UWORD) tempub1] + 8, enemyY[(UWORD) tempub1] + 16);
		if(GET_TILE() >= minSolidTile) continue;
		AIM_AT_TILE(enemyX[(UWORD) tempub1] + 8, enemyY[(UWORD) tempub1] + 24);
		if(GET_TILE() >= minSolidTile) continue;
		//Avoids the very inconvenient spawning-on-top-of-the-player situation
		Collision(enemyX[(UWORD) tempub1], enemyY[(UWORD) tempub1], 16, 32, playerX, playerY, 16, 32);
		if(tempub2) continue;
		enemyAlive[(UWORD) tempub1] = 1;
		tempub3++;
		enemyFrame[(UWORD) tempub1] = (tempub3 & 2) << 2;
	}
}