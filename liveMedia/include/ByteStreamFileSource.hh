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
// A file source that is a plain byte stream (rather than frames)
// C++ header

#ifndef _BYTE_STREAM_FILE_SOURCE_HH
#define _BYTE_STREAM_FILE_SOURCE_HH

#ifndef _FRAMED_FILE_SOURCE_HH
#include "FramedFileSource.hh"
#endif

// 单文件字节流源
class ByteStreamFileSource : public FramedFileSource {
public:
	static ByteStreamFileSource* createNew(UsageEnvironment& env,
		char const* fileName,
		unsigned preferredFrameSize = 0,
		unsigned playTimePerFrame = 0);
	// "preferredFrameSize" == 0 means 'no preference'
	// "playTimePerFrame" is in microseconds

	static ByteStreamFileSource* createNew(UsageEnvironment& env,
		FILE* fid,
		unsigned preferredFrameSize = 0,
		unsigned playTimePerFrame = 0);
	// an alternative version of "createNew()" that's used if you already have
	// an open file.

	u_int64_t fileSize() const { return fFileSize; }
	// 0 means zero-length, unbounded, or unknown0意味着零长度，无限的，或未知的

	void seekToByteAbsolute(u_int64_t byteNumber, u_int64_t numBytesToStream = 0);
	// if "numBytesToStream" is >0, then we limit the stream to that number of bytes, before treating it as EOF
	// 如果“numBytesToStream”> 0，那么我们限制流的字节数，将其视为EOF前

	// 跳寻到相对字节位置
	void seekToByteRelative(int64_t offset);
	// 跳寻到文件结尾
	void seekToEnd(); // to force EOF handling on the next read强制EOF处理在下一个读

protected:
	ByteStreamFileSource(UsageEnvironment& env,
		FILE* fid,
		unsigned preferredFrameSize,
		unsigned playTimePerFrame);
	// called only by createNew()

	virtual ~ByteStreamFileSource();

	// 文件可读处理程序(注意这是static方法)
	static void fileReadableHandler(ByteStreamFileSource* source, int mask);
	// 执行从文件读取
	void doReadFromFile();

private:
	// redefined virtual functions:
	virtual void doGetNextFrame();
	virtual void doStopGettingFrames();

protected:
	u_int64_t fFileSize;	//文件大小

private:
	unsigned fPreferredFrameSize;	//最佳帧大小
	unsigned fPlayTimePerFrame;	//每帧播放时间
	Boolean fFidIsSeekable;		//文件指针fFid可seek定位？
	unsigned fLastPlayTime;		//最后播放时间
	Boolean fHaveStartedReading;	//开始读取？
	Boolean fLimitNumBytesToStream;	//到流字节数受到限制?(表示获取帧的时候要检查)
	u_int64_t fNumBytesToStream; // used iff "fLimitNumBytesToStream" is True
};

#endif
