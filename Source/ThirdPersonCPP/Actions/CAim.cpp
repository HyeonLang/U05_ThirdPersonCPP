#include "CAim.h"
#include "global.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CHUD.h"


UCAim::UCAim()
{
	CHelpers::GetAsset(&Curve, "/Game/Game/Curve_Aim");

}

void UCAim::BeginPlay(ACharacter* InOwnerCharacter)
{
	OwnerCharacter = InOwnerCharacter;

	SpringArmComp = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	CameraComp = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);

	// Binding Timeline Event
	FOnTimelineFloat OnProgressDelegate;
	OnProgressDelegate.BindUFunction(this, "OnProgress");
	Timeline.AddInterpFloat(Curve, OnProgressDelegate);

	// get HUD
	APlayerController* PC = OwnerCharacter->GetWorld()->GetFirstPlayerController();
	CheckNull(PC);
	HUD = PC->GetHUD<ACHUD>();


}	

void UCAim::Tick(float DeltaTime)
{
	Timeline.TickTimeline(DeltaTime);
}

void UCAim::On()
{
	CheckFalse(CanAim());
	CheckTrue(bZooming);
	
	bZooming = true;

	HUD->VisibleAim();

	SpringArmComp->TargetArmLength = 100.f;
	SpringArmComp->SocketOffset = FVector(0, 30, 10);
	SpringArmComp->bEnableCameraLag = false;

	Timeline.PlayFromStart();
}

void UCAim::Off()
{
	CheckFalse(CanAim());
	CheckFalse(bZooming);

	bZooming = false;

	HUD->InvisibleAim();

	SpringArmComp->TargetArmLength = 200.f;
	SpringArmComp->SocketOffset = FVector(0, 0, 0);
	SpringArmComp->bEnableCameraLag = true;

	Timeline.ReverseFromEnd();
}

void UCAim::OnProgress(float Output)
{
	CameraComp->FieldOfView = Output;
}
