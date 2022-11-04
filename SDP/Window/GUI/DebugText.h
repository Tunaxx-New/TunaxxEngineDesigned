#pragma once
#include "StoredText.h"
#include <iostream>


//
// This class has own string field
//
template <typename T>
class DebugText : public StoredText
{
public:
	DebugText(HWND* parent, std::function<T()> getter) : getter_(getter), ::StoredText(parent) {}

	void update() {
		set(std::to_string(getter_()));
		::StoredText::update();
	}

private:
	std::function<T()> getter_;
};