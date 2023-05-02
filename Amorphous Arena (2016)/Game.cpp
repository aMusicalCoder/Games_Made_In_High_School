#include"Game.h"
#include"Map.h"
#include<iostream>
#include<SDL.h>
#include<SDL_ttf.h>
#include<string>
#include<sstream>

Game::Game()
{
	screenWidth = 640;
	screenHeight = 480;
	
	myWindow = NULL;
	mySurface = NULL;
	
	//Init TTF Stuff.
	if(TTF_Init() == -1)
	{
		std::cout << "Something went wrong with font initialization." << std::endl;
	}
	
	//Init SDL Stuff.
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		//In Case that causes Errors.
		std::cout << "Something went wrong with SDL Initialization..." << std::endl;
	}
	else
	{
		//Initialize the Window.
		myWindow = SDL_CreateWindow("Amorphous Arena!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
		if(myWindow == NULL)
		{
			//If that causes errors.
			std::cout << "SDL Could Not Create a Window." << std::endl;
		}
		else
		{
			//Initialize the renderer.
			myRenderer = SDL_CreateRenderer( myWindow, -1, SDL_RENDERER_ACCELERATED );
			if(myRenderer == NULL)
			{
				std::cout << "SDL Could Not Create a Renderer..." << std::endl;
			}
			//Don't think I need surfaces.
			//mySurface = SDL_GetWindowSurface( myWindow );
		}
	}
	
	resetState();
	startMenuState = true;
	quitGame = false;
	selectedX = -1;
	selectedY = -1;
	unitSelected = -1;
	initGraphics();
	resetUnits();
	
	map = Map();
	//End of Constructor Here.
}

Game::Game(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	
	myWindow = NULL;
	myRenderer = NULL;
	
	//Init SDL.
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		//If that causes errors.
		std::cout << "Something went wrong with SDL Initialization..." << std::endl;
	}
	else
	{
		//Initialize the window.
		myWindow = SDL_CreateWindow("Amorphous Arena!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
		if( myWindow == NULL )
		{
			//if that causes errors.
			std::cout << "SDL could not create a window..." << std::endl;
		}
		else
		{
			//Init the renderer.
			myRenderer = SDL_CreateRenderer( myWindow, -1, SDL_RENDERER_ACCELERATED );
			if(myRenderer == NULL)
			{
				//if that causes errors.
				std::cout << "SDL Could not create a renderer..." << std::endl;
			}
		}
	}
	
	resetState();
	startMenuState = true;
	quitGame = false;
	selectedX = -1;
	selectedY = -1;
	unitSelected = -1;
	initGraphics();
	
	//End of Constructor Here.
}

void Game::resetUnits()
{
	//Declare All Units to Default Values.
	for(int a = 0; a < 24; a++)
	{
		for(int b = 0; b < 12; b++)
		{
			myUnits[a][b].exists = false;
			myUnits[a][b].flying = false;
			
			enemyUnits[a][b].exists = false;
			enemyUnits[a][b].flying = false;
		}
	}
}

void Game::resetState()
{
	//resets States.
	startMenuState = false;
	gameMenuState = false;
	gameState = false;
	aboutState = false;
	winState = false;
	loseState = false;
}

bool Game::run()
{
	//Starting Point of Game.
	if(quitGame == false)
	{
		update();
		render();
		getInput();
	}
	else if(quitGame == true)
	{
		return false;
	}
	return true;
	//Practice Code Here.
	/*SDL_Rect bruh; 
	bruh.x = 100;
	bruh.y = 100;
	bruh.w = 100;
	bruh.h = 100;
	
	SDL_SetRenderDrawColor( myRenderer, 0, 0, 0, 255);
	//SDL_FillRect(mySurface, NULL, SDL_MapRGB(mySurface->format, 0x00, 0x00, 0x00));
	SDL_RenderClear( myRenderer );
	
	SDL_SetRenderDrawColor( myRenderer, 255, 0, 0, 255);
	SDL_RenderFillRect( myRenderer, &bruh );
	
	SDL_RenderPresent( myRenderer );
	//SDL_UpdateWindowSurface ( myWindow );
	SDL_Delay(2000);
	*/
	
}

void Game::removeDead()
{
	for(int a = 0; a < 24; a++)
	{
		for(int b = 0; b < 12; b++)
		{
			 if(myUnits[a][b].exists == true)
			 {
				 if(myUnits[a][b].health <= 0)
				 {
					 myUnits[a][b].exists = false;
				 }
			 }
			 
			 if(enemyUnits[a][b].exists == true)
			 {
				 if(enemyUnits[a][b].health <= 0)
				 {
					 enemyUnits[a][b].exists = false;
				 }
			 }
		}
	}
}

void Game::update()
{
	//Game Logic Here.
	if(gameState == true)
	{
		std::cout << "Your Tower Health: " << towerHealth << " Enemy Tower Health: " << enemyTowerHealth <<std::endl;
		removeDead();
		//Lose or Win.
		if(towerHealth <= 0)
		{
			//You Lose.
			resetState();
			loseState = true;
		}
		
		if(enemyTowerHealth <= 0)
		{
			//You Win.
			resetState();
			winState = true;
		}
		
		//Attack Stuff Here.
		if((SDL_GetTicks() - attackTimer) > attackDelay)
		{
			//Reset Attack.
			for(int a = 0; a < 24; a++)
			{
				for(int b = 0; b < 24; b++)
				{
					enemyUnits[a][b].justAttacked = false;
					myUnits[a][b].justAttacked = false;
				}
			}
			
			attack();
			attackTimer = SDL_GetTicks();
		}
		
		//AI Turn.
		if((SDL_GetTicks() - aiTimer) > aiDelay)
		{
			//Ai moves.
			switch(aiMoveCounter)
			{
				case 0:
					//First Move. Spawn Basics and drop down the right lane.
					enemyUnits[3][9].exists = true;
					enemyUnits[3][9].type = 1;
					enemyUnits[3][9].health = 2;
					enemyUnits[3][9].damage = 2;
					enemyUnits[3][9].flying = false;
					enemyUnits[3][9].xGoal = 0;
					enemyUnits[3][9].yGoal = 18;
					enemyUnits[3][9].justMoved = false;
					enemyUnits[3][9].justAttacked = false;
					
					enemyUnits[3][10].exists = true;
					enemyUnits[3][10].type = 1;
					enemyUnits[3][10].health = 2;
					enemyUnits[3][10].damage = 2;
					enemyUnits[3][10].flying = false;
					enemyUnits[3][10].xGoal = 0;
					enemyUnits[3][10].yGoal = 18;
					enemyUnits[3][10].justMoved = false;
					enemyUnits[3][10].justAttacked = false;
					break;
				case 1:
					//Second Move. Push the basics onto the tower. Drop an advanced. 
					//Push the advanced to the front of your tower.
					enemyUnits[21][10].xGoal = -2;
					enemyUnits[21][10].yGoal = 1;
					enemyUnits[21][9].xGoal = -1;
					enemyUnits[21][9].yGoal = 0;
					
					enemyUnits[3][5].exists = true;
					enemyUnits[3][5].type = 3;
					enemyUnits[3][5].health = 10;
					enemyUnits[3][5].damage = 5;
					enemyUnits[3][5].flying = false;
					enemyUnits[3][5].xGoal = 0;
					enemyUnits[3][5].yGoal = 17;
					enemyUnits[3][5].justMoved = false;
					enemyUnits[3][5].justAttacked = false;
					break;
				case 2:
					//Third Move. Drop a Flying and push it down the left lane.
					enemyUnits[3][2].exists = true;
					enemyUnits[3][2].type = 2;
					enemyUnits[3][2].health = 2;
					enemyUnits[3][2].damage = 2;
					enemyUnits[3][2].flying = true;
					enemyUnits[3][2].xGoal = 0;
					enemyUnits[3][2].yGoal = 18;
					enemyUnits[3][2].justMoved = false;
					enemyUnits[3][2].justAttacked = false;
					
					enemyUnits[3][1].exists = true;
					enemyUnits[3][1].type = 2;
					enemyUnits[3][1].health = 2;
					enemyUnits[3][1].damage = 2;
					enemyUnits[3][1].flying = true;
					enemyUnits[3][1].xGoal = 0;
					enemyUnits[3][1].yGoal = 18;
					enemyUnits[3][1].justMoved = false;
					enemyUnits[3][1].justAttacked = false;
					break;
				case 3:
					//Fourth Move. Push the flying onto my tower.
					enemyUnits[21][1].xGoal = 2;
					enemyUnits[21][1].yGoal = 1;
					
					enemyUnits[21][2].xGoal = 2;
					enemyUnits[21][2].yGoal = 0;
					break;
			}
			if(aiMoveCounter >= 3)
			{
				aiMoveCounter = 0;
			}
			else
			{
				aiMoveCounter++;
			}
			
			aiTimer = SDL_GetTicks();
		}
		
		//Movement Stuff Here.
		if((SDL_GetTicks() - movementTimer) > movementDelay)
		{
			//Reset Movement.
			for(int a=0;a < 24; a++)
			{
				for(int b = 0; b < 12; b++)
				{
					myUnits[a][b].justMoved = false;
					enemyUnits[a][b].justMoved = false;
				}
				
			}
			
			move();
			movementTimer = SDL_GetTicks();
		}
		
		//Timer and blob counting stuff here.
		blobGenerationDelay = 3000 - (250 * workerCount);//Can go down to 1.5 seconds.
		
		if((SDL_GetTicks() - (int)blobTimer) > blobGenerationDelay)
		{
			//Iterate Blob Count.
			if(blobCounter < 20)
			{
				blobCounter++;
			}
			blobTimer = (double)SDL_GetTicks();
		}
	}
}

void Game::move()
{
	//Move Your Units.
	for(int a = 0; a < 24; a++)
	{
		for(int b = 0; b < 12; b++)
		{
			if(myUnits[a][b].exists == true)
			{
				//Your Unit is on the map.
				if(myUnits[a][b].xGoal != 0)
				{
					//Your Unit wants to move.
					
					//Start with x movement. The current units are dumb btw.
					//Does it want to move in the x direction?
					int xMove = myUnits[a][b].xGoal;
					if(xMove != 0)//redundancy lol
					{
						//your unit wants to move in the x direction.
						if(xMove > 0)
						{
							//your unit wants to move right.
							if(enemyUnits[a][b + 1].exists == true)
							{
								
							}
							else
							{
								if(map.getMapValue(b + 1, a) == 0 && myUnits[a][b + 1].exists == false)
								{
									//no obstacles.
									if(myUnits[a][b].justMoved == true)
									{
										
									}
									else
									{									
										myUnits[a][b].xGoal -= 1;
										myUnits[a][b + 1] = myUnits[a][b];
										myUnits[a][b].exists = false;	
										myUnits[a][b + 1].justMoved = true;
									}
								}
							}
						}
						else if( xMove < 0)
						{
							//your unit wants to move left.
							
							//check if it can.
							if(enemyUnits[a][b - 1].exists == true)
							{
								
							}
							else
							{
								if(map.getMapValue(b - 1, a) == 0 && myUnits[a][b - 1].exists == false)
								{
									//no obstacles.
									if(myUnits[a][b].justMoved == true)
									{
										
									}
									else
									{
										myUnits[a][b].xGoal += 1;
										myUnits[a][b - 1] = myUnits[a][b];
										myUnits[a][b].exists = false;
										myUnits[a][b -1].justMoved = true;
									}
									
								}
							}
						}
					}
					
				}
				
				if(myUnits[a][b].yGoal != 0)
				{
					int yMove = myUnits[a][b].yGoal;
					
					if(yMove > 0)
					{
						//your unit wants to move down.
						if(enemyUnits[a + 1][b].exists == true)
						{
							
						}
						else
						{
							if(map.getMapValue(b, a + 1) == 0 && myUnits[a + 1][b].exists == false)
							{
								//no obstacles.
								if(myUnits[a][b].justMoved == true)
								{
									
								}
								else
								{
									myUnits[a][b].yGoal -= 1;
									myUnits[a + 1][b] = myUnits[a][b];
									myUnits[a][b].exists = false;
									myUnits[a + 1][b].justMoved = true;
								}
								
							}
						}
					}
					else if(yMove < 0)
					{
						//your unit wants to move up.
						if(enemyUnits[a - 1][b].exists == true)
						{
							
						}
						else
						{
							if(map.getMapValue(b, a - 1) == 0 && myUnits[a - 1][b].exists == false)
							{
								//no obstacles.
								if(myUnits[a][b].justMoved == true)
								{
									
								}
								else
								{
									myUnits[a][b].yGoal += 1;
									myUnits[a - 1][b] = myUnits[a][b];
									myUnits[a][b].exists = false;
									myUnits[a - 1][b].justMoved = true;
								}
								
							}
						}
					}
				}
				
			}
		}
	}
	
	//Move Enemy Units.
	for(int a = 0; a < 24; a++)
	{
		for(int b = 0; b < 12; b++)
		{
			if(enemyUnits[a][b].exists == true)
			{
				int xMove = enemyUnits[a][b].xGoal;
				int yMove = enemyUnits[a][b].yGoal;
				
				if(xMove != 0)
				{
					//Enemy Unit wants to move on the x axis.
					if(enemyUnits[a][b].justMoved!= true)
					{
						//allow it
						if(xMove > 0)
						{
							//Your unit wants to move right.
							if(myUnits[a][b + 1].exists == false)
							{
								if(map.getMapValue(b + 1, a) == 0 && enemyUnits[a][b + 1].exists == false)
								{
									//no obstacles.
									if(enemyUnits[a][b].justMoved == true)
									{
										
									}
									else
									{									
										enemyUnits[a][b].xGoal -= 1;
										enemyUnits[a][b + 1] = enemyUnits[a][b];
										enemyUnits[a][b].exists = false;	
										enemyUnits[a][b + 1].justMoved = true;
									}
								}
							}
						}
						
						
						if(xMove < 0)
						{
							//your unit wants to move left.
							if(myUnits[a][b - 1].exists == false)
							{
								if(map.getMapValue(b - 1, a) == 0 && enemyUnits[a][b - 1].exists == false)
								{
									//no obstacles.
									if(enemyUnits[a][b].justMoved == true)
									{
										
									}
									else
									{
										enemyUnits[a][b].xGoal += 1;
										enemyUnits[a][b - 1] = enemyUnits[a][b];
										enemyUnits[a][b].exists = false;
										enemyUnits[a][b - 1].justMoved = true;
									}
								}
							}
						}
					}
				}
				
				if(yMove > 0)
				{
					//Enemy Unit wants to move on the y axis.
					if(enemyUnits[a][b].justMoved != true)
					{
						//allow it.
						
						//Check Boundaries. They only moves down. so check that.
						//check walls, then check units.
						if(map.getMapValue(b, a + 1) == 0 && enemyUnits[a + 1][b].exists == false)
						{
							//allow movement. possibly.
							if(myUnits[a + 1][b].exists == true)
							{
								//bag of nope. no movement.
							}
							else
							{
								//yay, you can move down.
								enemyUnits[a][b].yGoal -= 1;
								enemyUnits[a + 1][b] = enemyUnits[a][b];
								enemyUnits[a][b].exists = false;
								enemyUnits[a + 1][b].justMoved = true;
								
							}
						}
						
					}
				}
			}
		}
	}
	//End of move.
}

void Game::attack()
{
	//Iterate.
	for(int a = 0; a < 24; a++)
	{
		for(int b = 0; b < 12; b++)
		{
			//Check up direction.
			if(myUnits[a][b].exists == true)
			{
				if(myUnits[a][b].justAttacked == false)
				{
					//You can attack.
					
					//Tower in front.
					if(map.getMapValue(b, a - 1) == 2 && myUnits[a][b].justAttacked == false)
					{
						//There is a tower in front of this unit.
						//Check if it's the enemy tower.
						
						if(a < 10)//arbritrary value that works.
						{
							///it is the enemy tower.
							//Attack it.
							enemyTowerHealth = enemyTowerHealth - myUnits[a][b].damage;
							myUnits[a][b].justAttacked = true;
						}
						
					}
					
					//Enemy in front.
					if(enemyUnits[a - 1][b].exists == true && myUnits[a][b].justAttacked == false)
					{
						if(enemyUnits[a - 1][b].flying == true)
						{
							
						}
						else
						{
						enemyUnits[a - 1][b].health -= myUnits[a][b].damage;
						myUnits[a][b].justAttacked = true;
						}
					}
					//Tower to the right.
					if(map.getMapValue(b + 1, a) == 2 && myUnits[a][b].justAttacked == false)
					{
						//There is a tower in front of this unit.
						//Check if it's the enemy tower.
						
						if(a < 10)//arbritrary value that works.
						{
							///it is the enemy tower.
							//Attack it.
							enemyTowerHealth = enemyTowerHealth - myUnits[a][b].damage;
							myUnits[a][b].justAttacked = true;
						}
						
					}
					
					//Enemy to the right.
					if(enemyUnits[a][b + 1].exists == true && myUnits[a][b].justAttacked == false)
					{
						if(enemyUnits[a][b + 1].flying == true)
						{
							
						}
						else
						{
						enemyUnits[a][b + 1].health -= myUnits[a][b].damage;
						myUnits[a][b].justAttacked = true;
						}
						
					}
					
					//Tower Under unit.
					if(map.getMapValue(b, a + 1) == 2 && myUnits[a][b].justAttacked == false)
					{
						//There is a tower in front of this unit.
						//Check if it's the enemy tower.
						
						if(a < 10)//arbritrary value that works.
						{
							///it is the enemy tower.
							//Attack it.
							enemyTowerHealth = enemyTowerHealth - myUnits[a][b].damage;
							myUnits[a][b].justAttacked = true;
						}
						
					}
					
					//Enemy Under unit.
					if(enemyUnits[a + 1][b].exists == true && myUnits[a][b].justAttacked == false)
					{
						if(enemyUnits[a + 1][b].flying == true)
						{
							
						}
						else
						{
						enemyUnits[a + 1][b].health -= myUnits[a][b].damage;
						myUnits[a][b].justAttacked = true;
						}
					}
					
					//Tower to left of unit.
					if(map.getMapValue(b - 1, a) == 2 && myUnits[a][b].justAttacked == false)
					{
						//There is a tower in front of this unit.
						//Check if it's the enemy tower.
						
						if(a < 10)//arbritrary value that works.
						{
							///it is the enemy tower.
							//Attack it.
							enemyTowerHealth = enemyTowerHealth - myUnits[a][b].damage;
							myUnits[a][b].justAttacked = true;
						}
						
					}
					//Enemy to left of unit.
					if(enemyUnits[a][b - 1].exists == true && myUnits[a][b].justAttacked == false)
					{
						//deal damage to them.
						if(enemyUnits[a][b - 1].flying == true)
						{
							
						}
						else
						{
						enemyUnits[a][b - 1].health -= myUnits[a][b].damage;
						myUnits[a][b].justAttacked = true;
						}
					}
				}
			}
			
			//Enemy Turn to attack.
			//Check up direction.
			if(enemyUnits[a][b].exists == true)
			{
				if(enemyUnits[a][b].justAttacked == false)
				{
					//You can attack.
					
					//Tower in front.
					if(map.getMapValue(b, a - 1) == 2 && enemyUnits[a][b].justAttacked == false)
					{
						//There is a tower in front of this unit.
						//Check if it's the enemy tower.
						
						if(a > 10)//arbritrary value that works.
						{
							///it is the enemy tower.
							//Attack it.
							towerHealth = towerHealth - enemyUnits[a][b].damage;
							enemyUnits[a][b].justAttacked = true;
						}
						
					}
					
					//Enemy in front.
					if(myUnits[a - 1][b].exists == true && enemyUnits[a][b].justAttacked == false)
					{
						if(myUnits[a - 1][b].flying == true)
						{
							
						}
						else
						{
						myUnits[a - 1][b].health -= enemyUnits[a][b].damage;
						enemyUnits[a][b].justAttacked = true;
						}
						
					}
					//Tower to the right.
					if(map.getMapValue(b + 1, a) == 2 && enemyUnits[a][b].justAttacked == false)
					{
						//There is a tower in front of this unit.
						//Check if it's the enemy tower.
						
						if(a > 10)//arbritrary value that works.
						{
							///it is the enemy tower.
							//Attack it.
							towerHealth = towerHealth - enemyUnits[a][b].damage;
							enemyUnits[a][b].justAttacked = true;
						}
						
					}
					
					//Enemy to the right.
					if(myUnits[a][b + 1].exists == true && enemyUnits[a][b].justAttacked == false)
					{
						if(myUnits[a][b + 1].flying == true)
						{
							
						}
						else
						{
						myUnits[a][b + 1].health -= enemyUnits[a][b].damage;
						enemyUnits[a][b].justAttacked = true;
						}
						
					}
					
					//Tower Under unit.
					if(map.getMapValue(b, a + 1) == 2 && enemyUnits[a][b].justAttacked == false)
					{
						//There is a tower in front of this unit.
						//Check if it's the enemy tower.
						
						if(a > 10)//arbritrary value that works.
						{
							///it is the enemy tower.
							//Attack it.
							towerHealth = towerHealth - enemyUnits[a][b].damage;
							enemyUnits[a][b].justAttacked = true;
						}
						
					}
					
					//Enemy Under unit.
					if(myUnits[a + 1][b].exists == true && enemyUnits[a][b].justAttacked == false)
					{
						if(myUnits[a + 1][b].flying == true)
						{
							
						}
						else
						{
						myUnits[a + 1][b].health -= enemyUnits[a][b].damage;
						enemyUnits[a][b].justAttacked = true;
						}
						
					}
					
					//Tower to left of unit.
					if(map.getMapValue(b - 1, a) == 2 && enemyUnits[a][b].justAttacked == false)
					{
						//There is a tower in front of this unit.
						//Check if it's the enemy tower.
						
						if(a > 10)//arbritrary value that works.
						{
							///it is the enemy tower.
							//Attack it.
							towerHealth = towerHealth - enemyUnits[a][b].damage;
							enemyUnits[a][b].justAttacked = true;
						}
						
					}
					//Enemy to left of unit.
					if(myUnits[a][b - 1].exists == true && enemyUnits[a][b].justAttacked == false)
					{
						//deal damage to them.
						if(myUnits[a][b - 1].flying == true)
						{
							
						}
						else
						{
						myUnits[a][b - 1].health -= enemyUnits[a][b].damage;
						enemyUnits[a][b].justAttacked = true;
						}
						
					}
				}
			}
		}
	}
}

void Game::initGraphics()
{
	//Load Textures.
	SDL_Surface* surf = NULL;
	
	//Load Fonts.
	libraSans = TTF_OpenFont( "fonts\\LibraSans.ttf", 12 );
	
	//You Win Texture.
	surf = SDL_LoadBMP("assets\\youWin.bmp");
	youWin = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//You Lose Texture.
	surf = SDL_LoadBMP("assets\\youLose.bmp");
	youLose = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Title Texture.
	surf = SDL_LoadBMP("assets\\amorArena.bmp");
	amorArena = SDL_CreateTextureFromSurface( myRenderer, surf);
	SDL_FreeSurface(surf);
	
	//Play Game Button Texture.
	surf = SDL_LoadBMP("assets\\playGame.bmp");
	playGame = SDL_CreateTextureFromSurface( myRenderer, surf);
	SDL_FreeSurface( surf );
	
	//About Button Texture.
	surf = SDL_LoadBMP("assets\\about.bmp");
	about = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Quit Button Texture.
	surf = SDL_LoadBMP("assets\\Quit.bmp");
	quit = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Return Button Texture.
	surf = SDL_LoadBMP("assets\\return.bmp");
	returnButton = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//About Screen Texture.
	surf = SDL_LoadBMP("assets\\aboutScreen.bmp");
	aboutText = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Game Menu Text Texture.
	surf = SDL_LoadBMP("assets\\gameMenu.bmp");
	gameMenuText = SDL_CreateTextureFromSurface( myRenderer, surf);
	SDL_FreeSurface( surf );
	
	//Wall Tile Texture.
	surf = SDL_LoadBMP("assets\\wallTile.bmp");
	wallTile = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Floor Tile Texture.
	surf = SDL_LoadBMP("assets\\floorTile.bmp");
	floorTile = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Red Tile Texture.
	surf = SDL_LoadBMP("assets\\redTile.bmp");
	redTile = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Up Arrow Texture.
	surf = SDL_LoadBMP("assets\\upArrow.bmp");
	upArrow = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Down Arrow Texture.
	surf = SDL_LoadBMP("assets\\downArrow.bmp");
	downArrow = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Selected Tile Texture.
	surf = SDL_LoadBMP("assets\\floorTileSelect.bmp");
	floorTileSelect = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Tower Texture.
	surf = SDL_LoadBMP("assets\\tower.bmp");
	towerTexture = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Enemy Tower Texture.
	surf = SDL_LoadBMP("assets\\enemyTower.bmp");
	enemyTowerTexture = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Worker Button Texture.
	surf = SDL_LoadBMP("assets\\workerButton.bmp");
	workerButton = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Basic Unit Button Texture.
	surf = SDL_LoadBMP("assets\\basicUnit.bmp");
	basicUnitButton = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Flying Unit Button Texture.
	surf = SDL_LoadBMP("assets\\flyingUnit.bmp");
	flyingUnitButton = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Advanced Unit Button Texture.
	surf = SDL_LoadBMP("assets\\advancedUnit.bmp");
	advancedUnitButton = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//UIBuffer texture.
	surf = SDL_LoadBMP("assets\\uiBuffer.bmp");
	UIBuffer = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Selected Worker Unit Button Texture.
	surf = SDL_LoadBMP("assets\\selectedWorkerUnitButton.bmp");
	selectedWorkerUnitButton = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Selected Basic Unit Button Texture.
	surf = SDL_LoadBMP("assets\\selectedBasicUnitButton.bmp");
	selectedBasicUnitButton = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Selected Flying Unit Button Texture.
	surf = SDL_LoadBMP("assets\\selectedFlyingUnitButton.bmp");
	selectedFlyingUnitButton = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Selected Advanced Unit Button Texture.
	surf = SDL_LoadBMP("assets\\selectedAdvancedUnitButton.bmp");
	selectedAdvancedUnitButton = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Worker Unit Up Texture.
	surf = SDL_LoadBMP("assets\\workerUnit.bmp");
	workerUnitUp = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	//Basic Unit Up Texture.
	surf = SDL_LoadBMP("assets\\basicUnitUp.bmp");
	basicUnitUp = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );	
	
	//Flying Unit Up Texture.
	surf = SDL_LoadBMP("assets\\flyingUnitUp.bmp");
	flyingUnitUp = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );	
	
	//Advanced Unit Up Texture.
	surf = SDL_LoadBMP("assets\\advancedUnitUp.bmp");
	advancedUnitUp = SDL_CreateTextureFromSurface( myRenderer, surf );
	blobCounterText = SDL_CreateTextureFromSurface( myRenderer, surf );//initialized to something random.
	blobTimerText =  SDL_CreateTextureFromSurface( myRenderer, surf );//initialized to something random.
	SDL_FreeSurface( surf );	
	
	//select worker up
	surf = SDL_LoadBMP("assets\\workerSelectUp.bmp");
	workerSelectUp = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//select basic up
	surf = SDL_LoadBMP("assets\\basicSelectUp.bmp");
	basicSelectUp = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//select flying up
	surf = SDL_LoadBMP("assets\\flyingSelectUp.bmp");
	flyingSelectUp = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//select advanced up.
	surf = SDL_LoadBMP("assets\\advancedSelectUp.bmp");
	advancedSelectUp = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//unselect button.
	surf = SDL_LoadBMP("assets\\unselect.bmp");
	unselectButton = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Enemy Basic Unit.
	surf = SDL_LoadBMP("assets\\enemyBasicUnitDown.bmp");
	enemyBasicUnit = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Enemy Flying Unit.
	surf = SDL_LoadBMP("assets\\enemyFlyingUnitDown.bmp");
	enemyFlyingUnit = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	//Enemy Advanced Unit.
	surf = SDL_LoadBMP("assets\\enemyAdvancedUnitDown.bmp");
	enemyAdvancedUnit = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
}

void Game::render()
{
	//Do Graphics Stuff Here.
	
	//Depends on the state...
	if(startMenuState == true)
	{
		//Draw the Start Menu Here.
		drawMenu();
	}
	else if(aboutState == true)
	{
		//Draw the about menu.
		drawAbout();
	}
	else if(gameMenuState == true)
	{
		//Draw the game menu.
		drawGameMenu();
	}
	else if(gameState == true)
	{
		//Draw the game.
		drawGame();
	}
	else if(winState == true)
	{
		//draw win screen.
		SDL_RenderClear( myRenderer);
		SDL_RenderCopy( myRenderer, youWin, NULL, NULL);
		SDL_RenderPresent( myRenderer );
	}
	else if(loseState == true)
	{
		//draw lose screen.
		SDL_RenderClear( myRenderer);
		SDL_RenderCopy( myRenderer, youLose, NULL, NULL);
		SDL_RenderPresent( myRenderer );
	}
}

void Game::drawMenu()
{
	SDL_SetRenderDrawColor( myRenderer, 0, 0, 0, 255);
	SDL_RenderClear( myRenderer );
	
	SDL_Rect rect;
	
	//Amorphous Arena Title.
	rect.x=120;rect.y=50;rect.w=400;rect.h=100;
	SDL_RenderCopy( myRenderer, amorArena, NULL, &rect );
	
	//Play Game Button.
	rect.x=270;rect.y=200;rect.w=100;rect.h=50;
	SDL_RenderCopy( myRenderer, playGame, NULL, &rect );
	
	//About Button.
	rect.x=270;rect.y=275;rect.w=100;rect.h=50;
	SDL_RenderCopy( myRenderer, about, NULL, &rect);
	
	//Quit Button.
	rect.x=270;rect.y=350;rect.w=100;rect.h=50;
	SDL_RenderCopy( myRenderer, quit, NULL, &rect );
	
	SDL_RenderPresent( myRenderer );
}

void Game::drawAbout()
{
	SDL_SetRenderDrawColor( myRenderer, 0, 0, 0, 255);
	SDL_RenderClear( myRenderer );
	
	SDL_Rect rect;
	rect.x=50;rect.y=25;rect.w=500;rect.h=400;
	SDL_RenderCopy( myRenderer, aboutText, NULL, &rect );
	
	rect.x=270;rect.y=400;rect.w=100;rect.h=50;
	SDL_RenderCopy( myRenderer, returnButton, NULL, &rect);
	
	SDL_RenderPresent( myRenderer );
}

void Game::drawGameMenu()
{
	SDL_SetRenderDrawColor( myRenderer, 0, 0, 0, 255);
	SDL_RenderClear( myRenderer );
	
	SDL_Rect rect;
	rect.x=50;rect.y=25;rect.w=500;rect.h=400;
	SDL_RenderCopy( myRenderer, gameMenuText, NULL, &rect );
	
	rect.x=195;rect.y=400;rect.w=100;rect.h=50;
	SDL_RenderCopy( myRenderer, returnButton, NULL, &rect );
	
	rect.x=345;rect.y=400;rect.w=100;rect.h=50;
	SDL_RenderCopy( myRenderer, playGame, NULL, &rect );
	
	SDL_RenderPresent( myRenderer );
}

void Game::drawGame()
{
	SDL_SetRenderDrawColor( myRenderer, 0, 0, 0, 255);
	SDL_RenderClear( myRenderer );
	
	drawMap();
	SDL_Rect rect;
	
	//Draw the Blob Transformation UI Bar, and the Blob Counter.
	//4 Units. Worker, Basic Attacker, Advanced Attacker, Flyer.
	//Draw the UI Buffer.
	rect.x=0;rect.y=374;rect.w=53;rect.h=53;
	SDL_RenderCopy( myRenderer, UIBuffer, NULL, &rect);
	rect.x=265;rect.w=106;
	SDL_RenderCopy( myRenderer, unselectButton, NULL, &rect);

	rect.x=371;rect.y=374;rect.w=53;rect.h=53;
	SDL_RenderCopy( myRenderer, UIBuffer, NULL, &rect);
	
	//Is the worker Selected?
	if(unitSelected == 0)
	{
		rect.x=53;
		SDL_RenderCopy( myRenderer, selectedWorkerUnitButton, NULL, &rect );
	}
	else
	{
		rect.x=53;
		SDL_RenderCopy( myRenderer, workerButton, NULL, &rect );
	}
	
	if(unitSelected == 1)
	{
		rect.x=106;
		SDL_RenderCopy( myRenderer, selectedBasicUnitButton, NULL, &rect );
	}
	else
	{
		rect.x=106;
		SDL_RenderCopy( myRenderer, basicUnitButton, NULL, &rect );
	}
	
	if(unitSelected == 2)
	{
		rect.x=159;
		SDL_RenderCopy( myRenderer, selectedFlyingUnitButton, NULL, &rect );
	}
	else
	{
		rect.x=159;
		SDL_RenderCopy( myRenderer, flyingUnitButton, NULL, &rect );
	}
	
	if(unitSelected == 3)
	{
		rect.x=212;
		SDL_RenderCopy( myRenderer, selectedAdvancedUnitButton, NULL, &rect );
	}
	else
	{
		rect.x=212;
		SDL_RenderCopy( myRenderer, advancedUnitButton, NULL, &rect );
	}
	
	//Draw the Blob Counter and Timer.
	SDL_Color color = {255, 255, 255};//White.
	SDL_Surface* surf = NULL;
	SDL_DestroyTexture( blobCounterText );
	SDL_DestroyTexture( blobTimerText );
	blobCounterText = NULL;
	blobTimerText = NULL;
	std::string counter;
	std::stringstream ss;
	std::stringstream s2;
	ss << blobCounter;
	s2 << ((double)(SDL_GetTicks() - (int)blobTimer) / 1000);
	counter = "Blobs: " + ss.str();
	
	surf = TTF_RenderUTF8_Solid(libraSans, counter.c_str() , color);
	blobCounterText = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	
	rect.x = 424;rect.y = 371;rect.w=159;rect.h=26;
	SDL_RenderCopy( myRenderer, blobCounterText, NULL, &rect );
	
	counter = "Timer: " + s2.str();
	rect.x = 424;rect.y=396;rect.w=159;rect.h=26;
	surf = NULL;
	surf = TTF_RenderUTF8_Solid(libraSans, counter.c_str(), color );
	blobTimerText = SDL_CreateTextureFromSurface( myRenderer, surf );
	SDL_FreeSurface( surf );
	SDL_RenderCopy( myRenderer, blobTimerText, NULL, &rect );
	
	//Draw the Arrows.
	rect.x=0;rect.y=0;rect.w=640;rect.h=53;
	SDL_RenderCopy( myRenderer, upArrow, NULL, &rect );
	rect.x=0;rect.y=427;rect.w=640;rect.h=53;
	SDL_RenderCopy( myRenderer, downArrow, NULL, &rect );
	
	SDL_RenderPresent( myRenderer );
}

void Game::drawMap()
{	
	int currentMapPosition = viewPosition;
	
	for(int a = currentMapPosition; a < currentMapPosition + 6; a++)
	{
		for(int b = 0; b < 12; b++)
		{
			int tileValue = map.getMapValue(b, a);
			int x = b;
			int y = a - currentMapPosition;
			//Create Position on screen.
			SDL_Rect rect;
			rect.x = x * 53;
			rect.y = (y * 53) + 53;
			rect.w=53;rect.h=53;
			
			//Decide with tile to use.
			switch(tileValue)
			{
				case 0:
					SDL_RenderCopy( myRenderer, floorTile, NULL, &rect);
					break;
				case 1:
					SDL_RenderCopy( myRenderer, wallTile, NULL, &rect);
					break;
				case 2:
					SDL_RenderCopy( myRenderer, redTile, NULL, &rect);
					break;
				default:
					std::cout << "Unknown Tile Value..." << std::endl;
					break;
			}
			
			//Draw Units.
			if(tileValue == 0)
			{
				//Draw Units if the tile works.
				if(myUnits[a][b].exists == true)
				{
					//Draw the unit.
					if(myUnits[a][b].type == 0)
					{
						//Draw Worker Unit Here.
						SDL_RenderCopy( myRenderer, workerUnitUp, NULL, &rect );
					}
					
					if(myUnits[a][b].type == 1)
					{
						//Draw Basic Unit Here.
						SDL_RenderCopy( myRenderer, basicUnitUp, NULL, &rect );
					}
					
					if(myUnits[a][b].type == 2)
					{
						//Draw Flying Unit Here.
						SDL_RenderCopy( myRenderer, flyingUnitUp, NULL, &rect );
					}
					
					if(myUnits[a][b].type == 3)
					{
						//Draw Advanced Unit Here.
						SDL_RenderCopy( myRenderer, advancedUnitUp, NULL, &rect );
					}
				}
				
				if(enemyUnits[a][b].exists == true)
				{
					if(enemyUnits[a][b].type == 1)
					{
						SDL_RenderCopy( myRenderer, enemyBasicUnit, NULL, &rect );
					}
					
					if(enemyUnits[a][b].type == 2)
					{
						SDL_RenderCopy( myRenderer, enemyFlyingUnit, NULL, &rect );
					}
					
					if(enemyUnits[a][b].type == 3)
					{
						SDL_RenderCopy( myRenderer, enemyAdvancedUnit, NULL, &rect);
					}
				}
			}
			
		}
	}
	
	//Draw the Enemy Tower.
	if(viewPosition < 3)
	{
		if(viewPosition == 0)
		{
			SDL_Rect rect;
			rect.x = 212;rect.y = 106;
			rect.w = 212; rect.h = 106;
			
			SDL_RenderCopy( myRenderer, enemyTowerTexture, NULL, &rect );
		}
		
		if(viewPosition == 1)
		{
			SDL_Rect rect;
			rect.x = 212;rect.y = 53;
			rect.w = 212; rect.h = 106;
			
			SDL_RenderCopy( myRenderer, enemyTowerTexture, NULL, &rect );
		}
		
		if(viewPosition == 2)
		{
			SDL_Rect rect;
			rect.x = 212;rect.y = 0;
			rect.w = 212; rect.h = 106;
			
			SDL_RenderCopy( myRenderer, enemyTowerTexture, NULL, &rect );
		}
	}
	
	//Draw the Tower.
	if(viewPosition > 15)
	{
		if(viewPosition == 16)
		{
			SDL_Rect rect;
			rect.x = 212;rect.y = 318;
			rect.w = 212;rect.h = 106;
			
			SDL_RenderCopy( myRenderer, towerTexture, NULL, &rect );
		}
		
		if(viewPosition == 17)
		{
			SDL_Rect rect;
			rect.x = 212;rect.y = 265;
			rect.w = 212; rect.h = 106;
			
			SDL_RenderCopy( myRenderer, towerTexture, NULL, &rect );
		}
		
		if(viewPosition == 18)
		{
			SDL_Rect rect;
			rect.x = 212;rect.y = 212;
			rect.w = 212; rect.h = 106;
			
			SDL_RenderCopy( myRenderer, towerTexture, NULL, &rect );
		}
	}
	
	//Draw Selected Tile.
	if(selectedX != -1 || selectedY != -1 )
	{			
		SDL_Rect rect;

		int x = selectedX * 53;
		int y = (selectedY - viewPosition) * 53;
		y = y + 53;
		
		if(map.getMapValue(selectedX, selectedY) == 0)
		{			
			rect.x = x;rect.y = y;rect.w = 53;rect.h=53;
			
			if(myUnits[selectedY][selectedX].exists == true)
			{
				//Draw the selected unit.
				//you selected a unit.
				mapUnitSelected = true;
				int value = myUnits[selectedY][selectedX].type;
				
				switch(value)
				{
					case 0:
						//worker.
						SDL_RenderCopy( myRenderer, workerSelectUp, NULL, &rect);
						break;
					case 1:
						//basic.
						SDL_RenderCopy( myRenderer, basicSelectUp, NULL, &rect );
						break;
					case 2:
						//flying.
						SDL_RenderCopy( myRenderer, flyingSelectUp, NULL, &rect );
						break;
					case 3:
						//advanced
						SDL_RenderCopy( myRenderer, advancedSelectUp, NULL, &rect );
						break;
				}
			}
			else
			{
				rect.x = x;rect.y = y;rect.w = 53;rect.h=53;
				SDL_RenderCopy( myRenderer, floorTileSelect, NULL, &rect);	
			}
			
		}
		else
		{
			selectedX = -1;
			selectedY = -1;
		}
	}
}

void Game::getInput()
{
	//Poll for user input here.
	if(SDL_PollEvent(&myEvent))
	{
		//If you quit the game.
		if(myEvent.type == SDL_QUIT)
		{
			quitGame = true;
		}
		
		//If you click the mouse.
		if(myEvent.type == SDL_MOUSEBUTTONDOWN)
		{
			//Did you click the mouse?
			getMouseInput(&myEvent);
		}
		
		//If you hit a key.
		if(myEvent.type == SDL_KEYDOWN)
		{
			//Did you press a key?
			getKeyInput(myEvent.key.keysym.sym);
		}
	}
}

void Game::getKeyInput(SDL_Keycode key)
{
	if(key == SDLK_ESCAPE)
	{
		quitGame = true;
	}
	
	if(key == SDLK_SPACE)
	{
		if(gameState == true)
		{
			selectedX = -1;
			selectedY = -1;
			unitSelected = -1;
		}
	}
	
	if(key == SDLK_BACKSPACE)
	{
		if(gameState != true)
		{
			resetState();
			startMenuState = true;
		}
	}
	
}

void Game::getMouseInput(SDL_Event* event)
{
	//Someone clicked the mouse.
	int mouseX = event->button.x;
	int mouseY = event->button.y;
	
	//Depends on the state...
	if(startMenuState == true)
	{
		//We are in the start menu.
		//Button Code Here.
		if(testBounds(mouseX, mouseY, 270, 200, 370, 250) == true)
		{
			//You Pressed Play Game.
			resetState();
			gameMenuState = true;
		}
		
		if(testBounds(mouseX, mouseY, 270, 275, 370, 325) == true)
		{
			//You Pressed the about button.
			resetState();
			aboutState = true;
		}
		
		if(testBounds(mouseX, mouseY, 270, 350, 370, 400) == true)
		{
			//You Pressed the Quit Button.
			quitGame = true;
		}
	}
	else if(aboutState == true)
	{
		//In About Menu.
		if(testBounds(mouseX, mouseY, 270, 400, 370, 450) == true)
		{
			//You Clicked Return.
			resetState();
			startMenuState = true;
		}
	}
	else if(gameMenuState == true)
	{
		//In Game Menu.
		if(testBounds(mouseX, mouseY, 195, 400, 295, 450) == true)
		{
			//You Clicked Return.
			resetState();
			startMenuState = true;
		}
		
		if(testBounds(mouseX, mouseY, 345, 400, 445, 450) == true)
		{
			//You Clicked Play Game.
			//Search to Here.
			resetUnits();
			resetState();
			gameState = true;
			mapUnitSelected = false;
			blobTimer = (double)SDL_GetTicks();
			blobCounter = 0;
			workerCount = 0;
			//Reset AI.
			aiDelay = 10000;//Waits 10 seconds before move.
			aiTimer = SDL_GetTicks();
			aiMoveCounter = 0;
			
			towerHealth = 50;
			enemyTowerHealth = 50;
			
			blobGenerationDelay = 3000;
			
			attackDelay = 1000;
			attackTimer = SDL_GetTicks();
			movementDelay = 250;
			moveSwitch = 0;
			movementTimer = SDL_GetTicks();
			viewPosition = 18;//max is 24
		}
		
	}
	else if(gameState == true)
	{
		//In Game.
		if(testBounds(mouseX, mouseY, 0, 0, 640, 53) == true)
		{
			//you pressed the top 53 pixels.
			if(viewPosition > 0)
			{
				viewPosition = viewPosition - 1;
			}
			
		}
		
		if(testBounds(mouseX, mouseY, 0, 427, 640, 480) == true)
		{
			if(viewPosition < 18)
			{
				viewPosition = viewPosition + 1;
			}
		}
		
		if(testBounds(mouseX, mouseY, 0, 53, 640, 374) == true)
		{
			//You pressed the map somewhere.
			int x = mouseX;
			int y = mouseY;
			y = y - 53;//Get Rid of the boundary at the top.
			y = y / 53;
			x = x / 53;
			y = y + viewPosition;//map reasons.
			
			if(mapUnitSelected == true)
			{
				//Get ready to create a goal position. for unit selected.
				if(map.getMapValue(x, y) == 0)
				{
					//you can move a unit here.
					myUnits[selectedY][selectedX].xGoal = x - selectedX;
					myUnits[selectedY][selectedX].yGoal = y - selectedY;
					
					//works //std::cout << "your unit wants to move X: " << (x - selectedX) << " Y: " << (y - selectedY) << std::endl;
				}
				
				mapUnitSelected = false;				
			}
			
			if(unitSelected != -1)
			{
				//Drop Unit code.
				if(map.getMapValue(x, y) == 0 && y > 20)
				{
					if(unitSelected == 0)
					{
						//Drop Worker Unit.
						if(blobCounter >= 2 && workerCount < 6)
						{								
							myUnits[y][x].exists = true;
							myUnits[y][x].type = 0;
							myUnits[y][x].health = 2;
							myUnits[y][x].damage = 0;
							myUnits[y][x].flying = false;
							myUnits[y][x].xGoal = 0;
							myUnits[y][x].yGoal = 0;
							myUnits[y][x].justMoved = false;
							myUnits[y][x].justAttacked = false;
							blobCounter = blobCounter - 2;
							workerCount++;
						}
					}
					
					if(unitSelected == 1)
					{
						//Drop Basic Unit.
						if(blobCounter >= 2)
						{
							myUnits[y][x].exists = true;
							myUnits[y][x].type = 1;
							myUnits[y][x].health = 4;
							myUnits[y][x].damage = 1;
							myUnits[y][x].flying = false;
							myUnits[y][x].xGoal = 0;
							myUnits[y][x].yGoal = 0;
							myUnits[y][x].justMoved = false;
							myUnits[y][x].justAttacked = false;
							blobCounter = blobCounter - 2;
						}
						
					}
					
					if(unitSelected == 2)
					{
						//Drop Flying Unit.
						if(blobCounter >= 4)
						{
							myUnits[y][x].exists = true;
							myUnits[y][x].type = 2;
							myUnits[y][x].health = 4;
							myUnits[y][x].damage = 1;
							myUnits[y][x].flying = true;	
							myUnits[y][x].xGoal = 0;
							myUnits[y][x].yGoal = 0;
							myUnits[y][x].justMoved = false;
							myUnits[y][x].justAttacked = false;
							blobCounter = blobCounter - 4;
						}
						
					}
					
					if(unitSelected == 3)
					{
						//Drop Advanced Unit.
						if(blobCounter >= 5)
						{
							myUnits[y][x].exists = true;
							myUnits[y][x].type = 3;
							myUnits[y][x].health = 10;
							myUnits[y][x].damage = 3;
							myUnits[y][x].flying = false;	
							myUnits[y][x].xGoal = 0;
							myUnits[y][x].yGoal = 0;
							myUnits[y][x].justMoved = false;
							myUnits[y][x].justAttacked = false;
							blobCounter = blobCounter - 5;
						}
						
					}
				}
				unitSelected = -1;//Redundant. Check Code Below. Doesn't matter tho. So I will keep this.
			}
			else
			{					
				if(enemyUnits[y][x].exists == true)
				{
					
				}
				else
				{
					selectedX = x;
					selectedY = y;
					//std::cout << "X: " << selectedX << " , Y: " << selectedY << std::endl;
				}
				
			}
		}
		
		if(testBounds(mouseX, mouseY, 53, 374, 265, 427) == true)
		{
			//Which unit did you select?
			if(testBounds(mouseX, mouseY, 53, 375, 105, 427) == true)
			{
				//You Pressed Worker Unit.
				//std::cout << "Worker Unit is Selected." << std::endl;
				selectedX=-1;selectedY=-1;
				unitSelected = 0;
			}
			
			if(testBounds(mouseX, mouseY, 106, 375, 158, 427) == true)
			{
				//You Pressed Basic unit.
				//std::cout << "Basic Unit is Selected." << std::endl;
				selectedX=-1;selectedY=-1;
				unitSelected = 1;
			}
			
			if(testBounds(mouseX, mouseY, 159, 375, 211, 427) == true)
			{
				//You Pressed Flying Unit.
				//std::cout << "Flying Unit is Selected." << std::endl;
				selectedX=-1;selectedY=-1;
				unitSelected = 2;
			}
			
			if(testBounds(mouseX, mouseY, 212, 375, 265, 427) == true)
			{
				//You Pressed Advanced Unit.
				//std::cout << "Advanced Unit is Selected." << std::endl;
				selectedX=-1;selectedY=-1;
				unitSelected = 3;
			}
		}
		else
		{
			//You 
			unitSelected = -1;
		}
		
		if(testBounds(mouseX, mouseY, 265, 374, 371, 427) == true)
		{
			//you clicked the unselect button.
			selectedX=-1;selectedY=-1;
			unitSelected=-1;
		}
		//End of Game State.
	}
	
	//End of Mouse Input.
}

bool Game::testBounds(int testX, int testY, int x, int y, int x2, int y2)
{
	if(testX >= x && testX <= x2 
	&& testY >= y && testY <= y2 )
	{
		return true;
	}
	
	return false;
}

Game::~Game()
{
	//Destructor.
	TTF_CloseFont( libraSans );
	libraSans = NULL;
	
	SDL_DestroyTexture( enemyAdvancedUnit );
	SDL_DestroyTexture( enemyFlyingUnit );
	SDL_DestroyTexture( enemyBasicUnit );
	SDL_DestroyTexture( youWin );
	SDL_DestroyTexture( youLose );
	SDL_DestroyTexture( unselectButton );
	SDL_DestroyTexture( workerSelectUp );
	SDL_DestroyTexture( basicSelectUp );
	SDL_DestroyTexture( flyingSelectUp );
	SDL_DestroyTexture( advancedSelectUp );
	SDL_DestroyTexture( workerUnitUp );
	SDL_DestroyTexture( basicUnitUp );
	SDL_DestroyTexture( flyingUnitUp );
	SDL_DestroyTexture( advancedUnitUp );
	SDL_DestroyTexture( selectedWorkerUnitButton );
	SDL_DestroyTexture( selectedBasicUnitButton );
	SDL_DestroyTexture( selectedFlyingUnitButton );
	SDL_DestroyTexture( selectedAdvancedUnitButton );
	SDL_DestroyTexture( UIBuffer );
	SDL_DestroyTexture( workerButton );
	SDL_DestroyTexture( basicUnitButton );
	SDL_DestroyTexture( advancedUnitButton );
	SDL_DestroyTexture( flyingUnitButton );
	SDL_DestroyTexture( enemyTowerTexture );
	SDL_DestroyTexture( towerTexture );
	SDL_DestroyTexture( floorTileSelect );
	SDL_DestroyTexture( downArrow );
	SDL_DestroyTexture( upArrow );
	SDL_DestroyTexture( redTile );
	SDL_DestroyTexture( floorTile );
	SDL_DestroyTexture( wallTile );
	SDL_DestroyTexture( gameMenuText );
	SDL_DestroyTexture( returnButton );
	SDL_DestroyTexture( aboutText );
	SDL_DestroyTexture( amorArena );
	SDL_DestroyTexture( playGame );
	SDL_DestroyTexture( about );
	SDL_DestroyTexture( quit );
	SDL_DestroyRenderer( myRenderer );
	SDL_DestroyWindow( myWindow );
	TTF_Quit();
	SDL_Quit();
}