#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <functional>
#include <exception>
#include <memory>
#include <string>
#include <vector>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define SMART_PTR(T) public: using P = std::shared_ptr<T>; using W = std::weak_ptr<T>; using U = std::unique_ptr<T>; private:
#define NO_COPY(T) public: T(T&) = delete; T(T&&) = delete; T& operator=(T&) = delete; private:

template <typename T>
class AutoReleasePtr
{
	T* m_ptr;

public:
	AutoReleasePtr() :m_ptr(nullptr) {}
	AutoReleasePtr(const AutoReleasePtr& other) :
		m_ptr(other.m_ptr)
	{
		if (m_ptr)
			m_ptr->AddRef();
	}
	AutoReleasePtr(AutoReleasePtr&& other) :
		m_ptr(other.m_ptr)
	{
		other.m_ptr = nullptr;
	}
	~AutoReleasePtr()
	{
		Release();
	}
	AutoReleasePtr& operator=(const AutoReleasePtr& other)
	{
		Release();
		m_ptr = other.m_ptr;
		if (m_ptr)
			m_ptr->AddRef();
	}
	void Release()
	{
		if (m_ptr)
		{
			m_ptr->Release();
			m_ptr = nullptr;
		}
	}
	operator bool() const
	{
		return m_ptr != nullptr;
	}
	bool operator!() const
	{
		return m_ptr == nullptr;
	}
	bool operator==(AutoReleasePtr& other) const
	{
		return m_ptr == other.m_ptr;
	}
	bool operator!=(AutoReleasePtr& other) const
	{
		return m_ptr != other.m_ptr;
	}
	bool operator==(T *ptr) const
	{
		return m_ptr == ptr;
	}
	bool operator!=(T *ptr) const
	{
		return m_ptr != ptr;
	}
	operator T*() const
	{
		return m_ptr;
	}
	T** operator&()
	{
		return &m_ptr;
	}
	T* operator->() const
	{
		return m_ptr;
	}
};

std::wstring ToWStr(const char *str);
std::string ToStr(const wchar_t *str);

void SetExeFolderName(HINSTANCE hInstance);

std::wstring ResolveFilename(std::wstring filename);
std::wstring GetFileExtension(LPCWSTR filename);