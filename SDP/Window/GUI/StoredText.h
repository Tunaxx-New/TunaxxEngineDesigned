#pragma once
#include "Text.h"
#include <iostream>


//
// This class has own string field
//
class StoredText : public Text
{
public:
	StoredText(HWND* parent) : ::Text(parent, text_) {}

	void set(const std::string& text) {
		text_ = text;
		apply();
	}

	void append(const std::string& text) {
		text_ += text + separator_;
		apply();
	}

	void update() {
		set("hello,");
		::Text::update();
	}

private:
	std::string text_;
};