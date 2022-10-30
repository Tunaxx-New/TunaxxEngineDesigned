#pragma once
#include "../Window/WindowFactory.h"
#include "../Observer/ISubject.h"
#include "../Compute/CppCompute.h"
#include "../Compute/OpclCompute.h"

class Engine : public IEngineManipulation
{
public:
	Engine()
	{
		compute_ = new OpclCompute();
		wf = new WindowFactory(this);
		wf->createWindow("lol");
		WindowClassTemp.x = 1000;
		wf->createConsole("lol2");
		wf->listWindows();

		subj = new ISubject();
		IObserver* c1 = wf->getConsoleObserver("lol2");
		//IObserver* c2 = wf->getConsoleObserver("lol2");
		subj->attach(c1);
		//subj->attach(c2);
		subj->notify(((OpclCompute*)compute_)->getInfo());

		bmp = (COLORREF*)calloc(640 * 480, sizeof(COLORREF));
	}

	bool broadcast()
	{
		compute_->compute(&bmp, 640 * 480);
		wf->paint("lol", &bmp);
		wf->broadcast();
		return true;
	}

	void changeCompute() {
		delete compute_;
		if (a) {
			subj->notify("Changed to CPU computation");
			compute_ = new CppCompute();
		}
		else {
			subj->notify("Changed to GPU computation");
			compute_ = new OpclCompute();
		}
		a = !a;
	}

	~Engine()
	{
		delete bmp;
		delete wf;
		delete bmp;
		delete compute_;
		delete subj;
	}

private:
	bool a = true;
	WindowFactory* wf;
	COLORREF* bmp;
	ICompute* compute_;
	ISubject* subj;
};