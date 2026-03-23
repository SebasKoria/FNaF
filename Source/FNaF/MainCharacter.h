// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainCharacter.generated.h"

class UTimeSubsystem;
class UPowerSubsystem;
class USecuritySystemComponent;
class UOfficeMovementComponent;
class UInputMappingContext;
class UInputAction;

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle            UMETA(DisplayName = "AtOffice"),
	UsingMonitor    UMETA(DisplayName = "UsingMonitor"),
	Transitioning   UMETA(DisplayName = "Transitioning")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMonitorStateChanged, bool, bIsMonitorOpen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverDelegate);

UCLASS()
class FNAF_API AMainCharacter : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Security UI")
	void OnMonitorOpened();

	UFUNCTION(BlueprintImplementableEvent, Category = "Security UI")
	void OnMonitorClosed();
	
	UFUNCTION(BlueprintCallable, Category = "Security UI")
	void ForceCloseMonitor();
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DisableCustomInput();
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	void ResetViewRotation();
	
	UPROPERTY(EditAnywhere, Category = "Events")
	FOnMonitorStateChanged OnMonitorStateChanged;
	
	UPROPERTY(EditAnywhere, Category = "Events")
	FGameOverDelegate OnGameOver;

	bool IsUsingMonitor() const { return bIsUsingMonitor; }

protected:
	UPROPERTY(BlueprintReadWrite, Category = "State")
	EPlayerState CurrentState = EPlayerState::Idle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* OfficeMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ClickAction;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsUsingMonitor = false;

private:
	
	UPROPERTY()
	UOfficeMovementComponent* MovementComp;
	
	UPROPERTY()
	USecuritySystemComponent* SecurityComp;
	
	UPROPERTY()
	UPowerSubsystem* CachedPowerSubsystem;
	
	UPROPERTY()
	UTimeSubsystem* CachedTimeSubsystem;
	
	UPROPERTY()
	AActor* OfficePawn;
	
	// Flag to avoid the camera monitor from opening until the player puts the cursor away from the camera toggle
	bool bCanToggleMonitor = true;
	// Flag to tell whether the player can use the monitor at all (i.e. there is power available)
	bool bCanUseMonitor = true;
	bool bIsInputEnabled = true;

	void HandleIdleState(float MouseX_Norm, float MouseY_Norm, float DeltaTime);
	void HandleMonitorState(float MouseY_Norm);
	void OnClick();
	void ChangeToNextLevel();
	
	UFUNCTION()
	void HandleGameOver();
	
	UFUNCTION()
	void HandleBatteryDrained();
	
	UFUNCTION()
	void HandleHourChange(int NewHour);
};
