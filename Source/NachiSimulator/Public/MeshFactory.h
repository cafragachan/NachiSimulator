// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define ZSPACE_STATIC_LIBRARY
#define ZSPACE_UNREAL_INTEROP
#define USING_CLR

#include <headers/zApp/include/zObjects.h>
#include <headers/zApp/include/zFnSets.h>
//#include <headers/zApp/include/zHousing.h>


using namespace zSpace;


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ProceduralMeshComponent.h"
#include "Math/Color.h"
#include "MeshFactory.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NACHISIMULATOR_API UMeshFactory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMeshFactory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	////zCode

public:

	static void CreateUMeshFromZMesh(zObjMesh&_mesh, UProceduralMeshComponent* _UMesh, bool HardEdges);

	static void UpdateUMeshFromZMesh(zObjMesh&_mesh, UProceduralMeshComponent* _UMesh, bool HardEdges);

	

		
};
