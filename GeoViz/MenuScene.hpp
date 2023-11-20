#pragma once
#include "olcPixelGameEngine.h"
#include "Scene.hpp"
#include "Button.hpp"
class Menu : public Scene
{
	SceneManager& manager;
	olc::PixelGameEngine* pge;
	float ct = 0;
	float timer[4] = { 0 };
	int y = 10;
	std::unordered_map<std::string, Button<Menu>> buttons;
public:
	std::string nextState;
	Menu(olc::PixelGameEngine* pge,SceneManager& manager): pge(pge), manager(manager) {}
	void OnCreate() {
		buttons["cnvx"] = Button<Menu>("Convex Hull", 100, 200, 100, 80, pge);
		auto temp = [](Menu* m) { m->switchState("main"); };
		buttons["cnvx"].setCallBack(this, temp);
	}
	void switchState(std::string nxt)
	{
		manager.SwitchTo(nxt);
	}
	void OnDestroy() {}
	void OnActivate() { std::cout << "Menu" << std::endl; }
	void input()
	{
		for (auto& b : buttons)
			b.second.input();
	}
	void update(float dt)
	{
		ct += dt;
		for (auto& b : buttons)
			b.second.update();

	}
	void render() {
		pge->Clear(olc::DARK_BLUE);
		pge->DrawString(pge->ScreenWidth() / 4, pge->ScreenHeight() / 6, "GeoViz",olc::WHITE, 8U);
		for (auto& b : buttons)
			b.second.render();
	}
	bool delay(float d, int i)
	{
		if (ct > timer[i] + d)
		{
			timer[i] = ct + d;
			return true;
		}
		return false;
	}
};



// i want my buttons it switch states // done
// i want my buttons to run algorithms 
// i want my buttons to choose mode of input

