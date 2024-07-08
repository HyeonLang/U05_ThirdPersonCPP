#include "CDoAction_Warp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CAttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstanceConstant.h"
#include "CAttachment.h"

void ACDoAction_Warp::BeginPlay()
{
	Super::BeginPlay();

	for (const auto& Child : OwnerCharacter->Children)
	{
		if (Child->IsA<ACAttachment>() && Child->GetActorLabel().Contains("Warp"))
		{
			PreviewMeshComp = CHelpers::GetComponent<USkeletalMeshComponent>(Child);
			break;
		}
	}

	// Todo.
	UMaterialInstanceConstant* BodyMaterialAsset;
	UMaterialInstanceConstant* LogoMaterialAsset;

	CHelpers::GetAssetDynamic(&BodyMaterialAsset, "MaterialInstanceConstant'/Game/Material/M_Warp_Inst.M_Warp_Inst'");
	CHelpers::GetAssetDynamic(&LogoMaterialAsset, "MaterialInstanceConstant'/Game/Material/M_Warp_Inst.M_Warp_Inst'");

	BodyMaterial = UMaterialInstanceDynamic::Create(BodyMaterialAsset, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(LogoMaterialAsset, this);

	PreviewMeshComp->SetMaterial(0, BodyMaterial);
	PreviewMeshComp->SetMaterial(1, LogoMaterial);
}

void ACDoAction_Warp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PreviewMeshComp->SetVisibility(false);
	
	CheckFalse(*bEquipped);

	FVector CurLoc;
	FRotator CurRot;

	if (GetCursorLocationAndRotation(CurLoc, CurRot))
	{
		PreviewMeshComp->SetVisibility(true);
		PreviewMeshComp->SetWorldLocation(CurLoc);
	}

}

void ACDoAction_Warp::DoAction()
{
	Super::DoAction();

	CheckFalse(StateComp->IsIdleMode());

	FRotator Temp;
	CheckFalse(GetCursorLocationAndRotation(Location, Temp));

	StateComp->SetActionMode();
	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? AttributeComp->SetMove() : AttributeComp->SetStop();
	
	PreviewMeshComp->SetVectorParameterValueOnMaterials("Emissive", FVector(20, 0, 0));
}

void ACDoAction_Warp::Begin_DoAction()
{
	Super::Begin_DoAction();

	FTransform Transform = Datas[0].EffectTransform;
	Transform.AddToTranslation(OwnerCharacter->GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Datas[0].Effect, Transform);
}

void ACDoAction_Warp::End_DoAction()
{
	Super::End_DoAction();

	Location.Z += OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	OwnerCharacter->SetActorLocation(Location);
	StateComp->SetIdleMode();
	AttributeComp->SetMove();

	PreviewMeshComp->SetVectorParameterValueOnMaterials("Emissive", FVector(0, 0, 20));
}

bool ACDoAction_Warp::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);

	FHitResult Hit;
	if (PC->GetHitResultUnderCursorForObjects(ObjectTypes, true, Hit))
	{
		OutLocation = Hit.Location;
		OutRotation = Hit.Normal.Rotation();
		return true;
	}

	return false;
}
