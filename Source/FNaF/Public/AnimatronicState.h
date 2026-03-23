// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AnimatronicState.generated.h"

class AAnimatronic;

UCLASS(Blueprintable, BlueprintType, Abstract)
class FNAF_API UAnimatronicState : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void EnterState(AAnimatronic* OwnerAnimatronic);
	virtual void UpdateState(float DeltaTime);
	virtual void ExitState();
	
protected:
	UPROPERTY(BlueprintReadOnly)
	AAnimatronic* Animatronic;
	
	UPROPERTY()
	FTimerHandle TimerHandle;
};
