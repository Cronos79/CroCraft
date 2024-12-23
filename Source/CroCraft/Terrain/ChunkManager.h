// Copyright (c) 2024 CronoGames.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CroChunk.h"
#include "ChunkManager.generated.h"

UCLASS(Blueprintable)
class CROCRAFT_API AChunkManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunkManager();

	// Method to update chunks based on player position 
	UFUNCTION(BlueprintCallable, Category = "Chunk Manager")
	void UpdateChunks(FVector PlayerPosition);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Method to load a chunk at a specific location 
	void LoadChunk(FIntVector ChunkCoords); 
	// Method to unload a chunk at a specific location 
	void UnloadChunk(FIntVector ChunkCoords);

public:
	// Map to store active chunks 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) 
	TMap<FIntVector, ACroChunk*> ActiveChunks;
	// Distance in chunks to load around the player 
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	int32 RenderDistance;	
	// Assuming each chunk is 1600 units 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ChunkSize;
	// Subclass of the cube actor to spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> SMCube;
};
