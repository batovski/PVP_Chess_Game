// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Templates/Tuple.h"
#include "GridDataComponent.generated.h"

USTRUCT(BlueprintType)
struct FGridNode
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 x;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 y;

	int ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	bool isBlocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	bool isOccupied = false;
};

USTRUCT(BlueprintType)
struct FGrid2DArray {
	GENERATED_BODY()
public:

	TArray<FGridNode*> Arr;

	FGridNode* operator[] (int32 i) {
		return Arr[i];
	}

	void Add(FGridNode node) {
		Arr.Add(new FGridNode());
	}
	void Add(FGridNode* node) {
		Arr.Add(new FGridNode());
	}
};

USTRUCT()
struct FAStarCell 
{
	GENERATED_BODY()
public:
	FAStarCell* parent;
	// f = g + h
	double f, g, h;
	int x, y;

	void SetParent(FAStarCell* cell);
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PVP_GAME_API UGridDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridDataComponent();
	//~UGridDataComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "Grid")
	void CreateGrid(int width, int height);

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void BlockNode(int x, int y);

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void UnBlockNode(int x, int y);

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void OccupieNode(int x, int y);

	UFUNCTION(BlueprintCallable, Category = "Grid")
	void UnOccupieNode(int x, int y);

	UFUNCTION(BlueprintCallable, Category = "Grid")
	TArray<FGridNode>& GetPossiblePaths(int x, int y, int depth);

	UFUNCTION(BlueprintCallable, Category = "Grid")
	TArray<FGridNode>& GetPath(int StartX, int StartY, int EndX, int EndY);
	
private:
	void DFS(int x, int y,int depth, TArray<FGridNode>& result);
	void BFS(int x, int y, int depth, TArray<FGridNode>& result);
	void AStar(int StartX, int StartY, int EndX, int EndY, TArray<FGridNode>& result);
	double CalculateHValue(int currentX, int currentY, int EndX, int EndY);
	void BackTraceAStar(FAStarCell* endCell, TArray<FGridNode>& result);

	UPROPERTY(EditAnywhere, Category = "Grid")
	int width;
	UPROPERTY(EditAnywhere, Category = "Grid")
	int height;
	TMap<int32, bool> VisitedMap;

	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<FGrid2DArray> Grid2DArray;
};
