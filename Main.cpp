#include "TestGame.h"
#include <crtdbg.h> 


int main()
{
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif 
	srand(static_cast <unsigned> (time(0)));
	TestGame Test;
	Test.load();
	Test.Start();

	Test.~TestGame();
}