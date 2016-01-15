
#include "TestGame.h"

int main()
{
	TestGame Test;

	Test.Start();

	Test.~TestGame();
}