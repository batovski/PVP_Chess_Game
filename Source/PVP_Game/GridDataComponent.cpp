// Fill out your copyright notice in the Description page of Project Settings.


#include "GridDataComponent.h"
#include "Templates/Tuple.h"

// Sets default values for this component's properties
UGridDataComponent::UGridDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	width = 0;
	height = 0;
	// ...
}
// Called when the game starts
void UGridDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGridDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGridDataComponent::CreateGrid(int _width, int _height)
{
	width = _width;
	height = _height;

	for (int i = 0; i < height; i++) {
		Grid2DArray.Add(FGrid2DArray());
		for (int j = 0; j < width; j++)
		{
			Grid2DArray[i].Add(new FGridNode());
		}
	}
	//Fill the Array with data:
	int CurrentID = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Grid2DArray[i][j]->y = j;
			Grid2DArray[i][j]->x = i;
			Grid2DArray[i][j]->ID = CurrentID;
			VisitedMap.Add(CurrentID, false);
			CurrentID++;
		}
	}
}
void UGridDataComponent::DFS(int x, int y, int depth, TArray<FGridNode>& result)
{
	VisitedMap[Grid2DArray[x][y]->ID] = true;
	int CurrentDepth = depth;
	//if (CurrentDepth > MaxDepth) { return; }

	if(CurrentDepth != 0)
		result.Add(*Grid2DArray[x][y]);
	for(int i = -1; i <= 1; ++i)
	{
		for(int j = -1; j <= 1; ++j)
		{
			if(x + i >= 0 && x + i < width && y + j >= 0 && y + j < height)
			{
				if(VisitedMap[Grid2DArray[x+i][y+j]->ID] == false)
				{
					DFS(x + i, y + j, CurrentDepth + 1, result);
				}
			}
		}
	}
}
void UGridDataComponent::BFS(int x, int y, int depth, TArray<FGridNode>& result)
{
	TTuple<bool, int>* Visited = new TTuple<bool, int>[width * height];
	for (int i = 0; i < width * height; i++)
	{
		Visited[i].Key = false;
		Visited[i].Value = INT_MAX;
	}
		
	TQueue<FGridNode> Queue;

	Visited[Grid2DArray[x][y]->ID].Key = true;
	Visited[Grid2DArray[x][y]->ID].Value = 0;

	Queue.Enqueue(*Grid2DArray[x][y]);

	while (!Queue.IsEmpty())
	{
		FGridNode ResultNode;
		
		Queue.Dequeue(OUT ResultNode);

		if (depth < Visited[ResultNode.ID].Value)
		{
			continue;
		}
		if (Visited[ResultNode.ID].Value != 0)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Node Added: X: %d, Y: %d"), ResultNode.x, ResultNode.y);
			result.Add(ResultNode);
		}
		for (int i = -1; i <= 1; ++i)
		{
			for (int j = -1; j <= 1; ++j)
			{
				if (i == 0 && j == 0) continue;
				if (ResultNode.x + i >= 0 && ResultNode.x + i < height
					&& ResultNode.y + j >= 0 
					&& ResultNode.y + j < width)
				{
					if (Visited[Grid2DArray[ResultNode.x + i][ResultNode.y + j]->ID].Key == false &&
						Grid2DArray[ResultNode.x + i][ResultNode.y + j]->isBlocked == false)
					{
						Visited[Grid2DArray[ResultNode.x + i][ResultNode.y + j]->ID].Key = true;
						Visited[Grid2DArray[ResultNode.x + i][ResultNode.y + j]->ID].Value = Visited[Grid2DArray[ResultNode.x][ResultNode.y]->ID].Value + 1;
						Queue.Enqueue(*Grid2DArray[ResultNode.x + i][ResultNode.y + j]);
					}
					else
					{
						if (Visited[Grid2DArray[ResultNode.x + i][ResultNode.y + j]->ID].Value > Visited[Grid2DArray[ResultNode.x][ResultNode.y]->ID].Value + 1)
							Visited[Grid2DArray[ResultNode.x + i][ResultNode.y + j]->ID].Value = Visited[Grid2DArray[ResultNode.x][ResultNode.y]->ID].Value + 1;
					}
				}
			}
		}
	}
	delete[] Visited;
}

void UGridDataComponent::AStar(int StartX, int StartY, int EndX, int EndY, TArray<FGridNode>& result)
{
	//there is an assumption that cell is valid:
	if (StartX == EndX && StartY == EndY) { return; }

	// Declare a 2D array of structure to hold the details
		// of that cell

	TArray<TArray<FAStarCell*>> cellDetails;
	for (int i = 0; i < height; i++) {
		TArray<FAStarCell*> arr;
		cellDetails.Add(arr);
		for (int j = 0; j < width; j++)
		{
			FAStarCell* cell = new FAStarCell();
			cellDetails[i].Add(cell);
		}
	}
	TArray<TArray<bool>> closedSet;
	for (int i = 0; i < height; i++) {
		TArray<bool> arr;
		closedSet.Add(arr);
		for (int j = 0; j < width; j++)
		{
			bool cell = false;
			closedSet[i].Add(cell);
		}
	}

	

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			cellDetails[i][j]->f = FLT_MAX;
			cellDetails[i][j]->g = FLT_MAX;
			cellDetails[i][j]->h = FLT_MAX;
			cellDetails[i][j]->x = i;
			cellDetails[i][j]->y = j;
			cellDetails[i][j]->parent = nullptr;
		}
	}

	cellDetails[StartX][StartY]->f = 0.0;
	cellDetails[StartX][StartY]->g = 0.0;
	cellDetails[StartX][StartY]->h = 0.0;

	TArray<FAStarCell*> openSet;

	FAStarCell* current;
	openSet.Reserve(height * width);

	openSet.Push(cellDetails[StartX][StartY]);

	while(openSet.Num() != 0)
	{
		//Find Max priaority element:
		int it_index = 0;
		current = openSet[0];

		for (int i = 0; i < openSet.Num(); i++) {
			auto cell = openSet[i];
			if (cell->f < current->f) {
				current = cell;
				it_index = i;
			}

			else if (cell->f == current->f) {
				if (cell->h < current->h)
				{
					current = cell;
					it_index = i;
				}
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("A* Closed Set: X: %d, Y: %d"), current->x, current->y);
		closedSet[current->x][current->y] = true;
		openSet.RemoveAt(it_index);

		double gNew, hNew, fNew;

		for (int i = -1; i <= 1; ++i)
		{
			for (int j = -1; j <= 1; ++j)
			{
				if (i == 0 && j == 0) continue;
				if (current->x + i >= 0
					&& current->x + i < height
					&& current->y + j >= 0
					&& current->y + j < width)
				{
					FAStarCell* Successor = cellDetails[current->x + i][current->y + j];
					if(Successor->x == EndX && Successor->y == EndY)
					{
						Successor->SetParent(current);
						BackTraceAStar(Successor, result);
						return;
					}

					else if(closedSet[current->x + i][current->y + j] == false && !Grid2DArray[current->x + i][current->y + j]->isBlocked)
					{
						gNew = cellDetails[current->x][current->y]->g + 1.0;
						hNew = CalculateHValue(current->x + i, current->y + j, EndX, EndY);
						fNew = gNew + hNew;

						if (cellDetails[current->x + i][current->y + j]->f == FLT_MAX
							|| cellDetails[current->x + i][current->y + j]->f > fNew) {

							// Update the details of this cell
							cellDetails[current->x + i][current->y + j]->f = fNew;
							cellDetails[current->x + i][current->y + j]->g = gNew;
							cellDetails[current->x + i][current->y + j]->h = hNew;
							cellDetails[current->x + i][current->y + j]->SetParent(current);
							openSet.Push(cellDetails[current->x + i][current->y + j]);
						}
					}
				}
			}
		}
	}

}
void FAStarCell::SetParent(FAStarCell* cell)
{
	parent = cell;
}
double UGridDataComponent::CalculateHValue(int currentX, int currentY, int EndX, int EndY)
{
	return ((double)sqrt(
		(currentX - EndX) * (currentX - EndX)
		+ (currentY - EndY) * (currentY - EndY)));
}
void UGridDataComponent::BackTraceAStar(FAStarCell* endCell, TArray<FGridNode>& result)
{
	FAStarCell* Current = endCell;

	while(Current != nullptr)
	{
		result.Insert(*Grid2DArray[Current->x][Current->y],0);
		Current = Current->parent;
	}
}
void UGridDataComponent::BlockNode(int x, int y)
{
	Grid2DArray[x][y]->isBlocked = true;
}
void UGridDataComponent::UnBlockNode(int x, int y)
{
	Grid2DArray[x][y]->isBlocked = false;
}

void UGridDataComponent::OccupieNode(int x, int y)
{
	Grid2DArray[x][y]->isOccupied = true;
}

void UGridDataComponent::UnOccupieNode(int x, int y)
{
	Grid2DArray[x][y]->isOccupied = false;
}

TArray<FGridNode>& UGridDataComponent::GetPossiblePaths(int x, int y, int depth)
{
	TArray<FGridNode>*Result = new TArray<FGridNode>();

	//UE_LOG(LogTemp, Warning, TEXT("Start Node: X: %d, Y: %d"), x, y);
	BFS(x, y, depth, *Result);
	return *Result;
}

TArray<FGridNode>& UGridDataComponent::GetPath(int StartX, int StartY, int EndX, int EndY)
{
	TArray<FGridNode>* Result = new TArray<FGridNode>();
	AStar(StartX, StartY, EndX, EndY, *Result);
	return *Result;
}