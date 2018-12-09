#pragma once
#include <condition_variable>
#include <thread>
#include <vector>
#include <queue>
#include <functional>

#include <string>
#include "Camera.h"
#include "Vector3.h"
#include <atomic>
#include "BoundingVolumeHierarchy.h"

struct ThreadTask
{
	ThreadTask()
	{}

	ThreadTask(int samples, int yStart, int yEnd, int width, int height, std::vector<Vector3>* pixelColour, Camera* cam, std::atomic<int>* counter, std::string* string, BoundingVolumeHierarchy* bvh)
	{
		_samples = samples;
		_yStart = yStart;
		_yEnd = yEnd;
		_width = width;
		_height = height;
		_pixelColour = pixelColour;
		_cam = cam;
		_counter = counter;
		_string = string;
		_bvh = bvh;
	}

	int _samples;
	int _yStart;
	int _yEnd;
	int _width;
	int _height;
	std::vector<Vector3>* _pixelColour = nullptr;
	Camera* _cam;
	std::atomic<int>* _counter = nullptr;
	std::string* _string = nullptr;
	BoundingVolumeHierarchy* _bvh;

	void(*foo) (int samples, int yStart, int yEnd, int width, int height, std::vector<Vector3>* pixelColour, Camera cam, std::atomic<int>* counter, std::string* string, BoundingVolumeHierarchy* bvh);
};

class ThreadPool
{
public:
	//using Task = std::function<void(int samples, int yStart, int yEnd, int width, int height, std::vector<Vector3>* pixelColour, Camera _cam, std::atomic<int>* _counter, std::string* _string, BoundingVolumeHierarchy* _bvh)>;
	using Task = ThreadTask;

	explicit ThreadPool(std::size_t numberOfThreads)
	{
		start(numberOfThreads);
	}
	~ThreadPool();

	void enqueue(Task _task);

	bool GetCompleted() { return complete; }

private:
	void start(std::size_t _numberOfThreads);
	void stop();

	std::vector<std::thread> threads;
	std::condition_variable eventVariable;
	std::mutex eventMutex;

	bool stopping = false;
	bool complete = false;
	std::queue<Task> taskQueue;

};
