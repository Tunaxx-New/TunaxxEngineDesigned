#pragma once
#include "ICompute.h"

//https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c
#include <chrono>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

class CppCompute : public ICompute
{
	void compute(unsigned long** bitmap, int size)
	{
		long double ratio = (long double)255 / (long double)size;
		int ms_ago = 0;
		int ms_prev = 0;
		int m = 10;
		for (unsigned long i = 0; i < size; i++) {

			auto t1 = high_resolution_clock::now();

			//(*bitmap)[i] = (COLORREF)((double)(rand() % 1000) / 1000.0f * 0xFFFFFFFF);
			for (int j = 0; j < m; j++) {
				//some O(m) computation
				unsigned long a = pow(i, j);
			}
			int color = ratio * i;
			(*bitmap)[i] = RGB(color, ms_prev, ms_ago);
			//

			auto t2 = high_resolution_clock::now();
			duration<double, std::milli> ms_double = t2 - t1;
			
			ms_prev = ms_double.count() * 100.0f * 255.0f;
			ms_ago += ms_double.count() * 1000;
		}
	}
};