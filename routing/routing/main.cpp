/* ISO/IEC 9899:1990 7.12.1: <time.h>
The macro `CLOCKS_PER_SEC' is the number per second of the value
returned by the `clock' function. */
/* CAE XSH, Issue 4, Version 2: <time.h>
The value of CLOCKS_PER_SEC is required to be 1 million on all
XSI-conformant systems. */
//#  define CLOCKS_PER_SEC  1000000l
//#define CLOCKS_PER_SEC  ((clock_t)1000)

#include <iostream>
#include <ctime>
#include "planner.h"

#define MAZEHEIGHT 25
#define MAZEWIDTH  50
#define DIRECTIONS  4
#define MAZEDENSITY 0.40
#define WALLSTOREMOVE 40
#define GOALS       4
#define AGENTS      4
//#define DISPLAY
#define COLLISION
#define DEBUG

void printStartMaze(Map *map) {
	for (int x = 0; x < MAZEWIDTH + 2; x++)
		std::cout << "X";
	std::cout << std::endl;
	for (int y = 0; y < MAZEHEIGHT; y++) {
		std::cout << "X";
		for (int x = 0; x < MAZEWIDTH; x++){
			if ((*map)(x, y)->cost == Map::Cell::COST_UNWALKABLE)
				std::cout << "X";
			else if ((*map)(x, y)->cost == 2)
				std::cout << "Y";
			else
				std::cout << " ";

		}
		std::cout << "X" << std::endl;

	}
	for (int x = 0; x < MAZEWIDTH + 2; x++)
		std::cout << "X";
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

}

void printDynMaze(int dyn_maze[][MAZEHEIGHT], int dim1){
	for (int x = 0; x < MAZEWIDTH + 2; ++x)
		std::cout << "X";
	std::cout << std::endl;
	for (int y = 0; y < MAZEHEIGHT; ++y) {
		std::cout << "X";
		for (int x = 0; x < MAZEWIDTH; ++x){
			if (dyn_maze[x][y] == 2)
				std::cout << "X";
			else if (dyn_maze[x][y] == 3)
				std::cout << "o";
			else if (dyn_maze[x][y] == 4)
				std::cout << "G";
			else if (dyn_maze[x][y] == 5)
				std::cout << "S";
			else if (dyn_maze[x][y] == 6)
				std::cout << "A";
			else if (dyn_maze[x][y] == 7)
				std::cout << "a";
			else if (dyn_maze[x][y] == 8)
				std::cout << "b";
			else if (dyn_maze[x][y] == 9)
				std::cout << "m";
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
	//end print dyn_maze
}



int main(){

	int startx, starty, goalx, goaly, tmpstartx, tmpstarty, oldgoaly, oldgoalx, primestartx, primestarty;
	int newx, newy;
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

	int *dyn_pathy = 0;
	dyn_pathy = new int[1000];

	int *dyn_pathx = 0;
	dyn_pathx = new int[1000];

	clock_t zeit1, zeit2;

	zeit1 = clock();

#ifdef COLLISION
	int agent_path[AGENTS][MAZEWIDTH][MAZEHEIGHT];
	for (int z = 0; z < AGENTS; z++)
		for (int i = 0; i < MAZEWIDTH; i++)
			for (int j = 0; j < MAZEHEIGHT; j++)
				agent_path[z][i][j] = 1;
#endif	

	int dyn_maze[MAZEWIDTH][MAZEHEIGHT];
	for (int i = 0; i < MAZEWIDTH; i++)
		for (int j = 0; j < MAZEHEIGHT; j++)
			dyn_maze[i][j] = 1;


	list<Planner> goals;
	std::list<Planner>::iterator it;
	list<Map::Cell *> path;	
	list<Map::Cell *> lastpath;
	list<Map::Cell *> agentpath;
	list<Map::Cell *> tmppath;

	Map *map = new Map(MAZEWIDTH, MAZEHEIGHT);
	//(*map).
	//more readable 
	//map->operator()
	for (int i = 0; i < MAZEWIDTH; ++i) {
		for (int j = 0; j < MAZEHEIGHT; ++j) {
			(*map)(i, j)->cost = 1;
		}
	}

	//print test
	(*map)(10, 10)->cost = 2;

	

	//NEEDED MORE RANDOMIZED OBSTACLES

	while (wallstoremove)
	{
		newx = rand() % MAZEWIDTH;
		newy = rand() % MAZEHEIGHT;
		(*map)(newx, newy)->cost = Map::Cell::COST_UNWALKABLE;
		dyn_maze[newx][newy] = 2;
		--wallstoremove;
	}

	//print test start maze
#ifdef DISPLAY
	printStartMaze(map);

	printDynMaze(dyn_maze, MAZEHEIGHT);
#endif
	
	//generate random goals and random starts
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
		// save starting coordinates as goals for last run
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

		//constructs map with initial start and initial goal
		Planner p(map, (*map) (tmpstartx, tmpstarty), (*map) (goalx, goaly));

		//saves goal as next start
		oldgoalx = goalx;
		oldgoaly = goaly;	

		//saves goal in array
		dyn_goalx[i+1] = goalx;
		dyn_goaly[i+1] = goaly;
		
		//checks if solution is found
		bool result = false;
		result = p.replan();

		//saves goals in goal list
		goals.push_back(p);
		//gets the solution path
		path = p.path();
#ifdef DISPLAY
		std::cout <<  "Path " << i << std::endl;		
		std::cout << "Result " << result << std::endl;
		std::cout << "Start x: " << tmpstartx << std::endl;
		std::cout << "Start y: " << tmpstarty << std::endl;
		std::cout << "Goal x: " << goalx << std::endl;
		std::cout << "Goal y: " << goaly << std::endl;
#endif
		//dyn_maze[tmpstartx][tmpstarty] = 5;
		//dyn_maze[goalx][goaly] = 4;

		dyn_maze[tmpstarty][tmpstartx] = 5;
		dyn_maze[goaly][goalx] = 4;	
#ifdef DISPLAY
		printDynMaze(dyn_maze, MAZEHEIGHT);
#endif
		int m = 0;
		int m_amount = 0;
		while (!path.empty()) {
			int r, c;
			r = path.front()->x();
			c = path.front()->y();

			dyn_pathx[m] = r;
			dyn_pathy[m] = c;
			dyn_maze[r][c] = 3;
			m++;
			m_amount++;
			path.pop_front();
#ifdef DISPLAY
			std::cout << r << ":" << c << std::endl;
#endif			
		}

		dyn_maze[tmpstarty][tmpstartx] = 5;
		dyn_maze[goaly][goalx] = 4;		
#ifdef DISPLAY
		std::cout << "mylist stores " << goals.size() << " paths.\n";

		printDynMaze(dyn_maze, MAZEHEIGHT);
#endif
	}

	Planner p(map, (*map) (oldgoalx, oldgoaly), (*map) (primestartx, primestarty));
	
	p.replan();
	goals.push_back(p);
	lastpath = p.path();

#ifdef DISPLAY
	std::cout << "Last Path" << std::endl;
	std::cout << "Start x: " << oldgoalx << std::endl;
	std::cout << "Start y: " << oldgoaly << std::endl;
	std::cout << "Goal x: " << primestartx << std::endl;
	std::cout << "Goal y: " << primestarty << std::endl;
#endif

	while (!lastpath.empty()) {
		int r, c;
		r = lastpath.front()->x();
		c = lastpath.front()->y();
		dyn_maze[r][c] = 3;
//		(*map)(r, c)->cost = 2;
		lastpath.pop_front();
#ifdef DISPLAY
		std::cout << r << ":" << c << std::endl;
#endif
	}

	dyn_maze[oldgoaly][oldgoalx] = 5;
	dyn_maze[primestarty][primestartx] = 4;
#ifdef DISPLAY
	std::cout << "mylist stores " << goals.size() << " paths.\n";
	std::cout << "Shortest Path between goals" << std::endl;

	//print out shortest path between goals
	printDynMaze(dyn_maze, MAZEHEIGHT);
#endif	
	//create random agents and set their initial goal

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

		
#ifdef DISPLAY
		std::cout << "Agent" << i << std::endl;
		std::cout << "Start x: " << dyn_agentx[i] << std::endl;
		std::cout << "Start y: " << dyn_agenty[i] << std::endl;
		std::cout << "First Goal x: " << dyn_goalx[i] << std::endl;
		std::cout << "First Goal y: " << dyn_goaly[i] << std::endl;
#endif
		//ACTUALLY UNNECESSARY AS WE ALREADY HAVE STORED THE SHORTEST PATH IN GOALS LIST
		//CAN BE USED FOR COLLISION AVOIDANCE
		//SO WE NEED ONLY THE PATH TO FIRST GOAL PER AGENT

		/*for (int j = 0; j < GOALS; j++) {
			if (j == 0)
				Planner p(map, (*map) (startx, starty), (*map) (dyn_goalx[j], dyn_goaly[j]));
			else
				Planner p(map, (*map) (dyn_goalx[j - 1], dyn_goaly[j-1]), (*map) (dyn_goalx[j], dyn_goaly[j]));
			p.replan();
			agentpath = p.path();
		}*/

		Planner p(map, (*map) (startx, starty), (*map) (dyn_goalx[i], dyn_goaly[i]));
		p.replan();
		agentpath = p.path();
		
		while (!agentpath.empty()) {
			int r, c;
			r = agentpath.front()->x();
			c = agentpath.front()->y();
			dyn_maze[r][c] = 7;
#ifdef COLLISION
			agent_path[i][r][c] = 9;
#endif
			agentpath.pop_front();
#ifdef DISPLAY
			std::cout << r << ":" << c << std::endl;
#endif
		}

		dyn_maze[dyn_agenty[i]][dyn_agentx[i]] = 6;
#ifdef DISPLAY
		printDynMaze(dyn_maze, MAZEHEIGHT);
#endif
		//dyn_maze[primestarty][primestartx] = 4;


	}

#ifdef COLLISION
	for (int z = 0; z < AGENTS; z++)
		for (int i = 0; i < MAZEWIDTH; i++)
			for (int j = 0; j < MAZEHEIGHT; j++)
				if (dyn_maze[i][j] == 3)
					agent_path[z][i][j] = 9;


	/*for (int z = 0; z < AGENTS; z++){
		std::cout << "Agent " << z << " moves." << std::endl;
		for (int i = 0; i < MAZEWIDTH; i++) {
			for (int j = 0; j < MAZEHEIGHT; j++){
				if (agent_path[z][i][j] == 9){
					std::cout << i << ":" << j << std::endl;
					
				}
			}
		}

	}*/

	//test
	
		
	for (int i = 0; i < MAZEWIDTH; i++) {
		for (int j = 0; j < MAZEHEIGHT; j++) {
			for (int z = 0; z < AGENTS; z++) {
				if (agent_path[z][i][j] == 9) {
					std::cout << "Agent " << z << " moves." << std::endl;
					std::cout << i << ":" << j << std::endl;
					dyn_maze[i][j] = 9;
					if (z < AGENTS - 1){
						agent_path[z + 1][i][j] = 8;
						dyn_maze[i][j] = 8;
						printDynMaze(dyn_maze, MAZEHEIGHT);

					}
				
				}
				else if (agent_path[z][i][j] == 8) {
					std::cout << "Agent " << z << " waits." << std::endl;
					agent_path[z][i][j] = 9;
					dyn_maze[i][j] = 9;

				}
			
			}

		}

	}
		
	printDynMaze(dyn_maze, MAZEHEIGHT);
#endif
	
	zeit2 = clock();

	cout << "Vergangene Zeit in Millisekunden: " << (zeit2 - zeit1) << "\n";
	

	//Planner p(map, (*map) (1, 1), (*map) (10, 15));	
	//p.replan();	
	//list<Map::Cell *> path = p.path();	

	

	return 0;
}