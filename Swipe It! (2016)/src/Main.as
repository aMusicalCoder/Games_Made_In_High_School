package
{
	import flash.display.Sprite;
	import starling.core.Starling;
	import flash.geom.Rectangle;

	public class Main extends Sprite 
	{
		//Create our starling object.
		private var _starling:Starling;
		
		//Main Method to Run Game From.
		public function Main()
		{
			//Handle if phone changes orientation.
			Starling.handleLostContext = true;
			
			//Get Native Resolution of app window on device.
			var screenWidth:int = stage.fullScreenWidth;
			var screenHeight:int = stage.fullScreenHeight;
			
			//Create Viewport so game will work on multiple devices.
			var viewPort:Rectangle = new Rectangle(0, 0, screenWidth, screenHeight);
			
			//Create Starling Object.
			_starling = new Starling(Game, stage, viewPort);
			
			//Create Scale Screen to work across multiple devices with multiple resolutions.
			_starling.stage.stageWidth = 320;
			_starling.stage.stageHeight = 480;
			
			//Start Game.
			_starling.start();
			_starling.showStats = true;
		}
		
	}
	
}