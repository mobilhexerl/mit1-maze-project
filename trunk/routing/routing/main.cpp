#include <iostream>
#include <ctime>
#include "planner.h"

#define MAZEHEIGHT 25
#define MAZEWIDTH  50
#define DIRECTIONS  4
#define MAZEDENSITY 0.40
#define WALLSTOREMOVE 40
#define GOALS       4
#define AGENTS      2

int main(){

	int startx, starty, goalx, goaly, tmpstartx, tmpstarty, oldgoaly, oldgoalx, primestartx, primestarty;
	int mapx, mapy, newx, newy;
	int wallstoremove = WALLSTOREMOVE;

	int agent_startx, agent_starty;

	int *dyn_goalx = 0;

	dyn_goalx = new int[GOALS];

	int *dyn_goaly = 0;
	dyn_goaly = new int[GOALS];

	int *dyn_agentx = 0;
	dyn_agentx= new int[AGENTS];

	int *dyn_agenty = 0;
	dyn_agenty = new int[AGENTS];



	list<Planner> goals;
	std::list<Planner>::iterator it;
	list<Map::Cell *> path;	
	list<Map::Cell *> lastpath;
	list<Map::Cell *> agentpath;

	Map *map = new Map(MAZEWIDTH, MAZEHEIGHT);
	//(*map).
	//more readable 
	//map->operator()
	for (int i = 0; i < MAZEWIDTH; ++i) {
		for (int j = 0; j < MAZEHEIGHT; ++j) {
			(*map)(i, j)->cost = 1;
		}
	}

	//NEEDED MORE RANDOMIZED OBSTACLES

	while (wallstoremove)
	{
		newx = rand() % MAZEWIDTH;
		newy = rand() % MAZEHEIGHT;
		(*map)(newx, newy)->cost = Map::Cell::COST_UNWALKABLE;
		--wallstoremove;
	}

	for (int x = 0; x < MAZEWIDTH + 2; ++x)
		std::cout << "X";
	std::cout << std::endl;
	for (int y = 0; y < MAZEHEIGHT; ++y) {
		std::cout << "X";
		for (int x = 0; x < MAZEWIDTH; ++x){
			if ((*map)(x, y)->cost == Map::Cell::COST_UNWALKABLE)
				std::cout << "X";
			else
				std::cout << " ";

		}
		std::cout << "X" << std::endl;

	}
	for (int x = 0; x < MAZEWIDTH + 2; ++x)
		std::cout << "X";
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	

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
			dyn_goalx[i] = startx;
			dyn_goaly[i] = starty;
		}
		else {
			tmpstartx = oldgoalx;
			tmpstarty = oldgoaly;
		}


		Planner p(map, (*map) (tmpstartx, tmpstarty), (*map) (goalx, goaly));
		oldgoalx = goalx;
		oldgoaly = goaly;	

		
		dyn_goalx[i+1] = goalx;
		dyn_goaly[i+1] = goaly;

		p.replan();
		goals.push_back(p);
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
	p.replan();
	lastpath = p.path();

	std::cout << "Last Path" << std::endl;
	std::cout << "Start x: " << oldgoalx << std::endl;
	std::cout << "Start y: " << oldgoaly << std::endl;
	std::cout << "Goal x: " << primestartx << std::endl;
	std::cout << "Goal y: " << primestarty << std::endl;

	while (!lastpath.empty()) {
		int r, c;
		r = lastpath.front()->x();
		c = lastpath.front()->y();
		lastpath.pop_front();
		std::cout << r << ":" << c << std::endl;
	}

	for (int i = 0; i < AGENTS; i++) {
		goaly = (rand() % ((MAZEHEIGHT + 1) / 2)) * 2;
		goalx = (rand() % ((MAZEWIDTH + 1) / 2)) * 2;
		while (1)
		{
			starty = (rand() % ((MAZEHEIGHT + 1) / 2)) * 2;
			startx = (rand() % ((MAZEWIDTH + 1) / 2)) * 2;
			if (startx != goalx || starty != goaly)
				break;
		}

		agent_startx = startx;
		agent_starty = starty;

		dyn_agentx[i] = startx;
		dyn_agenty[i] = starty;

		std::cout << "Agent" << i << std::endl;
		std::cout << "Start x: " << dyn_agentx[i] << std::endl;
		std::cout << "Start y: " << dyn_agenty[i] << std::endl;
		std::cout << "First Goal x: " << dyn_goalx[i] << std::endl;
		std::cout << "First Goal y: " << dyn_goaly[i] << std::endl;

		//ACTUALLY UNNECESSARY AS WE ALREADY HAVE STORED THE SHORTEST PATH IN GOALS LIST
		//SO WE NEED ONLY THE PATH TO FIRST GOAL PER AGENT

		for (int j = 0; j < GOALS; j++) {
			if (j == 0)
				Planner p(map, (*map) (startx, starty), (*map) (dyn_goalx[j], dyn_goaly[j]));
			else
				Planner p(map, (*map) (dyn_goalx[j - 1], dyn_goaly[j-1]), (*map) (dyn_goalx[j], dyn_goaly[j]));
			p.replan();
			agentpath = p.path();
		}
		
		while (!agentpath.empty()) {
			int r, c;
			r = agentpath.front()->x();
			c = agentpath.front()->y();
			agentpath.pop_front();
			std::cout << r << ":" << c << std::endl;
		}


	}

	
	

	//Planner p(map, (*map) (1, 1), (*map) (10, 15));	
	//p.replan();	
	//list<Map::Cell *> path = p.path();	

	

	return 0;
}