// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CActionChangeWidget.generated.h"

class ACPlayer;

UCLASS()
class THIRDPERSONCPP_API UCActionChangeWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UFUNCTION(BlueprintCallable)
	void SetOnFist(ACPlayer* Player);

	UFUNCTION(BlueprintCallable)
	void SetOnOneHand(ACPlayer* Player);

	UFUNCTION(BlueprintCallable)
	void SetOnTwoHand(ACPlayer* Player);

	UFUNCTION(BlueprintCallable)
	void SetOnMagicBall(ACPlayer* Player);

	UFUNCTION(BlueprintCallable)
	void SetOnWarp(ACPlayer* Player);

	UFUNCTION(BlueprintCallable)
	void SetOnWhirlWind(ACPlayer* Player);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetAction();
};
