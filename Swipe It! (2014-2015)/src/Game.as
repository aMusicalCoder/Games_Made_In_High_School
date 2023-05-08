package  
{
	import flash.display.Bitmap;
	import flash.events.MouseEvent;
	import flash.geom.Matrix;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	import starling.display.Button;
	import starling.display.Sprite;
	import starling.events.EnterFrameEvent;
	import starling.events.Event;
	import starling.display.Quad;
	import starling.events.TouchEvent;
	import starling.events.TouchPhase;
	import starling.events.Touch;
	import starling.text.TextField;
	import starling.textures.Texture;
	import starling.utils.VertexData;
	import starling.core.Starling;
	
	/**
	 * ...
	 * @author Benjamin "G" Wagner
	 */
	
	public class Game extends Sprite
	{
		//Information for the geometry, yo.
		private var q:Quad;
		private var vertices:VertexData = new VertexData(4);
		private var v0:Point = new Point(1, 1);
		private var v1:Point = new Point(1, 1);
		private var v2:Point = new Point(1, 1);
		private var v3:Point = new Point(1, 1);
		//Booleans for Game Logic.
		private var gameOver:Boolean = false;
		private var frameCounter:int;
		private var score:Number = 0;
		//Positions of touch onTouch and onRelease.
		private var pos1X:Number = 0;
		private var pos1Y:Number = 0;
		private var pos2X:Number = 0;
		private var pos2Y:Number = 0;
		private var startMidX:Number = 0;
		private var startMidY:Number = 0;
		private var endMidX:Number = 0;
		private var endMidY:Number = 0;
		//Text Fields for User Aid.
		private var highScoreText:TextField = new TextField(200, 25, "");
		private var scoreText:TextField = new TextField(200, 25, "");
		private var reset:Button = new Button(Texture.fromColor(50, 30, 0xff0000) , "Reset.");
		//Resolution stuffs.
		private var scaleFactor:Number = 0;
		private var defaultWidth:Number = 320;
		private var defaultHeight:Number = 480;
		private var currentWidth:Number =  0;
		private var currentHeight:Number = 0;
		
		public function Game() 
		{
			addEventListener(Event.ADDED_TO_STAGE, onAdded);
		}
		
		private function onAdded( e:Event):void
		{
			//Get Scale Factor.
			currentWidth = Starling.current.viewPort.width;
			currentHeight = Starling.current.viewPort.height;
			scaleFactor = currentWidth / defaultWidth;
			
			//Create Quad.
			q = new Quad((50 * scaleFactor), (200 * scaleFactor));
			q.color = 0xFF0000;
			q.setVertexColor(2, 0x000000);
			q.setVertexColor(3, 0x000000);
			q.copyVertexDataTo(vertices);
			
			//Position quad.
			q.x = ((stage.stageWidth - q.width) / 2) + 50;
			q.y = ((stage.stageHeight - q.height) / 2) + 50;
			
			//Position Score Text.
			scoreText.x = (stage.stageWidth - scoreText.width) / 2;
			scoreText.y = (stage.stageHeight - 50);
			
			//Position Reset Button.
			reset.x = (stage.stageWidth - reset.width) / 2;
			reset.y = (stage.stageHeight - 90);
			
			addChild( q );
			addChild( scoreText );
			addChild(reset);
			//Create and Add a Red Quad to the screen.
			
			//Event Listeners, obviously.
			addEventListener(EnterFrameEvent.ENTER_FRAME, onFrame);
			this.addEventListener(TouchEvent.TOUCH, onTouch);
			reset.addEventListener(TouchEvent.TOUCH, onReset);
		}
		
		private function onFrame( e:Event):void
		{
			q.alignPivot();
			frameCounter++;
			
			scoreText.text = "Score: " + score;
			//Every Half a second run this block of code.
			if (frameCounter >= 30)
			{
				
				gameOver = true;
				
				
			}
			
			if (gameOver == true)
			{
				removeChild(q);
				highScoreText.text = "Your Horrible Score: " + score;
				highScoreText.x = 100;
				highScoreText.y = 200;
				addChild(highScoreText);
				scoreText.text = "Game Over. You Suck.";
				removeEventListener(EnterFrameEvent.ENTER_FRAME, onFrame);
			}
			
			
		}
		
		private function onTouch( e:TouchEvent):void
		{
			var touch:Touch = e.getTouch(this, TouchPhase.BEGAN);
			var release:Touch = e.getTouch(this, TouchPhase.ENDED);
			
			if (touch)
			{
				pos1X = touch.globalX;
				pos1Y = touch.globalY;
				//Get the first position of the On Touch Position.
				
				q.copyVertexDataTransformedTo(vertices);
				vertices.getPosition(0, v0);
				vertices.getPosition(1, v1);
				v0 = q.localToGlobal(v0);
				v1 = q.localToGlobal(v1);
				//Get The Startzone information.
				
				startMidX = (v0.x + v1.x) / 2;
				startMidY = (v0.y + v1.y) / 2;
				//Get Startzone Midpoint to radiate around.
				
			}
			else if (release)
			{
				pos2X = release.globalX;
				pos2Y = release.globalY;
				//Get the second position of on release.
				
				q.copyVertexDataTransformedTo(vertices);
				vertices.getPosition(2, v2);
				vertices.getPosition(3, v3);
				v2 = q.localToGlobal(v2);
				v3 = q.localToGlobal(v3);
				//Get The Endzone Information.
				
				endMidX = (v2.x + v3.x) / 2;
				endMidY = (v2.y + v3.y) / 2;
				//Get The Endzone Midpoint to radiate around.
				
				if (pos1X > startMidX - (50 * scaleFactor) && pos1X < startMidX + (50 * scaleFactor) && 
					pos1Y > startMidY - (50 * scaleFactor) && pos1Y < startMidY + (50 * scaleFactor) &&
					pos2X > endMidX - (50 * scaleFactor) && pos2X < endMidX + (50 * scaleFactor) &&
					pos2Y > endMidY - (50 * scaleFactor) && pos2Y < endMidY + (50 * scaleFactor))
				{
					q.rotation = (Math.PI / 180) * ((Math.random() * 360) + 1);
					frameCounter = 0;
					score++;
					
				}
				//If Statement for if you did the thing correctly.
				
			}
		}
		
		private function onReset( e:TouchEvent ):void
		{
			var touch:Touch = e.getTouch(this, TouchPhase.BEGAN);
			
			if (touch)
			{
				removeEventListener(EnterFrameEvent.ENTER_FRAME, onFrame);
				addEventListener(EnterFrameEvent.ENTER_FRAME, onFrame);
				gameOver = false;
				removeChild(highScoreText);
				removeChild(q);
				addChild(q);
				frameCounter = 0;
				score = 0;
			}
		}
		
	}

}