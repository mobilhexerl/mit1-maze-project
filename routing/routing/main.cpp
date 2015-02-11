#include <iostream>
#include <ctime>
#include "planner.h"

#define MAZEHEIGHT 25
#define MAZEWIDTH  25
#define GOALS       4
#define AGENTS      2

int main(){

	int startx, starty, goalx, goaly, tmpstartx, tmpstarty, oldgoaly, oldgoalx, primestartx, primestarty;

	list<Planner> agent;
	std::list<Planner>::iterator it;
	list<Map::Cell *> path;	

	Map *map = new Map(MAZEWIDTH, MAZEHEIGHT);
	//(*map).
	//more readable 
	//map->operator()
	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 20; ++j) {
			(*map)(i, j)->cost = 1;
		}
	}

	(*map)(1, 5)->cost = Map::Cell::COST_UNWALKABLE;

	std::srand(std::time(0));

	goaly = (rand() % ((MAZEHEIGHT + 1) / 2)) * 2;
	goalx = (rand() % ((MAZEWIDTH + 1) / 2)) * 2;

	for (int i = 0; i < GOALS; i++) {	
		goaly = (rand() % ((MAZEHEIGHT + 1) / 2)) * 2;
		goalx = (rand() % ((MAZEWIDTH + 1) / 2)) * 2;

		while (1)
		{
			starty = (rand() % ((MAZEHEIGHT + 1) / 2)) * 2;
			startx = (rand() % ((MAZEWIDTH + 1) / 2)) * 2;
			if (startx != goalx || starty != goaly)
				break;
		}
		if (i == 0) {
			tmpstartx = startx;
			tmpstarty = starty;
			primestartx = startx;
			primestarty = starty;
		}
		else {
			tmpstartx = oldgoalx;
			tmpstarty = oldgoaly;
		}

		Planner p(map, (*map) (tmpstartx, tmpstarty), (*map) (goalx, goaly));
		oldgoalx = goalx;
		oldgoaly = goaly;
		agent.push_back(p);	
		p.replan();
		path = p.path();
		std::cout <<  "Path" << i << std::endl;
		std::cout << "Start x: " << tmpstartx << std::endl;
		std::cout << "Start y: " << tmpstarty << std::endl;
		std::cout << "Goal x: " << goalx << std::endl;
		std::cout << "Goal y: " << goaly << std::endl;
		while (!path.empty()) {
			int r, c;
			r = path.front()->x();
			c = path.front()->y();
			path.pop_front();
			std::cout << r << ":" << c << std::endl;
		}
	}

	Planner p(map, (*map) (oldgoalx, oldgoaly), (*map) (primestartx, primestarty));

	std::cout << "Last Path" << std::endl;
	std::cout << "Start x: " << oldgoalx << std::endl;
	std::cout << "Start y: " << oldgoaly << std::endl;
	std::cout << "Goal x: " << primestartx << std::endl;
	std::cout << "Goal y: " << primestarty << std::endl;

	while (!path.empty()) {
		int r, c;
		r = path.front()->x();
		c = path.front()->y();
		path.pop_front();
		std::cout << r << ":" << c << std::endl;
	}

	
	

	//Planner p(map, (*map) (1, 1), (*map) (10, 15));	
	//p.replan();	
	//list<Map::Cell *> path = p.path();	

	

	return 0;
}