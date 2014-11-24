#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#define Direction short
#define RIGHT 		0
#define UP    		1
#define LEFT  		2
#define DOWN  		3
#define NONE  		4

using namespace std;

short Generate_Random_Number(short maximum)
{
	return rand()%maximum;
}

struct Pair
{
	Pair(){};

	Pair(short x, short y)
	{
		this->x = x;
		this->y = y;
	}

	Pair& operator+=(const Pair& rhs) 
	{    
		this->x += rhs.x;
		this->y += rhs.y;

    	return *this; 
  	}

  	Pair& operator-=(const Pair& rhs) 
	{    
		this->x -= rhs.x;
		this->y -= rhs.y;

    	return *this; 
  	}

	friend Pair operator+(Pair lhs, const Pair& rhs)
  	{
    	return lhs += rhs;     	
  	}

  	friend Pair operator-(Pair lhs, const Pair& rhs)
  	{
    	return lhs -= rhs; 
  	}

  	friend bool operator==(Pair &lhs, Pair &rhs)
	{
    	return (lhs.x==rhs.x && lhs.y==rhs.y);
	}

	friend bool operator!=(Pair &lhs, Pair &rhs)
	{
    	return !(lhs==rhs);
	}

	short x;
	short y;
};


class Cell
{
public:
	Cell()
	{
		this->Reset();
	}

	void Reset()
	{
		is_Visited = false;

		this->Add_Wall(RIGHT);
		this->Add_Wall(UP);
		this->Add_Wall(LEFT);
		this->Add_Wall(DOWN);		
	}

	void Add_Wall(Direction dir)
	{
		this->walls[dir] = true;
	}

	void Remove_Wall(Direction dir)
	{                           
		this->walls[dir] = false;		    	
  	}

  	bool Is_Unvisited()
  	{
  		return (!this->is_Visited);
  	}

  	bool Has_Walls(Direction dir)
  	{
  		return this->walls[dir];
  	}

  	short Get_Number_Of_Walls()
  	{
  		short sum = 0;

  		for (Direction dir=RIGHT; dir<NONE; dir++)
  			if ( this->Has_Walls(dir) )
  				sum++;

  		return sum;
  	}

  	bool Is_A_Junction()
  	{
  		return (this->Get_Number_Of_Walls()<=1) ? true : false;
  	}

  	Direction Get_Forward_Direction(Direction dir)
  	{
  		Direction temp;

  		if (!this->Has_Walls(dir)) 
  		{
  			return dir%4;
		}
		else
		{
			temp = (dir+1)%4;
			if (!this->Has_Walls(temp))  			
			{
				return temp;
			}
  			else
  			{
  				temp = (dir+3)%4;
  				if (!this->Has_Walls(temp))  		
  				{
  					return temp;
  				}
  				else
  					return NONE;
  			}
  		}  		

  	}

  	void Visit()
  	{
  		this->is_Visited = true;
  	}

private:
	bool walls[4];
	bool is_Visited;
};


class Maze
{
public:
	Maze(Pair size, short way_Width)
	{
		srand(time(NULL));				
		
		this->way_Width = way_Width;
		this->size = size;
		this->cells.resize(this->size.x, vector<Cell>(this->size.y));		
		
		this->direction_Vectors.push_back(Pair(1,0));
		this->direction_Vectors.push_back(Pair(0,1));
		this->direction_Vectors.push_back(Pair(-1,0));
		this->direction_Vectors.push_back(Pair(0,-1));
	}

	Cell Get_Next_Cell(Direction dir)
	{
		this->temp = this->current_Position + this->direction_Vectors[dir];
		
		return this->cells[this->temp.x][this->temp.y];								
	}

	void Visit_Next_Cell(Direction dir)
	{
		this->temp = this->current_Position + this->direction_Vectors[dir];		
		this->cells[this->temp.x][this->temp.y].Visit();								
	}

	bool Is_Still_Inside_The_Boundaries(Direction dir)
	{
		this->temp = this->current_Position + this->direction_Vectors[dir];

		if  ( (this->temp.x >=0) && (this->temp.x < this->size.x) && (this->temp.y >= 0) && (this->temp.y < this->size.y))
			return true;
		else
			return false;
	}

	Direction Randomly_Pick_Any_Unvisited_Neighbour()
	{
		vector<Direction> temp;

		for (Direction dir=RIGHT; dir<NONE; dir++)	
			if ( Is_Still_Inside_The_Boundaries(dir) && this->Get_Next_Cell(dir).Is_Unvisited() )
					temp.push_back(dir);		

		if ( temp.size() == 0 )
			return NONE;
		else
			return temp[ Generate_Random_Number( temp.size() ) ];		
	}

	void Connect_Current_Cell_To_Next_Cell(Direction dir)
	{
		this->cells[current_Position.x][this->current_Position.y].Remove_Wall(dir);
		temp = current_Position + direction_Vectors[dir];
		this->cells[temp.x][temp.y].Remove_Wall(this->Invert_Direction(dir));
	}

	void Push_Current_Cell_To_Stack()
	{
		this->stack.push_back(this->current_Position);
	}

	void Select_Next_Cell(Direction dir)
	{
		this->current_Position += this->direction_Vectors[dir];
	}

	void Make_The_Initial_Cell_The_Current_Cell(Pair initial_Position)
	{
		this->current_Position = initial_Position;
		this->cells[this->current_Position.x][this->current_Position.y].Visit();
	}

	Direction Invert_Direction(Direction dir)
	{
		return (dir + 2)%4;		
	}

	Direction Get_Direction_Between_Two_Cells(Pair cell_Position1, Pair cell_Position2)
	{
		Pair direction_Vector = cell_Position2 - cell_Position1;
		Direction direction = NONE;

		for (int dir=RIGHT; dir<NONE; dir++)
			if ( this->direction_Vectors[dir] == direction_Vector )
				direction = dir;		

		return direction;
	}

	vector<Direction> Get_Direction_Within_A_Cell(Pair position1, Pair position2)
	{
		Pair direction_Vector = position2 - position1;
		vector<Direction> dirs;
		Direction temp_Direction;

		if ( direction_Vector.x < 0 )
			temp_Direction = LEFT;
		else 
			temp_Direction = RIGHT;

		for (int i=0; i<abs(direction_Vector.x); i++)
				dirs.push_back(temp_Direction);


		if ( direction_Vector.y < 0 )
			temp_Direction = DOWN;
		else 
			temp_Direction = UP;

		for (int i=0; i<abs(direction_Vector.y); i++)
				dirs.push_back(temp_Direction);

		return dirs;
	}

	Pair Convert_Grid_Coordinate_To_Cell_Coordinate(Pair grid_coordinate)
	{
		return Pair(grid_coordinate.x/this->way_Width, grid_coordinate.y/this->way_Width);
	}

	string Repeat_String(string str, int number)
	{
		string buffer = "";

		for (int i=0; i<number; i++)
			buffer += str;

		return buffer;
	}

	void Print_Maze_Layout(short margin)
	{
		string buffer  			= "\n Maze Layout: \n";
		string newLine 			= "\n";
		string hWall   			= "_";
		string vWall   			= "|";
		string empty   			= " ";
		string subcell_Marker   = ".";		

		buffer += this->Repeat_String(empty, margin);
		buffer += this->Repeat_String(hWall, (2*this->way_Width)*this->size.x-1);
		buffer += newLine;

		for (int j=0; j<this->size.y; j++)
		{
			for (int w=0; w<this->way_Width; w++)
			{
				buffer += this->Repeat_String(empty, margin-1) + vWall;

				for (int i=0; i<this->size.x; i++)
				{
					if ( w == way_Width-1 )
					{
						if ( this->cells[i][j].Has_Walls(UP) )
							buffer += this->Repeat_String(hWall, 2*this->way_Width-1);
						else
							buffer += this->Repeat_String(empty, 2*this->way_Width-1);
					}
					else
					{
						buffer += this->Repeat_String(empty+subcell_Marker, this->way_Width-1);						
					}

					if ( w!=way_Width-1 )
						buffer += empty;

					if ( this->cells[i][j].Has_Walls(RIGHT) )
							buffer += vWall;
					else						
					{
						if ( w!=way_Width-1 )
							buffer += empty;
						else if ( j+1 < this->size.y )
							if ( this->cells[i][j+1].Has_Walls(RIGHT) )
								buffer += empty;
							else							
								buffer += hWall;							
						else
							buffer += hWall;							
					}
				}

				buffer += newLine;
			}

		}

		cout << buffer << newLine << newLine;
	}

	void Print_Maze_Info()
	{
		cout << endl << endl << "     Dimensions of the maze: " << this->size.x <<  " X " << this->size.y << " (number of cells)" << endl;
		cout << "     Dimensions of the grid: " << this->size.x*this->way_Width <<  " X " << this->size.y*this->way_Width << " (number of sub-cells)" << endl;
		cout << "                  Way-width: " << this->way_Width << endl;
		cout << " Coordinate axes directions: " << "Upper left and lower right corners are at [0, 0] and [";
		cout << this->size.x*this->way_Width-1 << ", " << this->size.y*this->way_Width-1 << "] respectively." << endl;
	}

	void Generate_Maze(Pair initial_Position)
	{
		Direction dir;
		bool There_Are_Not_Any_More_Unvisited_Cells = false;
				
		this->Make_The_Initial_Cell_The_Current_Cell(initial_Position);

		while (!There_Are_Not_Any_More_Unvisited_Cells)
		{			 
			 dir = this->Randomly_Pick_Any_Unvisited_Neighbour();

			 if (dir != NONE)
			 {
			 	this->Push_Current_Cell_To_Stack();
			 	this->Connect_Current_Cell_To_Next_Cell(dir);
			 	this->Visit_Next_Cell(dir);
			 	this->Select_Next_Cell(dir);			 	
			 }
			 else if ( this->stack.size() != 0)
			 {
			 	current_Position = this->stack.back();
			 	this->stack.pop_back();			 	
			 }
			 else
			 {
			 	There_Are_Not_Any_More_Unvisited_Cells = true;

			 	for (int i=0; i<this->size.x; i++)
			 		for (int j=0; j<this->size.y; j++)
			 			if (this->cells[i][j].Is_Unvisited())
			 			{
			 				There_Are_Not_Any_More_Unvisited_Cells = false;
			 				this->current_Position = Pair(i, j);
			 				i = this->size.x;
			 				j = this->size.y;
			 			}

			 }

		}

		this->stack.clear();		
	}

	void Print_Solution()
	{
		int line_Width;
		int max_Line_Width = 10;
		int solution_Length = this->stack_Grid.size();

		printf(" Solution Length: %d \n",  solution_Length);	
		printf("     Start Point: [%2d, %2d] \n", this->start_Position_Grid.x, this->start_Position_Grid.y);	
		printf("       End Point: [%2d, %2d] \n\n", this->end_Position_Grid.x, this->end_Position_Grid.y);	
		printf("     ");

		for (int i=0; i<this->stack_Grid.size(); i++)
		{
			printf("[%2d, %2d]", this->stack_Grid[i].x, this->stack_Grid[i].y);	
			
			line_Width = (line_Width + 1)%max_Line_Width;
			if (line_Width == 0)
				printf("\n     ");
			else
				printf(" ");
		}
	
		printf("\n\n");
	}

	void Solve(Pair start_Position_Grid, Pair end_Position_Grid)
	{	
		//This part finds a solution for cells

		bool has_Reached_DeadEnd = false;
		
		this->start_Position_Grid = start_Position_Grid;
		this->end_Position_Grid = end_Position_Grid;		

		this->start_Position = this->Convert_Grid_Coordinate_To_Cell_Coordinate(this->start_Position_Grid);
		this->end_Position = this->Convert_Grid_Coordinate_To_Cell_Coordinate(this->end_Position_Grid);

		Direction dir = RIGHT;
		Direction temp_Direction = dir;

		this->backup_cells = this->cells;
		this->stack.clear();

		this->current_Position = start_Position;			

		while ( !has_Reached_DeadEnd && this->current_Position != this->end_Position )
		{
			dir = this->backup_cells[this->current_Position.x][this->current_Position.y].Get_Forward_Direction(dir);			
			
			if ( dir != NONE )
			{
				has_Reached_DeadEnd = false;				

				if (this->backup_cells[this->current_Position.x][this->current_Position.y].Is_A_Junction() || this->current_Position == this->start_Position )
				{
					this->nodes_Index.push_back(this->stack.size());
					this->nodes.push_back(this->current_Position);						
					this->backup_cells[this->current_Position.x][this->current_Position.y].Add_Wall(dir);
					this->nodes_Direction.push_back(temp_Direction);
				}

				this->stack.push_back(current_Position);							
				this->current_Position += this->direction_Vectors[dir];				
				temp_Direction = dir;								
			}
			else
				has_Reached_DeadEnd = true;		

			
			if (has_Reached_DeadEnd)
			{
				this->stack.erase(this->stack.begin() + this->nodes_Index.back(), this->stack.end());
				this->current_Position = this->nodes.back();
				dir = this->nodes_Direction.back();

				this->nodes.pop_back();
				this->nodes_Index.pop_back();
				this->nodes_Direction.pop_back();

				if (this->backup_cells[this->start_Position.x][this->start_Position.y].Get_Number_Of_Walls() < 4 || this->stack.size() != 0)
					has_Reached_DeadEnd = false;								
			}
		}

		this->stack.push_back(this->end_Position);

		
		//This part extends the solution to grid

		short index = 0;

		this->current_Position_Grid = this->start_Position_Grid;
		this->stack_Grid.push_back(this->current_Position_Grid);		

		while ( index < this->stack.size()-1 )
		{
			dir = this->Get_Direction_Between_Two_Cells(this->stack[index], this->stack[index+1]);
			this->current_Position_Grid += this->direction_Vectors[dir];
			this->stack_Grid.push_back(this->current_Position_Grid);
			this->current_Position = this->Convert_Grid_Coordinate_To_Cell_Coordinate(this->current_Position_Grid);

			if (  this->current_Position == this->stack[index+1] )
			 	index++;			
		}

		vector<Direction> dirs = this->Get_Direction_Within_A_Cell(this->current_Position_Grid, this->end_Position_Grid);

		for (int i=0; i<dirs.size(); i++)
		{
			this->current_Position_Grid += this->direction_Vectors[dirs[i]];
			this->stack_Grid.push_back(this->current_Position_Grid);
		}

	}

private:

	short way_Width;

	Pair size;
	Pair current_Position;
	Pair current_Position_Grid;
	Pair start_Position;
	Pair end_Position;
	Pair start_Position_Grid;
	Pair end_Position_Grid;
	Pair temp;

	vector<Pair> direction_Vectors;
	vector< vector<Cell> > cells;
	vector< vector<Cell> > backup_cells;
	vector<Pair> stack;
	vector<Pair> stack_Grid;

	vector<Pair> nodes;
	vector<int> nodes_Index;
	vector<Direction> nodes_Direction;

};


int Run_Program(int nargs, char **args)
{	
	Pair maze_Size;
	Pair initial_Position;
	short way_Width;
	short margin = 5;

	if (nargs < 4)
	{
		fprintf( stderr, "Usage: %s [width] [height] [way-width]\n", args[0] );		
		return 1;
	}

	maze_Size.x = atoi(args[1]);
	maze_Size.y = atoi(args[2]);
	way_Width = atoi(args[3]);

	if (maze_Size.x<=0 || maze_Size.y<=0 || way_Width<=0)
	{
		fprintf( stderr, "Maze width, height and way width must be greater then 0.\n" );
		return 1;
	}

	initial_Position.x = (int)(maze_Size.x/2);
	initial_Position.y = (int)(maze_Size.y/2);

	Maze maze(maze_Size, way_Width);
	maze.Generate_Maze(initial_Position);
	maze.Print_Maze_Info();
	maze.Print_Maze_Layout(margin);
	maze.Solve(Pair(0, 0), Pair(way_Width*maze_Size.x-1, way_Width*maze_Size.y-1));	
	maze.Print_Solution();	

}

int main(int nargs, char **args)
{
	return Run_Program(nargs, args);
}


