#pragma once
#define all(x) x.begin(), x.end()
#include <iostream>
#include "Point.h"
class Algorithm
{
public:
	std::vector<Point> points;
	std::vector<Point> Hull;
	std::vector<std::vector<Point>> HullStates;
	Algorithm() {}
	void grahamScan()
	{
		Hull.clear();
		HullStates.clear();
		std::cout << "running Graham Scan" << std::endl;
		std::sort(all(points));
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
					HullStates.push_back(Hull);
				}
				Hull.push_back(c);
				HullStates.push_back(Hull);
			}
			Hull.pop_back();
			HullStates.push_back(Hull);
			std::reverse(all(points));
		}
		for (auto& i : points)
			std::cout << i.xy << " ";
		std::cout << std::endl;
	}
	void brute_force()
	{
		Hull.clear();
		HullStates.clear();
		std::cout << "running brute force" << std::endl;
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
					HullStates.push_back(Hull);
				}		
			}
		}
		
	}

};

