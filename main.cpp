#include "Window.h"

int main(int argc, char* argv[])
{
	Window w("Game", 800, 600);
	if (!w.loadResources())
		return -1;
	w.runEventLoop();
	return 0;
}

