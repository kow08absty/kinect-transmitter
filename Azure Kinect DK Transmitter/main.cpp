#include "WebSocketLib.h"
#include "KinectLib.h"
#include <iostream>
#include <string>
#include <Windows.h>

const int LISTEN_PORT = 50008;

WebSocketLib::Server server;
KinectLib::Kinect kinect;

static BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
	switch (fdwCtrlType) {
		// Handle the CTRL-C signal.
	case CTRL_C_EVENT:
		std::cout << "Receipt Ctrl-C event" << std::endl << std::endl;
		server.Stop();
		kinect.Stop();
		return TRUE;

		// CTRL-CLOSE: confirm that the user wants to exit.
	case CTRL_CLOSE_EVENT:
		//Beep(600, 200);
		//printf("Ctrl-Close event\n\n");
		return TRUE;

		// Pass other signals to the next handler.
	case CTRL_BREAK_EVENT:
		//Beep(900, 200);
		//printf("Ctrl-Break event\n\n");
		return FALSE;

	case CTRL_LOGOFF_EVENT:
		//Beep(1000, 200);
		//printf("Ctrl-Logoff event\n\n");
		return FALSE;

	case CTRL_SHUTDOWN_EVENT:
		//Beep(750, 500);
		//printf("Ctrl-Shutdown event\n\n");
		return FALSE;

	default:
		return FALSE;
	}
}

static void frameCallback(std::string msg) {
	server.BroadCastMessage(msg);
}

int main() {
	if (SetConsoleCtrlHandler(CtrlHandler, TRUE)) {
		kinect.Init();
		kinect.StartCapture(frameCallback);
		server.Run(LISTEN_PORT);
	}

	return 0;
}