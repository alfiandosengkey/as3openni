/**
 * AS3OpenNI (MIT License)
 *
 * AS3OpenNI - Copyright (c) 2011 Tony Birleffi
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 **/

package org.as3openni
{
	import flash.desktop.NativeApplication;
	import flash.desktop.NativeProcess;
	import flash.desktop.NativeProcessStartupInfo;
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	import flash.events.NativeProcessExitEvent;
	import flash.events.ProgressEvent;
	import flash.filesystem.File;
	import flash.system.Capabilities;
	import flash.utils.ByteArray;
	import flash.utils.setTimeout;
	
	import org.as3openni.buffers.DepthBuffer;
	import org.as3openni.buffers.VideoBuffer;
	import org.as3openni.events.AS3OpenNIEvent;
	import org.as3openni.events.ClientSocketEvent;
	import org.as3openni.events.OpenNIEvent;
	import org.as3openni.global.Definitions;
	import org.as3openni.util.ClientSocket;
	
	public class AS3OpenNI extends EventDispatcher
	{
		public var binaryPath:String = "";
		public var trackPadColumns:Number = 4;
		public var trackPadRows:Number = 9;
		
		public var debug:Boolean = false;
		public var video:Boolean = false;
		public var depthMap:Boolean = false;
		public var depthMapBackground:Boolean = false;
		public var mirrorModeOff:Boolean = false;
		public var userTracking:Boolean = false;
		
		private var bridgeReady:Boolean = false;
		private var clientReady:Boolean = false;
		private var pauseBuffers:Boolean = false;
		private var bridge:NativeProcess;
		private var clientSocket:ClientSocket;
		private var videoBuffer:VideoBuffer;
		private var depthBuffer:DepthBuffer;
		
		/**
		 * AS3OpenNI - Constructor.
		 * @target	IEventDispatcher
		 */
		public function AS3OpenNI(target:IEventDispatcher=null)
		{
			super(target);
			trace(Definitions.HEADER);
		}
		
		public function init():void
		{
			if(NativeProcess.isSupported)
			{
				if(this.binaryPath.length > 0)
				{
					// Startup the AS3OpenNI bridge.
					this.startupBridge();
					
					// Add required listeners.
					this.addListeners();
					
					// Setup the client socket.
					setTimeout(this.setupClientSocket, 2000);
				}
			}
		}
		
		public function getVideoBuffer():void
		{
			if(this.isReady() && this.video && !this.pauseBuffers) this.videoBuffer.getBuffer();
		}
		
		public function getDepthBuffer():void
		{
			if(this.isReady() && this.depthMap && !this.pauseBuffers) this.depthBuffer.getBuffer();
		}
		
		public function isReady():Boolean
		{
			return (this.clientReady && this.bridgeReady) ? true : false;
		}
		
		/**
		 * Protected methods will go here.
		 */
		protected function setupFeatures():void
		{
			if(this.clientReady && this.bridgeReady)
			{
				// Setup the video buffer.
				if(this.video) this.videoBuffer = new VideoBuffer(this.clientSocket);
				
				// Setup the depth map buffer.
				if(this.depthMap) this.depthBuffer = new DepthBuffer(this.clientSocket);
			}
		}
		
		protected function setupClientSocket():void
		{
			this.clientSocket = new ClientSocket();
			this.clientSocket.addEventListener(ClientSocketEvent.ON_DATA, onDataReceived);
			this.clientSocket.addEventListener(ClientSocketEvent.ON_ERROR, onClientSocketError);
			this.clientSocket.addEventListener(ClientSocketEvent.ON_CONNECT, onClientSocketConnected);
			this.clientSocket.connect();
		}
		
		protected function onDataReceived(event:ClientSocketEvent):void
		{
			if(this.clientReady)
			{
				var first:Number = event.data.first as Number;
				var second:Number = event.data.second as Number;
				var buffer:ByteArray = event.data.buffer as ByteArray;
				
				trace('First: ' + event.data.first);
				trace('Second: ' + event.data.second);
				
				switch(first)
				{
					case Definitions.AS3OPENNI_ID:
							switch(second)
							{
								case Definitions.AS3OPENNI_SERVER_INIT:
									this.log(Definitions.BRIDGE_CONNECTED);
									break;
								
								case Definitions.AS3OPENNI_SERVER_READY:
									this.bridgeReady = true;
									this.setupFeatures();
									this.log(Definitions.BRIDGE_READY);
									this.dispatchEvent(new AS3OpenNIEvent(AS3OpenNIEvent.READY));
									break;
							}
						break;
					
					case Definitions.OPENNI_ID:
							switch(second)
							{
								case Definitions.OPENNI_GET_DEPTH:
									if(this.depthMap) 
									{
										this.dispatchEvent(new OpenNIEvent(OpenNIEvent.ON_DEPTH, buffer));
										this.depthBuffer.busy = false;
									}
									break;
								
								case Definitions.OPENNI_GET_VIDEO:
									if(this.video) 
									{
										this.dispatchEvent(new OpenNIEvent(OpenNIEvent.ON_VIDEO, buffer));
										this.videoBuffer.busy = false;
									}
									break;
								
								case Definitions.OPENNI_NEW_USER:
									this.pauseBuffers = true;
									var userId:Number = buffer.readInt();
									trace('New User Found: ' + userId);
									setTimeout(this.resumeBuffers, 100);
									break;
								
								case Definitions.OPENNI_USER_LOST:
									this.pauseBuffers = true;
									var lostUserId:Number = buffer.readInt();
									trace('User Lost: ' + lostUserId);
									setTimeout(this.resumeBuffers, 100);
									break;
								
								case Definitions.OPENNI_GET_SKEL:
									break;
							}
						break;
					
					case Definitions.NITE_ID:
						break;
				}
			}
		}
		
		protected function resumeBuffers():void
		{
			this.pauseBuffers = false;
		}
		
		protected function onClientSocketConnected(event:ClientSocketEvent):void
		{
			this.clientReady = true;
			this.log(Definitions.CLIENT_SOCKET_CONNECTED);
			this.dispatchEvent(new ClientSocketEvent(event.type, event.data));
		}
		
		protected function onClientSocketError(event:ClientSocketEvent):void
		{
			this.log(Definitions.CLIENT_SOCKET_ERROR);
			this.dispatchEvent(new ClientSocketEvent(event.type, event.data));
		}
		
		protected function startupBridge():void
		{     
			var file:File = new File();
			var startupInfo:NativeProcessStartupInfo = new NativeProcessStartupInfo();
			
			if (Capabilities.os.toLowerCase().indexOf("win") > -1)
			{
				var ext:String = this.binaryPath.substr((this.binaryPath.length-4), this.binaryPath.length);
				var path:String = (ext == '.exe') ? this.binaryPath : (this.binaryPath + '.exe');
				file = File.applicationDirectory.resolvePath(path);
			} 
			else if (Capabilities.os.toLowerCase().indexOf("mac") > -1) 
			{
				file = File.applicationDirectory.resolvePath(this.binaryPath);
			}
			
			// Continue...
			var processArgs:Vector.<String> = new Vector.<String>();
			
			// Turn on the UserTracking feature.
			if(this.userTracking) processArgs.push("-outf");
			
			// Turn off the mirror mode.
			if(this.mirrorModeOff) processArgs.push("-mrev");
			
			// Turn on the DepthMapCapture feature only testing one or the other in this file.
			if(this.depthMap) 
			{
				// Pass to the binary.
				processArgs.push("-odmc");
				
				// Turn the DepthMaptCapture background on.
				if(this.depthMapBackground) processArgs.push("-dmbg");
			}
			
			// Turn on the RGBCapture feature only testing one or the other in this file.
			if(this.video) processArgs.push("-orgbc");
			
			startupInfo.arguments = processArgs;
			startupInfo.executable = file;
			
			// Startup the bridge.
			this.bridge = new NativeProcess();
			this.bridge.addEventListener(ProgressEvent.STANDARD_OUTPUT_DATA, this.onOutputData);
			this.bridge.start(startupInfo);
		}
		
		/**
		 * Adding the listener methods here.
		 */
		protected function addListeners():void
		{
			NativeApplication.nativeApplication.addEventListener(Event.EXITING, this.onClose);
			this.bridge.addEventListener(ProgressEvent.STANDARD_OUTPUT_DATA, this.onOutputData);
			this.bridge.addEventListener(ProgressEvent.STANDARD_ERROR_DATA, this.onErrorData);
			this.bridge.addEventListener(NativeProcessExitEvent.EXIT, this.onExit);
		}
		
		protected function onClose(event:Event):void
		{
			this.dispatchEvent(new Event(event.type, event.bubbles, event.cancelable));
			if(this.bridge) this.bridge.exit(true);
		}
		
		protected function onOutputData(event:ProgressEvent):void
		{
			this.dispatchEvent(new ProgressEvent(event.type, event.bubbles, event.cancelable, event.bytesLoaded, event.bytesTotal));
			var msg:String = this.bridge.standardOutput.readMultiByte(this.bridge.standardOutput.bytesAvailable, File.systemCharset);
			this.log(msg);
		}
		
		protected function onErrorData(event:ProgressEvent):void
		{
			this.dispatchEvent(new ProgressEvent(event.type, event.bubbles, event.cancelable, event.bytesLoaded, event.bytesTotal));
			if(this.bridge)
			{
				var ba:Number = this.bridge.standardOutput.bytesAvailable;
				if(ba > 0) 
				{
					var msg:String = this.bridge.standardError.readMultiByte(ba, File.systemCharset);
					this.log(msg);
				}
				this.bridge.closeInput();
			}
		}
		
		protected function onExit(event:NativeProcessExitEvent):void
		{
			this.bridgeReady = false;
			this.clientReady = false;
			this.dispatchEvent(new NativeProcessExitEvent(event.type, event.bubbles, event.cancelable, event.exitCode));
			if(this.clientSocket && this.clientSocket.connected) this.clientSocket.close();
		}
		
		protected function log(msg:String):void
		{
			if(this.debug) trace(msg);
		}
	}
}