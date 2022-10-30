#include <iostream>
#include "Engine/Engine.h"


int main()
{
	Engine* engine = new Engine();
	while (engine->broadcast()) {}
	delete engine;
}