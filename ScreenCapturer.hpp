#pragma once
#include <memory>
#include <stdexcept>
#include <Windows.h>

class AutoCloseDCHandle final
{
public:
	explicit AutoCloseDCHandle(HDC hdc) :
		m_dc_handle(hdc)
	{}
	virtual ~AutoCloseDCHandle()
	{
		try
		{
			ReleaseDC(nullptr, m_dc_handle);
		}
		catch (...) {}
	};

	AutoCloseDCHandle(const AutoCloseDCHandle&) = delete;
	AutoCloseDCHandle& operator=(AutoCloseDCHandle) = delete;

	AutoCloseDCHandle(AutoCloseDCHandle&&) = delete;
	AutoCloseDCHandle& operator=(AutoCloseDCHandle&&) = delete;

	HDC get() const { return m_dc_handle; }

private:
	HDC m_dc_handle;
};

class Bitmap final
{
public:
	explicit Bitmap(HDC screen, int width, int height) :
		m_bitmap(_s_create_bitmap(screen, width, height))
	{}

	explicit Bitmap(HBITMAP bitmap) :
		m_bitmap(bitmap)
	{}
	virtual ~Bitmap()
	{
		try
		{
			DeleteObject(m_bitmap);
		}
		catch (...) {}
	};

	Bitmap(const Bitmap&) = delete;
	Bitmap& operator=(Bitmap) = delete;

	Bitmap(Bitmap&&) = delete;
	Bitmap& operator=(Bitmap&&) = delete;

	HBITMAP get() const { return m_bitmap; }


private:

	static HBITMAP _s_create_bitmap(HDC screen, int width, int height)
	{
				const auto bitmap = CreateCompatibleBitmap(screen, width, height);
		if (nullptr == bitmap)
		{
						throw std::runtime_error("Failed to create bitmap");
		}

		return bitmap;
	}

	HBITMAP m_bitmap;

};

using BitmapPtr = std::shared_ptr<Bitmap>;

class ScreenCapturer final
{
public:
	ScreenCapturer() :
		m_width(GetSystemMetrics(SM_CXSCREEN)),
		m_height(GetSystemMetrics(SM_CYSCREEN)),
		m_screen(GetDC(nullptr)),
		m_memory(CreateCompatibleDC(m_screen.get()))
	{}
	virtual ~ScreenCapturer() = default;

	ScreenCapturer(const ScreenCapturer&) = delete;
	ScreenCapturer& operator=(ScreenCapturer) = delete;

	ScreenCapturer(ScreenCapturer&&) = delete;
	ScreenCapturer& operator=(ScreenCapturer&&) = delete;

	BitmapPtr capture()
	{
		auto bitmap = std::make_shared<Bitmap>(m_screen.get(), m_width,m_height);
		SelectObject(m_memory.get(), bitmap.get());
		BitBlt(m_memory.get(), 0, 0, m_width, m_height, m_screen.get(), 0, 0, SRCCOPY);

		return bitmap;
	}

private:
	int m_width;
	int m_height;
	AutoCloseDCHandle m_screen;
	AutoCloseDCHandle m_memory;
};
