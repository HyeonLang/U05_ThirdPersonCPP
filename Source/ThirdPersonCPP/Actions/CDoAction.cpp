#include "CDoAction.h"
#include "GameFramework/Character.h"

ACDoAction::ACDoAction()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACDoAction::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	AttributeComp = CHelpers::GetComponent<UCAttributeComponent>(OwnerCharacter);
	StateComp = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);

	Super::BeginPlay();
}

void ACDoAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ACDoAction::SetDatas(const TArray<FDoActionData>& InDatas)
{
	Datas = InDatas;
}

