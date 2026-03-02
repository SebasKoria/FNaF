// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle            UMETA(DisplayName = "En Oficina"),
	UsingMonitor    UMETA(DisplayName = "Viendo Cámaras"),
	WearingMask     UMETA(DisplayName = "Con Máscara"),
	Transitioning   UMETA(DisplayName = "En Transición")
};

UCLASS()
class FNAF_API AMainCharacter : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Security UI")
	void OnMonitorOpened();

	UFUNCTION(BlueprintImplementableEvent, Category = "Security UI")
	void OnMonitorClosed();

protected:
	UPROPERTY(BlueprintReadWrite, Category = "State")
	EPlayerState CurrentState = EPlayerState::Idle;

private:
	class UOfficeMovementComponent* MovementComp;
	class USecuritySystemComponent* SecurityComp;
	
	AActor* OfficePawn;
	bool CanToggleMonitor = true;

	void HandleIdleState(float MouseX_Norm, float MouseY_Norm, float DeltaTime);
	void HandleMonitorState(float MouseY_Norm);
};
