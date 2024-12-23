// Copyright (c) 2024 CronoGames.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainGenerator.generated.h"

USTRUCT(BlueprintType)
struct FCroCubeData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 Index = -1;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsAir = false;
};

USTRUCT(BlueprintType)
struct FChunkData
{
	GENERATED_BODY()
public:
	TArray<FCroCubeData> ChunkData;
	//Vector3 ChunkLocation;
};

USTRUCT(BlueprintType)
struct FNeighbors
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 Up;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 Down;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 Left;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 Right;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 Forward;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	int32 Backward;
};

UCLASS(BlueprintType)
class CROCRAFT_API ATerrainGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATerrainGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Initialize(int32 chunkSize, int32 cubeSize);
	UFUNCTION(BlueprintCallable)
	void GenerateChunk(int32 X, int32 Y, int32 Z);
	UFUNCTION(BlueprintCallable)
	FCroCubeData GetCubeData(int32 X, int32 Y, int32 Z);
	UFUNCTION(BlueprintCallable)
	FNeighbors GetNeighbors(int32 x, int32 y, int32 z, TArray<FCroCubeData> OneDArray);
	UFUNCTION(BlueprintCallable)
	void GetXYZCoordinates(int32 Index, int32& X, int32& Y, int32& Z);
	UFUNCTION(BlueprintCallable)
	bool NextToAir(int32 x, int32 y, int32 z, TArray<FCroCubeData> OneDArray);

private:
	TArray<FCroCubeData> Make1DArray();
	
	

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> SMCube;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 CubeSize = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ChunkSize = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ArraySizeTotal = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FChunkData> Chunks;
	
	
};
