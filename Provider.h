#pragma once
#include "Observer.h"

class Provider
{
public:
	Provider();
	~Provider();


	virtual void subscribe(Observer *observer);

	virtual void unsubscribe(Observer *observer);

	virtual void notify();

};

