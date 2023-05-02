package com.WormFishing 
{
	import flash.display.BitmapData;
	import net.flashpunk.Entity;
	import net.flashpunk.graphics.Graphiclist;
	import net.flashpunk.graphics.Text;
	import net.flashpunk.Graphic;
	import net.flashpunk.graphics.Image;
	import net.flashpunk.utils.Input;
	
	public class button extends Entity
	{
		[Embed(source = "/../assets/redSquare.png")]private const pic:Class;
		public var buttonText:String;
		public var text:Text;
		public var display:Graphiclist;
		public function button(initx:int = 0, inity:int = 0,t:String = "") 
		{
			buttonText = t;
			text = new Text(buttonText, x, y);
			display = new Graphiclist(new Image(pic), text);
			x = initx;
			y = inity;
		}
		
		public function getMouseX():Number
		{
			return (Input.mouseX);
		}
		
		public function getMouseY():Number
		{
			return (Input.mouseY);
		}
		
		public function isClicked():Boolean
		{
			if (Input.mouseX >= x && Input.mousePressed == true && Input.mouseX <= x + 100
				&& Input.mouseY >= y && Input.mouseY <= y + 50)
				{
					return true;
				}
				else
				{
					return false;
				}
		}
		
		override public function update():void
		{
			graphic = display;
			text.text = buttonText;
		}
		
	}

}