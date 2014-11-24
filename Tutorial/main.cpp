#include <iostream>
#include "planner.h"

int main(){

	Map *map = new Map(20, 20);
	//(*map).
	//more readable 
	//map->operator()
	for (int i=0; i < 20; ++i) {
		for(int j=0; j < 20; ++j) {
			(*map)(i, j)->cost = 1;
		}
	}

	(*map)(1, 5)->cost = Map::Cell::COST_UNWALKABLE;

	Planner p(map, (*map) (1, 1), (*map) (10, 15));
	p.replan();
	list<Map::Cell *> path = p.path();
	
	while(!path.empty()) {
		int r, c;
		r = path.front()->x();
		c = path.front()->y();
		path.pop_front();
		std::cout << r << ":" << c << std::endl;
	}

	return 0;
}