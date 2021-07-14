#pragma once
#include <Windows.h>

class Timer
{
	int startTime;
	int endTime;
public:
	Timer(int endTime) { this->endTime = endTime; startTime = 0; }

	void Start() { startTime = GetTickCount(); }
	void Stop() { startTime = 0; }

	bool IsTimeUp() { return GetTickCount() - startTime > endTime; }

	int GetStartTime() { return startTime; }
	int GetLimitedTime() { return endTime; }
	void SetLimitedTime(int temp) { endTime = temp; }
};

