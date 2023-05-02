package com.WormFishing
{
	import flash.display.Sprite;
	import net.flashpunk.Engine;
	import net.flashpunk.FP;
	import flash.ui.Mouse;
	/**
	 * ...
	 * @author Benjamin Wagner
	 */
	[SWF(width="640", height="480", backgroundColor="#ff0000", frameRate="60")]
	public class Main extends Engine
	{
		public function Main()
		{
			super(640, 480, 60, false);
			FP.screen.color = 0xffffff;
			FP.world = new MenuScreen;
			
			Mouse.hide();
		}
		
		
		
	}
	
}