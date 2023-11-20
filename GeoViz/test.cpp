#include <iostream>
#include "olcPixelGameEngine.h"
#include "Scene.hpp"
#include "MenuScene.hpp"
#include "ConvexHullScene.hpp"



class Game : public olc::PixelGameEngine
{
	SceneManager manager;
	float ct = 0;
	float timer[4] = { 0 };

public:
	Game() {
		sAppName = "hecker";
	}
	bool OnUserCreate() override
	{
		auto menu = std::make_shared<Menu>(this, manager);
		manager.Add("menu", menu);
		auto main = std::make_shared<Main>(this, manager);
		 manager.Add("main", main);
		menu->nextState = "main";
		main->nextState = "menu";
		manager.SwitchTo("menu");
		float ct = 0;
		return true;
	}
	bool OnUserUpdate(float dt) override
	{
		input();
		update(dt);
		render();
		return true;
	}
	void input()
	{
		manager.input(); }
	void update(float dt) {
		ct += dt;
		manager.update(dt);
	}
	void render() {
		manager.render();
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


int main(void)
{
	Game game;
	if (game.Construct(600, 600, 1, 1, false, true))
		game.Start();
	return 0;

}