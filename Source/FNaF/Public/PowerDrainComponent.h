// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PowerDrainComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FNAF_API UPowerDrainComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPowerDrainComponent();
	
	UFUNCTION(BlueprintCallable, Category = PowerDrainComponent)
	void StartUsingPower();
	
	UFUNCTION(BlueprintCallable, Category = PowerDrainComponent)
	void StopUsingPower();
};
