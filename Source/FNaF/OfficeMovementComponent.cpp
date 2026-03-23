// Fill out your copyright notice in the Description page of Project Settings.


#include "OfficeMovementComponent.h"

// Sets default values for this component's properties
UOfficeMovementComponent::UOfficeMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UOfficeMovementComponent::UpdateRotation(float InputDirection, float DeltaTime)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	TargetYaw += (InputDirection * TurnSpeed * DeltaTime);
	TargetYaw = FMath::Clamp(TargetYaw, MinYaw, MaxYaw);

	FRotator CurrentRot = Owner->GetActorRotation();
	FRotator TargetRot = FRotator(CurrentRot.Pitch, TargetYaw, CurrentRot.Roll);

	FRotator NewRotation = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, InterpSpeed);

	Owner->SetActorRotation(NewRotation);
}

void UOfficeMovementComponent::ResetRotation()
{
	AActor* Owner = GetOwner();
	if (!Owner) return;
	
	Owner->SetActorRotation(FRotator(0,-90,0));
}



