#include "misc.h"
#include <windows.h>

bool ReadInMem(const char *filename, char *&data, unsigned int &size)
{
	auto file = CreateFileA(filename, GENERIC_READ, 0, nullptr, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
	if(file == INVALID_HANDLE_VALUE || GetLastError() == ERROR_FILE_NOT_FOUND)
	{
		data = nullptr;
		size = 0;
		return false;
	}

	size = GetFileSize(file, nullptr);
	if(size != INVALID_FILE_SIZE)
	{
		data = new char[size];
		DWORD readBytes;
		if(!ReadFile(file, data, size, &readBytes, nullptr))
		{
			delete[] data;
			CloseHandle(file);
			data = nullptr;
			size = 0;
			return false;
		}
	}

	CloseHandle(file);
	return true;
}	