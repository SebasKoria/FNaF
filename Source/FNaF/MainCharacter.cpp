// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"
#include "OfficeMovementComponent.h"
#include "Public/SecuritySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractionComponent.h"
#include "PowerSubsystem.h"
#include "TimeSubsystem.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (OfficeMappingContext)
		{
			Subsystem->AddMappingContext(OfficeMappingContext, 0);
		}
	}
	
	CachedPowerSubsystem = GetWorld()->GetSubsystem<UPowerSubsystem>();
	if (CachedPowerSubsystem) CachedPowerSubsystem->OnBatteryDrained.AddDynamic(this, &AMainCharacter::HandleBatteryDrained);
	
	CachedTimeSubsystem = GetWorld()->GetSubsystem<UTimeSubsystem>();
	if (CachedTimeSubsystem) CachedTimeSubsystem->OnHourChanged.AddDynamic(this, &AMainCharacter::HandleHourChange);
	
	OnGameOver.AddDynamic(this, &AMainCharacter::HandleGameOver);
	bShowMouseCursor = true;
	bCanToggleMonitor = true;
	bIsInputEnabled = true;
	
	const FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);

	OfficePawn = GetPawn();
	if (OfficePawn)
	{
		MovementComp = OfficePawn->FindComponentByClass<UOfficeMovementComponent>();
		SecurityComp = OfficePawn->FindComponentByClass<USecuritySystemComponent>(); 
	}
	
	if (MovementComp) MovementComp->ResetRotation();
}

void AMainCharacter::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (ClickAction)
		{
			EnhancedInputComp->BindAction(ClickAction, ETriggerEvent::Started, this, &AMainCharacter::OnClick);
		}
	}
}

void AMainCharacter::DisableCustomInput()
{
	bIsInputEnabled = false;
}

void AMainCharacter::ResetViewRotation()
{
	if (MovementComp) MovementComp->ResetRotation();
}

void AMainCharacter::OnClick()
{
	UE_LOG(LogTemp, Log, TEXT("InteractionSystem::Click action"));
	
	if (const auto InteractionComponent = GetPawn()->FindComponentByClass<UInteractionComponent>())
	{
		InteractionComponent->TryInteract(this);
	}
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsInputEnabled) return;

	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	float MouseX, MouseY;
	if (!GetMousePosition(MouseX, MouseY) || ViewportSizeX == 0 || ViewportSizeY == 0)
	{
		return;
	}

	const float NormalizedX = MouseX / static_cast<float>(ViewportSizeX);
	const float NormalizedY = MouseY / static_cast<float>(ViewportSizeY);

	switch (CurrentState)
	{
	case EPlayerState::Idle:
		HandleIdleState(NormalizedX, NormalizedY, DeltaTime);
		break;

	case EPlayerState::UsingMonitor:
		HandleMonitorState(NormalizedY);
		break;

	case EPlayerState::Transitioning:
		break;
	}
}

void AMainCharacter::HandleIdleState(float MouseX_Norm, float MouseY_Norm, float DeltaTime)
{
	float TurnInput = 0.0f;
	if (MouseX_Norm < 0.2f) TurnInput = -1.0f;
	else if (MouseX_Norm > 0.8f) TurnInput = 1.0f;

	if (MovementComp)
	{
		MovementComp->UpdateRotation(TurnInput, DeltaTime);
	}

	if (bCanUseMonitor)
	{
		if (MouseY_Norm > 0.9f)
		{
			if (bCanToggleMonitor)
			{
				if (SecurityComp)
				{
					bCanToggleMonitor = false;
					SecurityComp->OpenMonitor(this, OfficePawn);
					CurrentState = EPlayerState::UsingMonitor;
				
					OnMonitorOpened();
					OnMonitorStateChanged.Broadcast(true);
				}
			}
		}
		else
		{
			bCanToggleMonitor = true;
		}
	}
}

void AMainCharacter::HandleMonitorState(float MouseY_Norm)
{
	if (MouseY_Norm > 0.9f) 
	{
		if (bCanToggleMonitor)
		{
			if (SecurityComp)
			{
				bCanToggleMonitor = false;
				SecurityComp->CloseMonitor(this, OfficePawn);
				CurrentState = EPlayerState::Idle;
				
				OnMonitorClosed();
				OnMonitorStateChanged.Broadcast(false);
			}
		}
	}
	else
	{
		bCanToggleMonitor = true;
	}
}

void AMainCharacter::ForceCloseMonitor()
{
	if (SecurityComp)
	{
		bCanToggleMonitor = false;
		SecurityComp->CloseMonitor(this, OfficePawn);
		CurrentState = EPlayerState::Idle;
				
		OnMonitorClosed();
		OnMonitorStateChanged.Broadcast(false);
	}
	
	bCanUseMonitor = false;
}

// TODO: Implement in a GameSubsystem
// TODO: Make this function actually change to the next level and make another function that returns to main menu on GameOver
void AMainCharacter::ChangeToNextLevel()
{
	const FName LevelName = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld()));
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void AMainCharacter::HandleGameOver()
{
	// TODO: Implement GameOver screen
	OnGameOver.RemoveDynamic(this, &AMainCharacter::HandleGameOver);
	ChangeToNextLevel();
}

void AMainCharacter::HandleBatteryDrained()
{
	bCanUseMonitor = false;
	OnMonitorClosed();
	if (CachedPowerSubsystem) CachedPowerSubsystem->OnBatteryDrained.RemoveDynamic(this, &AMainCharacter::HandleBatteryDrained);
	
	// TODO: This is a placeholder power outage
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMainCharacter::HandleGameOver, 5.0f, false);
}

void AMainCharacter::HandleHourChange(int NewHour)
{
	if (NewHour >= 6)
	{
		// TODO: Implement 6am win screen
		DisableCustomInput();
		if (CachedTimeSubsystem) CachedTimeSubsystem->OnHourChanged.AddDynamic(this, &AMainCharacter::HandleHourChange);
		
		// TODO: Go to next level
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMainCharacter::HandleGameOver, 5.0f, false);
	}
}
