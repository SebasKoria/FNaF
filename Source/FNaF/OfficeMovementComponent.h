// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OfficeMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FNAF_API UOfficeMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOfficeMovementComponent();
	
	void UpdateRotation(float InputDirection, float DeltaTime);

protected:
	
	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	float TurnSpeed = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	float MaxYaw = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	float MinYaw = -60.0f;

	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	float InterpSpeed = 5.0f;
	
private:
	float TargetYaw = 0.0f;
		
};
