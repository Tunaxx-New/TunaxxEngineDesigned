#pragma once
#include "Window.h"
#include "../Observer/IObserver.h"

class Console : public Window, public IObserver
{
	void update(const std::string& message)
	{
		std::cout << title_ << ' ' << message << '\n';
		w->print(message, 'a', "text1", "widget1");
	}
};