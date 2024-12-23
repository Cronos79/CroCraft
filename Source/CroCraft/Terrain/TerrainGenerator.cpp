// Copyright (c) 2024 CronoGames.


#include "TerrainGenerator.h"


// Sets default values
ATerrainGenerator::ATerrainGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATerrainGenerator::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ATerrainGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ATerrainGenerator::Initialize(int32 sizeX, int32 sizeY, int32 sizeZ)
{
	ArraySizeX = sizeX;
	ArraySizeY = sizeY;
	ArraySizeZ = sizeZ;
	ChunkData = Make1DArray(ArraySizeX, ArraySizeY, ArraySizeZ);
}

void ATerrainGenerator::GenerateChunk(int32 X, int32 Y, int32 Z)
{
	// for each loop through the 1D array
	for (int32 i = 0; i < ChunkData.Num(); i++)
	{
		int32 X, Y, Z;
		GetXYZCoordinates(i, X, Y, Z);
		FCroCubeData CubeData = GetCubeData(X, Y, Z);
		FNeighbors Neighbors = GetNeighbors(X, Y, Z, ChunkData);
		if (NextToAir(X, Y, Z, ChunkData))
		{
			// make a vector and spawn a cube
			FVector Location = FVector(X * 100, Y * 100, Z * 100);
			GetWorld()->SpawnActor<AActor>(SMCube, Location, FRotator::ZeroRotator);
		}
	}	
}

FCroCubeData ATerrainGenerator::GetCubeData(int32 X, int32 Y, int32 Z)
{
	FCroCubeData CubeData;
	if ((X * ArraySizeY * ArraySizeZ) + (Y * ArraySizeZ) + Z > (ArraySizeTotal - 1))
	{
		CubeData.Index = 0;
		CubeData.bIsAir = true;
		return CubeData;
	}
	CubeData.Index = (X * ArraySizeY * ArraySizeZ) + (Y * ArraySizeZ) + Z;
	return CubeData;
}

FNeighbors ATerrainGenerator::GetNeighbors(int32 x, int32 y, int32 z, TArray<FCroCubeData> OneDArray)
{
	FNeighbors Neighbors;

	if (z - 1 < 0)
	{
		Neighbors.Down = -1;
	}
	else
	{
		Neighbors.Down = OneDArray[GetCubeData(x, y, z - 1).Index].Index;
	}
	
	if (z + 1 >= ArraySizeY)
	{
		Neighbors.Up = -1;
	}
	else
	{
		Neighbors.Up = OneDArray[GetCubeData(x, y, z + 1).Index].Index;
	}
	
	if (x - 1 < 0)
	{
		Neighbors.Right = -1;
	}
	else
	{
		Neighbors.Right = OneDArray[GetCubeData(x - 1, y, z).Index].Index;
	}
	
	if (x + 1 >= ArraySizeX)
	{
		Neighbors.Left = -1;
	}
	else
	{
		Neighbors.Left = OneDArray[GetCubeData(x + 1, y, z).Index].Index;
	}

	if (y + 1 >= ArraySizeZ)
	{
		Neighbors.Forward = -1;
	}
	else
	{
		Neighbors.Forward = OneDArray[GetCubeData(x, y + 1, z).Index].Index;
	}

	if (y - 1 < 0)
	{
		Neighbors.Backward = -1;
	}
	else
	{
		Neighbors.Backward = OneDArray[GetCubeData(x, y - 1, z).Index].Index;
	}

	return Neighbors;
}

void ATerrainGenerator::GetXYZCoordinates(int32 Index, int32& X, int32& Y, int32& Z)
{
	X = Index / (ArraySizeY * ArraySizeZ);
	Y = (Index % (ArraySizeY * ArraySizeZ)) / ArraySizeZ;
	Z = (Index % (ArraySizeY * ArraySizeZ)) % ArraySizeZ;
}

bool ATerrainGenerator::NextToAir(int32 x, int32 y, int32 z, TArray<FCroCubeData> OneDArray)
{
	bool bNextToAir = false;
	FNeighbors Neighbors = GetNeighbors(x, y, z, OneDArray);
	if (Neighbors.Down == -1 || Neighbors.Up == -1 || Neighbors.Left == -1 || Neighbors.Right == -1 || Neighbors.Forward == -1 || Neighbors.Backward == -1)
	{
		return true;
	}
	bNextToAir = OneDArray[Neighbors.Down].bIsAir || OneDArray[Neighbors.Up].bIsAir || OneDArray[Neighbors.Left].bIsAir || OneDArray[Neighbors.Right].bIsAir || OneDArray[Neighbors.Forward].bIsAir || OneDArray[Neighbors.Backward].bIsAir;
	
	return bNextToAir;
}

TArray<FCroCubeData> ATerrainGenerator::Make1DArray(int32 X, int32 Y, int32 Z)
{
	// Create a 3d array
	TArray<FCroCubeData> Array;
	Array.Init(FCroCubeData(), X * Y * Z);
	for (int32 i = 0; i < X; i++)
	{
		for (int32 j = 0; j < Y; j++)
		{
			for (int32 k = 0; k < Z; k++)
			{
				ArraySizeTotal++;
				Array[GetCubeData(i, j, k).Index].Index = i * Y * Z + j * Z + k;
			}
		}
	}
	return Array;
}

