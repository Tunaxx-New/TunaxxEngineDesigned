#pragma once
#include <string>

class IObserver
{
public:
	virtual void update(const std::string& message) = 0;
	void setTitle(const std::string& message) {
		title_ = message;
	}
protected:
	std::string title_;
};