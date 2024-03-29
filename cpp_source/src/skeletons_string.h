#ifndef __SKELETONS_STRING_H__
#define __SKELETONS_STRING_H__

#include <XnOpenNI.h>
#include <XnTypes.h>
#include <XnCppWrapper.h>
#include <string>
#include <iostream>

using namespace xn;
using namespace std;

extern UserGenerator _userGenerator;
extern SceneMetaData _sceneData;
extern DepthGenerator _depth;

extern XnBool _convertRealWorldToProjective;
extern XnBool _printUserTracking;
extern XnBool _useSockets;

#define SKEL_FORMAT "user_tracking:%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f||"
#define COM_FORMAT "user_found:%d,%f,%f,%f||"
#define MAC_MAX_USERS 5

void renderSkeleton();
extern void sendToSocket(int socket, const char *data);

#endif