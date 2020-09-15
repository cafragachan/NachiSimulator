// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshFactory.h"

// Sets default values for this component's properties
UMeshFactory::UMeshFactory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMeshFactory::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMeshFactory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UMeshFactory::CreateUMeshFromZMesh(zObjMesh&_mesh, UProceduralMeshComponent* _UMesh, bool HardEdges)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> VertexColors;

	zInt2DArray fTris;
	zFnMesh fnMesh(_mesh);
	fnMesh.triangulate();
	fnMesh.getMeshTriangles(fTris);

	//fnMesh.to("C:/Users/cesar.fragachan/Desktop/myUmesh.obj", zOBJ);

	for (zItMeshVertex it(_mesh); !it.end(); it++)
	{
		zVector pos = it.getPosition();
		FVector vertex = FVector(pos.x, pos.y, pos.z);

		FTransform T(FRotator(0, 0, 0), FVector(0, 0, 0), FVector(100, -100, 100));
		vertex = T.TransformPosition(vertex);

		Vertices.Add(vertex);

		if (HardEdges)
		{
			zVector norm = it.getNormal();
			Normals.Add(FVector(norm.x, norm.y, norm.z));
		}
		else
		{
			Normals.Add(FVector(0, 0, 1));
		}

		//VertexColors.Add(FLinearColor(it.getColor().r, it.getColor().g, it.getColor().b, 1.0));
		VertexColors.Add(FLinearColor(1, 0, 0, 1));
	}

	for (int i = 0; i < fTris.size(); i++)
	{
		for (int j = 0; j < fTris[i].size(); j++)
		{
			Triangles.Add(fTris[i][j % fTris[i].size()]);
			Triangles.Add(fTris[i][(j + 1) % fTris[i].size()]);
			Triangles.Add(fTris[i][(j + 2) % fTris[i].size()]);
		}
	}

	_UMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);

	// Enable collision data
	_UMesh->ContainsPhysicsTriMeshData(true);
}

void UMeshFactory::UpdateUMeshFromZMesh(zObjMesh & _mesh, UProceduralMeshComponent * _UMesh, bool HardEdges)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> VertexColors;

	for (zItMeshVertex it(_mesh); !it.end(); it++)
	{
		zVector pos = it.getPosition();
		FVector vertex = FVector(pos.x, pos.y, pos.z);

		FTransform T(FRotator(0, 0, 0), FVector(0, 0, 0), FVector(100, -100, 100));
		vertex = T.TransformPosition(vertex);

		Vertices.Add(vertex);

		if (HardEdges)
		{
			zVector norm = it.getNormal();
			Normals.Add(FVector(norm.x, norm.y, norm.z));
		}
		else
		{
			Normals.Add(FVector(0, 0, 1));
		}

		//VertexColors.Add(FLinearColor(it.getColor().r, it.getColor().g, it.getColor().b, 1.0));
		VertexColors.Add(FLinearColor(1, 0, 0, 1));
	}

	_UMesh->UpdateMeshSection_LinearColor(0, Vertices, Normals, UV0, VertexColors, Tangents);
}
