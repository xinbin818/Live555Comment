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
// Copyright (c) 1996-2012, Live Networks, Inc.  All rights reserved
// LIVE555 Media Server
// main program

#include <BasicUsageEnvironment.hh>
#include "DynamicRTSPServer.hh"
#include "version.hh"

//需要导入静态库
#pragma comment(lib,"ws2_32.lib")	//gethostname
#pragma comment(lib,"BasicUsageEnvironment.lib")
#pragma comment(lib,"groupsock.lib")
#pragma comment(lib,"liveMedia.lib")
#pragma comment(lib,"UsageEnvironment.lib")
#pragma comment(lib,"WindowsAudioInputDevice.lib")

int main(int argc, char** argv)
{
	// Begin by setting up our usage environment:
	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
	UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);

	UserAuthenticationDatabase* authDB = NULL;
#ifdef ACCESS_CONTROL	//访问控制
	// To implement client access control to the RTSP server, do the following:
	// 实现客户端访问控制的RTSP服务器，做到以下几点：
	authDB = new UserAuthenticationDatabase;
	authDB->addUserRecord("username1", "password1"); // replace these with real strings真正的字符串(user/passwd)取代这些
	// Repeat the above with each <username>, <password> that you wish to allow
	// access to the server.
	// 重复上述每个<用户名>，<密码>，你想让访问服务器。
#endif

	// Create the RTSP server.  Try first with the default port number (554),
	// 创建RTSP服务，先尝试用默认端口号(554)
	// and then with the alternative port number (8554):
	// 然后选择端口号（8554）：
	RTSPServer* rtspServer;
	portNumBits rtspServerPortNum = 554;
	rtspServer = DynamicRTSPServer::createNew(*env, rtspServerPortNum, authDB);
	if (rtspServer == NULL) {
		rtspServerPortNum = 8554;
		rtspServer = DynamicRTSPServer::createNew(*env, rtspServerPortNum, authDB);
	}
	if (rtspServer == NULL) {
		*env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
		exit(1);
	}

	*env << "LIVE555 Media Server\n";
	*env << "\tversion " << MEDIA_SERVER_VERSION_STRING
		<< " (LIVE555 Streaming Media library version "
		<< LIVEMEDIA_LIBRARY_VERSION_STRING << ").\n";

	char* urlPrefix = rtspServer->rtspURLPrefix();
	*env << "Play streams from this server using the URL\n\t"
		<< urlPrefix << "<filename>\nwhere <filename> is a file present in the current directory.\n";
	*env << "Each file's type is inferred from its name suffix:\n";
	*env << "\t\".264\" => a H.264 Video Elementary Stream file\n";
	*env << "\t\".aac\" => an AAC Audio (ADTS format) file\n";
	*env << "\t\".ac3\" => an AC-3 Audio file\n";
	*env << "\t\".amr\" => an AMR Audio file\n";
	*env << "\t\".dv\" => a DV Video file\n";
	*env << "\t\".m4e\" => a MPEG-4 Video Elementary Stream file\n";
	*env << "\t\".mkv\" => a Matroska audio+video+(optional)subtitles file\n";
	*env << "\t\".mp3\" => a MPEG-1 or 2 Audio file\n";
	*env << "\t\".mpg\" => a MPEG-1 or 2 Program Stream (audio+video) file\n";
	*env << "\t\".ts\" => a MPEG Transport Stream file\n";
	*env << "\t\t(a \".tsx\" index file - if present - provides server 'trick play' support)\n";
	*env << "\t\".wav\" => a WAV Audio file\n";
	*env << "\t\".webm\" => a WebM audio(Vorbis)+video(VP8) file\n";
	*env << "See http://www.live555.com/mediaServer/ for additional documentation.\n";

	// Also, attempt to create a HTTP server for RTSP-over-HTTP tunneling.
	// Try first with the default HTTP port (80), and then with the alternative HTTP
	// port numbers (8000 and 8080).
	// 同时，尝试创建一个HTTP服务器在RTSP-over-HTTP(用HTTP来传输RTSP)隧道。先尝试用默认的HTTP端口的第一（80），然后尝试其他HTTP端口号（8000和8080）。
	if (rtspServer->setUpTunnelingOverHTTP(80) || rtspServer->setUpTunnelingOverHTTP(8000) || rtspServer->setUpTunnelingOverHTTP(8080)) {
		*env << "(We use port " << rtspServer->httpServerPortNum() << " for optional RTSP-over-HTTP tunneling, or for HTTP live streaming (for indexed Transport Stream files only).)\n";
	}
	else {
		*env << "(RTSP-over-HTTP tunneling is not available.)\n";
	}

	env->taskScheduler().doEventLoop(); // does not return

	return 0; // only to prevent compiler warning
}
