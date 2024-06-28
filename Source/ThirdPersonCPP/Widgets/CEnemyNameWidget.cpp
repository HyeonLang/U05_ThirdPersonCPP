#include "CEnemyNameWidget.h"
#include "Components/TextBlock.h"

void UCEnemyNameWidget::SetName(FString InContrallerName, FString InPawnName)
{
	ControllerName->SetText(FText::FromString(InContrallerName));
	PawnName->SetText(FText::FromString(InPawnName));
}