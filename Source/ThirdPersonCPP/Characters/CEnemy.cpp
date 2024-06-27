#include "CEnemy.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Components/CAttributeComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"

ACEnemy::ACEnemy()
{
	//Create Actor Component
	CHelpers::CreateActorComponent(this, &ActionComp, "ActionComp");
	CHelpers::CreateActorComponent(this, &MontagesComp, "MontagesComp");
	CHelpers::CreateActorComponent(this, &AttributeComp, "AttributeComp");
	CHelpers::CreateActorComponent(this, &StateComp, "StateComp");

	//Component Settings
	//-> MeshComp
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* MeshAsset;
	CHelpers::GetAsset(&MeshAsset, "/Game/Character/Mesh/SK_Mannequin");
	GetMesh()->SetSkeletalMesh(MeshAsset);

	TSubclassOf<UAnimInstance> AnimInstanceClass;
	CHelpers::GetClass<UAnimInstance>(&AnimInstanceClass, "/Game/Enemys/ABP_CEnemy");
	GetMesh()->SetAnimInstanceClass(AnimInstanceClass);


	//->Movement
	GetCharacterMovement()->MaxWalkSpeed = AttributeComp->GetSprintSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

}

void ACEnemy::BeginPlay()
{
	// Create Dynamic MAterial
	UMaterialInstanceConstant* BodyMaterialAsset;
	UMaterialInstanceConstant* LogoMaterialAsset;

	CHelpers::GetAssetDynamic(&BodyMaterialAsset, "/Game/Character/Materials/MI_UE4Man_Body");
	CHelpers::GetAssetDynamic(&LogoMaterialAsset, "/Game/Character/Materials/M_UE4Man_ChestLogo");

	BodyMaterial = UMaterialInstanceDynamic::Create(BodyMaterialAsset, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(LogoMaterialAsset, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	ActionComp->SetUnarmedMode();

	Super::BeginPlay();
}

void ACEnemy::ChangeBodyColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
}


