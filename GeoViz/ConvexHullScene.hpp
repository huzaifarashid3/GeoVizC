#pragma once
#include "Scene.hpp"
#include "Button.hpp"
#include "Algorithms.h"


void foo() {}

class Main : public Scene
{
public:
	Algorithm algo;
	int button_size = 50;
	bool animate = false;
	int cntr = 0;
	SceneManager& manager;
	olc::PixelGameEngine* pge;
	double ct = 0;
	double timer[4] = { 0 };
	std::unordered_map<std::string, Button<Main>> buttons;
	std::unordered_map<std::string, Frame<Main>> frames;
	std::string nextState;

public:
	Main(olc::PixelGameEngine* pge,SceneManager& manager): pge(pge), manager(manager) {}
	void OnCreate() {
		buttons["back"] =  Button<Main>("back", 20, 20,button_size ,button_size , pge);
		auto temp = [](Main* m) { m->switchState(); };
		buttons["back"].setCallBack(this, temp);

		buttons["graham"] = Button <Main> ("graham", 100, 20, button_size + 20, button_size, pge);
		auto temp1 = [](Main* m) { m->algo.grahamScan();  m->animate = false;  };
		buttons["graham"].setCallBack(this, temp1);

		buttons["brute"] = Button <Main> ("brute", 200, 20, button_size, button_size, pge);
		auto temp3 = [](Main* m) { m->algo.brute_force();  m->animate = false;  };
		buttons["brute"].setCallBack(this, temp3);

		buttons["animate"] = Button <Main>("animate", 400, 20, button_size+ 20, button_size, pge);
		auto temp2 = [](Main* m) { m->cntr = 0;  m->animate = true; };
		buttons["animate"].setCallBack(this, temp2);
		
		buttons["clear"] = Button <Main>("clear", 300, 20, button_size+ 20, button_size, pge);
		auto temp4 = [](Main* m) { m->algo.points.clear(); m->animate = false; };
		buttons["clear"].setCallBack(this, temp4);

		frames["draw"] = Frame<Main>(100, 100, 400, 400, pge);
	}
	void switchState()
	{
		manager.SwitchTo(nextState);
	}
	void OnDestroy() {}
	void OnActivate() { std::cout << "Main" << std::endl; }
	void input()
	{
		for (auto& f : frames)
		{
			f.second.input();
			if (f.second.pressed())
			{
				inputPoints();
			}
		}
		for (auto& b : buttons)
			b.second.input();
	}
	void inputPoints()
	{
		// if within bouding box
		auto mouse_loc = pge->GetMousePos();
		algo.points.push_back(Point(mouse_loc));

	}
	void update(float dt)
	{
		ct += dt;
		for (auto& b : buttons)
			b.second.update();
		for (auto& f : frames)
				f.second.update();
	}
	void render()
	{
		pge->Clear(olc::BLACK);
		for (auto& b : buttons)
			b.second.render();
		for (auto& f : frames)
			f.second.render();
		showPoints();

		// convex hull animation
		if (animate)
		{
			DrawHull();
		}
	}

	void DrawHull()
	{
		cntr = std::min(cntr, (int)algo.HullStates.size() - 1);
		auto& h = algo.HullStates[cntr];	
		for (int i = 0; i < (int)h.size(); i++)
		{
			olc::vi2d p1 = h[i].xy, p2 = h[(i + 1) % int(h.size())].xy;
			pge->DrawLine(p1, p2, olc::DARK_GREEN);
		}
		
		cntr += delay(0.1, 0);
	}

	void Animate(int k)
	{
		if (k >= algo.HullStates.size() || k < 0) return;

		auto& h = algo.HullStates[k];	
		for (int i = 0; i < (int)h.size(); i++)
		{
			olc::vi2d p1 = h[i].xy, p2 = h[(i + 1) % int(h.size())].xy;
			pge->DrawLine(p1, p2, olc::DARK_GREEN);
		}
			
	}
	void showPoints()
	{
		for (auto& p :algo.points)
		{
			pge->FillCircle(p.xy, 5, olc::MAGENTA);
		}
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
