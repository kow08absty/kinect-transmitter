#include "WebSocketLib.h"
#include <iostream>
#include <Windows.h>

const int LISTEN_PORT = 50008;

WebSocketLib::Server server;

static BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
	switch (fdwCtrlType) {
		// Handle the CTRL-C signal.
	case CTRL_C_EVENT:
		server.Stop();
		printf("Receipt Ctrl-C event\n\n");
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

int main() {
	if (SetConsoleCtrlHandler(CtrlHandler, TRUE)) {
		server.Init(LISTEN_PORT);
	}
}