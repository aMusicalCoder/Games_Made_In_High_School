package 
{
	//Graphics/UI Imports.
	import starling.display.MovieClip;
	import starling.display.Sprite;
	import starling.display.Button;
	import starling.display.ButtonState;
	import starling.display.Image;
	import starling.display.Quad;
	import starling.textures.TextureAtlas;
	import starling.text.TextField;
	import starling.animation.Tween;
	import starling.textures.Texture;
	
	//Flash imports.
	import flash.geom.Point;
	import flash.geom.Rectangle;
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.utils.*;
	import flash.data.SQLConnection;
	import flash.data.SQLStatement;
	import flash.events.SQLErrorEvent;
	import flash.events.SQLEvent;
	import flash.filesystem.File;
	import flash.data.SQLResult;
	import flash.errors.IllegalOperationError;
	
	//Utility Imports.
	import starling.core.Starling;
	import starling.utils.VertexData;
	import starling.utils.Color;
	
	//Event Imports.
	import starling.events.EnterFrameEvent;
	import starling.events.Event;
	import starling.events.Touch;
	import starling.events.TouchEvent;
	import starling.events.TouchPhase;
	
	public class Game extends Sprite
	{
		//Start of Game Class.
		
		//Data.
		private var sqlConnect:SQLConnection;
		private var rootFolder:File;
		private var sqlFile:File;
		private var sqlStmt:SQLStatement;
		private var sqlText:String;
		
		//Game States.
		private var splashState:Boolean;//Starting Splash Screen.
		private var menuState:Boolean;//Menu. The Home Base of the Game.
		private var aboutState:Boolean;//Information on Developer.
		private var introState:Boolean;//First time Playing Intro.
		private var gameState:Boolean;//In Game.
		private var scoreState:Boolean;//High Score Screen.
		private var helpState:Boolean;//Help Screen.
		private var endgameState:Boolean;//Once You Lose.
		
		//Temp Stuff.
		private var splashText:TextField;
		
		//Menu Screen UI Elements.
		private var titleText:TextField;
		private var newGame:Button;
		private var highScore:Button;
		private var highScoreText:TextField;
		private var about:Button;
		private var aboutText:TextField;
		private var back:Button;
		private var menuBg:Texture;
		private var restartButton:Button;
		private var easy:Button;
		private var medium:Button;
		private var hard:Button;
		private var resetHigh:Button;
		
		//Graphics.
		[Embed(source="../res/SwipeWire.png")]
		public static const SwipeWire:Class;
		private var wireImage:Image;
		private var phantomImage:Image;
		[Embed(source="../res/FireWire.png")]
		public static const FireWire:Class;
		private var fireImage:Image;
		[Embed(source="../res/mainBG.png")]
		public static const mainBG:Class;
		[Embed(source="mainBG.xml", mimeType="application/octet-stream")]
		public static const mainBGXml:Class;
		private var mainBgTexture:Texture;
		private var bgMovie:MovieClip;
		private var bgAtlas:TextureAtlas;
		private var bgXML:XML;
		
		//Game Elements.
		private var timerStop:int = 1000;//1000 is Easy, 750 is Medium, 500 is Hard.
		private var scoreMultiplier:int = 1;//Increases for every 5 correct Swipes.
		private var scoreIterator:int = 10;//10 for Easy, 25 for Medium, 50 for Hard.
		private var swipeCounter:int = 0;//Iterates for every correct Swipe in a row.
		private var score:int;
		private var scoreText:TextField;
		private var multiText:TextField;
		private var promoteText:TextField;
		private var gameOverText:TextField;
		private var powerText:TextField;
		private var swipeWire:Quad;
		private var wireTexture:Texture;
		private var gameBg:Texture;
		private var bearGuru:Texture;
			//Touch Event Stuff.
		private var startPt:Point;
		private var endPt:Point;
			//Vertex Data Points for Quad Location.
		private var wireVert:VertexData;
		private var vert0:Point;
		private var vert1:Point;
		private var vert2:Point;
		private var vert3:Point;
			//Collision Detection Points.
		private var midTop:Point;
		private var midBottom:Point;
			//Game Logic.
		private var startCondition:Boolean;
		private var timer:int;
		private var powerTimer:int;
		private var tapeTimer:int;
		private var tapeTouch:Boolean;
		private var powerType:int;
		private var powerUp:Boolean;
		private var firePresent:Boolean;
		private var fireTimer:int;
		private var zapPresent:Boolean;
		
		//Function List:
		//Game() Constructor. Starts Game.
		//startGame( e:Event ) Loads Data, Splash Screen. Start Game Loop.
		//mainLoop( e:Event ) Main Game Loop. State Based Design.
		//splashScreen() Runs Splash Screen for 1.5 Seconds.
		//menuScreen() Runs Start Menu. Hub for all areas.
		//aboutScreen() Shows Developer Information. Contact Information.
		//introScreen() Runs Intro Animation. 
		//scoreScreen() Displays High Score Information.
		//gameScreen() Main Game. All Game Logic Here.
			//onTouch() Event for touching the quad.
			//logicLoop() Tracks time and conducts game logic.
			//checkSwipe() Returns Boolean Value of if Swipe was success or not.
		//endScreen() Game Over Screen. Once you Lose.
		//resetState() Sets all states to false, to prevent unintentional states.
		//getScore() Return Score from SQL Database. -1 if data could not be accessed.
		//updateScore() Current Score is placed into the SQL Database.
		//getDif() Returns difficulty setting.
		//updateDif(diff:int) updates difficulty setting.
		
		public function Game() 
		{
			//Start the Game.
			addEventListener(Event.ADDED_TO_STAGE, startGame);
			
		}
		
		private function startGame( e:Event ):void
		{
			//Start Splash Screen.
			resetState();
			splashState = true;
			
			//Do Other start up stuff here. i.e. load data, load assets...
			swipeWire = new Quad(200, 300, 0xff0000);
			wireVert = new VertexData(4);
			vert0 = new Point(1, 1);
			vert1 = new Point(1, 1);
			vert2 = new Point(1, 1);
			vert3 = new Point(1, 1);
			
			//Database.
			try
			{
				rootFolder = File.applicationStorageDirectory;
				sqlFile = rootFolder.resolvePath("DBFile.db");
				sqlConnect = new SQLConnection();
				sqlConnect.open(sqlFile);//Must Be Synchronous Execution here.
				sqlStmt = new SQLStatement();
				sqlStmt.sqlConnection = sqlConnect;//Or this will error.
				
				sqlText = 
				"CREATE TABLE IF NOT EXISTS highScore (" +
				"score INTEGER," +
				"dif INTEGER" + 
				")";
				sqlStmt.text = sqlText;
				sqlStmt.execute();
				
			}catch (e:SQLErrorEvent)
			{
				//trace("Problem Creating/Accessing Database: " + e.error);
			}
			//Pull Graphics.
			var wireBitmap:Bitmap = new SwipeWire();
			wireTexture = Texture.fromBitmap(wireBitmap,true, false,0.75);
			wireImage = new Image(wireTexture);
			phantomImage = new Image(wireTexture);
			
			//fire wire.
			var fireBitmap:Bitmap = new FireWire();
			var fireTexture:Texture = Texture.fromBitmap(fireBitmap, true, false, 0.75);
			fireImage = new Image(fireTexture);
			
			fireImage.touchable = false;
			wireImage.touchable = false;
			phantomImage.touchable = false;
			
			//Main Background texture.
			
			//Create Background Animation.
			mainBgTexture = Texture.fromEmbeddedAsset(mainBG);
			bgXML = XML(new mainBGXml());
			bgAtlas = new TextureAtlas(mainBgTexture, bgXML);
			
			bgMovie = new MovieClip(bgAtlas.getTextures("bg_"), 10);
			//Start Game Loop.
			addEventListener(EnterFrameEvent.ENTER_FRAME, mainLoop);
		}
		
		private function mainLoop( e:Event ):void
		{
			if (splashState == true)
			{
				//Splash Screen for 3 Seconds.
				splashScreen();
			}
			else if (menuState == true)
			{
				//Menu, Home Base of Game.
				menuScreen();
			}
			else if (aboutState == true)
			{
				//Display Developer Information.
				aboutScreen();
			}
			else if (introState == true)
			{
				
			}
			else if (gameState == true)
			{
				//The actual Game.
				gameScreen();
			}
			else if (scoreState == true)
			{
				scoreScreen();
			}
			else if (endgameState == true)
			{
				//Game Over. You Lose. You Suck. Goodbye.
				endScreen();
			}
		}
		
		//Splash Screen.
		private function splashScreen():void
		{
			//Load Splash Screen.
			splashText = new TextField(100, 100, "Trash Developer Games");
			splashText.x = 110;
			splashText.y = 100;
			splashText.visible = true;
			addChild(splashText);
			
			//Rest for 3 seconds.
			var time:int = getTimer();//Initial Time.
			
			while (true)
			{
				//Must continue rendering to display splash screen.
				Starling.current.render();
				
				//Wait 3 seconds.
				if (getTimer() - time >= 1500)//Check if new Time exceeds 1.5 seconds after initial time.
				{
					break;
				}
			}
			
			//Remove Splash Screen.
			removeChild(splashText);
			splashText.dispose();
			
			//Switch to Menu.
			resetState();
			menuState = true;
		}
		
		private function menuScreen():void
		{
			//When Menu is Started.
			
			addChild(bgMovie);
			bgMovie.play();
			Starling.juggler.add(bgMovie);
			//Create Title.
			if (titleText == null)
			{
				titleText = new TextField(250, 50, "Swipe It!", "Verdana", 36);
				titleText.x = 25;
				titleText.y = 50;
			}
			//Create New Game Button.
			if (newGame == null)
			{
				newGame = new Button(Texture.fromBitmap(new Bitmap(new BitmapData(100, 50))), "New Game!");
				newGame.x = 100;
				newGame.y = 150;
				newGame.color = 0x99CCFF;
				newGame.addEventListener(Event.TRIGGERED, function() : void
				{
					resetState();
					gameState = true;
					newGame.removeEventListeners();
					newGame.dispose();
					newGame = null;
					startCondition = false;
				});
			}
			//Create Load Game Button.
			if (highScore == null)
			{
				highScore = new Button(Texture.fromBitmap(new Bitmap(new BitmapData(100, 50))), "High Scores!");
				highScore.x = 100;
				highScore.y = 200;
				highScore.color = 0x99CCFF;
				highScore.addEventListener(Event.TRIGGERED, function() : void
				{
					resetState();
					scoreState = true;
					highScore.removeEventListeners();
					highScore.dispose();
					highScore = null;
				});
			}
			//Create About Button.
			if (about == null)
			{
				about = new Button(Texture.fromBitmap(new Bitmap(new BitmapData(100, 50))), "About!");
				about.x = 100;
				about.y = 250;
				about.color = 0x99CCFF;
				about.addEventListener(Event.TRIGGERED, function() : void
				{
					resetState();
					aboutState = true;
					about.removeEventListeners();
					about.dispose();
					about = null;
				});
			}
			
			if (easy == null)
			{
				easy = new Button(Texture.fromBitmap(new Bitmap(new BitmapData(50, 50))), "Easy");
				easy.x = 50;
				easy.y = 350;
				easy.color = 0x99CCFF;
				easy.addEventListener(Event.TRIGGERED, function() :void
				{
					updateDif(0);
				});
			}
			
			if (medium == null)
			{
				medium = new Button(Texture.fromBitmap(new Bitmap(new BitmapData(50, 50))), "Medium");
				medium.x = 125;
				medium.y = 350;
				medium.color = 0x99CCFF;
				medium.addEventListener(Event.TRIGGERED, function() :void
				{
					updateDif(1);
				});
			}
			
			if (hard == null)
			{
				hard = new Button(Texture.fromBitmap(new Bitmap(new BitmapData(50, 50))), "Hard");
				hard.x = 200;
				hard.y = 350;
				hard.color = 0x99CCFF;
				hard.addEventListener(Event.TRIGGERED, function() : void
				{
					updateDif(2);
				});
			}
			
			addChild(titleText);
			addChild(newGame);
			addChild(highScore);
			addChild(about);
			addChild(easy);
			addChild(medium);
			addChild(hard);
		}
		
		private function aboutScreen():void
		{
			//Runs About Screen.
			if (aboutText == null)
			{
				aboutText = new TextField(300, 480, 
				"Hi. My Name is Ben. I made this game. How are you liking it?"
				+ "You can get in touch with me at TrashDeveloperGames@gmail.com"
				+ " I would love to hear how you like my game or don't :(" 
				+ " I am looking forward to hearing any feedback ofcourse. :) ");
				aboutText.x = 10;
				aboutText.y = 0;
				aboutText.touchable = false;
			}
			
			if (back == null)
			{
				back = new Button(Texture.fromBitmap(new Bitmap(new BitmapData(50, 50))), "Back.");
				back.x = 50;
				back.y = 25;
				back.color = 0x99CCFF;
				back.addEventListener(Event.TRIGGERED, function():void
				{
					//On Button Click.
					resetState();
					menuState = true;
					back.removeEventListeners();
					back.dispose();
					back = null;
				});
			}
			
			addChild(back);
			addChild(aboutText);
		}
		
		private function scoreScreen():void
		{
			//Display High Scores Here.
			if (back == null)
			{
				back = new Button(Texture.fromBitmap(new Bitmap(new BitmapData(50, 50))), "Back.");
				back.x = 50;
				back.y = 25;
				back.color = 0x99CCFF;
				back.addEventListener(Event.TRIGGERED, function():void
				{
					//On Button Click.
					resetState();
					menuState = true;
					back.removeEventListeners();
					back.dispose();
					back = null;
				});
			}
			addChild(back);
			
			//If you want your high to be 0.
			if (resetHigh == null)
			{
				resetHigh = new Button(Texture.fromBitmap(new Bitmap(new BitmapData(200, 50))), "Reset High Score!");
				resetHigh.x = 50;
				resetHigh.y = 200;
				resetHigh.color = 0x99CCFF;
				resetHigh.addEventListener(Event.TRIGGERED, function () :void
				{
					sqlText = "SELECT score FROM highScore";
					sqlStmt.text = sqlText;
					sqlStmt.execute();
					
					try
					{
						sqlText = "UPDATE highScore SET score = 0";
						sqlStmt.text = sqlText;
						sqlStmt.execute();
					}
					catch ( e:Error)
					{}
					catch ( e:SQLErrorEvent )
					{}
				});
			}
			addChild(resetHigh);
			
			//Display Player's High Score.
			if (highScoreText == null)
			{
				highScoreText = new TextField(200, 100, "Your High Score: " + getScore());
				highScoreText.x = 50;
				highScoreText.y = 100;
			}
			//Check if a Score has ever been recorded.
			if (getScore() == -1)
			{
				highScoreText.text = "You haven't played yet.";
			}
			else
			{
				highScoreText.text = "Your High Score: " + getScore();
			}
			addChild(highScoreText);
		}
		
		private function introScreen():void
		{
			
		}
		
		private function gameScreen():void
		{
			//Add Back Button.
			if (back == null)
			{
				back = new Button(Texture.fromBitmap(new Bitmap(new BitmapData(50, 50))), "Back.");
				back.x = 50;
				back.y = 25;
				back.color = 0x99CCFF;
				back.addEventListener(Event.TRIGGERED, function():void
				{
					//On Button Click.
					resetState();
					menuState = true;
					back.removeEventListeners();
					back.dispose();
					back = null;
				});
			}
			//if startCondition not Triggered.
			if (startCondition == false)
			{
				addChild(back);
			}else
			{
				removeChild(back);
			}
			
			//Add Score.
			if (scoreText == null)
			{
				scoreText = new TextField(100, 50, "Score: " + score);
				scoreText.x = 100;
				scoreText.y = 350;
			}
			addChild(scoreText);
			scoreText.text = "Score: " + score;
			
			//Add Promotional Text.
			if (promoteText == null)
			{
				promoteText = new TextField(100, 50, "Swipe up the Wire to begin..");
				promoteText.x = 100;
				promoteText.y = 400;
			}
			addChild(promoteText);
			
			//Multiplier Text.
			if (multiText == null)
			{
				multiText = new TextField(50, 50, "x1");
				multiText.x = 200;
				multiText.y = 350;
			}
			addChild(multiText);
			multiText.text = "x" + scoreMultiplier;
			
			//Powerup Text.
			if (powerText == null)
			{
				powerText = new TextField(100, 100, "No Powerup!");
				powerText.color = 0xff0000;
				powerText.fontSize = 16;
				powerText.bold = true;
				powerText.x = 100;
				powerText.y = 25;
			}
			addChild(powerText);
			
			//GAME LOGIC HERE.
			
			//Add Quad.
			swipeWire.copyVertexDataTransformedTo(wireVert);
			swipeWire.alpha = 0;
			swipeWire.x = 160;
			swipeWire.y = 220;
			swipeWire.setVertexColor(2, 0x0000ff);
			swipeWire.setVertexColor(3, 0x0000ff);
			swipeWire.alignPivot();
			addChild(swipeWire);
			
			wireImage.x = 160;
			wireImage.y = 220;
			wireImage.alignPivot();
			
			if (firePresent)//Add the fire wire.
			{
				removeChild(wireImage);
				addChild(fireImage);
			}
			else//add the regular wire.
			{
				removeChild(fireImage);
				addChild(wireImage);
			}
			
			if (zapPresent)//Zap is happening. add the phantom wire.
			{
				addChild(phantomImage);
			}
			else
			{
				removeChild(phantomImage);
			}
			
			//Call Logic Loop.
			logicLoop();// Timers for stuff.
			swipeWire.addEventListener(TouchEvent.TOUCH, onTouch);
		}
		
		private function logicLoop():void
		{
			//Time in Game Events, and Conduct Logic.
			if (getDif() == 0)//easy
			{
				timerStop = 1000;
				scoreIterator = 10;
			}
			else if (getDif() == 1)//medium
			{
				timerStop = 750;
				scoreIterator = 25;
			}
			else if (getDif() == 2)//hard
			{
				timerStop = 500;
				scoreIterator = 50;
			}
			
			
			if (startCondition == false)
			{
				//One has not touched quad yet.
				swipeWire.rotation = 0;
				wireImage.rotation = 0;
				score = 0;
				scoreMultiplier = 1;
				swipeCounter = 0;
				timer = getTimer();
				tapeTimer = getTimer();
				tapeTouch = false;
				powerTimer = getTimer();
				powerUp = false;
				powerType = 0;
				firePresent = false;
				fireTimer = 0;
				zapPresent = false;
				scoreText.text = "Score: 0";
				multiText.text = "x1";
				promoteText.text = "Swipe up the Wire to begin...";
				powerText.text = "No Powerup!";
			}
			else if (startCondition == true)
			{
				//Check for Lose Condition.
				if (getTimer() - timer >= timerStop)
				{
					if (scoreMultiplier < 5)
					{
						startCondition = false;
						resetState();
						endgameState = true;
						swipeWire.removeEventListeners();
					}
					else //Give Revive.
					{
						scoreMultiplier = 1;
						swipeCounter = 0;
						timer = getTimer();
						
						firePresent = false;
						swipeWire.rotation = (Math.PI / 180) * ((Math.random() * 360) + 1);
						wireImage.rotation = swipeWire.rotation;//New Swipe Position.
						
						promoteText.text = "Second Chance!";
					}
					
				}
				
				//Powerup Timer Stuff.
				if (powerUp == true)
				{
					switch(powerType)//Run powerup code.
					{
						case 0://Duct Tape.
							powerText.text = "Duct Tape!!! Hold Down!";
							//Delay Failure.
							timer = getTimer();
							
							//Check if 3 seconds have passed.
							if (getTimer() - powerTimer >= 5000)
							{
								powerUp = false;
								powerTimer = getTimer();
								tapeTouch = false;
							}
							
							if (getTimer() - tapeTimer >= 1)
							{
								tapeTimer = getTimer();
								if (tapeTouch)
								{
									score += 7;
								}
							}
							
							break;
						case 1://Lightning.
							powerText.text = "Lightning!!! x3 Bonus Multiplier";
							
							if (getTimer() - powerTimer >= 5000)//Check if 3 seconds.
							{
								powerUp = false;
								powerTimer = getTimer();
							}
							break;
						case 2://Power Outage.
							powerText.text = "Power Outage! Lose Half Points.";
							
							if (getTimer() - powerTimer >= 5000)//check if 5 seconds.
							{
								powerUp = false;
								powerTimer = getTimer();
							}
							break;
						case 3://Fire
							powerText.text = "Fire! Watch Out!";
							
							if (getTimer() - powerTimer >= 5000)//check if five seconds.
							{
								powerUp = false;
								powerTimer = getTimer();
								firePresent = false;
								wireImage.rotation = swipeWire.rotation;//New Swipe Position.
							}
							
							fireImage.x = 160;
							fireImage.y = 220;
							fireImage.alignPivot();
							if (firePresent)
							{
								//Delay Failure.
								timer = getTimer();
								if (getTimer() - fireTimer >= timerStop)//good job you didnt touch fire.
								{
									firePresent = false;
									swipeWire.rotation = (Math.PI / 180) * ((Math.random() * 360) + 1);
									wireImage.rotation = swipeWire.rotation;
								}
							}
							
							break;
						case 4://Zap!
							powerText.text = "Zap! What's going on?";
							zapPresent = true;
							
							phantomImage.x = 160;
							phantomImage.y = 220;
							phantomImage.alpha = 0.50;
							phantomImage.alignPivot();
							
							if (getTimer() - powerTimer >= 5000)
							{
								powerUp = false;
								zapPresent = false;
								powerTimer = getTimer();
							}
							break;
						case 5://idk
							
							break;
					}
				}
				else
				{
					//No Powerup.
					powerText.text = "No Powerup!";
					if (getTimer() - powerTimer >= 7000)
					{
						powerTimer = getTimer();
						powerUp = true;
						powerType = (int)(Math.random() * 5);
					}
				}
			}
		}
		
		private function onTouch(e:TouchEvent):void
		{
			var touch:Touch = e.getTouch(this, TouchPhase.BEGAN);
			var release:Touch = e.getTouch(this, TouchPhase.ENDED);
			
			//Power Up Logic Here.
			if (powerUp == true)
			{
				//Run Powerup touch mechanics.
				switch(powerType)
				{
					case 0://Duct Tape.
						if (touch)
						{
							tapeTouch = true;//You started holding down.
						}
						
						if (release)
						{
							tapeTouch = false;//You stopped holding down.
						}
						break;
					case 1://Lightning. x3 Multiplier.
						if (touch)
						{
							startPt = new Point(touch.globalX, touch.globalY);
							//Get Swipe Data.
							swipeWire.copyVertexDataTransformedTo(wireVert);
							wireVert.getPosition(2, vert2);
							wireVert.getPosition(3, vert3);
							vert2 = swipeWire.localToGlobal(vert2);
							vert3 = swipeWire.localToGlobal(vert3);
							
							midBottom = new Point(((vert2.x + vert3.x) / 2), ((vert2.y + vert3.y) / 2));//Point to radiate hitbox around.
						}
						else if (release)
						{
							endPt = new Point(release.globalX, release.globalY);
							swipeWire.copyVertexDataTransformedTo(wireVert);
							wireVert.getPosition(0, vert0);
							wireVert.getPosition(1, vert1);
							vert0 = swipeWire.localToGlobal(vert0);
							vert1 = swipeWire.localToGlobal(vert1);
							
							midTop = new Point(((vert0.x + vert1.x) / 2), ((vert0.y + vert1.y) / 2));//Point to radiate hitbox around.
							
							if (checkSwipe())
							{
								//You Swiped Correctly. With lightning multiplier. Nice.
								promoteText.text = "Good Luck!";
								timer = getTimer();
								score = score + (scoreIterator * scoreMultiplier * 3);
								if (swipeCounter == 5)//After 5 Swipes. More Multiplier.
								{
									if (scoreMultiplier < 5)
									{
										swipeCounter = 0;
										scoreMultiplier++;
									}
								}
								swipeWire.rotation = (Math.PI / 180) * ((Math.random() * 360) + 1);
								wireImage.rotation = swipeWire.rotation;//New Swipe Position.
							}
						}
						
						break;
					case 2://Power Outage.
						if (touch)
						{
							startPt = new Point(touch.globalX, touch.globalY);
							//Get Swipe Data.
							swipeWire.copyVertexDataTransformedTo(wireVert);
							wireVert.getPosition(2, vert2);
							wireVert.getPosition(3, vert3);
							vert2 = swipeWire.localToGlobal(vert2);
							vert3 = swipeWire.localToGlobal(vert3);
							
							midBottom = new Point(((vert2.x + vert3.x) / 2), ((vert2.y + vert3.y) / 2));//Point to radiate hitbox around.
						}
						else if (release)
						{
							endPt = new Point(release.globalX, release.globalY);
							swipeWire.copyVertexDataTransformedTo(wireVert);
							wireVert.getPosition(0, vert0);
							wireVert.getPosition(1, vert1);
							vert0 = swipeWire.localToGlobal(vert0);
							vert1 = swipeWire.localToGlobal(vert1);
							
							midTop = new Point(((vert0.x + vert1.x) / 2), ((vert0.y + vert1.y) / 2));//Point to radiate hitbox around.
							
							if (checkSwipe())
							{
								//You Swiped Correctly. Uh oh. You lose half points. :(
								promoteText.text = "Good Luck!";
								timer = getTimer();
								score = score + ((scoreIterator * scoreMultiplier)/ 2);
								if (swipeCounter == 5)//After 5 Swipes. More Multiplier.
								{
									if (scoreMultiplier < 5)
									{
										swipeCounter = 0;
										scoreMultiplier++;
									}
								}
								swipeWire.rotation = (Math.PI / 180) * ((Math.random() * 360) + 1);
								wireImage.rotation = swipeWire.rotation;//New Swipe Position.
							}
						}
						
						break;
					case 3://Fire.
						if (touch)
						{
							startPt = new Point(touch.globalX, touch.globalY);
							//Get Swipe Data.
							swipeWire.copyVertexDataTransformedTo(wireVert);
							wireVert.getPosition(2, vert2);
							wireVert.getPosition(3, vert3);
							vert2 = swipeWire.localToGlobal(vert2);
							vert3 = swipeWire.localToGlobal(vert3);
							
							midBottom = new Point(((vert2.x + vert3.x) / 2), ((vert2.y + vert3.y) / 2));//Point to radiate hitbox around.
						}
						else if (release)
						{
							endPt = new Point(release.globalX, release.globalY);
							swipeWire.copyVertexDataTransformedTo(wireVert);
							wireVert.getPosition(0, vert0);
							wireVert.getPosition(1, vert1);
							vert0 = swipeWire.localToGlobal(vert0);
							vert1 = swipeWire.localToGlobal(vert1);
							
							midTop = new Point(((vert0.x + vert1.x) / 2), ((vert0.y + vert1.y) / 2));//Point to radiate hitbox around.
							
							if (checkSwipe())
							{
								//You Swiped Correctly.
								//but is fire present.
								if (firePresent)
								{
									//Ha ha, you lose.
									timer = getTimer() - 5000;//You Lose!
									firePresent = false;
								}
								else
								{
									promoteText.text = "Good Luck!";
									timer = getTimer();
									score = score + (scoreIterator * scoreMultiplier);
									if (swipeCounter == 5)//After 5 Swipes. More Multiplier.
									{
										if (scoreMultiplier < 5)
										{
											swipeCounter = 0;
											scoreMultiplier++;
										}
									}
									
									if ((Math.random() * 100 + 1) <= 50)
									{
										firePresent = true;
										fireTimer = getTimer();
									}
									else
									{
										firePresent = false;
									}
									
									swipeWire.rotation = (Math.PI / 180) * ((Math.random() * 360) + 1);
									if (firePresent)
									{
										fireImage.rotation = swipeWire.rotation;
									}
									else
									{
										wireImage.rotation = swipeWire.rotation;
									}
									
								}
							}
						}
						
						break;
					case 4://Zap!
						if (touch)
						{
							startPt = new Point(touch.globalX, touch.globalY);
							//Get Swipe Data.
							swipeWire.copyVertexDataTransformedTo(wireVert);
							wireVert.getPosition(2, vert2);
							wireVert.getPosition(3, vert3);
							vert2 = swipeWire.localToGlobal(vert2);
							vert3 = swipeWire.localToGlobal(vert3);
							
							midBottom = new Point(((vert2.x + vert3.x) / 2), ((vert2.y + vert3.y) / 2));//Point to radiate hitbox around.
						}
						else if (release)
						{
							endPt = new Point(release.globalX, release.globalY);
							swipeWire.copyVertexDataTransformedTo(wireVert);
							wireVert.getPosition(0, vert0);
							wireVert.getPosition(1, vert1);
							vert0 = swipeWire.localToGlobal(vert0);
							vert1 = swipeWire.localToGlobal(vert1);
							
							midTop = new Point(((vert0.x + vert1.x) / 2), ((vert0.y + vert1.y) / 2));//Point to radiate hitbox around.
							
							if (checkSwipe())
							{
								//You Swiped Correctly.
								promoteText.text = "Good Luck!";
								timer = getTimer();
								score = score + (scoreIterator * scoreMultiplier);
								if (swipeCounter == 5)//After 5 Swipes. More Multiplier.
								{
									if (scoreMultiplier < 5)
									{
										swipeCounter = 0;
										scoreMultiplier++;
									}
								}
								swipeWire.rotation = (Math.PI / 180) * ((Math.random() * 360) + 1);
								wireImage.rotation = swipeWire.rotation;//New Swipe Position.
								phantomImage.rotation = (Math.PI / 180) * ((Math.random() * 360) + 1);
							}
						}
						
						break;
					case 5://something i don't know yet.
						
						break;
				}
			}
			else //Normal Code Execution.
			{
				if (touch)//If one starts their touch on the wire.
				{
					startCondition = true;
					//Get Touch Position.
					startPt = new Point(touch.globalX, touch.globalY);
					
					swipeWire.copyVertexDataTransformedTo(wireVert);
					//Copy Swipe Figure Data into Points.
					wireVert.getPosition(2, vert2);
					wireVert.getPosition(3, vert3);
					
					vert2 = swipeWire.localToGlobal(vert2);
					vert3 = swipeWire.localToGlobal(vert3);
					
					midBottom = new Point(((vert2.x + vert3.x) / 2), ((vert2.y + vert3.y) / 2));//Point to radiate hitbox around.
				}
				else if (release)//When they release said touch.
				{
					//Get Release Position.
					endPt = new Point(release.globalX, release.globalY);
					
					swipeWire.copyVertexDataTransformedTo(wireVert);
					//Copy Swipe Figure Data into Points.
					wireVert.getPosition(0, vert0);
					wireVert.getPosition(1, vert1);
					
					vert0 = swipeWire.localToGlobal(vert0);
					vert1 = swipeWire.localToGlobal(vert1);
					
					midTop = new Point(((vert0.x + vert1.x) / 2), ((vert0.y + vert1.y) / 2));//Point to radiate hitbox around.
					//Check Game Logic.
					if (checkSwipe())
					{
						promoteText.text = "Good Luck!";
						timer = getTimer();
						score = score + (scoreIterator * scoreMultiplier);
						swipeCounter++;
						if (swipeCounter == 5)
						{
							if (scoreMultiplier < 5)
							{
								swipeCounter = 0;
								scoreMultiplier++;
							}
						}
						
						swipeWire.rotation = (Math.PI / 180) * ((Math.random() * 360) + 1);
						wireImage.rotation = swipeWire.rotation;
					}
				}
			}
		}
		
		private function checkSwipe():Boolean
		{
			//Check if you did the thing.
			var thresh:int = 125;
			
			if (startPt.x > midBottom.x - thresh && startPt.x < midBottom.x + thresh
				&& startPt.y > midBottom.y - thresh && startPt.y < midBottom.y + thresh)
			{
				if (endPt.x > midTop.x - thresh && endPt.x < midTop.x + thresh
					&& endPt.y > midTop.y - thresh && endPt.y < midTop.y + thresh)
				{
					return true;//If you did the thing right, then this return true.
				}
			}
			
			//If you did not do the thing. Then it defaults to false.
			return false;
		}
		
		private function endScreen():void
		{
			//Place High Score in SQL Database, if there is a high score.
			if (score > getScore())
			{
				updateScore();
			}
			
			if (back == null)
			{
				back = new Button(Texture.fromBitmap(new Bitmap(new BitmapData(50, 50))), "Main Menu.");
				back.x = 50;
				back.y = 25;
				back.color = 0x99CCFF;
				back.addEventListener(Event.TRIGGERED, function():void
				{
					//On Button Click.
					resetState();
					menuState = true;
					back.removeEventListeners();
					back.dispose();
					back = null;
				});
			}
			addChild(back);
			
			if (gameOverText == null)
			{
				gameOverText = new TextField(200, 50, "You Can Do Better. Final Score: " + score);
				gameOverText.x = 60;
				gameOverText.y = 200;
			}
			gameOverText.text = "You Can Do Better. Final Score: " + score;
			addChild(gameOverText);
			
			if (restartButton == null)
			{
				restartButton = new Button(Texture.fromBitmap(new Bitmap(new BitmapData(100, 50))), "Play Again.");
				restartButton.x = 110;
				restartButton.y = 300;
				restartButton.color = 0x99CCFF;
				restartButton.addEventListener(Event.TRIGGERED, function():void
				{
					resetState();
					gameState = true;
					restartButton.removeEventListeners();
					restartButton.dispose();
					restartButton = null;
				});
			}
			addChild(restartButton);
		}
		
		private function resetState():void
		{
			//Resets States to eliminate unintentional states.
			splashState = false;
			menuState = false;
			aboutState = false;
			introState = false;
			scoreState = false;
			gameState = false;
			endgameState = false;
			removeChildren();
		}
		
		private function getDif():int
		{
			sqlText = "SELECT dif FROM highScore";
			sqlStmt.text = sqlText;
			sqlStmt.execute();
			try
			{
				var result:SQLResult = sqlStmt.getResult();
				var data:Object = result.data[result.lastInsertRowID];
				return data.dif;
			}catch ( e:SQLErrorEvent){}catch ( e:Error){}
			
			return -1;
		}
		
		
		private function getScore():int
		{
			//Retrieve the high score from the database.
			sqlText = "SELECT score FROM highScore";
			sqlStmt.text = sqlText;
			sqlStmt.execute();
			try
			{
				var result:SQLResult = sqlStmt.getResult();
				var data:Object = result.data[result.lastInsertRowID];
				return data.score;
			}catch ( e:SQLErrorEvent )
			{
				//trace("Data could not be accessed.");
			}
			catch ( e:Error)
			{
				//trace("Error Prob a null reference");
			}
			
			return -1;//Must be -1 to show error to prevent infinite sql database rows.
		}
		
		private function updateDif(diff:int):void
		{
			//try to update difficulty.
			sqlText = "SELECT dif FROM highScore";
			sqlStmt.text = sqlText;
			sqlStmt.execute();
			
			try
			{
				if (getDif() == -1)
				{
					sqlText =  "INSERT INTO highScore (score, dif) VALUES (" + getScore() + ", " + diff + ")";
					sqlStmt.text = sqlText;
					sqlStmt.execute();
				}
				else
				{
					sqlText = "UPDATE highScore SET dif = " + diff;
					sqlStmt.text = sqlText;
					sqlStmt.execute();
				}
			}catch ( e:SQLErrorEvent){} catch ( e:Error){}
		}
		
		private function updateScore():void
		{
			//Add the current score to the database.
			
			//First, Check if there is data already in Database.
			sqlText = "SELECT score FROM highScore";
			sqlStmt.text = sqlText;
			sqlStmt.execute();
			
			try
			{
				if (getScore() == -1)//-1 means that data was not accessed.
				{					 //If there is data. It should never return -1.
					sqlText = "INSERT INTO highScore VALUES (" + score + ", "+ getDif() + ")";
					sqlStmt.text = sqlText;
					sqlStmt.execute();
				}
				else
				{
					sqlText = "UPDATE highScore SET score = " + score;
					sqlStmt.text = sqlText;
					sqlStmt.execute();
				}
			}
			catch ( e:Error )
			{
				//trace("error");
			}
			catch ( e:SQLErrorEvent )
			{
				//trace("error");
			}
			
		}
		
		//End of Game Class.
	}

}