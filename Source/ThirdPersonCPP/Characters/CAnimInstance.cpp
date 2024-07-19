#include "CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"

static TAutoConsoleVariable<bool> CVarIkMode(TEXT("Tore.IkMode"), true, TEXT("Enable IkMode"), ECVF_Cheat);

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	CheckNull(TryGetPawnOwner());

	UCActionComponent* ActionComp = CHelpers::GetComponent<UCActionComponent>(TryGetPawnOwner());
	CheckNull(ActionComp);

	ActionComp->OnActionTypeChanged.AddDynamic(this, &UCAnimInstance::OnActionTypeChanged);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* Character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(Character);

	Speed = Character->GetVelocity().Size2D();
	Direction = CalculateDirection(Character->GetVelocity(), Character->GetControlRotation());

	UCFeetComponent* FeetComp = CHelpers::GetComponent<UCFeetComponent>(Character);
	if (FeetComp)
		FeetData = FeetComp->GetData();

	bIkMode = CVarIkMode.GetValueOnGameThread();
}

void UCAnimInstance::OnActionTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	ActionType = InNewType;
}
