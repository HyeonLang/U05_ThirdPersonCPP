#include "CPatrolPath.h"
#include "global.h"
#include "Components/SplineComponent.h"
#include "Components/TextRenderComponent.h"

ACPatrolPath::ACPatrolPath()
{
	CHelpers::CreateSceneComponent(this, &RootComp, "RootComp");
	CHelpers::CreateSceneComponent(this, &SplineComp, "SplineComp", RootComp);
	CHelpers::CreateSceneComponent(this, &TextRenderComp, "TextRenderComp", RootComp);
	
	SplineComp->SetRelativeLocation(FVector(0, 0, 30));

	TextRenderComp->SetRelativeLocation(FVector(0, 0, 100));
	TextRenderComp->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	TextRenderComp->TextRenderColor = FColor::Red;

}

void ACPatrolPath::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	TextRenderComp->SetText(GetName());

	SplineComp->SetClosedLoop(bClosedLoop);
}

void ACPatrolPath::BeginPlay()
{
	Super::BeginPlay();
	
	TextRenderComp->SetVisibility(false);
}

