#pragma once
#include "olcPixelGameEngine.h"
template<class T>
class Button
{
public:
	int x, y, w, h;
	int r, l, t, b;
	olc::PixelGameEngine* pge;
	bool hover, held, press, release;
	std::function<void(T*)> callback;
	std::string label;
	T* ptr;
	Button() {}
	Button(std::string label, int x, int y, int w, int h,
		olc::PixelGameEngine* pge) :label(label), hover(false), held(false), press(false), release(false), ptr(nullptr),
		x(x), y(y), w(w), h(h), pge(pge), callback(nullptr) {
		r = x + w;
		l = x;
		t = y;
		b = y + h;
	}
	void input()
	{
		press = pge->GetMouse(0).bPressed;
		held = pge->GetMouse(0).bHeld;
		release = pge->GetMouse(0).bReleased;
		auto loc = pge->GetMousePos();
		hover = loc.x <= r && loc.x >= l && loc.y <= b && loc.y >= t;

	}
	virtual void render()
	{
		if (clicked())
			pge->FillRect(x, y, w, h, olc::RED);
		else if (hover)
			pge->FillRect(x, y, w, h, olc::GREY);
		else
			pge->FillRect(x, y, w, h, olc::WHITE);
		pge->DrawString({ x + (r-l) / 8,y + (b-t) / 8 }, label, olc::DARK_CYAN);
	}
	void update()
	{
		onClick();
	}
	bool clicked() const
	{
		return hover && held;
	}
	bool pressed() const
	{
		return hover && press;
	}
	bool released() const
	{
		return hover && release;
	}
	bool hovered()
	{
		return hover;
	}
	// event handler
	void setCallBack(T* ptr, std::function<void(T*)> callback)
	{
		this->ptr = ptr;
		this->callback = callback;
	}
	void onClick()
	{
		if (pressed() && callback != nullptr)
		{
			callback(ptr);
		}
	};
};

template <class T>
class Frame  {

public:
	int x, y, w, h;
	int r, l, t, b;
	olc::PixelGameEngine* pge;
	bool hover, held, press, release;
	std::function<void(T*)> callback;
	T* ptr;
	Frame() {}
	Frame(int x, int y, int w, int h,
		olc::PixelGameEngine* pge) : hover(false), held(false), press(false), release(false), ptr(nullptr),
		x(x), y(y), w(w), h(h), pge(pge), callback(nullptr) {
		r = x + w;
		l = x;
		t = y;
		b = y + h;
	}
	void input()
	{
		press = pge->GetMouse(0).bPressed;
		held = pge->GetMouse(0).bHeld;
		release = pge->GetMouse(0).bReleased;
		auto loc = pge->GetMousePos();
		hover = loc.x <= r && loc.x >= l && loc.y <= b && loc.y >= t;

	}
	virtual void render()
	{
		pge->FillRect(x, y, w, h, olc::DARK_BLUE);
		if (hover)
			pge->FillCircle(pge->GetMousePos(),15, olc::YELLOW);
	}
	void update()
	{
		onClick();
	}
	bool clicked() const
	{
		return hover && held;
	}
	bool pressed() const
	{
		return hover && press;
	}
	bool released() const
	{
		return hover && release;
	}
	bool hovered()
	{
		return hover;
	}
	// event handler
	void setCallBack(T* ptr, std::function<void(T*)> callback)
	{
		this->ptr = ptr;
		this->callback = callback;
	}
	void onClick()
	{
		if (pressed() && callback != nullptr)
		{
			callback(ptr);
		}
	};
};
