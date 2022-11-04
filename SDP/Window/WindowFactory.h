#pragma once
#include "Console.h"
#include "Window.h"
#include "../Engine/IEngineManipulation.h"


class WindowFactory
{
public:
	WindowFactory(IEngineManipulation* engine) {
		engine_ = engine;
	}

	void createWindow(std::string name)
	{
		Window* window = new Window();
		windows_[name] = (BaseWindow*)window;
	}

	void paint(std::string name, COLORREF** bitmap)
	{
		Window* window = (Window*)windows_[name];
		window->setBitmap(bitmap);
	}

	void createConsole(std::string name)
	{
		WindowClassTemp.style |= WS_CLIPCHILDREN;
		Console* console = new Console();
		rollbackBaseWindowClass();
		console->setTitle(name);
		console->buildCommonText();
		console->buildCommonButton(std::bind(&IEngineManipulation::changeCompute, engine_), "b1");
		console->buildCommonButton(std::bind(&IEngineManipulation::scaleUp, engine_), "b2");
		console->buildCommonButton(std::bind(&IEngineManipulation::scaleDown, engine_), "b3");

		windows_[name] = (BaseWindow*)console;
	}

	IObserver* getConsoleObserver(std::string name)
	{
		return (IObserver*)(Console*)windows_[name];
	}

	bool broadcast()
	{
		for (auto i = windows_.begin(); i != windows_.end(); ++i)
		{
			if (i->second == nullptr)
				return false;
			i->second->broadcast();
		}
		return true;
	}

	void listWindows()
	{
		for (auto i = windows_.begin(); i != windows_.end(); ++i)
		{
			std::cout << i->first << ' ' << i->second << '\n';
		}
	}

private:
	std::map<std::string, BaseWindow*> windows_{};
	IEngineManipulation* engine_;
};