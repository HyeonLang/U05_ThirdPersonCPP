#include "CAIController.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "CEnemy_AI.h"


ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "BlackboardComp");
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ensure(OwnerEnemy->GetBehaviorTree());
	OwnerEnemy = Cast<ACEnemy_AI>(InPawn);
	UseBlackboard(OwnerEnemy->GetBehaviorTree()->BlackboardAsset, Blackboard);

	RunBehaviorTree(OwnerEnemy->GetBehaviorTree());
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();


}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
