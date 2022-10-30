#pragma once
#include <list>

#include "IObserver.h"


class ISubject
{
public:
	//
	// Attaching observer
	// @param1 IObserver* adds observer into observer list observers_
	void attach(IObserver* observer)
	{
		observers_.push_back(observer);
	}

	//
	// Detaching observer from observer list observers_
	//
	void detach(IObserver* observer)
	{
		observers_.remove(observer);
	}

	void notify(const std::string& message)
	{
		std::list<IObserver*>::iterator iterator = observers_.begin();
		while (iterator != observers_.end())
		{
			(*iterator)->update(message);
			++iterator;
		}
	}

protected:
	//
	// The list of observer that would notified
	//
	std::list<IObserver*> observers_;
};