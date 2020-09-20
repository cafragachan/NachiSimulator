// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#define ZSPACE_STATIC_LIBRARY
#define ZSPACE_UNREAL_INTEROP
#define USING_CLR

#include <headers/zApp/include/zObjects.h>
#include <headers/zApp/include/zFnSets.h>

//// zSpace Toolsets
#include <headers/zToolsets/digifab/zTsRobot.h>


using namespace zSpace;


#include "CoreMinimal.h"

#include "ProceduralMeshComponent.h"
#include "MeshFactory.h"

#include "GameFramework/Actor.h"
#include "zSpaceManager.generated.h"

UCLASS()
class NACHISIMULATOR_API AzSpaceManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AzSpaceManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	/////z-ue INterface

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* Joint_01_Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* Joint_02_Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* Joint_03_Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* Joint_04_Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* Joint_05_Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* Joint_06_Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* EE_Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* ObjectToCut;

	//////////////////////////////////////////////////////////
	zObjMesh mesh;

	zUtilsCore coreUtils;

	zTsRobot Nachi;

	vector<zObjMesh> r_meshObjs;
	zObjGraph r_graphObj;

	zObjMesh zObjectMesh;

	zTransform robotTarget;
	zTransform robotEE;

	////////////////////////////////////////////////////////// Others

	bool zRobot_IMPORT = true;
	bool zRobot_IK = false;
	bool zRobot_DISPLAY_targetFrame = false;
	bool zRobot_TARGETS = false;
	bool zRobot_GCODE = false;
	bool zRobot_GCODE_Export = false;

	int targetCounter = 0;

	//string dirPath = "D:/Repos/Alice/ALICE_PLATFORM/x64/Release/EXE/data/NACHI_MZ07";
	string dirPath;
	string robotName = "Nachi_MZ07.json";
	vector<string> robotFile;

	double zRobotVel = 500;
	double increment = 1.0;


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<float> initialRotations;

	void RobotSetup();
	void UpdateRobot();

public:

	UFUNCTION(BlueprintCallable)
	void SetJointRotations(float j1, float j2, float j3, float j4, float j5, float j6);

	void SetJointRotations(TArray<float> rotations);

	UFUNCTION(BlueprintCallable)
	void SetRobotIK(bool isIK);

	UFUNCTION(BlueprintCallable)
	void SetToolPath(FString ToolPath);

	UFUNCTION(BlueprintCallable)
	void SetObjectPath(FString ObjPath);

	UFUNCTION(BlueprintCallable)
	void SetTargetCounter(int32 Counter);

	UFUNCTION(BlueprintCallable)
	int32 GetTargetCount();

	UFUNCTION(BlueprintCallable)
	void ExportJointRotations();

	UFUNCTION(BlueprintCallable)
	void ResetRobot();

private:

	void DisplayRobot();
	void DrawRobotMesh();
	
};
