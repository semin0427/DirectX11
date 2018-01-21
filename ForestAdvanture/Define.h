#pragma once

template <typename T>
void Safe_Release(T& Pointer)
{
	if (NULL != Pointer)
	{
		Pointer->Release();
		Pointer = NULL;
	}
};

template <typename T>
void Safe_Delete(T& Pointer)
{
	if (NULL != Pointer)
	{
		delete Pointer;
		Pointer = NULL;
	}
};

template <typename T>
void Safe_Delete_Array(T& Pointer)
{
	{
		if (NULL != Pointer)
		{
			delete[] Pointer;
			Pointer = NULL;
		}
	}
};