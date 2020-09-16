// Fill out your copyright notice in the Description page of Project Settings.


#include "zSpaceManager.h"

// Sets default values
AzSpaceManager::AzSpaceManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BaseMesh = CreateDefaultSubobject<UProceduralMeshComponent>("BaseMesh");
	Joint_01_Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Joint_01_Mesh");
	Joint_02_Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Joint_02_Mesh");
	Joint_03_Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Joint_03_Mesh");
	Joint_04_Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Joint_04_Mesh");
	Joint_05_Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Joint_05_Mesh");
	Joint_06_Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("Joint_06_Mesh");
	EE_Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("EE_Mesh");
	ObjectToCut = CreateDefaultSubobject<UProceduralMeshComponent>("ObjectToCut");
}

// Called when the game starts or when spawned
void AzSpaceManager::BeginPlay()
{
	Super::BeginPlay();

	RobotSetup();
	DisplayRobot();



}

// Called every frame
void AzSpaceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	UpdateRobot();

	DrawRobotMesh();
}


/////////SETUP _ UPDATE

void AzSpaceManager::RobotSetup()
{
	int nF = coreUtils.getNumfiles_Type(dirPath, zOBJ);
	if (nF < 8) nF = 8;

	r_meshObjs.assign(nF, zObjMesh());

	Nachi = zTsRobot(r_graphObj, r_meshObjs);

	Nachi.createRobotfromFile(dirPath + "/" + robotName, zJSON);

	Nachi.createRobotJointMeshesfromFile(dirPath, zOBJ, true);

	//////// Set Endeffector Transform

	robotEE.setIdentity();
	robotEE(3, 2) -= 37.87;

	Nachi.setEndEffector(robotEE);

	//////////
	//Nachi.createTargetsfromFile(dirPath + "/path.txt", zTXT);
}

void AzSpaceManager::UpdateRobot()
{
	if (zRobot_IK)
	{
		//////// Set Targets Transform and Compute Inverse Kinematics

		if (Nachi.robotTargets.empty())
		{
			UE_LOG(LogTemp, Warning, TEXT("\nNo target frames have been assigned "));
			zRobot_IK = !zRobot_IK;
		}
		else
		{
			robotTarget.setIdentity();
			robotTarget = Nachi.robotTargets[targetCounter];

			targetCounter = targetCounter % Nachi.robotTargets.size();

			Nachi.setTarget(robotTarget);

			zVector posIK = Nachi.inverseKinematics();

			//////// Store G-Code

			if (zRobot_GCODE)
			{
				for (int i = 0; i < Nachi.robotTargets.size(); i++)
				{
					Nachi.setTarget(Nachi.robotTargets[i]);

					posIK = Nachi.inverseKinematics();
					Nachi.gCode_store(posIK, zRobotVel, zMoveJoint, zEENeutral);
				}

				zRobot_GCODE = !zRobot_GCODE;
			}
		}
	}

	else
	{
		Nachi.forwardKinematics(zJoint);
	}
}


//////SET

void AzSpaceManager::SetJointRotations(float j1, float j2, float j3, float j4, float j5, float j6)
{
	Nachi.jointRotations[0].rotation = j1;
	Nachi.jointRotations[1].rotation = j2;
	Nachi.jointRotations[2].rotation = j3;
	Nachi.jointRotations[3].rotation = j4;
	Nachi.jointRotations[4].rotation = j5;
	Nachi.jointRotations[5].rotation = j6;
}

void AzSpaceManager::SetRobotIK(bool isIK)
{
	zRobot_IK = isIK;
}

void AzSpaceManager::SetToolPath(FString ToolPath)
{
	std::string toolPath = std::string(TCHAR_TO_UTF8(*ToolPath));
	Nachi.createTargetsfromFile(toolPath, zTXT);
}

///////DRAW


void AzSpaceManager::SetObjectPath(FString ObjPath)
{
	std::string objPath = std::string(TCHAR_TO_UTF8(*ObjPath));

	zFnMesh fnMesh(zObjectMesh);
	fnMesh.from(objPath, zOBJ, true);

	UMeshFactory::CreateUMeshFromZMesh(zObjectMesh, ObjectToCut, true);
}

void AzSpaceManager::SetTargetCounter(int32 Counter)
{
	targetCounter = Counter;
}

int32 AzSpaceManager::GetTargetCount()
{
	return Nachi.robotTargets.size();
}

void AzSpaceManager::DisplayRobot()
{
	UMeshFactory::CreateUMeshFromZMesh(r_meshObjs[0], BaseMesh, true);
	UMeshFactory::CreateUMeshFromZMesh(r_meshObjs[1], Joint_01_Mesh, true);
	UMeshFactory::CreateUMeshFromZMesh(r_meshObjs[2], Joint_02_Mesh, true);
	UMeshFactory::CreateUMeshFromZMesh(r_meshObjs[3], Joint_03_Mesh, true);
	UMeshFactory::CreateUMeshFromZMesh(r_meshObjs[4], Joint_04_Mesh, true);
	UMeshFactory::CreateUMeshFromZMesh(r_meshObjs[5], Joint_05_Mesh, true);
	UMeshFactory::CreateUMeshFromZMesh(r_meshObjs[6], Joint_06_Mesh, true);
	UMeshFactory::CreateUMeshFromZMesh(r_meshObjs[7], EE_Mesh, true);

}

void AzSpaceManager::DrawRobotMesh()
{
	UMeshFactory::UpdateUMeshFromZMesh(r_meshObjs[0], BaseMesh, true);
	UMeshFactory::UpdateUMeshFromZMesh(r_meshObjs[1], Joint_01_Mesh, true);
	UMeshFactory::UpdateUMeshFromZMesh(r_meshObjs[2], Joint_02_Mesh, true);
	UMeshFactory::UpdateUMeshFromZMesh(r_meshObjs[3], Joint_03_Mesh, true);
	UMeshFactory::UpdateUMeshFromZMesh(r_meshObjs[4], Joint_04_Mesh, true);
	UMeshFactory::UpdateUMeshFromZMesh(r_meshObjs[5], Joint_05_Mesh, true);
	UMeshFactory::UpdateUMeshFromZMesh(r_meshObjs[6], Joint_06_Mesh, true);
	UMeshFactory::UpdateUMeshFromZMesh(r_meshObjs[7], EE_Mesh, true);
}
