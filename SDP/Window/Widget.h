#pragma once
#include <functional>
#include "BaseWindow.h"
#include "GUI/UI.h"

class Widget : public BaseWindow
{
public:
	Widget(HWND* parent) : ::BaseWindow(beforeConstruct())
	{
		SetParent(handle_, *parent);
		rollbackBaseWindowClass();
	}

	LRESULT process(HWND handle, UINT message, WPARAM wparam, LPARAM lparam) override
	{
		switch (message)
		{

		default:
			return DefWindowProc(handle, message, wparam, lparam);
		}

		return NULL;
	}

	void appendUI(UI* ui, std::string name)
	{
		uis_[name] = ui;
	}

	void updateAllUI(std::string name)
	{
		for (auto i = uis_.begin(); i != uis_.end(); ++i)
		{
			if (i->second == nullptr)
				break;
			i->second->update();
		}
	}

	UI* getUI(std::string name)
	{
		return uis_[name];
	}

	int getCount() {
		return uis_.size();
	}

private:
	int beforeConstruct()
	{
		WindowClassTemp.style = WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW;
		WindowClassTemp.width = 640;
		WindowClassTemp.height = 480;
		return 0;
	}

	std::map<std::string, UI*> uis_;
};