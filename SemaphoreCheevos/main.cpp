#if _DEBUG
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-main-d.lib")
#pragma comment(lib, "sfml-network-d.lib")
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-audio-d.lib")
#else
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-main.lib")
#pragma comment(lib, "sfml-network.lib")
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfml-audio.lib")
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")	//get rid of console window for release build
#endif

#include "CheevoTracker.h"

using namespace std;

int main() {

	CheevoTracker tracker;

	std::cout << tracker.isUnlocked("test_cheevo") << std::endl;

	tracker.unlock("test_cheevo");
	std::cout << "unlocked test_cheevo..." << std::endl;

	std::cout << tracker.isUnlocked("test_cheevo") << std::endl;

	std::cout << tracker.getGamerScore("test_cheevo") << std::endl;

	cin.get();

	return EXIT_SUCCESS;
}