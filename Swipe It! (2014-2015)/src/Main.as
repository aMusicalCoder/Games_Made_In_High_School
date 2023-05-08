package  
{
	import flash.display.Sprite;
	import starling.core.Starling;
	import starling.text.TextField;
	
	/**
	 * ...
	 * @author Benjamin "G" Wagner
	 */
	public class Main extends Sprite
	{
		
		private var _starling:Starling;
		
		public function Main() 
		{
			_starling = new Starling(Game, stage);
			_starling.start();
			_starling.showStats = true;
		}
		
	}

}