package com.WormFishing 
{
	import net.flashpunk.Entity;
	/**
	 * ...
	 * @author Benjamin Wagner
	 */
	public class MenuButtons extends Entity
	{
		public var button1:button = new button(270, 200);

		public function MenuButtons()
		{
			
		}
		override public function update():void
		{
			if (button1.isClicked == true)
			{
				trace("You Win!");
			}
		}
	}

}