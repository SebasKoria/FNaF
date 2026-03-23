// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animatronic.h"
#include "AnimatronicState.h"
#include "RoamingAnimatronic.generated.h"

class AMainCharacter;
class ALightActor;
/**
 * 
 */
UCLASS()
class FNAF_API ARoamingAnimatronic : public AAnimatronic
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Environment")
	ALightActor* HallLight;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	ERooms InitialRoom;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	TMap<ERooms, FRoomConnections> RoomConnections;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Poses")
	TMap<ERooms, UAnimSequence*> RoomPoses;
};


UCLASS()
class FNAF_API URoamingState : public UAnimatronicState
{
	GENERATED_BODY()

public:
	virtual void EnterState(AAnimatronic* OwnerAnimatronic) override;
	virtual void ExitState() override;
	
	UPROPERTY(BlueprintReadOnly, Category = "AI")
	ERooms CurrentRoom;

private:
	
	UPROPERTY()
	ARoamingAnimatronic* RoamingAnimatronic;
	
	UPROPERTY()
	AAnimatronicWaypoint* CurrentWaypoint;
	
	void TryToMove();
	void MoveToRoom(const ERooms Room);
	bool GetPossibleWaypoints(const ERooms Room, TArray<AAnimatronicWaypoint*>& OutWaypoints);
};


UCLASS()
class FNAF_API UAtDoorState : public UAnimatronicState
{
	GENERATED_BODY()
	
public:
	
	virtual void EnterState(AAnimatronic* OwnerAnimatronic) override;
	virtual void ExitState() override;

protected:
	
	UFUNCTION()
	void HandleMonitorChanged(bool bIsMonitorOpen);
	
private:
	
	UPROPERTY()
	ARoamingAnimatronic* RoamingAnimatronic;
	
	void TryEnterOffice();
};


UCLASS()
class FNAF_API UInsideOfficeState : public UAnimatronicState
{
	GENERATED_BODY()
	
public:
	
	virtual void EnterState(AAnimatronic* OwnerAnimatronic) override;
	virtual void ExitState() override;

protected:
	
	UFUNCTION()
	void HandleMonitorChanged(bool bIsMonitorOpen);
	
private:
	
	UPROPERTY()
	AMainCharacter* CachedPlayer;
	
	UPROPERTY()
	ARoamingAnimatronic* RoamingAnimatronic;
	
	void Jumpscare();
};
