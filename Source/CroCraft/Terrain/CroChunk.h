// Copyright (c) 2024 CronoGames.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CroChunk.generated.h"

USTRUCT(BlueprintType)
struct FCroBlockData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Type;
	
	FCroBlockData() : Position(FVector::ZeroVector), Type(0) {}
};

UCLASS(BlueprintType)
class CROCRAFT_API ACroChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACroChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Method to generate the chunk
	UFUNCTION(BlueprintCallable)
	void GenerateChunk(TSubclassOf<AActor> SMCube, FVector ChunkPosition);

	// Method to convert 3D coordinates to a 1D index 
	UFUNCTION(BlueprintCallable)
	int32 GetIndex(int32 X, int32 Y, int32 Z) const;

	// Method to convert 1D index to 3D coordinates 
	UFUNCTION(BlueprintCallable)
	FIntVector GetCoordinates(int32 Index) const;

	// Method to set block type by index 
	UFUNCTION(BlueprintCallable)
	void SetBlockTypeByIndex(int32 Index, int32 Type);

	// Method to get the neighboring block data by index 
	UFUNCTION(BlueprintCallable)
	TArray<FCroBlockData> GetNeighboringBlocks(int32 Index) const;

	// Method to check if a neighboring block is invalid or of type 0 
	UFUNCTION(BlueprintCallable)
	bool IsNeighborBlockAirOrInvalid(int32 Index) const;

	// Getter for BlockSize 
	UFUNCTION(BlueprintCallable)
	int32 GetBlockSize() const;
	// Getter for ChunkSize 
	UFUNCTION(BlueprintCallable)
	int32 GetChunkSize() const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BlockSize;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ChunkSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FCroBlockData> Blocks;
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	TSubclassOf<AActor> SMCube;
};
