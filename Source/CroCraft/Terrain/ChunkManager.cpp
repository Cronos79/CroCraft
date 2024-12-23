// Copyright (c) 2024 CronoGames.


#include "ChunkManager.h"


// Sets default values
AChunkManager::AChunkManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RenderDistance = 2; // Load chunks within a 2-chunk radius around the player	
	ChunkSize = 0;
}


void AChunkManager::UpdateChunks(FVector PlayerPosition)
{
	FIntVector PlayerChunkCoords = FIntVector(FMath::FloorToInt(PlayerPosition.X / ChunkSize), FMath::FloorToInt(PlayerPosition.Y / ChunkSize), 0);
	if (ChunkSize == 0)
	{
		ACroChunk* CroChunk = GetWorld()->SpawnActor<ACroChunk>(FVector(0, 0, 0), FRotator::ZeroRotator);
		ChunkSize = CroChunk->GetChunkSize() * CroChunk->GetBlockSize();
	}
	// Assuming each chunk is 1600 units 
	for (int32 X = PlayerChunkCoords.X - RenderDistance; X <= PlayerChunkCoords.X + RenderDistance; ++X)
	{
		for (int32 Y = PlayerChunkCoords.Y - RenderDistance; Y <= PlayerChunkCoords.Y + RenderDistance; ++Y)
		{
			FIntVector ChunkCoords = FIntVector(X, Y, 0);
			if (!ActiveChunks.Contains(ChunkCoords))
			{
				LoadChunk(ChunkCoords);
			}
		}
	} 
	// Unload chunks that are no longer in the render distance 
	for (auto It = ActiveChunks.CreateIterator(); It; ++It)
	{
		FIntVector ChunkCoords = It.Key();
		if (FMath::Abs(ChunkCoords.X - PlayerChunkCoords.X) > RenderDistance || FMath::Abs(ChunkCoords.Y - PlayerChunkCoords.Y) > RenderDistance)
		{
			UnloadChunk(ChunkCoords);
		}
	}
}

// Called when the game starts or when spawned
void AChunkManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChunkManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Assuming you have a reference to the player's position 
	FVector PlayerPosition = FVector(0, 0, 0); //GetPlayerPosition();
	// Replace with actual player position retrieval 
	UpdateChunks(PlayerPosition);
}

void AChunkManager::LoadChunk(FIntVector ChunkCoords)
{
	FVector ChunkPosition = FVector(ChunkCoords.X * ChunkSize, ChunkCoords.Y * ChunkSize, 0);
	ACroChunk* NewChunk = GetWorld()->SpawnActor<ACroChunk>(ChunkPosition, FRotator::ZeroRotator);
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, ChunkPosition, NewChunk, ChunkCoords]()
		{
			NewChunk->GenerateChunk(SMCube, ChunkPosition);
			ActiveChunks.Add(ChunkCoords, NewChunk);
		});
	
}

void AChunkManager::UnloadChunk(FIntVector ChunkCoords)
{
	ACroChunk* ChunkToUnload = ActiveChunks[ChunkCoords];
	if (ChunkToUnload)
	{
		ChunkToUnload->Destroy();
		ActiveChunks.Remove(ChunkCoords);
	}
}

