/**********
This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version. (See <http://www.gnu.org/copyleft/lesser.html>.)

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
**********/
// "liveMedia"
// Copyright (c) 1996-2012 Live Networks, Inc.  All rights reserved.
// A template for a MediaSource encapsulating an audio/video input device
// 
// NOTE: Sections of this code labeled "%%% TO BE WRITTEN %%%" are incomplete, and needto be written by the programmer
// (depending on the features of the particulardevice).
// C++ header

#ifndef _DEVICE_SOURCE_HH
#define _DEVICE_SOURCE_HH

#ifndef _FRAMED_SOURCE_HH
#include "FramedSource.hh"
#endif

// The following class can be used to define specific encoder parameters
// 下面的类可以用来定义特定编码器参数
class DeviceParameters {
	//%%% TO BE WRITTEN %%%
	//%%%  要被写入 %%%
};

class DeviceSource : public FramedSource {
public:
	static DeviceSource* createNew(UsageEnvironment& env,
		DeviceParameters params);

public:
	static EventTriggerId eventTriggerId;

protected:
	DeviceSource(UsageEnvironment& env, DeviceParameters params);
	// called only by createNew(), or by subclass constructors
	virtual ~DeviceSource();

private:
	// redefined virtual functions:
	virtual void doGetNextFrame();

private:
	static void deliverFrame0(void* clientData);
	void deliverFrame();

private:
	static unsigned referenceCount; // used to count how many instances of this class currently exist
	DeviceParameters fParams;
};

#endif
