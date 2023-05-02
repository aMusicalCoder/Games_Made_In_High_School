package com.WormFishing 
{
	import flash.geom.Point;
	import net.flashpunk.Entity;
	import net.flashpunk.graphics.Image;
	import net.flashpunk.utils.Input;
	import net.flashpunk.FP;
	
	public class Bucket extends Entity
	{
		[Embed(source = "/../assets/bucket.png")] private var bucketImage:Class;
		
		public var bucketThing:Image = new Image(bucketImage);
		public var xa:int = 0;
		public var ya:int = 0;
		public var mousexa:int = 0;
		public var mouseya:int = 0;
		public var counter:int = 0;
		public var degrees:int = 0;
		public var grav:int = 0;
		public var dog:int = 0;
		
		public function Bucket() 
		{
			graphic = bucketThing;
		}
		
		public function getBucketAngle():Number
		{
			return bucketThing.angle;
		}
		
		public function isBucketUpsideDown():Boolean
		{
			if ((bucketThing.angle >= -200 && bucketThing.angle <= -160) || 
				(bucketThing.angle <= 200 && bucketThing.angle >= 160))
				{
					return true;
				}
				else 
				{
					return false;
				}
		}
		
		public function getXa():Number
		{
			
			if (mousexa > 0 && mousexa < 1)
			{
				return 1;
			}
			else if (mousexa > 1 && mousexa < 2)
			{
				return 2;
			}
			else if (mousexa > 2)
			{
				return 3;
			}
			else if ( mousexa == 0)
			{
				return 0;
			}
			else if (mousexa < 0 && mousexa > -1)
			{
				return -1;
			}
			else if (mousexa < -1 && mousexa > -2)
			{
				return -2;
			}
			else if (mousexa < -2)
			{
				return -3;
			}
			else
			{
				return 0;
			}
			
			
		}
		
		public function getYa():Number
		{
			
			if (mouseya > 0 && mouseya < 1)
			{
				return 1;
			}
			else if (mouseya > 1 && mouseya < 2)
			{
				return 2;
			}
			else if (mouseya > 2)
			{
				return 3;
			}
			else if ( mouseya == 0)
			{
				return 0;
			}
			else if (mouseya < 0 && mouseya > -1)
			{
				return -1;
			}
			else if (mouseya < -1 && mouseya > -2)
			{
				return -2;
			}
			else if (mouseya < -2)
			{
				return -3;
			}
			else
			{
				return 0;
			}
			
		}
		
		override public function update():void
		{
			dog = 0;
			//Math.PI * 56 is upside down.
			
			mousexa = Input.mouseX - x - 20;
			mouseya = Input.mouseY - y;
			//Original
			//degrees = -Math.PI * mousexa;
			
			//If angle, and mouseya, then stabilize.
			if (bucketThing.angle > -200 && bucketThing.angle <= -160 && mouseya > 1)
			{
				dog = -180;
			}
			if (bucketThing.angle < 200 && bucketThing.angle >= 160 && mouseya > 1)
			{
				dog = 180;
			}
			
			//if angle, then gravity.
			if (degrees > 0)
			{
				grav = -degrees / 5;
			}
			else if (degrees < 0)
			{
				grav = -degrees / 5;
			}
			
			//Rotation based on mouseya, but depends on mousexa.
			//y acceleration while on the left side.
			if (mouseya > 2 && mousexa > 0)
			{
				if (bucketThing.angle > -180)
				{
					degrees += degrees / 5;
					degrees += -Math.PI * 6;
				}
				if (bucketThing.angle > -200 && bucketThing.angle <= -160)
				{
					grav += -degrees / 5;
					degrees += -Math.PI * 24;
					dog = -180;
					
				}
				
			}//y acceleration while on the right side.
			else if (mouseya > 2 && mousexa < 0)
			{
				if (bucketThing.angle < 180)
				{
					degrees += degrees / 5;
					degrees += Math.PI * 6;
				}
				if (bucketThing.angle < 200 && bucketThing.angle > 160)
				{
					grav += -degrees / 5;
					degrees += Math.PI * 24;
					dog = 180;
				}
				
			}
			
			//Rotation based on mousexa
			if (mousexa > 0 && mousexa < 2)
			{
				degrees += -Math.PI * 3;
			}
			else if (mousexa > 2 && mousexa < 4)
			{
				degrees += -Math.PI * 6;
			}
			else if (mousexa > 4 )
			{
				degrees += -Math.PI * 9;
			}
			else if (mousexa == 0)
			{
				
			}
			else if ( mousexa < 0 && mousexa > -2)
			{
				degrees += Math.PI * 3;
			}
			else if (mousexa < -2 && mousexa > -4)
			{
				degrees += Math.PI * 6;
			}
			else if (mousexa < -4 )
			{
				degrees += Math.PI * 9;
			}
				
			degrees += grav;
			x = Input.mouseX - 20;
			y = Input.mouseY;
			bucketThing.angle = degrees;
			if (dog != 0)
			{
				bucketThing.angle = dog;
			}
			//End of Update.
		}
		
	}


}