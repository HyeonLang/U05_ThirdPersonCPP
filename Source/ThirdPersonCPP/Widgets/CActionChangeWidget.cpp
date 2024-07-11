#include "CActionChangeWidget.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CStateComponent.h"
#include "Components/CActionComponent.h"


void UCActionChangeWidget::SetOnFist(ACPlayer* Player)
{
	CheckNull(Player);
	CheckFalse(Player->GetStateComp()->IsIdleMode());
	Player->GetActionComp()->SetFistMode();
}

void UCActionChangeWidget::SetOnOneHand(ACPlayer* Player)
{
	CheckNull(Player);
	CheckFalse(Player->GetStateComp()->IsIdleMode());
	Player->GetActionComp()->SetOneHandMode();
}

void UCActionChangeWidget::SetOnTwoHand(ACPlayer* Player)
{
	CheckNull(Player);
	CheckFalse(Player->GetStateComp()->IsIdleMode());
	Player->GetActionComp()->SetTwoHandMode();
}

void UCActionChangeWidget::SetOnMagicBall(ACPlayer* Player)
{
	CheckNull(Player);
	CheckFalse(Player->GetStateComp()->IsIdleMode());
	Player->GetActionComp()->SetMagicBallMode();
}

void UCActionChangeWidget::SetOnWarp(ACPlayer* Player)
{
	CheckNull(Player);
	CheckFalse(Player->GetStateComp()->IsIdleMode());
	Player->GetActionComp()->SetWarpMode();
}

void UCActionChangeWidget::SetOnWhirlWind(ACPlayer* Player)
{
	CheckNull(Player);
	CheckFalse(Player->GetStateComp()->IsIdleMode());
	Player->GetActionComp()->SetWhirlwindMode();
}
