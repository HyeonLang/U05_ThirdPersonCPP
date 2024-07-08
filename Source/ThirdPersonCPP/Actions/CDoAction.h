#pragma once

#include "CoreMinimal.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CAttributeComponent.h"
#include "Components/CStateComponent.h"
#include "CActionData.h"
#include "CDoAction.generated.h"

class ACharacter;
class UCAttributeComponent;
class UCStateComponent;

UCLASS()
class THIRDPERSONCPP_API ACDoAction : public AActor
{
	GENERATED_BODY()
	
public:	
	ACDoAction();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void DoAction() {};
	virtual void Begin_DoAction() {};
	virtual void End_DoAction() {};

	virtual void Begin_SubAction() {};
	virtual void End_SubAction() {};

	virtual void Abort() {};

public:
	UFUNCTION()
	virtual void OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter) {};
	UFUNCTION()
	virtual void OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter) {};

public:
	void SetDatas(const TArray<FDoActionData>& InDatas);
	void SetEquipped(const bool* InEquipped);

protected:
	UPROPERTY(BlueprintReadOnly)
	ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
	UCAttributeComponent* AttributeComp;

	UPROPERTY(BlueprintReadOnly)
	UCStateComponent* StateComp;

protected:
	TArray<FDoActionData> Datas;
	const bool* bEquipped;
};
