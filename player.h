UBYTE playerNextFrame;
UBYTE playerDied;

void SetPlayerTiles()
{
	//Facing left or right
	if(playerNextFrame & 8)
	{
		//Head
		//Facing right
		if(playerNextFrame & 4)
		{
			set_sprite_tile(0, 12);
			set_sprite_tile(2, 8);
			set_sprite_prop(0, SPR_FLIPX);
			set_sprite_prop(2, SPR_FLIPX);
		}
		//Facing left
		else
		{
			set_sprite_tile(0, 8);
			set_sprite_tile(2, 12);
			set_sprite_prop(0, SPR_DEFLT);
			set_sprite_prop(2, SPR_DEFLT);
		}
		//Legs
		switch(playerNextFrame)
		{
			//Arms aligned, facing left
			case 8:
			case 10:
				set_sprite_tile(1, 10);
				set_sprite_tile(3, 14);
				set_sprite_prop(1, SPR_DEFLT);
				set_sprite_prop(3, SPR_DEFLT);
				break;
			//Arms aligned, facing right
			case 12:
			case 14:
				set_sprite_tile(1, 14);
				set_sprite_tile(3, 10);
				set_sprite_prop(1, SPR_FLIPX);
				set_sprite_prop(3, SPR_FLIPX);
				break;
			//Frontmost arm ahead, facing left
			case 9:
				set_sprite_tile(1, 16);
				set_sprite_tile(3, 18);
				set_sprite_prop(1, SPR_DEFLT);
				set_sprite_prop(3, SPR_DEFLT);
				break;
			//Frontmost arm behind, facing left
			case 11:
				set_sprite_tile(1, 20);
				set_sprite_tile(3, 22);
				set_sprite_prop(1, SPR_DEFLT);
				set_sprite_prop(3, SPR_DEFLT);
				break;
			//Frontmost arm ahead, facing right
			case 13:
				set_sprite_tile(1, 18);
				set_sprite_tile(3, 16);
				set_sprite_prop(1, SPR_FLIPX);
				set_sprite_prop(3, SPR_FLIPX);
				break;
			//Frontmost arm behind, facing right
			case 15:
				set_sprite_tile(1, 22);
				set_sprite_tile(3, 20);
				set_sprite_prop(1, SPR_FLIPX);
				set_sprite_prop(3, SPR_FLIPX);
				break;
		}
	}
	//Facing up or down
	else
	{
		//Head
		//Facing up
		if(playerNextFrame & 4)
		{
			set_sprite_tile(0, 24);
			set_sprite_tile(2, 24);
			set_sprite_prop(0, SPR_DEFLT);
			set_sprite_prop(2, SPR_FLIPX);
		}
		//Facing down
		else
		{
			set_sprite_tile(0, 0);
			set_sprite_tile(2, 0);
			set_sprite_prop(0, SPR_DEFLT);
			set_sprite_prop(2, SPR_FLIPX);
		}
		//Legs
		switch(playerNextFrame)
		{
			//Legs aligned, player facing down
			case 0:
			case 2:
				set_sprite_tile(1, 2);
				set_sprite_tile(3, 2);
				set_sprite_prop(1, SPR_DEFLT);
				set_sprite_prop(3, SPR_FLIPX);
				break;
			//Legs aligned, player facing up
			case 4:
			case 6:
				set_sprite_tile(1, 26);
				set_sprite_tile(3, 26);
				set_sprite_prop(1, SPR_DEFLT);
				set_sprite_prop(3, SPR_FLIPX);
				break;
			//Leftmost leg ahead
			case 1:
			case 5:
				set_sprite_tile(1, 4);
				set_sprite_tile(3, 6);
				set_sprite_prop(1, SPR_DEFLT);
				set_sprite_prop(3, SPR_DEFLT);
				break;
			//Rightmost leg ahead
			case 3:
			case 7:
				set_sprite_tile(1, 6);
				set_sprite_tile(3, 4);
				set_sprite_prop(1, SPR_FLIPX);
				set_sprite_prop(3, SPR_FLIPX);
				break;
		}
	}
}

void SetPlayerPosition()
{
	playerY += 16;
	playerY -= scrollY;
	playerX -= scrollX;
	move_sprite(0, playerX + 8, playerY);
	move_sprite(1, playerX + 8, playerY + 16);
	move_sprite(2, playerX + 16, playerY);
	move_sprite(3, playerX + 16, playerY + 16);
	playerY -= 16;
	playerY += scrollY;
	playerX += scrollX;
	
	if(playerDied)
	{
		move_sprite(0, 0, 0);
		move_sprite(1, 0, 0);
		move_sprite(2, 0, 0);
		move_sprite(3, 0, 0);
	}
}


void UpdatePlayer()
{
	if(playerDied) return;
	
	tempub1 = joypad();
	
	playerNextFrame &= 12;
	
	//Rock update start
	if(!rockAlive)
	{
		if((tempub1 & J_A) && (!(oldJoypad & J_A)))
		{
			rockAlive = 1;
			rockX = playerX + 4;
			rockY = playerY + 12;
			rockDirection = playerNextFrame >> 2;
			rockAge = 0;
		}
	}
	else
	{
		rockAge++;
		Collision(rockX, rockY, 8, 8, vaseX, vaseY, 16, 24);
		if(tempub2)
		{
			rockAlive = 0;
			vaseAlive = 0;
			snakeX = vaseX;
			snakeY = vaseY + 8;
			snakeAlive = 1;
			snakeAge = 0;
		}
		switch(rockDirection)
		{
			case 0:
				rockY += 3;
				break;
			case 1:
				rockY -= 3;
				break;
			case 2:
				rockX -= 3;
				break;
			case 3:
				rockX += 3;
				break;
		}
		AIM_AT_TILE(rockX + 4, rockY + 4);
		if(GET_TILE() >= minSolidTile) rockAlive = 0;
		if(rockAge > 40) rockAlive = 0;
	}
	//Rock update end
	oldJoypad = tempub1;
	
	//Delicious spaghetti
	tempub2 = 1;
	if(tempub1 & J_LEFT) goto MoveLeft;
	else if(tempub1 & J_RIGHT) goto MoveRight;
	else if(tempub1 & J_UP) goto MoveUp;
	else if(tempub1 & J_DOWN) goto MoveDown;
	goto EndMovement;
	
	MoveLeft:
		playerNextFrame = 8 + ((time >> 2) & 3);
		AIM_AT_TILE(playerX - 1, playerY + 16);
		tempub1 = (GET_TILE() >= minSolidTile);
		AIM_AT_TILE(playerX - 1, playerY + 31);
		tempub1 |= (GET_TILE() >= minSolidTile) << 1;
		switch(tempub1)
		{
			case 3:
				break;
			case 1:
				if(tempub2)
				{
					tempub2 = 0;
					goto MoveDown;
				}
				break;
			case 2:
				if(tempub2)
				{
					tempub2 = 0;
					goto MoveUp;
				}
				break;
			case 0:
				playerX--;
		}
		goto EndMovement;
	
	MoveRight:
		playerNextFrame = 12 + ((time >> 2) & 3);
		AIM_AT_TILE(playerX + 16, playerY + 16);
		tempub1 = (GET_TILE() >= minSolidTile);
		AIM_AT_TILE(playerX + 16, playerY + 31);
		tempub1 |= (GET_TILE() >= minSolidTile) << 1;
		switch(tempub1)
		{
			case 3:
				break;
			case 1:
				if(tempub2)
				{
					tempub2 = 0;
					goto MoveDown;
				}
				break;
			case 2:
				if(tempub2)
				{
					tempub2 = 0;
					goto MoveUp;
				}
				break;
			case 0:
				playerX++;
		}
		goto EndMovement;
	
	MoveUp:
		playerNextFrame = 4 + ((time >> 2) & 3);
		AIM_AT_TILE(playerX, playerY + 15);
		tempub1 = (GET_TILE() >= minSolidTile);
		AIM_AT_TILE(playerX + 15, playerY + 15);
		tempub1 |= (GET_TILE() >= minSolidTile) << 1;
		switch(tempub1)
		{
			case 3:
				break;
			case 1:
				if(tempub2)
				{
					tempub2 = 0;
					goto MoveRight;
				}
				break;
			case 2:
				if(tempub2)
				{
					tempub2 = 0;
					goto MoveLeft;
				}
				break;
			case 0:
				playerY--;
		}
		goto EndMovement;
	
	MoveDown:
		playerNextFrame = 0 + ((time >> 2) & 3);
		AIM_AT_TILE(playerX, playerY + 32);
		tempub1 = (GET_TILE() >= minSolidTile);
		AIM_AT_TILE(playerX + 15, playerY + 32);
		tempub1 |= (GET_TILE() >= minSolidTile) << 1;
		switch(tempub1)
		{
			case 3:
				break;
			case 1:
				if(tempub2)
				{
					tempub2 = 0;
					goto MoveRight;
				}
				break;
			case 2:
				if(tempub2)
				{
					tempub2 = 0;
					goto MoveLeft;
				}
				break;
			case 0:
				playerY++;
		}
	
	EndMovement:
	
	//Checks for pickup (crescent, dagger) collision - 1 out of 8 pickups per frame to be faster
	tempub3 = time & 7;
	if(pickupAlive[(UWORD) tempub3])
	{
		Collision(pickupX[(UWORD) tempub3], pickupY[(UWORD) tempub3], 8, 8, playerX, playerY, 16, 32);
		if(tempub2)
		{
			//Dagger
			if(pickupType[(UWORD) tempub3] == 0)
			{
				numberOfDaggers++;
			}
			
			//Crescent
			if(pickupType[(UWORD) tempub3] == 1)
			{
				extraLives++;
				if(extraLives > 3) extraLives = 3;
			}
			
			SetWindowText();
			
			KillPickup(tempub3);
		}
	}
	
	//Checks for collisions with the enemies
	if(enemyAlive[0])
	{
		Collision(enemyX[0], enemyY[0], 15, 31, playerX, playerY, 15, 31);
		if(tempub2) playerDied = 1;
	}
	if(enemyAlive[1])
	{
		Collision(enemyX[1], enemyY[1], 15, 31, playerX, playerY, 15, 31);
		if(tempub2) playerDied = 1;
	}
	if(enemyAlive[2])
	{
		Collision(enemyX[2], enemyY[2], 15, 31, playerX, playerY, 15, 31);
		if(tempub2) playerDied = 1;
	}
	if(enemyAlive[3])
	{
		Collision(enemyX[3], enemyY[3], 15, 31, playerX, playerY, 15, 31);
		if(tempub2) playerDied = 1;
	}
	
	scrollX = playerX - 72;
	if(playerY > 56)
	{
		if(playerY < 168) scrollY = playerY - 56;
		else scrollY = 112;
	}
	else scrollY = 0;
}