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

	// 1. Calcular el nuevo Yaw objetivo basado en el input (-1 o 1)
	TargetYaw += (InputDirection * TurnSpeed * DeltaTime);

	// 2. Limitar el ángulo para que no dé la vuelta completa (FNaF style)
	TargetYaw = FMath::Clamp(TargetYaw, MinYaw, MaxYaw);

	// 3. Obtener la rotación actual
	FRotator CurrentRot = Owner->GetActorRotation();
	FRotator TargetRot = FRotator(CurrentRot.Pitch, TargetYaw, CurrentRot.Roll);

	// 4. Interpolar suavemente
	FRotator NewRotation = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, InterpSpeed);

	// 5. Aplicar la rotación
	Owner->SetActorRotation(NewRotation);
}

