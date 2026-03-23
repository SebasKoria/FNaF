// Fill out your copyright notice in the Description page of Project Settings.


#include "RoamingAnimatronic.h"

#include "AnimatronicWaypoint.h"
#include "EngineUtils.h"
#include "InteractableActor.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "FNaF/MainCharacter.h"
#include "Kismet/GameplayStatics.h"


/*
 * =============================================================================
 *                              ROAMING STATE
 * =============================================================================
 */

void URoamingState::EnterState(AAnimatronic* OwnerAnimatronic)
{
	if ((RoamingAnimatronic = Cast<ARoamingAnimatronic>(OwnerAnimatronic)))
	{
		Super::EnterState(OwnerAnimatronic);
		MoveToRoom(RoamingAnimatronic->InitialRoom);
	
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&URoamingState::TryToMove,
			Animatronic->MovingAttemptTime,
			true
			);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot use RoamingState on an Animatronic that is not of type ARoamingAnimatronic"));
	}
	
}

void URoamingState::ExitState()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	Super::ExitState();
}

void URoamingState::TryToMove()
{
	if (FMath::RandRange(1, 20) <= Animatronic->AILevel)
	{
		TArray<ERooms> PossibleRooms = RoamingAnimatronic->RoomConnections[CurrentRoom].ConnectedRooms;
		
		const int Tries = (Animatronic->AILevel + 5) / 6;
		int NextRoomIndex = 0;
		
		for (auto It = 0; It < Tries; ++It)
		{
			NextRoomIndex = FMath::Max(NextRoomIndex, FMath::RandRange(0, PossibleRooms.Num() - 1));
		}
		
		MoveToRoom(PossibleRooms[NextRoomIndex]);
	}
}

void URoamingState::MoveToRoom(const ERooms Room)
{
	TArray<AAnimatronicWaypoint*> PossibleWaypoints;
	if (GetPossibleWaypoints(Room, PossibleWaypoints))
	{
		AAnimatronicWaypoint* NextWaypoint = PossibleWaypoints[FMath::RandRange(0, PossibleWaypoints.Num() - 1)];
		CurrentWaypoint = NextWaypoint;
		
		Animatronic->SetActorLocation(NextWaypoint->GetActorLocation());
		Animatronic->SetActorRotation(NextWaypoint->GetActorRotation());
		
		CurrentRoom = Room;
		
		if (CurrentRoom == ERooms::WestDoor || CurrentRoom == ERooms::EastDoor)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
			Animatronic->ChangeState(UAtDoorState::StaticClass());
		}
	}
}

bool URoamingState::GetPossibleWaypoints(const ERooms Room, TArray<AAnimatronicWaypoint*>& OutWaypoints)
{
	if (const UWorld* World = GetWorld())
	{
		for (TActorIterator<AAnimatronicWaypoint> It(World); It; ++It)
		{
			AAnimatronicWaypoint* Waypoint = *It;
			if (Waypoint->RoomTag == Room && Waypoint->AllowedAnimatronics.Contains(Animatronic->AnimatronicID))
			{
				if (CurrentWaypoint != Waypoint)
				{
					OutWaypoints.Add(Waypoint);
				}
			}
		}
	}
	
	return OutWaypoints.Num() > 0;
}

/*
 * =============================================================================
 *                              AT DOOR STATE
 * =============================================================================
 */

void UAtDoorState::EnterState(AAnimatronic* OwnerAnimatronic)
{
	Super::EnterState(OwnerAnimatronic);
	
	if ((RoamingAnimatronic = Cast<ARoamingAnimatronic>(OwnerAnimatronic)))
	{
		Super::EnterState(OwnerAnimatronic);
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&UAtDoorState::TryEnterOffice,
			Animatronic->TryToEnterOfficeTime,
			false
			);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot use AtDoorState on an Animatronic that is not of type ARoamingAnimatronic"));
	}
}

void UAtDoorState::TryEnterOffice()
{
	if (Animatronic->BlockingDoor->bIsClosed)
	{
		// Door closed, return to roaming state
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		Animatronic->ChangeState(URoamingState::StaticClass());
	}
	else
	{
		if (AMainCharacter* Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			if (Player->IsUsingMonitor())
			{
				// Player is distracted watching cameras, get in the office
				UE_LOG(LogTemp, Log, TEXT("An Animatronic has entered the office"));
				Animatronic->ChangeState(UInsideOfficeState::StaticClass());
			}
			else
			{
				// Lock the door and turn off the light
				if (ADoorActor* BlockingDoor = RoamingAnimatronic->BlockingDoor)
					BlockingDoor->Lock();
				
				if (ALightActor* HallLight = RoamingAnimatronic->HallLight)
				HallLight->Lock();
				
				// Wait until the player uses the monitor to enter the office
				Player->OnMonitorStateChanged.AddDynamic(this, &UAtDoorState::HandleMonitorChanged);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Couldn't find a player"))
		}
	}
}

void UAtDoorState::HandleMonitorChanged(bool bIsMonitorOpen)
{
	if (bIsMonitorOpen)
	{
		// Player is distracted watching cameras, get in the office
		UE_LOG(LogTemp, Log, TEXT("An Animatronic has entered the office"));
				Animatronic->ChangeState(UInsideOfficeState::StaticClass());
	}
}

void UAtDoorState::ExitState()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	if (AMainCharacter* Player = Cast<AMainCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		Player->OnMonitorStateChanged.RemoveDynamic(this, &UAtDoorState::HandleMonitorChanged);
	}
	
	Super::ExitState();
}

/*
 * =============================================================================
 *                            INSIDE OFFICE STATE
 * =============================================================================
 */

void UInsideOfficeState::EnterState(AAnimatronic* OwnerAnimatronic)
{
	Super::EnterState(OwnerAnimatronic);
	CachedPlayer = Cast<AMainCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	if (CachedPlayer)
	{
		CachedPlayer->OnMonitorStateChanged.AddDynamic(this, &UInsideOfficeState::HandleMonitorChanged);
		
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&UInsideOfficeState::Jumpscare,
			Animatronic->AttackTime,
			false
			);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't find a player"))
	}
	
}

void UInsideOfficeState::HandleMonitorChanged(bool bIsMonitorOpen)
{
	if (!bIsMonitorOpen)
	{
		Jumpscare();
	}
}

void UInsideOfficeState::Jumpscare()
{
	Animatronic->SetActorLocation(FVector(0,2025,0));  // Hardcoded location (center of the office)
	
	if (CachedPlayer)
	{
		CachedPlayer->ForceCloseMonitor();
		CachedPlayer->ResetViewRotation();
		CachedPlayer->DisableCustomInput();
	}
	
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			FTimerDelegate::CreateLambda([this]()
			{
				if (CachedPlayer) CachedPlayer->OnGameOver.Broadcast();
			}),
			3.0f,
			false
			);
}

void UInsideOfficeState::ExitState()
{
	//CachedPlayer->OnMonitorStateChanged.RemoveDynamic(this, &UInsideOfficeState::HandleMonitorChanged);
	Super::ExitState();
}
