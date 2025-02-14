#include "CAIController.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "CEnemy_AI.h"
#include "CPlayer.h"


ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	BehaviorRange = 150.f;
	bDrawRange = true; 
	Segment = 32;

	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "BlackboardComp");
	CHelpers::CreateActorComponent<UCBehaviorComponent>(this, &BehaviorComp, "BehaviorComp");
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &PerceptionComp, "PerceptionComp");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600.f;
	Sight->LoseSightRadius = 800.f;
	Sight->PeripheralVisionAngleDegrees = 90.f;

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;

	PerceptionComp->ConfigureSense(*Sight);
	PerceptionComp->SetDominantSense(Sight->GetSenseImplementation()); // == Sight->GetClass
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnerEnemy = Cast<ACEnemy_AI>(InPawn);

	SetGenericTeamId(OwnerEnemy->GetTeamID());

	ensure(OwnerEnemy->GetBehaviorTree());
	UseBlackboard(OwnerEnemy->GetBehaviorTree()->BlackboardAsset, Blackboard);
	BehaviorComp->SetBlackboardComp(Blackboard);

	RunBehaviorTree(OwnerEnemy->GetBehaviorTree());

	PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

	PerceptionComp->OnPerceptionUpdated.Clear();
}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Center = OwnerEnemy->GetActorLocation();

	ACPlayer* Player = Cast<ACPlayer>(Blackboard->GetValueAsObject("PlayerKey"));
	if (Player)
	{
		UCStateComponent* StateComp = CHelpers::GetComponent<UCStateComponent>(Player);
		if (StateComp && StateComp->IsDeadMode())
		{
			Blackboard->SetValueAsObject("PlayerKey", nullptr);
		}
	}


	CheckFalse(bDrawRange);
	DrawDebugSphere(GetWorld(), Center, Sight->SightRadius, Segment, FColor::Green);
	DrawDebugSphere(GetWorld(), Center, BehaviorRange, Segment, FColor::Red);

	
}

float ACAIController::GetSightRadius()
{
	return Sight->SightRadius;
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> PerceivedActors;
	PerceptionComp->GetCurrentlyPerceivedActors(nullptr, PerceivedActors);

	ACPlayer* Player = nullptr;
	for (const auto& Actor : PerceivedActors)
	{
		Player = Cast<ACPlayer>(Actor);

		if (Player)
		{
			break;
		}
	}

	Blackboard->SetValueAsObject("PlayerKey", Player);
}
