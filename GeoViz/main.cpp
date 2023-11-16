#define OLC_PGE_APPLICATION
#include <iostream>
#include <array>
#include <vector>
#include "olcPixelGameEngine.h"
#include "Point.h"
#include "Line.h"
#define all(x) x.begin(), x.end()
using namespace std;

class Game : public olc::PixelGameEngine
{
	vector<Point> points;
	vector<Point> Hull;
	vector<vector<Point>> hull_display;
	vector<Line> lines;
	int state;
	int mx, my;
	olc::vi2d mouse_loc;
	int line_cnt = 0;
	Point t1, t2;
	bool partial_line = false;
	bool run_algo = true;
	long double gt = 0.0;
	float delay  = 0.0;
	int k;
	float dt = 0;
public:

	Game()
	{
		sAppName = "Game";
	}
	bool OnUserCreate() override
	{
		state = 0;
		return true;
	}

	bool OnUserUpdate(float dt) override
	{
		gt += dt;
		this->dt = dt;
		input();
		update();
		render();
		return true;
	}


	void input() {

		mouse_loc = GetMousePos();
		mx = mouse_loc.x, my = mouse_loc.y;
		

		if (GetKey(olc::Key::Q).bPressed)
			state--;
		if (GetKey(olc::Key::E).bPressed)
			state++;

		state = max(state, 0);
		state = min(state, 2);
		
		if (state != 1) line_cnt = 0;
		if (state != 2) run_algo = true;
			
		if (state == 0)
		{
			if (GetMouse(0).bPressed)
			{
				points.push_back(Point(mouse_loc));
			}
		}
		if (state == 1)
		{
			if (GetMouse(0).bPressed)
			{
				if (line_cnt == 0)
				{
					t1.xy = mouse_loc;
					partial_line = true;
					line_cnt = 1;
				}
				else
				{
					t2.xy = mouse_loc;
					lines.push_back(Line(Point(t1), Point(t2)));
					line_cnt = 0;
				}
			}
		}
	}
	void update() {
		if (state == 2)
		{
			if (run_algo)
			{
				//Graham_scan();
				brute_force();
				run_algo = false;
			}	
		}
	}
	void render() {
		Clear(olc::DARK_BLUE);
		FillCircle(mouse_loc, 10, olc::YELLOW);
		if (state == 0)
		{
			DrawPoints();
			DrawLines();
		}
		else if (state == 1)
		{
			DrawPoints();
			DrawLines();
			if (line_cnt == 1)
				DrawPartialLine();
			
		}
		else if (state == 2)
		{
			DrawPoints();
			DrawLines();

			DrawHull();
		}
	}

	void DrawPoints()
	{
		for (auto& p : points)
		{
			FillCircle(p.xy, 5);
		}
	}
	void DrawLines()
	{
		for (auto& l : lines)
		{
			FillCircle(l.p1.xy, 5);
			FillCircle(l.p2.xy, 5);
			DrawLine(l.p1.xy, l.p2.xy, olc::BLACK);
		}
	}
	void DrawPartialLine()
	{

		FillCircle(t1.xy, 5);
		DrawLine(t1.xy, mouse_loc, olc::YELLOW);
	}
	void DrawHull()
	{
		k = min(k, (int)hull_display.size() - 1);
		auto& h = hull_display[k];	
		for (int i = 0; i < (int)h.size(); i++)
		{
			olc::vi2d p1 = h[i].xy, p2 = h[(i + 1) % int(h.size())].xy;
			DrawLine(p1, p2, olc::DARK_GREEN);
		}
		if (gt >= delay)
		{
			delay = gt + 0.2;
			k++;
		}
	}


	void Graham_scan()
	{
		Hull.clear();
		sort(all(points));
		for (int i = 0; i < 2; i++)
		{
			int s = (int)Hull.size();
			for (auto& c : points)
			{
				while ((int)Hull.size() >= 2 + s)
				{
					Point a = Hull.end()[-1];
					Point b = Hull.end()[-2];
					if (a.cross(b, c) <= 0)
					{
						break;
					}
					Hull.pop_back();
					hull_display.push_back(Hull);
				}
				Hull.push_back(c);
				hull_display.push_back(Hull);
			}
			Hull.pop_back();
			hull_display.push_back(Hull);
			reverse(all(points));
		}
	}

	void Jarvis_march() {}

	void brute_force()
	{
		Hull.clear();
		int n = points.size();
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (i == j) continue;
				bool flag = true;
				for (int k = 0; k < n; k++)
				{
					if (k == i || k == j) continue;
					flag &= points[i].cross(points[j], points[k]) > 0;

				}
				if (flag)
				{
					Hull.push_back(points[i]);
					Hull.push_back(points[j]);
					hull_display.push_back(Hull);
				}		
			}
		}
		
	}

};

int main(void)
{
	Game game;
	if (game.Construct(600, 600, 1, 1))
		game.Start();

	return 0;

}