package org.as3openni.objects
{
	public class NiPoint2D
	{
		public var user:Number;
		public var pointX:Number;
		public var pointY:Number;
		
		public function NiPoint2D(){}
		
		public function toString():String
		{
			var str:String = "id: " + user + 
							 ", x: " + this.pointX + 
							 ", y: " + this.pointY;
			return str;
		}
	}
}