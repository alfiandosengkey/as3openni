package org.as3openni.flash.nite.sockets
{
	import flash.events.ProgressEvent;
	
	import org.as3openni.flash.nite.events.NitePointEvent;
	import org.as3openni.flash.objects.NiPoint3D;
	import org.as3openni.flash.util.FLBasicSocket;
	
	public class FLNitePointSocket extends FLBasicSocket
	{
		public static const NITE_POINT_SOCKET:String = "nite_point_socket";
		
		public function FLNitePointSocket()
		{
			super();
			this.name = NITE_POINT_SOCKET;
			this.port = 9500;
		}
		
		override protected function onClientSocketData(event:ProgressEvent):void
		{
			super.onClientSocketData(event);
			
			if(this.socketMessage.length > 0)
			{
				var arr:Array = this.socketMessage.split(',');
				var point3D:NiPoint3D = new NiPoint3D();
				point3D.user = Number(arr[0]);
				point3D.pointX = Number(arr[1]);
				point3D.pointY = Number(arr[2]);
				point3D.pointZ = Number(arr[3]);
				point3D.pointTime = Number(arr[4]);
				this.dispatchEvent(new NitePointEvent(NitePointEvent.POINT_DETECTED, point3D));
			}
		}
	}
}