// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Identifiers.h"
#include "Animatronic.generated.h"


class UTimeSubsystem;
class UPowerSubsystem;
class ADoorActor;
class UAnimatronicState;
class AAnimatronicWaypoint;

USTRUCT(BlueprintType)
struct FRoomConnections
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ERooms> ConnectedRooms;
};

UCLASS()
class FNAF_API AAnimatronic : public AActor
{
	GENERATED_BODY()
	
public:	
	AAnimatronic();
	virtual void BeginPlay() override;
	
	void ChangeState(TSubclassOf<UAnimatronicState> NewStateClass);
	virtual void Tick( float DeltaTime ) override;
	
	UPROPERTY(EditAnywhere, Category = "Info")
	EAnimatronicID AnimatronicID;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	USkeletalMeshComponent* MeshComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float AILevel;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float MovingAttemptTime;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float TryToEnterOfficeTime = 10.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackTime = 10.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<UAnimatronicState> InitialState;
	
	UPROPERTY(EditAnywhere, Category = "Environment")
	ADoorActor* BlockingDoor;
	
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	UAnimatronicState* CurrentState;
	
protected:
	
private:
	
	UPROPERTY()
	UPowerSubsystem* CachedPowerSubsystem;
	
	UPROPERTY()
	UTimeSubsystem* CachedTimeSubsystem;
	
	void StopStateMachine();
	
	UFUNCTION()
	void HandleBatteryDrained();
	
	UFUNCTION()
	void HandleHourChanged(int NewHour);
};
