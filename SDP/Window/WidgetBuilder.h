#pragma once
#include "Widget.h"
#include "GUI/Button.h"
#include "GUI/Text.h"
#include "GUI/DependText.h"
#include "GUI/StoredText.h"
#include "GUI/DebugText.h"


class WidgetBuilder
{
public:
	void createWidget(HWND* parent, std::string name)
	{
		Widget* widget = new Widget(parent);
		widgets_[name] = widget;
	}
	void buildButton(std::function<void()> action, const std::string& name, const std::string& widgetName)
	{
		Widget* widget = widgets_[widgetName];

		if (!widget) {
			std::cout << "No widget with this name " + widgetName + "!";
			return;
		}

		WindowClassTemp.y = widget->getCount() * 60;
		Button* button = new Button(widget->getHandle(), action);
		widget->appendUI(button, name);
	}

	void buildText(const std::string& name, const std::string& widgetName, std::function<int()> getter) {
		Widget* widget = widgets_[widgetName];

		if (!widget) {
			std::cout << "No widget with this name " + widgetName + "!";
			return;
		}

		WindowClassTemp.y = widget->getCount() * 60;
		DebugText<int>* text = new DebugText<int>(widget->getHandle(), getter);
		//text->set(str);
		//text->append("SYA");
		widget->appendUI(text, name);
		widget->updateAllUI("");
		text->update();
	}

	void updateAll(std::string name) {
		widgets_[name]->updateAllUI("");
	}

	void print(const std::string& message, char flag, const std::string& name, const std::string& widgetName) {
		Widget* widget = widgets_[widgetName];

		if (!widget) {
			std::cout << "No widget with this name " + widgetName + "!";
			return;
		}

		StoredText* text = (StoredText*)widget->getUI(name);
		if (flag == 'a')
			text->append(message);
		else
			text->set(message);
	}

	bool broadcast()
	{
		for (auto i = widgets_.begin(); i != widgets_.end(); ++i)
		{
			if (i->second == nullptr)
				return false;
			i->second->broadcast();
		}
		return true;
	}

private:
	std::map<std::string, Widget*> widgets_{};
};