// Copyright (c) 2024 CronoGames.
#include "CroChunk.h"
#include "Async/Async.h"

// Sets default values
ACroChunk::ACroChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ChunkSize = 16;	
	BlockSize = 100;

	Blocks.SetNum(ChunkSize * ChunkSize * ChunkSize);
}

// Called when the game starts or when spawned
void ACroChunk::BeginPlay()
{
	Super::BeginPlay();
	//GenerateChunk();
}

// Called every frame
void ACroChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACroChunk::GenerateChunk(TSubclassOf<AActor> SMCube, FVector ChunkPosition)
{
	UWorld* World = GetWorld();
// 	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, SMCube, ChunkPosition, World]()
// 		{
			for (int32 X = 0; X < ChunkSize; ++X)
			{
				for (int32 Y = 0; Y < ChunkSize; ++Y)
				{
					for (int32 Z = 0; Z < ChunkSize; ++Z)
					{
						int32 Index = X + ChunkSize * (Y + ChunkSize * Z);//GetIndex(X, Y, Z);
						FCroBlockData BlockData;
						BlockData.Position = ChunkPosition + FVector(X * BlockSize, Y * BlockSize, Z * BlockSize);
						BlockData.Type = 1;
						Blocks[Index] = BlockData;

						// Check if the block should be spawned based on its neighbors 
						if (IsNeighborBlockAirOrInvalid(Index))
						{
							AsyncTask(ENamedThreads::GameThread, [World, SMCube, BlockData]()
								{
									// Spawn the SMCube actor at the block's position 
									if (SMCube)
									{
										FVector Position = BlockData.Position;
										World->SpawnActor<AActor>(SMCube, Position, FRotator::ZeroRotator);
									}
								});
						}
					}
				}
			}
		//});
}

// void ACroChunk::GenerateChunk(TSubclassOf<AActor> SMCube, FVector ChunkPosition)
// {
// 	UWorld* World = GetWorld();
// 	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, SMCube, ChunkPosition, World]()
// 		{
// 			for (int32 X = 0; X < ChunkSize; ++X)
// 			{
// 				for (int32 Y = 0; Y < ChunkSize; ++Y)
// 				{
// 					for (int32 Z = 0; Z < ChunkSize; ++Z)
// 					{
// 						int32 Index = X + ChunkSize * (Y + ChunkSize * Z);//GetIndex(X, Y, Z);
// 						FCroBlockData BlockData;
// 						BlockData.Position = ChunkPosition + FVector(X * BlockSize, Y * BlockSize, Z * BlockSize);
// 						BlockData.Type = 1;
// 						Blocks[Index] = BlockData;
// 
// 						// Check if the block should be spawned based on its neighbors 
// 						if (IsNeighborBlockAirOrInvalid(Index))
// 						{
// 							AsyncTask(ENamedThreads::GameThread, [World, SMCube, BlockData]() 
// 								{
// 									// Spawn the SMCube actor at the block's position 
// 									if (SMCube)
// 									{
// 										FVector Position = BlockData.Position;
// 										World->SpawnActor<AActor>(SMCube, Position, FRotator::ZeroRotator);
// 									}
// 								});
// 						}
// 					}
// 				}
// 			}
// 		});
// }

int32 ACroChunk::GetIndex(int32 X, int32 Y, int32 Z) const
{
	return X + ChunkSize * (Y + ChunkSize * Z);
}

FIntVector ACroChunk::GetCoordinates(int32 Index) const
{
	int32 Z = Index / (ChunkSize * ChunkSize);
	Index -= Z * (ChunkSize * ChunkSize);
	int32 Y = Index / ChunkSize;
	int32 X = Index % ChunkSize;
	return FIntVector(X, Y, Z);
}

void ACroChunk::SetBlockTypeByIndex(int32 Index, int32 Type)
{
	if (Blocks.IsValidIndex(Index))
	{
		Blocks[Index].Type = Type;
	}
}

TArray<FCroBlockData> ACroChunk::GetNeighboringBlocks(int32 Index) const
{
	TArray<FCroBlockData> Neighbors;
	FIntVector Coordinates = GetCoordinates(Index);
	int32 X = Coordinates.X;
	int32 Y = Coordinates.Y;
	int32 Z = Coordinates.Z;
	
	TArray<FIntVector> Directions = {
		FIntVector(1, 0, 0), FIntVector(-1, 0, 0),
		FIntVector(0, 1, 0), FIntVector(0, -1, 0),
		FIntVector(0, 0, 1), FIntVector(0, 0, -1)
	}; 

	for (FIntVector Direction : Directions)
	{
		int32 NeighborX = X + Direction.X;
		int32 NeighborY = Y + Direction.Y;
		int32 NeighborZ = Z + Direction.Z;

		if (NeighborX >= 0 && NeighborX < ChunkSize &&
			NeighborY >= 0 && NeighborY < ChunkSize &&
			NeighborZ >= 0 && NeighborZ < ChunkSize)
		{
			int32 NeighborIndex = GetIndex(NeighborX, NeighborY, NeighborZ);

			if (Blocks.IsValidIndex(NeighborIndex))
			{
				Neighbors.Add(Blocks[NeighborIndex]);
			}
		}
	} 
	return Neighbors;
}

bool ACroChunk::IsNeighborBlockAirOrInvalid(int32 Index) const
{
	TArray<FCroBlockData> Neighbors = GetNeighboringBlocks(Index); 
	for (const FCroBlockData& Neighbor : Neighbors)
	{
		if (Neighbor.Type == 0)
		{
			return true; // Neighbor block is of type 0
		}
	}

	// Check if there are any missing neighbors (out of bounds) 
	FIntVector Coordinates = GetCoordinates(Index);
	int32 X = Coordinates.X;
	int32 Y = Coordinates.Y;
	int32 Z = Coordinates.Z;
	TArray<FIntVector> Directions = {
		FIntVector(1, 0, 0), FIntVector(-1, 0, 0),
		FIntVector(0, 1, 0), FIntVector(0, -1, 0),
		FIntVector(0, 0, 1), FIntVector(0, 0, -1)
	};

	for (FIntVector Direction : Directions)
	{ 
		int32 NeighborX = X + Direction.X;
		int32 NeighborY = Y + Direction.Y;
		int32 NeighborZ = Z + Direction.Z;

		if (NeighborX < 0 || NeighborX >= ChunkSize || NeighborY < 0 || NeighborY >= ChunkSize || NeighborZ < 0 || NeighborZ >= ChunkSize)
		{ 
			return true; // Neighbor block is out of bounds
		}
	}

	return false; // All neighbor blocks are valid and not of type 0
}

int32 ACroChunk::GetBlockSize() const
{
	return BlockSize;
}

int32 ACroChunk::GetChunkSize() const
{
	return ChunkSize;
}

