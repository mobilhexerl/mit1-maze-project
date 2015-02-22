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

#define MAZEHEIGHT 2500
#define MAZEWIDTH  5000
#define DIRECTIONS  4
#define MAZEDENSITY 0.40
#define WALLSTOREADD 40
#define GOALS       100 
#define AGENTS      100
//#define DISPLAY
//#define COLLISION
//#define COLLISION_DISPLAY
//#define BLOCKING_DISPLAY
//#define DEBUG

void printStartMaze(Map *map) {
	for (int x = 0; x < MAZEWIDTH + 2; x++)
		std::cout << "x";
	std::cout << std::endl;
	for (int y = 0; y < MAZEHEIGHT; y++) {
		std::cout << "x";
		for (int x = 0; x < MAZEWIDTH; x++){
			if ((*map)(y, x)->cost == Map::Cell::COST_UNWALKABLE)
				std::cout << "x";
			else if ((*map)(y, x)->cost == 2)
				std::cout << "Y";
			else
				std::cout << " ";

		}
		std::cout << "x" << std::endl;

	}
	for (int x = 0; x < MAZEWIDTH + 2; x++)
		std::cout << "x";
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

}

void printDynMaze(int dyn_maze[][MAZEHEIGHT], int dim1){
	for (int x = 0; x < MAZEWIDTH + 2; x++)
		std::cout << "X";
	std::cout << std::endl;
	for (int y = 0; y < MAZEHEIGHT; y++) {
		std::cout << "X";
		for (int x = 0; x < MAZEWIDTH; x++){
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
				std::cout << ".";
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
	//end print dyn_maze
}

void printDynAgentMaze(int dyn_maze[][MAZEHEIGHT], int dim1){
	for (int x = 0; x < MAZEWIDTH + 2; x++)
		std::cout << "X";
	std::cout << std::endl;
	for (int y = 0; y < MAZEHEIGHT; y++) {
		std::cout << "X";
		for (int x = 0; x < MAZEWIDTH; x++){
			if (dyn_maze[x][y] == 2)
				std::cout << "X";				
			else if (dyn_maze[x][y] == 8)
				std::cout << "B";			
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
	//end print agent_maze
}

void printDynBlockingMaze(int dyn_maze[][MAZEHEIGHT], int dim1){
	for (int x = 0; x < MAZEWIDTH + 2; x++)
		std::cout << "X";
	std::cout << std::endl;
	for (int y = 0; y < MAZEHEIGHT; y++) {
		std::cout << "X";
		for (int x = 0; x < MAZEWIDTH; x++){
			if (dyn_maze[x][y] == 2)
				std::cout << "X";
			else if (dyn_maze[x][y] == 4)
				std::cout << "G";
			else if (dyn_maze[x][y] == 5)
				std::cout << "S";
			else if (dyn_maze[x][y] == 6)
				std::cout << "A";
			else if (dyn_maze[x][y] == 8)
				std::cout << "b";
			else if (dyn_maze[x][y] == 9)
				std::cout << ".";
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
	//end print agent_maze
}

int main(){

	int startx, starty, goalx, goaly, tmpstartx, tmpstarty, oldgoaly, oldgoalx, primestartx, primestarty;
	int newx, newy;
	int wallstoreadd = WALLSTOREADD;

	enum celltype  { EMPTY=1, WALL=2, PATH=3, GOAL=4, START=5, AGENT=6, AGENTPATH=7, BLOCK=8, MOVE=9 };

	int agent_startx, agent_starty;

	int *dyn_goalx = 0;
	dyn_goalx = new int[GOALS];

	int *dyn_goaly = 0;
	dyn_goaly = new int[GOALS];

	int *dyn_agentx = 0;
	dyn_agentx= new int[AGENTS];

	int *dyn_agenty = 0;
	dyn_agenty = new int[AGENTS];
	
	clock_t zeit1, zeit2;

	zeit1 = clock();

#ifdef COLLISION
	int agent_path[AGENTS][MAZEWIDTH][MAZEHEIGHT];
	for (int z = 0; z < AGENTS; z++)
		for (int i = 0; i < MAZEWIDTH; i++)
			for (int j = 0; j < MAZEHEIGHT; j++)
				agent_path[z][i][j] = EMPTY;
#endif	

	int dyn_maze[MAZEWIDTH][MAZEHEIGHT];
	for (int i = 0; i < MAZEWIDTH; i++)
		for (int j = 0; j < MAZEHEIGHT; j++)
			dyn_maze[i][j] = EMPTY;

	int blocked_maze[MAZEWIDTH][MAZEHEIGHT];
	for (int i = 0; i < MAZEWIDTH; i++)
		for (int j = 0; j < MAZEHEIGHT; j++)
			blocked_maze[i][j] = EMPTY;
	
	list<Planner> goals;	
	list<Map::Cell *> path;	
	list<Map::Cell *> lastpath;
	list<Map::Cell *> agentpath;	

	
	Map *map = new Map(MAZEHEIGHT, MAZEWIDTH);
	
	for (int i = 0; i < MAZEHEIGHT; ++i) {
		for (int j = 0; j < MAZEWIDTH; ++j) {
			(*map)(i, j)->cost = EMPTY;
		}
	}

	
	//FOR MORE RANDOMIZED OBSTACLES INCREASE WALLSTOREADD

	while (wallstoreadd)
	{
		newx = rand() % MAZEWIDTH;
		newy = rand() % MAZEHEIGHT;
		(*map)(newy, newx)->cost = Map::Cell::COST_UNWALKABLE;
		dyn_maze[newx][newy] = WALL;
		--wallstoreadd;
	}

	//print test start maze
#ifdef DEBUG
	printStartMaze(map);
#endif

#ifdef DISPLAY
	printDynMaze(dyn_maze, MAZEHEIGHT);
#endif
	
	//generate random goals and random starts
	std::srand(std::time(0));

	goaly = (rand() % ((MAZEHEIGHT + 1) / 2)) * 2;
	goalx = (rand() % ((MAZEWIDTH + 1) / 2)) * 2;	

	for (int i = 0; i < GOALS - 1; ++i) {
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
		Planner p(map, (*map) (tmpstarty, tmpstartx), (*map) (goaly, goalx));

		//saves goal as next start
		oldgoalx = goalx;
		oldgoaly = goaly;	

		if (i < GOALS - 1) {
			//saves goal in array
			dyn_goalx[i + 1] = goalx;
			dyn_goaly[i + 1] = goaly;
		}

		//checks if solution is found
		bool result = false;
		result = p.replan();

		//saves path to goals in goal list
		goals.push_back(p);
		//gets the solution path
		path = p.path();
#ifdef DISPLAY
		std::cout << "Path " << i << std::endl;
		std::cout << "Result " << result << std::endl;
		std::cout << "Start x: " << tmpstartx << std::endl;
		std::cout << "Start y: " << tmpstarty << std::endl;
		std::cout << "Goal x: " << goalx << std::endl;
		std::cout << "Goal y: " << goaly << std::endl;
#endif
		dyn_maze[tmpstartx][tmpstarty] = START;
		dyn_maze[goalx][goaly] = GOAL;

#ifdef DISPLAY
		printDynMaze(dyn_maze, MAZEHEIGHT);
#endif
		
		while (!path.empty()) {
			int r, c;
			r = path.front()->x();
			c = path.front()->y();
			dyn_maze[r][c] = PATH;			
			path.pop_front();
#ifdef DISPLAY
			std::cout << r << ":" << c << std::endl;
#endif			
		}
	
		dyn_maze[tmpstartx][tmpstarty] = START;
		dyn_maze[goalx][goaly] = GOAL;
	
#ifdef DISPLAY
		std::cout << "mylist stores " << goals.size() << " paths.\n";

		printDynMaze(dyn_maze, MAZEHEIGHT);
#endif
	}

	Planner p(map, (*map) (oldgoaly, oldgoalx), (*map) (primestarty, primestartx));
	
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
		if (dyn_maze[r][c] != START && dyn_maze[r][c] != GOAL)
			dyn_maze[r][c] = PATH;
		
		lastpath.pop_front();
#ifdef DISPLAY
		std::cout << r << ":" << c << std::endl;
#endif
	}

#ifdef DISPLAY
	
#endif

#ifdef DISPLAY
	std::cout << "mylist stores " << goals.size() << " paths.\n";
	std::cout << "GOALS" << std::endl;
	for (int i = 0; i < sizeof(*dyn_goalx); i++)
		std::cout << dyn_goalx[i] << ":" << dyn_goaly[i] << std::endl;
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
		Planner p(map, (*map) (starty, startx), (*map) (dyn_goaly[i], dyn_goalx[i]));
		p.replan();
		agentpath = p.path();
		
		while (!agentpath.empty()) {
			int r, c;
			r = agentpath.front()->x();
			c = agentpath.front()->y();
			if (dyn_maze[r][c] != START && dyn_maze[r][c] != GOAL && dyn_maze[r][c] != AGENT)
				dyn_maze[r][c] = AGENTPATH;

#ifdef COLLISION
			agent_path[i][r][c] = MOVE;
#endif
			agentpath.pop_front();
#ifdef DISPLAY
			std::cout << r << ":" << c << std::endl;
#endif
		}		
		dyn_maze[dyn_agentx[i]][dyn_agenty[i]] = AGENT;
#ifdef DISPLAY
		printDynMaze(dyn_maze, MAZEHEIGHT);
#endif
	
	}

#ifdef COLLISION
	for (int z = 0; z < AGENTS; z++)
		for (int i = 0; i < MAZEWIDTH; i++)
			for (int j = 0; j < MAZEHEIGHT; j++)
				if (dyn_maze[i][j] == PATH)
					agent_path[z][i][j] = MOVE;

#ifdef DEBUG
	for (int z = 0; z < AGENTS; z++){
		std::cout << "Agent " << z << " moves." << std::endl;
		for (int i = 0; i < MAZEWIDTH; i++) {
			for (int j = 0; j < MAZEHEIGHT; j++){
				if (agent_path[z][i][j] == MOVE){
					std::cout << i << ":" << j << std::endl;
					
				}
			}
		}

	}
#endif
	
	
		
	for (int i = 0; i < MAZEWIDTH; i++) {
		for (int j = 0; j < MAZEHEIGHT; j++) {
			for (int z = 0; z < AGENTS; z++) {
				if (agent_path[z][i][j] == MOVE && blocked_maze[i][j] == EMPTY) {
					std::cout << "Agent " << z << " moves." << std::endl;
					std::cout << i << ":" << j << std::endl;
					if (dyn_maze[i][j] != START && dyn_maze[i][j] != GOAL && dyn_maze[i][j] != AGENT)
						dyn_maze[i][j] = MOVE;
					blocked_maze[i][j] = z;

					if (j > 0 && blocked_maze[i][j - 1] == z && agent_path[z][i][j] == MOVE){
						blocked_maze[i][j-1] = EMPTY;					
					}
#ifdef COLLISION_DISPLAY
					printDynAgentMaze(dyn_maze, MAZEHEIGHT);
#endif
				}
				else if (agent_path[z][i][j] == BLOCK) {
					std::cout << "Agent " << z << " waits." << std::endl;
#ifdef BLOCKING_DISPLAY
					printDynBlockingMaze(blocked_maze, MAZEHEIGHT);
#endif					
				}
				
			}

		}

	}
		
	printDynMaze(dyn_maze, MAZEHEIGHT);
#endif
	
	zeit2 = clock();

	cout << "Vergangene Zeit in Millisekunden: " << (zeit2 - zeit1) << "\n";
	

	return 0;
}