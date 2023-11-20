#pragma once
#include <unordered_map>

class Scene
{
public:
	Scene() {}
	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnActivate() {}
	virtual void OnDeactivate() {}
	virtual void input() {};
	virtual void update(float dt) {};
	virtual void render() {};
};

class SceneManager
{
	std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
	std::shared_ptr<Scene> curr_scene;
	int id = 0;
public:
	SceneManager(): id(0),  scenes(0), curr_scene(nullptr) {}

	void input() {
		if (curr_scene)
			curr_scene->input();
	}
	void update(float dt) {
		if (curr_scene)
			curr_scene->update(dt);
	}
	void render() {
		if (curr_scene)
			curr_scene->render();
	}
	bool Add(std::string name, std::shared_ptr<Scene> scene) {
		if (scenes.count(name))
		{
			std::cout << "name already used" << std::endl;
			return false;
		}
		auto ins = scenes.insert({ name, scene });
		scenes[name] = scene;
		scenes[name]->OnCreate();
		return true;
	}
	void Remove(std::string name) {
		auto it = scenes.find(name);
		if (it != scenes.end())
		{
			if (curr_scene == it->second)
			{
				curr_scene = nullptr;
			}
			it->second->OnDestroy();
			scenes.erase(it);
		}
	}
	void SwitchTo(std::string name) 
	{
		auto it = scenes.find(name);
		if (it != scenes.end())
		{
			if (curr_scene)
				curr_scene->OnDeactivate();
			curr_scene = scenes[name];
			curr_scene->OnActivate();
		}
	}

};