#include "CDoAction_WhirlWind.h"
#include "Global.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CAttributeComponent.h"
#include "CAttachment.h"


ACDoAction_WhirlWind::ACDoAction_WhirlWind()
{
	DamageToTime = 0.1f;
	ActiveTime = 5.f;
	AroundSpeed = 300.f;
	Radius = 150.f;
}

void ACDoAction_WhirlWind::BeginPlay()
{
	Super::BeginPlay();

	for (const auto& Child : OwnerCharacter->Children)
	{
		if (Child->IsA<ACAttachment>() && Child->GetName().Contains("WhirlWind"))
		{
			BoxComp = CHelpers::GetComponent<UBoxComponent>(Child);
			break;
		}
	}

	CheckNull(BoxComp);
	ACAttachment* Attachment = Cast<ACAttachment>(BoxComp->GetOwner());
	CheckNull(Attachment);

	Attachment->OffCollision();
}

void ACDoAction_WhirlWind::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Center = OwnerCharacter->GetActorLocation();

	CurrentYaw += DeltaTime * AroundSpeed;
	CurrentYaw = FMath::Fmod(CurrentYaw, 360.f);

	FVector AwayFrom = FVector(Radius, 0, 0);
	FVector CollisionLocation = Center + AwayFrom.RotateAngleAxis(CurrentYaw, FVector::UpVector);

	BoxComp->SetWorldLocation(CollisionLocation);
}

void ACDoAction_WhirlWind::DoAction()
{
	Super::DoAction();

	CheckFalse(Datas.Num() > 0);
	CheckFalse(StateComp->IsIdleMode());

	CheckTrue(bActivating);

	StateComp->SetActionMode();
	bActivating = true;

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? AttributeComp->SetMove() : AttributeComp->SetStop();

	CheckNull(BoxComp);
	ACAttachment* Attachment = Cast<ACAttachment>(BoxComp->GetOwner());
	CheckNull(Attachment);

	Attachment->OnCollision();

	UKismetSystemLibrary::K2_SetTimer(this, "TickDamage", DamageToTime, true);
}

void ACDoAction_WhirlWind::Begin_DoAction()
{
	Super::Begin_DoAction();

	CurrentYaw = OwnerCharacter->GetActorForwardVector().Rotation().Yaw;

	AttachedParticleComp = UGameplayStatics::SpawnEmitterAttached(Datas[0].Effect, BoxComp, "");
	AttachedParticleComp->SetRelativeTransform(Datas[0].EffectTransform);
}

void ACDoAction_WhirlWind::End_DoAction()
{
	Super::End_DoAction();

	StateComp->SetIdleMode();
	AttributeComp->SetMove();

	FTimerDynamicDelegate FinishDelegate;
	FinishDelegate.BindUFunction(this, "Finish");
	UKismetSystemLibrary::K2_SetTimerDelegate(FinishDelegate, ActiveTime, false);
}

void ACDoAction_WhirlWind::Abort()
{
	Finish();
}

void ACDoAction_WhirlWind::TickDamage()
{
	FDamageEvent DamageEvent;

	for (int32 i = 0; i < HittedCharacters.Num(); i++)
	{
		UCStateComponent* OtherStateComp = CHelpers::GetComponent<UCStateComponent>(HittedCharacters[i]);

		bool bIgnore = false;
		bIgnore |= (OtherStateComp->IsDeadMode());
		bIgnore |= (HittedCharacters[i]->IsPendingKill());
		bIgnore |= (HittedCharacters[i] == nullptr);
		if (bIgnore)
		{
			continue;
		}
		
		HittedCharacters[i]->TakeDamage(Datas[0].Power, DamageEvent, OwnerCharacter->GetController(), this);
	}
}

void ACDoAction_WhirlWind::Finish()
{
	bActivating = false;

	if (AttachedParticleComp)
	{
		AttachedParticleComp->DestroyComponent();
	}

	ACAttachment* Attachment = Cast<ACAttachment>(BoxComp->GetOwner());
	if (Attachment)
	{
		Attachment->OffCollision();
	}

	UKismetSystemLibrary::K2_ClearTimer(this, "TickDamage");
}

void ACDoAction_WhirlWind::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InCauser, InOtherCharacter);

	HittedCharacters.AddUnique(InOtherCharacter);
}

void ACDoAction_WhirlWind::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InCauser, InOtherCharacter);

	HittedCharacters.Remove(InOtherCharacter);
}
