package com.WormFishing 
{
	import net.flashpunk.World;

	public class MenuScreen extends World
	{
		public var button1:button = new button(270, 200,"Hello World!");
		public function MenuScreen() 
		{
			//add(button1);
			add(new Bucket());
		}
		
		override public function update():void
		{
			super.update();
			
			if (button1.isClicked() == true)
			{
				trace("You Win!");
			}
		}
	}

}