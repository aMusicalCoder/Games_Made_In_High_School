package com.WormFishing 
{
	import net.flashpunk.Entity;
	import net.flashpunk.graphics.Image;
	/**
	 * ...
	 * @author Benjamin Wagner
	 */
	public class Worm extends Entity
	{
		[Embed(source = "/../assets/flatworm.png")] private const Worm1:Class;
		public function Worm() 
		{
			graphic = new Image(Worm1);
		}
		
		override public function update():void
		{
			
		}
		
	}

}