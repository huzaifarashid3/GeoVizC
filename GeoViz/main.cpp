#include "olcPixelGameEngine.h"
#include <vector>
#include <iostream>
#include <array>
#include "Point.h"
#include "Line.h"
#include "Algorithms.h"
#define all(x) x.begin(), x.end()
using namespace std;
class Game : public olc::PixelGameEngine
{
	Algorithm algo;
	vector<Line> closest_lines_display;
	vector<Line> lines;
	int state;
	int mx, my;
	olc::vi2d mouse_loc;
	int line_cnt = 0;
	Point t1, t2;
	bool partial_line = false;
	bool run_algo = true;
	long double gt = 0.0;
	float checkpoint  = 0.0;
	int k;
	int cst_p = 0;
	float dt = 0;
	Line closest_line;
	int cd;
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
				algo.points.push_back(Point(mouse_loc));
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
				algo.grahamScan();
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
			DrawClosestPair();
		}
	}

	void DrawPoints()
	{
		for (auto& p : algo.points)
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
		k = min(k, (int)algo.HullStates.size() - 1);
		auto& h = algo.HullStates[k];	
		for (int i = 0; i < (int)h.size(); i++)
		{
			olc::vi2d p1 = h[i].xy, p2 = h[(i + 1) % int(h.size())].xy;
			DrawLine(p1, p2, olc::DARK_GREEN);
		}
		
		k += Delay(0.2);
		
	}
	void DrawClosestPair()
	{
		/*for (auto& l : closest_lines_display)
			DrawLine(l.p1.xy, l.p2.xy);*/

		DrawLine(closest_line.p1.xy, closest_line.p2.xy, olc::RED);
	}

	bool Delay(float t)
	{
		if (gt > t + checkpoint)
		{
			checkpoint = gt;
			return true;
		}
		return false;
	}

	void Graham_scan()
	{
		algo.Hull.clear();
		sort(all(algo.points));
		for (int i = 0; i < 2; i++)
		{
			int s = (int)algo.Hull.size();
			for (auto& c : algo.points)
			{
				while ((int)algo.Hull.size() >= 2 + s)
				{
					Point a = algo.Hull.end()[-1];
					Point b = algo.Hull.end()[-2];
					if (a.cross(b, c) <= 0)
					{
						break;
					}
					algo.Hull.pop_back();
					algo.HullStates.push_back(algo.Hull);
				}
				algo.Hull.push_back(c);
				algo.HullStates.push_back(algo.Hull);
			}
			algo.Hull.pop_back();
			algo.HullStates.push_back(algo.Hull);
			reverse(all(algo.points));
		}
	}

	void Jarvis_march() {}

	void brute_force()
	{
		algo.Hull.clear();
		int n = algo.points.size();
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (i == j) continue;
				bool flag = true;
				for (int k = 0; k < n; k++)
				{
					if (k == i || k == j) continue;
					flag &= algo.points[i].cross(algo.points[j], algo.points[k]) > 0;

				}
				if (flag)
				{
					algo.Hull.push_back(algo.points[i]);
					algo.Hull.push_back(algo.points[j]);
					algo.HullStates.push_back(algo.Hull);
				}		
			}
		}
		
	}

	int sq(const int& a)
	{
		return a * a;
	}
	int dist(const Point& a, const Point& b)
	{
		return sq((a.x - b.x)) + sq((a.y - b.y));
	}
	void closest_pair()
	{
		sort(all(algo.points));
		auto res = recur(algo.points);
		cd = get<0>(res);
		closest_line = get<1>(res);
			
	}
	tuple<int, Line> recur(vector<Point>& points)
	{
		int n = points.size();
		if (n <= 1)
			return { INT32_MAX, Line()};
		vector<Point> left = vector<Point>(points.begin(), points.begin() + n / 2);
		vector<Point> right = vector<Point>(points.begin() + n / 2, points.end());
		auto d1 = recur(left);
		auto d2 = recur(right);
		Line l(get<1>(d1));
		int d = get<0>(d1);
		if (get<0>(d2) < get<0>(d1))
		{
			d = get<0>(d2);
			l = get<1>(d2);
		}
		int mx = left.back().x;
		vector<Point> stripe;
		for (auto i : left)
			if (d > sq(mx - i.x))
				stripe.push_back(i);
		for (auto i : right)
			if (sq(i.x - mx) < d)
				stripe.push_back(i);

		int m = stripe.size();
		sort(all(stripe), [&](const Point& a, const Point& b)
			{ return a.y < b.y; });

		for (int i = 0; i < m; i++)
		{
			for (int j = i + 1; j < m && sq(stripe[j].y - stripe[i].y) < d; j++)
			{
				closest_lines_display.push_back(Line(stripe[i], stripe[j]));
				int temp = dist(stripe[i], stripe[j]);
				if (temp < d)
				{
					d = temp;
					l = Line(stripe[i], stripe[j]);
				}
			}
			
		}
		return {d,l};
	}

};
int main(void)
{
	Game game;
	if (game.Construct(600, 600, 1, 1))
		game.Start();

	return 0;
}