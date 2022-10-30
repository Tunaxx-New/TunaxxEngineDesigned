#pragma once
#pragma comment(lib, "OpenCL.lib")
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#define __CL_ENABLE_EXCEPTIONS
#define CL_HPP_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include <vector>
#include <stdio.h>
#include "ICompute.h"
#include "../Window/Constants.h"

#define METHODS_PAINT_CL   {"fill", "mandelbrotSet", "conwayGoL"}
#define FILE_CL            "Compute/gpu.cl"

using namespace strings;

class OpclCompute : public ICompute
{
public:
	OpclCompute() {
		initialize();
	}
	void compute(unsigned long** bitmap, int size)
	{
		//for (int i = 0; i < 200 * 200; i++)
		//	(*bitmap)[i] = (COLORREF)((double)(rand() % 10) / 10.0f * 0xFFFFFFFF);
		execute(bitmap, size);

		if (timer < 0) {
			timer = 10;
			if (threadCount > 256)
				threadCount = 0;
			else
				threadCount++;
		}
		timer--;
	}

	std::string getInfo()
	{
		return info_;
	}

private:
	struct platform
	{
		cl::Platform pform;
		std::vector<cl::Device> devices;
	};

	void execute(unsigned long** bitmap, int bitmapSize)
	{
 		if (active_) {
			int size = bitmapSize * sizeof((*bitmap)[0]);
			cl::Buffer BITMAP(context_, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, size, (*bitmap));
			kernels_[4].setArg(0, BITMAP);
			kernels_[4].setArg(1, (size_t)bitmapSize);
			kernels_[4].setArg(2, (size_t)threadCount);
			queue_.enqueueNDRangeKernel(kernels_[4], cl::NullRange, cl::NDRange(threadCount), cl::NullRange);
			queue_.finish();
		}
	}

	int timer = 10;
	int threadCount = 1;

	int initialize()
	{
		try {
			//Some near local variables
			std::vector<cl::Platform> platforms;

			//Getting platforms, checking emptiness, reading info
			cl::Platform::get(&platforms);
			if (platforms.empty())
				return 1;

			info_ += "PLATFORMS: ";
			info_ += NEW_LINE;

			//
			// Prioritize to COORD currentPD_
			//

			int i = 1;
			for (auto p = platforms.begin(); p != platforms.end(); p++) {
				//Getting info about platform
				info_ += std::to_string(i) + ". ";
				info_ += "Name: " + (*p).getInfo<CL_PLATFORM_NAME>() + NEW_LINE;
				info_ += "Extensions: " + (*p).getInfo<CL_PLATFORM_EXTENSIONS>() + NEW_LINE;
				info_ += "Profile: " + (*p).getInfo<CL_PLATFORM_PROFILE>() + NEW_LINE;
				info_ += "Vendor: " + (*p).getInfo<CL_PLATFORM_VENDOR>() + NEW_LINE;
				info_ += "Version: " + (*p).getInfo<CL_PLATFORM_VERSION>() + NEW_LINE;

				platform temp;
				temp.pform = (*p);

				//Getting devices
				(*p).getDevices(CL_DEVICE_TYPE_GPU, &temp.devices);
				if (temp.devices.empty())
					return 2;

				if (currentPD_.X == -1)
					currentPD_.Y = i - 1;

				info_ += "AVAILABEL DEVICES: ";
				info_ += NEW_LINE;
				int j = 1;
				for (auto d = temp.devices.begin(); d != temp.devices.end(); d++) {
					//If device available
					if (!(*d).getInfo<CL_DEVICE_AVAILABLE>()) continue;


					//Main device info------------------------
					std::string extensions = (*d).getInfo<CL_DEVICE_EXTENSIONS>();
					info_ += std::to_string(j) + ". ";
					info_ += "Name: " + (*d).getInfo<CL_DEVICE_NAME>() + NEW_LINE;
					info_ += "Extensions: " + extensions + NEW_LINE;
					info_ += "Profile: " + (*d).getInfo<CL_DEVICE_PROFILE>() + NEW_LINE;
					info_ += "Vendor: " + (*d).getInfo<CL_DEVICE_VENDOR>() + NEW_LINE;
					info_ += "Device version: " + (*d).getInfo<CL_DEVICE_VERSION>() + NEW_LINE;
					info_ += "Driver version: " + (*d).getInfo<CL_DRIVER_VERSION>() + NEW_LINE;
					info_ += "OPENCL C version: " + (*d).getInfo<CL_DEVICE_OPENCL_C_VERSION>() + NEW_LINE;
					info_ += "Kernels: " + (*d).getInfo<CL_DEVICE_BUILT_IN_KERNELS>() + NEW_LINE;
					info_ += "Max work item sizes: ";
					std::vector<size_t> maxItemSizes = (*d).getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();


					for (auto size = maxItemSizes.begin(); size != maxItemSizes.end(); size++) {
						info_ += std::to_string((int)(*size));
						info_ += ' ';
					}
					info_ += NEW_LINE;
					//----------------------------------------

					//Availability to double percussion--------
					bool khr64 = extensions.find("cl_khr_fp64") != std::string::npos;
					bool amd64 = extensions.find("cl_amd_fp64") != std::string::npos;
					if (khr64 || amd64) {
						info_ += "Double percussion is available";
						currentPD_.X = j - 1;
					}
					else {
						info_ += "Double percussion is not available";
					}
					info_ += NEW_LINE;
					//-----------------------------------------

					info_ += NEW_LINE;
					j++;
				}
				i++;
				platforms_.push_back(temp);
			}

			if (currentPD_.X == -1)
				currentPD_.X = 0;


			platforms_ = { platforms_[currentPD_.Y] };
			platforms_[0].devices = { platforms_[0].devices[currentPD_.X] };
			context_ = cl::Context(platforms_[currentPD_.Y].devices);

			maxItemSize_ = platforms_[0].devices[0].getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
			for (auto& item : maxItemSize_)
				item--;

			std::string file = FILE_CL;
			std::vector<std::string> commands = METHODS_PAINT_CL;

			const char* source = kernelSource(file);
			cl::Program program;
			cl::Program::Sources sources(1, std::make_pair(source, strlen(source) + 1));
			int err = 0;
			program = cl::Program(context_, sources);

			try {
				program.compile("-D GPU_FORCE_64BIT_PTR=1 -D GPU_USE_SYNC_OBJECTS=1 -D GPU_MAX_ALLOC_PERCENT=100");
				program.build(platforms_[0].devices, "-D GPU_FORCE_64BIT_PTR=1 -D GPU_USE_SYNC_OBJECTS=1 -D GPU_MAX_ALLOC_PERCENT=100");
			}
			catch (const cl::Error&) {
				std::cerr
					<< "OpenCL compilation error\n"
					<< program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(platforms_[0].devices[0])
					<< '\n';
				return 3;
			}
			program.createKernels(&kernels_);
			info_ += program.getInfo<CL_PROGRAM_KERNEL_NAMES>() + NEW_LINE;
			queue_ = cl::CommandQueue(context_, platforms_[currentPD_.Y].devices[currentPD_.X], CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);
		}
		catch (const cl::Error& err) {
			std::cerr
				<< "OpenCL error: "
				<< err.what() << '(' << err.err() << ")\n";
			return 4;
		}

		active_ = true;
		return 0;
	}

	const char* kernelSource(std::string file)
	{
		//https://stackoverflow.com/questions/29121443/read-opencl-kernel-from-seperate-file
		FILE* fp;
		char* source_str;
		size_t source_size, program_size;

		fopen_s(&fp, file.c_str(), "rb");
		if (!fp) {
			printf("Failed to load kernel\n");
			return 0;
		}

		fseek(fp, 0, SEEK_END);
		program_size = ftell(fp);
		rewind(fp);
		source_str = (char*)malloc(program_size + 1);
		if (source_str) {
			source_str[program_size] = '\0';
			fread(source_str, sizeof(char), program_size, fp);
		}
		fclose(fp);
		return source_str;
	}

	//
	// Info about all platforms, devices
	//
	std::string info_ = "";

	//
	//
	//
	cl::Context context_;

	//
	//
	//
	cl::CommandQueue queue_;

	//
	//
	//
	std::vector<cl::Kernel> kernels_;

	//
	//
	//
	std::vector<platform> platforms_;

	//
	// Current platform and device
	// Y - Platform
	// X - Device
	COORD currentPD_ = { -1, 0 };

	//
	// if Active
	//
	bool active_ = false;

	//
	// Max item size - 1
	//
	std::vector<size_t> maxItemSize_ = { 0, 0, 0 };
};