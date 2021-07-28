#pragma once
#include <Windows.h>

class Timer
{
	DWORD startTime;
	DWORD endTime;
public:
	Timer(int endTime) { this->endTime = endTime; startTime = 0; }

	void Start() { startTime = GetTickCount64(); }
	void Reset() { startTime = GetTickCount64(); }
	void Stop() { startTime = 0; }

	bool IsTimeUp() { return GetTickCount64() - startTime > endTime; }

	DWORD GetStartTime() { return startTime; }
	DWORD GetLimitedTime() { return endTime; }
	void SetLimitedTime(int temp) { endTime = temp; }
};

