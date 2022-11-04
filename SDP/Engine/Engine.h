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
		OpclCompute* gpu = new OpclCompute();
		compute_ = (ICompute*)gpu;
		cmat_ = (ComputeMatrix*)gpu;

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
		if (!a)
		compute_->compute(&bmp, 640 * 480);
		else
		tempFuncitonal();

		wf->paint("lol", &bmp);
		wf->broadcast();
		return true;
	}

	void changeCompute() {

		//delete compute_;
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
	ComputeMatrix* cmat_;

	ISubject* subj;




	// TEMP FUNCTIONAL

	float angleX_ = 0;
	float angleY_ = 0;
	float angleZ_ = 0;

	std::vector<std::vector<double>> ax =
	{
	{ 1, 0,             0           , 0},
	{0, cos(angleX_), -sin(angleX_), 0},
	{0, sin(angleX_),  cos(angleX_), 0},
	{0, 0           ,  0           , 0}
	};
	std::vector<std::vector<double>> ay =
	{
		{cos(angleY_), 0, sin(angleY_), 0},
		{0, 1, 0, 0 },
		{-sin(angleY_), 0, cos(angleX_), 0},
		{0, 0, 0, 0}
	};
	std::vector<std::vector<double>> az =
	{
	{cos(angleZ_), -sin(angleZ_), 0, 0},
	{sin(angleZ_), cos(angleZ_), 0, 0},
	{0, 0, 1, 0},
	{0, 0, 0, 0}
	};
	#define PI 3.1416
	void rotate(float x, float y, float z)
	{
		angleX_ += x * PI / 180.0f;
		angleY_ += y * PI / 180.0f;
		angleZ_ += z * PI / 180.0f;

		ax[1][1] = cos(angleX_);
		ax[1][2] = -sin(angleX_);
		ax[2][1] = sin(angleX_);
		ax[2][2] = cos(angleX_);

		ay[0][0] = cos(angleY_);
		ay[0][2] = sin(angleY_);
		ay[2][0] = -sin(angleY_);
		ay[2][2] = cos(angleY_);

		az[0][0] = cos(angleZ_);
		az[0][1] = -sin(angleZ_);
		az[1][0] = sin(angleZ_);
		az[1][1] = cos(angleZ_);
	}

	double fov = 1.0f / tanf(45 + PI / 180.0f);
	double ratio = 480.0f / 640.0f;
	double f = 2;
	double n = 1;

	std::vector<std::vector<double>> p =
	{
		{fov * ratio, 0, 0, 0},
		{0, fov * ratio, 0, 0},
		{0, 0, - f / (f - n), -1},
		{0, 0, 0.01, 0}
	};

	std::vector<std::vector<double>> s =
	{
		{100, 0, 0, 0},
		{0, 100, 0, 0},
		{0, 0, 100, 0},
		{0, 0, 0, 0}
	};

	std::vector<std::vector<double>> t =
	{
		{1, 0, 0, -0.5},
		{0, 1, 0, -0.5},
		{0, 0, 1, -0.5},
		{0, 0, 0, 0}
	};

	std::vector<std::vector<double>> v =
	{
		{0, 0, 0},
		{0, 0, 1},
		{0, 1, 0},
		{0, 1, 1},
		{1, 0, 0},
		{1, 0, 1},
		{1, 1, 0},
		{1, 1, 1}
	};

	void scaleUp()
	{
		s[0][0] *= 2;
		s[1][1] *= 2;
		s[2][2] *= 2;
	}
	void scaleDown()
	{
		s[0][0] /= 2;
		s[1][1] /= 2;
		s[2][2] /= 2;
	}

	void tempFuncitonal()
	{
		memset(bmp, 0, 640 * 480 * sizeof(COLORREF));
		//t[0][3] += 0.1;
		//t[1][3] += 0.1;
		t[2][3] += 0.01;
		std::vector<std::vector<double>> vtemp = v;
		cmat_->multiply(s, vtemp, 0);
		rotate(1, 1, 1);
		cmat_->multiply(ax, vtemp, 0);
		cmat_->multiply(ay, vtemp, 0);
		cmat_->multiply(az, vtemp, 0);
		cmat_->multiply(t, vtemp, 0);
		cmat_->multiply(p, vtemp, 'd');

		for (int i = 0; i < vtemp.size(); i++)
		{	
			int index = (int)vtemp[i][1] * 640 + (int)vtemp[i][0] + 640 * 480 / 2 + 320;
			if (index < 0 || index > 640 * 480 - 1)
				continue;
			if (index)
				bmp[index] = RGB(255, 255, 255);
		}

	}
};