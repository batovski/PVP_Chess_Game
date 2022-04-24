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
	UE_LOG(LogTemp, Warning, TEXT("Added to the array: X: %d, Y: %d [DEPTH: %d]"), Grid2DArray[x][y]->x, Grid2DArray[x][y]->y, CurrentDepth);
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
			UE_LOG(LogTemp, Warning, TEXT("Node Added: X: %d, Y: %d"), ResultNode.x, ResultNode.y);
			result.Add(ResultNode);
		}
		for (int i = -1; i <= 1; ++i)
		{
			for (int j = -1; j <= 1; ++j)
			{
				if (i == 0 && j == 0) continue;
				if (ResultNode.x + i >= 0 && ResultNode.x + i < width 
					&& ResultNode.y + j >= 0 
					&& ResultNode.y + j < height)
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
TArray<FGridNode>& UGridDataComponent::GetPossiblePaths(int x, int y, int depth)
{
	TArray<FGridNode>*Result = new TArray<FGridNode>();

	UE_LOG(LogTemp, Warning, TEXT("Start Node: X: %d, Y: %d"), x, y);
	BFS(x, y, depth, *Result);
	return *Result;
}
