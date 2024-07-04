#include "ThirdPersonCPPGameModeBase.h"
#include "Global.h"
#include "CHUD.h"


AThirdPersonCPPGameModeBase::AThirdPersonCPPGameModeBase()
{
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "/Game/Player/BP_CPlayer");
	CHelpers::GetClass<AHUD>(&HUDClass, "/Game/Game/BP_HUD");
}