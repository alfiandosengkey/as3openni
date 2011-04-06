package org.as3openni.faast.gestures
{
	import flash.events.EventDispatcher;
	
	import org.as3openni.faast.events.FAASTEvent;
	import org.as3openni.objects.NiPoint3D;
	import org.as3openni.objects.NiSkeleton;
	import org.as3openni.util.math.NiPoint3DUtil;

	public class FAASTRightFootGestures extends FAASTBasicGestures
	{
		public function FAASTRightFootGestures(senseRange:Number = 450, useInches:Boolean = false)
		{
			super(senseRange, useInches);
		}
		
		override public function configure(skeleton:NiSkeleton):void
		{
			// Call the super.
			super.configure(skeleton);
			
			// Define Left Foot.
			var leftHip:NiPoint3D = skeleton.leftHip;
			var leftKnee:NiPoint3D = skeleton.leftKnee;
			var leftFoot:NiPoint3D = skeleton.leftFoot;
			
			// Define Right Foot.
			var rightHip:NiPoint3D = skeleton.rightHip;
			var rightKnee:NiPoint3D = skeleton.rightKnee;
			var rightFoot:NiPoint3D = skeleton.rightFoot;
			
			// Define Ranges.
			var rightFootRangeX:Number = (Math.max(rightHip.pointX, rightKnee.pointX, rightFoot.pointX) - Math.min(rightHip.pointX, rightKnee.pointX, rightFoot.pointX));
			var rightFootRangeY:Number = (Math.max(rightHip.pointY, rightKnee.pointY, rightFoot.pointY) - Math.min(rightHip.pointY, rightKnee.pointY, rightFoot.pointY));
			var rightFootRangeZ:Number = (Math.max(rightHip.pointZ, rightKnee.pointZ, rightFoot.pointZ) - Math.min(rightHip.pointZ, rightKnee.pointZ, rightFoot.pointZ));
			var leftFootRangeZ:Number = (Math.max(leftHip.pointZ, leftKnee.pointZ, leftFoot.pointZ) - Math.min(leftHip.pointZ, leftKnee.pointZ, leftFoot.pointZ));
			
			// Right Foot Sideways.
			if(rightFootRangeY <= this.senseRange && rightFootRangeZ <= this.senseRange
				&& rightFoot.pointX > rightHip.pointX)
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(rightFootRangeX) : rightFootRangeX;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.RIGHT_FOOT_SIDEWAYS, this.distance));
			}
			
			// Right Foot Forward.
			if(rightFootRangeX <= (this.senseRange/2.5) && rightFootRangeZ >= (this.senseRange/2.5)
				&& rightFoot.pointZ < leftFoot.pointZ)
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(rightFootRangeY) : rightFootRangeY;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.RIGHT_FOOT_FORWARD, this.distance));
			}
			
			// Right Foot Backward.
			if(rightFootRangeX <= (this.senseRange/2.5) && rightFootRangeZ >= (this.senseRange/1.5)
				&& rightFoot.pointZ > leftFoot.pointZ)
			{
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(rightFootRangeY) : rightFootRangeY;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.RIGHT_FOOT_BACKWARD, this.distance));
			}
			
			// Right Foot Up.
			if(rightFootRangeX <= this.senseRange && rightFootRangeY <= this.senseRange
				&& rightFoot.pointY > leftFoot.pointY)
			{
				var val:Number = Math.abs(rightFoot.pointZ-leftFoot.pointZ);
				this.distance = (this.useInches) ? NiPoint3DUtil.convertMMToInches(val) : val;
				this.dispatchEvent(new FAASTEvent(FAASTEvent.RIGHT_FOOT_UP, this.distance));
			}
		}
	}
}