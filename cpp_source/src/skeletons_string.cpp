#include "skeletons_string.h"

#if (XN_PLATFORM == XN_PLATFORM_WIN32)
	#include "socket.h"
	extern SOCKET USER_TRACKING_SOCKET;
#else
	extern int USER_TRACKING_SOCKET;
#endif

struct NIPlayer
{
	std::string *players;
};

NIPlayer *_niPlayer;

void renderSkeleton()
{
	_niPlayer = (NIPlayer*)malloc(sizeof(NIPlayer)*MAC_MAX_USERS*375);
	_niPlayer->players = new std::string();
	
	XnUserID aUsers[MAC_MAX_USERS];
	XnUInt16 nUsers = MAC_MAX_USERS;
	_userGenerator.GetUsers(aUsers, nUsers);
	
	for (int i = 0; i < nUsers; ++i)
	{
		char * playerData;
		int playerLength;
		XnUserID player;
		XnPoint3D com;
		player = aUsers[i];
		_userGenerator.GetCoM(player, com);
		
		// If a user is being tracked then do this.
		if(_userGenerator.GetSkeletonCap().IsTracking(aUsers[i]))
		{
			XnSkeletonJointPosition head, neck, left_shoulder, left_elbow, left_hand, right_shoulder, right_elbow, right_hand;
			XnSkeletonJointPosition torso, left_hip, left_knee, left_foot, right_hip, right_knee, right_foot;
			
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_HEAD, head);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_NECK, neck);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_TORSO, torso);
			
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_SHOULDER, left_shoulder);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_ELBOW, left_elbow);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_HAND, left_hand);
			
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_SHOULDER, right_shoulder);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_ELBOW, right_elbow);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_HAND, right_hand);
		
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_HIP, left_hip);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_KNEE, left_knee);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_LEFT_FOOT, left_foot);
			
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_HIP, right_hip);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_KNEE, right_knee);
			_userGenerator.GetSkeletonCap().GetSkeletonJointPosition(player, XN_SKEL_RIGHT_FOOT, right_foot);
			
			if(_convertRealWorldToProjective)
			{
				// Top and Body.
				if (head.fConfidence < 0.5) return;
				XnPoint3D pt_head[1];
				pt_head[0] = head.position;
				_depth.ConvertRealWorldToProjective(1, pt_head, pt_head);
				
				if (neck.fConfidence < 0.5) return;
				XnPoint3D pt_neck[1];
				pt_neck[0] = neck.position;
				_depth.ConvertRealWorldToProjective(1, pt_neck, pt_neck);
				
				if (torso.fConfidence < 0.5) return;
				XnPoint3D pt_torso[1];
				pt_torso[0] = torso.position;
				_depth.ConvertRealWorldToProjective(1, pt_torso, pt_torso);
				
				// Left Arm.
				if (left_shoulder.fConfidence < 0.5) return;
				XnPoint3D pt_left_shoulder[1];
				pt_left_shoulder[0] = left_shoulder.position;
				_depth.ConvertRealWorldToProjective(1, pt_left_shoulder, pt_left_shoulder);
				
				if (left_elbow.fConfidence < 0.5) return;
				XnPoint3D pt_left_elbow[1];
				pt_left_elbow[0] = left_elbow.position;
				_depth.ConvertRealWorldToProjective(1, pt_left_elbow, pt_left_elbow);
				
				if (left_hand.fConfidence < 0.5) return;
				XnPoint3D pt_left_hand[1];
				pt_left_hand[0] = left_hand.position;
				_depth.ConvertRealWorldToProjective(1, pt_left_hand, pt_left_hand);
				
				// Right Arm.
				if (right_shoulder.fConfidence < 0.5) return;
				XnPoint3D pt_right_shoulder[1];
				pt_right_shoulder[0] = right_shoulder.position;
				_depth.ConvertRealWorldToProjective(1, pt_right_shoulder, pt_right_shoulder);
				
				if (right_elbow.fConfidence < 0.5) return;
				XnPoint3D pt_right_elbow[1];
				pt_right_elbow[0] = right_elbow.position;
				_depth.ConvertRealWorldToProjective(1, pt_right_elbow, pt_right_elbow);
				
				if (right_hand.fConfidence < 0.5) return;
				XnPoint3D pt_right_hand[1];
				pt_right_hand[0] = right_hand.position;
				_depth.ConvertRealWorldToProjective(1, pt_right_hand, pt_right_hand);
				
				// Left Leg.
				if (left_hip.fConfidence < 0.5) return;
				XnPoint3D pt_left_hip[1];
				pt_left_hip[0] = left_hip.position;
				_depth.ConvertRealWorldToProjective(1, pt_left_hip, pt_left_hip);
				
				if (left_knee.fConfidence < 0.5) return;
				XnPoint3D pt_left_knee[1];
				pt_left_knee[0] = left_knee.position;
				_depth.ConvertRealWorldToProjective(1, pt_left_knee, pt_left_knee);
				
				if (left_foot.fConfidence < 0.5) return;
				XnPoint3D pt_left_foot[1];
				pt_left_foot[0] = left_foot.position;
				_depth.ConvertRealWorldToProjective(1, pt_left_foot, pt_left_foot);
				
				// Right Leg.
				if (right_hip.fConfidence < 0.5) return;
				XnPoint3D pt_right_hip[1];
				pt_right_hip[0] = right_hip.position;
				_depth.ConvertRealWorldToProjective(1, pt_right_hip, pt_right_hip);
				
				if (right_knee.fConfidence < 0.5) return;
				XnPoint3D pt_right_knee[1];
				pt_right_knee[0] = right_knee.position;
				_depth.ConvertRealWorldToProjective(1, pt_right_knee, pt_right_knee);
				
				if (right_foot.fConfidence < 0.5) return;
				XnPoint3D pt_right_foot[1];
				pt_right_foot[0] = right_foot.position;
				_depth.ConvertRealWorldToProjective(1, pt_right_foot, pt_right_foot);
				
				#if (XN_PLATFORM == XN_PLATFORM_WIN32)
					playerLength = _snprintf
					(
						NULL, 0, SKEL_FORMAT, player,
						pt_head[0].X, pt_head[0].Y, pt_head[0].Z, 
						pt_neck[0].X, pt_neck[0].Y, pt_neck[0].Z,
						pt_torso[0].X, pt_torso[0].Y, pt_torso[0].Z,
						
						pt_left_shoulder[0].X, pt_left_shoulder[0].Y, pt_left_shoulder[0].Z,
						pt_left_elbow[0].X, pt_left_elbow[0].Y, pt_left_elbow[0].Z,
						pt_left_hand[0].X, pt_left_hand[0].Y, pt_left_hand[0].Z,
						
						pt_right_shoulder[0].X, pt_right_shoulder[0].Y, pt_right_shoulder[0].Z,
						pt_right_elbow[0].X, pt_right_elbow[0].Y, pt_right_elbow[0].Z,
						pt_right_hand[0].X, pt_right_hand[0].Y, pt_right_hand[0].Z,
						
						pt_left_hip[0].X, pt_left_hip[0].Y, pt_left_hip[0].Z,
						pt_left_knee[0].X, pt_left_knee[0].Y, pt_left_knee[0].Z,
						pt_left_foot[0].X, pt_left_foot[0].Y, pt_left_foot[0].Z,
						
						pt_right_hip[0].X, pt_right_hip[0].Y, pt_right_hip[0].Z,
						pt_right_knee[0].X, pt_right_knee[0].Y, pt_right_knee[0].Z,
						pt_right_foot[0].X, pt_right_foot[0].Y, pt_right_foot[0].Z
					);
				#else
					playerLength = snprintf
					(
						NULL, 0, SKEL_FORMAT, player,
						pt_head[0].X, pt_head[0].Y, pt_head[0].Z, 
						pt_neck[0].X, pt_neck[0].Y, pt_neck[0].Z,
						pt_torso[0].X, pt_torso[0].Y, pt_torso[0].Z,
						
						pt_left_shoulder[0].X, pt_left_shoulder[0].Y, pt_left_shoulder[0].Z,
						pt_left_elbow[0].X, pt_left_elbow[0].Y, pt_left_elbow[0].Z,
						pt_left_hand[0].X, pt_left_hand[0].Y, pt_left_hand[0].Z,
						
						pt_right_shoulder[0].X, pt_right_shoulder[0].Y, pt_right_shoulder[0].Z,
						pt_right_elbow[0].X, pt_right_elbow[0].Y, pt_right_elbow[0].Z,
						pt_right_hand[0].X, pt_right_hand[0].Y, pt_right_hand[0].Z,
						
						pt_left_hip[0].X, pt_left_hip[0].Y, pt_left_hip[0].Z,
						pt_left_knee[0].X, pt_left_knee[0].Y, pt_left_knee[0].Z,
						pt_left_foot[0].X, pt_left_foot[0].Y, pt_left_foot[0].Z,
						
						pt_right_hip[0].X, pt_right_hip[0].Y, pt_right_hip[0].Z,
						pt_right_knee[0].X, pt_right_knee[0].Y, pt_right_knee[0].Z,
						pt_right_foot[0].X, pt_right_foot[0].Y, pt_right_foot[0].Z
					);
				#endif
				
				// Character object that will store the string.
				playerData = (char*) malloc((playerLength + 1) * sizeof(char));
				
				// Print string in to format.
				#if (XN_PLATFORM == XN_PLATFORM_WIN32)
					_snprintf
					(
						playerData, playerLength, SKEL_FORMAT, player,
						pt_head[0].X, pt_head[0].Y, pt_head[0].Z, 
						pt_neck[0].X, pt_neck[0].Y, pt_neck[0].Z,
						pt_torso[0].X, pt_torso[0].Y, pt_torso[0].Z,
						
						pt_left_shoulder[0].X, pt_left_shoulder[0].Y, pt_left_shoulder[0].Z,
						pt_left_elbow[0].X, pt_left_elbow[0].Y, pt_left_elbow[0].Z,
						pt_left_hand[0].X, pt_left_hand[0].Y, pt_left_hand[0].Z,
						
						pt_right_shoulder[0].X, pt_right_shoulder[0].Y, pt_right_shoulder[0].Z,
						pt_right_elbow[0].X, pt_right_elbow[0].Y, pt_right_elbow[0].Z,
						pt_right_hand[0].X, pt_right_hand[0].Y, pt_right_hand[0].Z,
						
						pt_left_hip[0].X, pt_left_hip[0].Y, pt_left_hip[0].Z,
						pt_left_knee[0].X, pt_left_knee[0].Y, pt_left_knee[0].Z,
						pt_left_foot[0].X, pt_left_foot[0].Y, pt_left_foot[0].Z,
						
						pt_right_hip[0].X, pt_right_hip[0].Y, pt_right_hip[0].Z,
						pt_right_knee[0].X, pt_right_knee[0].Y, pt_right_knee[0].Z,
						pt_right_foot[0].X, pt_right_foot[0].Y, pt_right_foot[0].Z
					);
				#else
					snprintf
					(
						playerData, playerLength, SKEL_FORMAT, player,
						pt_head[0].X, pt_head[0].Y, pt_head[0].Z, 
						pt_neck[0].X, pt_neck[0].Y, pt_neck[0].Z,
						pt_torso[0].X, pt_torso[0].Y, pt_torso[0].Z,
						
						pt_left_shoulder[0].X, pt_left_shoulder[0].Y, pt_left_shoulder[0].Z,
						pt_left_elbow[0].X, pt_left_elbow[0].Y, pt_left_elbow[0].Z,
						pt_left_hand[0].X, pt_left_hand[0].Y, pt_left_hand[0].Z,
						
						pt_right_shoulder[0].X, pt_right_shoulder[0].Y, pt_right_shoulder[0].Z,
						pt_right_elbow[0].X, pt_right_elbow[0].Y, pt_right_elbow[0].Z,
						pt_right_hand[0].X, pt_right_hand[0].Y, pt_right_hand[0].Z,
						
						pt_left_hip[0].X, pt_left_hip[0].Y, pt_left_hip[0].Z,
						pt_left_knee[0].X, pt_left_knee[0].Y, pt_left_knee[0].Z,
						pt_left_foot[0].X, pt_left_foot[0].Y, pt_left_foot[0].Z,
						
						pt_right_hip[0].X, pt_right_hip[0].Y, pt_right_hip[0].Z,
						pt_right_knee[0].X, pt_right_knee[0].Y, pt_right_knee[0].Z,
						pt_right_foot[0].X, pt_right_foot[0].Y, pt_right_foot[0].Z
					);
				#endif
			}
			
			// Don't convert from real world to projective.
			else
			{
				#if (XN_PLATFORM == XN_PLATFORM_WIN32)
					playerLength = _snprintf
					(
						NULL, 0, SKEL_FORMAT, player,
						head.position.X, head.position.Y, head.position.Z, 
						neck.position.X, neck.position.Y, neck.position.Z,
						torso.position.X, torso.position.Y, torso.position.Z,
						
						left_shoulder.position.X, left_shoulder.position.Y, left_shoulder.position.Z,
						left_elbow.position.X, left_elbow.position.Y, left_elbow.position.Z,
						left_hand.position.X, left_hand.position.Y, left_hand.position.Z,
						
						right_shoulder.position.X, right_shoulder.position.Y, right_shoulder.position.Z,
						right_elbow.position.X, right_elbow.position.Y, right_elbow.position.Z,
						right_hand.position.X, right_hand.position.Y, right_hand.position.Z,
						
						left_hip.position.X, left_hip.position.Y, left_hip.position.Z,
						left_knee.position.X, left_knee.position.Y, left_knee.position.Z,
						left_foot.position.X, left_foot.position.Y, left_foot.position.Z,
						
						right_hip.position.X, right_hip.position.Y, right_hip.position.Z,
						right_knee.position.X, right_knee.position.Y, right_knee.position.Z,
						right_foot.position.X, right_foot.position.Y, right_foot.position.Z
					);
				#else
					playerLength = snprintf
					(
						NULL, 0, SKEL_FORMAT, player,
						head.position.X, head.position.Y, head.position.Z, 
						neck.position.X, neck.position.Y, neck.position.Z,
						torso.position.X, torso.position.Y, torso.position.Z,
						
						left_shoulder.position.X, left_shoulder.position.Y, left_shoulder.position.Z,
						left_elbow.position.X, left_elbow.position.Y, left_elbow.position.Z,
						left_hand.position.X, left_hand.position.Y, left_hand.position.Z,
						
						right_shoulder.position.X, right_shoulder.position.Y, right_shoulder.position.Z,
						right_elbow.position.X, right_elbow.position.Y, right_elbow.position.Z,
						right_hand.position.X, right_hand.position.Y, right_hand.position.Z,
						
						left_hip.position.X, left_hip.position.Y, left_hip.position.Z,
						left_knee.position.X, left_knee.position.Y, left_knee.position.Z,
						left_foot.position.X, left_foot.position.Y, left_foot.position.Z,
						
						right_hip.position.X, right_hip.position.Y, right_hip.position.Z,
						right_knee.position.X, right_knee.position.Y, right_knee.position.Z,
						right_foot.position.X, right_foot.position.Y, right_foot.position.Z
					);
				#endif
				
				// Character object that will store the string.
				playerData = (char*) malloc((playerLength + 1) * sizeof(char));
				
				// Print string in to format.
				#if (XN_PLATFORM == XN_PLATFORM_WIN32)
					_snprintf
					(
						playerData, playerLength, SKEL_FORMAT, player,
						head.position.X, head.position.Y, head.position.Z, 
						neck.position.X, neck.position.Y, neck.position.Z,
						torso.position.X, torso.position.Y, torso.position.Z,
						
						left_shoulder.position.X, left_shoulder.position.Y, left_shoulder.position.Z,
						left_elbow.position.X, left_elbow.position.Y, left_elbow.position.Z,
						left_hand.position.X, left_hand.position.Y, left_hand.position.Z,
						
						right_shoulder.position.X, right_shoulder.position.Y, right_shoulder.position.Z,
						right_elbow.position.X, right_elbow.position.Y, right_elbow.position.Z,
						right_hand.position.X, right_hand.position.Y, right_hand.position.Z,
						
						left_hip.position.X, left_hip.position.Y, left_hip.position.Z,
						left_knee.position.X, left_knee.position.Y, left_knee.position.Z,
						left_foot.position.X, left_foot.position.Y, left_foot.position.Z,
						
						right_hip.position.X, right_hip.position.Y, right_hip.position.Z,
						right_knee.position.X, right_knee.position.Y, right_knee.position.Z,
						right_foot.position.X, right_foot.position.Y, right_foot.position.Z
					);
				#else
					snprintf
					(
						playerData, playerLength, SKEL_FORMAT, player,
						head.position.X, head.position.Y, head.position.Z, 
						neck.position.X, neck.position.Y, neck.position.Z,
						torso.position.X, torso.position.Y, torso.position.Z,
						
						left_shoulder.position.X, left_shoulder.position.Y, left_shoulder.position.Z,
						left_elbow.position.X, left_elbow.position.Y, left_elbow.position.Z,
						left_hand.position.X, left_hand.position.Y, left_hand.position.Z,
						
						right_shoulder.position.X, right_shoulder.position.Y, right_shoulder.position.Z,
						right_elbow.position.X, right_elbow.position.Y, right_elbow.position.Z,
						right_hand.position.X, right_hand.position.Y, right_hand.position.Z,
						
						left_hip.position.X, left_hip.position.Y, left_hip.position.Z,
						left_knee.position.X, left_knee.position.Y, left_knee.position.Z,
						left_foot.position.X, left_foot.position.Y, left_foot.position.Z,
						
						right_hip.position.X, right_hip.position.Y, right_hip.position.Z,
						right_knee.position.X, right_knee.position.Y, right_knee.position.Z,
						right_foot.position.X, right_foot.position.Y, right_foot.position.Z
					);
				#endif
			}
		}
		// Else, just track the user's center point.
		else
		{
			#if (XN_PLATFORM == XN_PLATFORM_WIN32)
				playerLength = _snprintf(NULL, 0, COM_FORMAT, player, com.X, com.Y, com.Z);
			#else
				playerLength = snprintf(NULL, 0, COM_FORMAT, player, com.X, com.Y, com.Z);
			#endif
			
			// Character object that will store the string.
			playerData = (char*) malloc((playerLength + 1) * sizeof(char));
			
			#if (XN_PLATFORM == XN_PLATFORM_WIN32)
				_snprintf(playerData, playerLength, COM_FORMAT, player, com.X, com.Y, com.Z);
			#else
				snprintf(playerData, playerLength, COM_FORMAT, player, com.X, com.Y, com.Z);
			#endif
		}
		
		// Copy each result over to the buffer.
		_niPlayer->players->append(playerData);
		
		// Free each player's data memory block.
		free(playerData);
	}
	
	// Pass along the player data.
	if(_niPlayer->players->length() > 0)
	{
		if(_printUserTracking) cout<<"AS3OpenNI :: Players: "<< _niPlayer->players->c_str() <<"\n";
		if(_useSockets) sendToSocket(USER_TRACKING_SOCKET, _niPlayer->players->c_str());
	}
	
	// Reset the players string.
	_niPlayer->players->clear();
	free(_niPlayer);
}