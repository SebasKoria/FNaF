// Fill out your copyright notice in the Description page of Project Settings.

#include "Animatronic.h"

#include "AnimatronicState.h"
#include "PowerSubsystem.h"
#include "TimeSubsystem.h"


AAnimatronic::AAnimatronic()
{
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(FName("MeshComp"));
}

void AAnimatronic::BeginPlay()
{
	if (InitialState)
	{
		ChangeState(InitialState);
	}
	
	CachedPowerSubsystem = GetWorld()->GetSubsystem<UPowerSubsystem>();
	if (CachedPowerSubsystem)
	{
		CachedPowerSubsystem->OnBatteryDrained.AddDynamic(this, &AAnimatronic::HandleBatteryDrained);
	}
	
	CachedTimeSubsystem = GetWorld()->GetSubsystem<UTimeSubsystem>();
	if (CachedTimeSubsystem)
	{
		CachedTimeSubsystem->OnHourChanged.AddDynamic(this, &AAnimatronic::HandleHourChanged);
	}
}

void AAnimatronic::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	UE_LOG(LogTemp, Log, TEXT("Tick"));
	if (CurrentState)
	{
		CurrentState->UpdateState(DeltaTime);
	}
}

void AAnimatronic::HandleBatteryDrained()
{
	StopStateMachine();
	if (CachedPowerSubsystem)
	{
		CachedPowerSubsystem->OnBatteryDrained.RemoveDynamic(this, &AAnimatronic::HandleBatteryDrained);
	}
}

void AAnimatronic::HandleHourChanged(int NewHour)
{
	if (NewHour >= 2 && NewHour <= 4)
	{
		AILevel++;
	}
	
	if (NewHour >= 6)
	{
		StopStateMachine();
		if (CachedTimeSubsystem)
		{
			CachedTimeSubsystem->OnHourChanged.RemoveDynamic(this, &AAnimatronic::HandleHourChanged);
		}
	}
}

void AAnimatronic::StopStateMachine()
{
	if (CurrentState)
	{
		CurrentState->ExitState();
	}
}

void AAnimatronic::ChangeState(TSubclassOf<UAnimatronicState> NewState)
{
	if (CurrentState)
	{
		CurrentState->ExitState();
	}
	
	if (NewState)
	{
		CurrentState = NewObject<UAnimatronicState>(this, NewState);
		CurrentState->EnterState(this);
	}
}
