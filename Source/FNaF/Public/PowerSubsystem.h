#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PowerSubsystem.generated.h"

class UTimeSubsystem;
class AMainCharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBatteryChanged, float, NewBatteryLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUsageChanged, int32, NewUsageLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBatteryDrained);

/**
 * 
 */
UCLASS()
class FNAF_API UPowerSubsystem : public UWorldSubsystem, public FTickableGameObject
{
    GENERATED_BODY()

public:
    virtual void OnWorldBeginPlay(UWorld& InWorld) override;

    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override;
    virtual bool IsTickable() const override;

    UFUNCTION(BlueprintCallable, Category = "Power System")
    void IncreaseUsage();

    UFUNCTION(BlueprintCallable, Category = "Power System")
    void DecreaseUsage();

    UPROPERTY(BlueprintAssignable, Category = "Power System|Events")
    FOnBatteryChanged OnBatteryChanged;

    UPROPERTY(BlueprintAssignable, Category = "Power System|Events")
    FOnUsageChanged OnUsageChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Power System|Events")
    FOnBatteryDrained OnBatteryDrained;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power System")
    float MaxBattery = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power System")
    float BaseDrainRate = 0.1f;

private:
    float CurrentBattery;
    int32 CurrentUsageLevel;
    bool bIsPowerDead;
    bool bIsGameOver;
    
    UPROPERTY()
    AMainCharacter* CachedPlayer;
    
    UPROPERTY()
    UTimeSubsystem* CachedTimeSubsystem;
    
    UFUNCTION()
    void HandleHourChanged(int NewHour);
};