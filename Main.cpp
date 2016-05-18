#include "TestGame.h"

int main()
{
	srand(static_cast <unsigned> (time(0)));
	TestGame Test;

	Test.Start();

	Test.~TestGame();
}