#pragma once
#include "Text.h"
#include <iostream>


//
// This class prefer read only string in constructor and it's own field
//
class DependText : public Text
{
public:
	DependText(HWND* parent, std::string& text) : ::Text(parent, text) {}

	void update() {
		//set("l");
		::Text::update();
	}
};