#include "ThirdPersonCPPGameModeBase.h"
#include "Global.h"

AThirdPersonCPPGameModeBase::AThirdPersonCPPGameModeBase()
{
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "/Game/Player/BP_CPlayer");
}