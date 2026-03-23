// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"

#include "PowerSubsystem.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	UPowerSubsystem* PowerSubsystem = GetWorld()->GetSubsystem<UPowerSubsystem>();
	if (PowerSubsystem)
	{
		PowerSubsystem->OnBatteryDrained.AddDynamic(this, &AInteractableActor::Lock);
	}
}

void AInteractableActor::Interact_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("InteractableActor::Implementation"));
}
