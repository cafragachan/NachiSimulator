// Fill out your copyright notice in the Description page of Project Settings.


#include "zSpaceManager.h"

#include "Misc/Paths.h"
#include "Engine/GameEngine.h"

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

	//////////get path of nachi's parts needed when packaged project

	FString pth = FPaths::ProjectDir();
	std::string stpPath = std::string(TCHAR_TO_UTF8(*pth));

	std::string x = "\\";
	std::string	y = "/";
	size_t pos;
	while ((pos = stpPath.find(x)) != std::string::npos) {
		stpPath.replace(pos, 1, y);
	}

	dirPath = stpPath + "NACHI_MZ07";

	///////////
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
	initialRotations.Empty();

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
	for (int i = 0; i < 6; i++)
	{
		initialRotations.Add(Nachi.jointRotations[i].rotation);
	}
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

void AzSpaceManager::SetJointRotations(TArray<float> rotations)
{
	for (int i = 0; i < 6; i++)
		Nachi.jointRotations[i].rotation = initialRotations[i];
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

void AzSpaceManager::CreateOBJ(TArray<FString> ObjString)
{
	vector<std::string> lineObj;
	
	for (auto line : ObjString)
	{
		auto objStringAnsi = StringCast<ANSICHAR>(*line);
		const char* objStringAnsiPtr = objStringAnsi.Get();
		std::string objString(objStringAnsiPtr);

		lineObj.push_back(objString);
	}
	

	zFnMesh fnMesh(zObjectMesh);
	fnMesh.fromOBJ_string(lineObj);

	UE_LOG(LogTemp, Warning, TEXT("number of face polys: %i "), fnMesh.numPolygons());
	//UE_LOG(LogTemp, Warning, TEXT("FILE %s"), *FString(objString.c_str()));

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

void AzSpaceManager::ExportJointRotations()
{
	zVector posIK;

	for (int i = 0; i < Nachi.robotTargets.size(); i++)
	{
		Nachi.setTarget(Nachi.robotTargets[i]);

		posIK = Nachi.inverseKinematics();
		Nachi.gCode_store(posIK, zRobotVel, zMoveJoint, zEENeutral);
	}

	////

	FString projDir = FPaths::ProjectDir();
	std::string infilename = std::string(TCHAR_TO_UTF8(*projDir)) + "/joint_rotations.txt";
	ofstream myfile;
	myfile.open(infilename.c_str());

	if (myfile.fail())
	{
		UE_LOG(LogTemp, Warning, TEXT(" error in opening file  "));
		return;
	}

	for (int i = 0; i < Nachi.robot_gCode.size(); i++)
	{
	std::string jointSequence =   std::to_string(Nachi.robot_gCode[i].rotations[0].rotation) + ","
								+ std::to_string(Nachi.robot_gCode[i].rotations[1].rotation) + ","
								+ std::to_string(Nachi.robot_gCode[i].rotations[2].rotation) + ","
								+ std::to_string(Nachi.robot_gCode[i].rotations[3].rotation) + ","
								+ std::to_string(Nachi.robot_gCode[i].rotations[4].rotation) + ","
								+ std::to_string(Nachi.robot_gCode[i].rotations[5].rotation) + ",";

		//myfile << jointSequence << endl;
	}

	//close file
	myfile.close();

	UE_LOG(LogTemp, Warning, TEXT(" nG-CODE Exported Succesfully "));
}

FString AzSpaceManager::GetIKRotations()
{
	FString Rotations;
	zVector posIK;

	for (int i = 0; i < Nachi.robotTargets.size(); i++)
	{
		Nachi.setTarget(Nachi.robotTargets[i]);

		posIK = Nachi.inverseKinematics();
		Nachi.gCode_store(posIK, zRobotVel, zMoveJoint, zEENeutral);
	}

	for (int i = 0; i < Nachi.robot_gCode.size(); i++)
	{
		std::string jointSequence = std::to_string(Nachi.robot_gCode[i].rotations[0].rotation) + ","
									+ std::to_string(Nachi.robot_gCode[i].rotations[1].rotation) + ","
									+ std::to_string(Nachi.robot_gCode[i].rotations[2].rotation) + ","
									+ std::to_string(Nachi.robot_gCode[i].rotations[3].rotation) + ","
									+ std::to_string(Nachi.robot_gCode[i].rotations[4].rotation) + ","
									+ std::to_string(Nachi.robot_gCode[i].rotations[5].rotation) + ",";

		FString line(jointSequence.c_str());
		Rotations.Append(line);
	}

	return Rotations;
}

void AzSpaceManager::ResetRobot()
{
	zRobot_IK = false;

	RobotSetup();
	DisplayRobot();
	
	SetJointRotations(initialRotations);
	
	ObjectToCut->ClearAllMeshSections();

	UpdateRobot();
	DrawRobotMesh();
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

