#include "CActionData.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "CEquipment.h"
#include "CAttachment.h"
#include "CDoAction.h"
#include "CAction.h"

void UCActionData::BeginPlay(ACharacter* InOwnerCharacter, UCAction** OutAction)
{
	FTransform Transform;

	ACAttachment* Attachment = nullptr;
	if (AttachmentClass)
	{
		Attachment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACAttachment>(AttachmentClass, Transform, InOwnerCharacter);
		//Attachment->SetActorLabel(MakeActorLabel(InOwnerCharacter, "Attachment"));
		Attachment->FinishSpawning(Transform);
	}

	ACEquipment* Equipment = nullptr;
	if (EquipmentClass) 
	{
		Equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EquipmentClass, Transform, InOwnerCharacter);
		Equipment->SetData(EquipmentData);
		Equipment->SetColor(EquipmentColor);
		//Equipment->SetActorLabel(MakeActorLabel(InOwnerCharacter, "Equipment"));
		Equipment->FinishSpawning(Transform);

		if (Attachment)
		{
			Equipment->OnEquipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnEquip);
			Equipment->OnUnequipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnUnequip);
		}
	}

	ACDoAction* DoAction = nullptr;
	if (DoActionClass)
	{
		DoAction = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACDoAction>(DoActionClass, Transform, InOwnerCharacter);
		DoAction->SetDatas(DoActionDatas);
		//DoAction->SetActorLabel(MakeActorLabel(InOwnerCharacter, "DoAction"));
		DoAction->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		DoAction->FinishSpawning(Transform);

		if (Attachment) 
		{
			Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &ACDoAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(DoAction, &ACDoAction::OnAttachmentEndOverlap);
		}

		if (Equipment)
		{
			DoAction->SetEquipped(Equipment->IsEquipped());
		}
	}
	
	*OutAction = NewObject<UCAction>();
	(*OutAction)->Attachment = Attachment;
	(*OutAction)->Equipment = Equipment;
	(*OutAction)->DoAction = DoAction;
}

FString UCActionData::MakeActorLabel(ACharacter* InOwnerCharacter, FString InMiddleName)
{
	FString Label;
	/*Label.Append(InOwnerCharacter->GetActorLabel());
	Label.Append("_");
	Label.Append(InMiddleName);
	Label.Append("_");
	Label.Append(GetName().Replace(TEXT("DA_"), TEXT("")));*/

	return Label;
}
