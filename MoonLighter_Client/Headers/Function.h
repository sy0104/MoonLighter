#pragma once

template <class T>
void Safe_Delete(T& rObj)
{
	if (rObj)
	{
		delete rObj;
		rObj = nullptr;
	}
}

template <class T>
void Safe_DeleteArray(T& rObj)
{
	if (rObj)
	{
		delete[] rObj;
		rObj = nullptr;
	}
}

// 참조를 지워줌
template <class T>
void Safe_Release(T& rObj)
{
	if (rObj)
		rObj->Release();
}