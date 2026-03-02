#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PowerManagerComponent.generated.h"

// 1. Declaramos los "Delegados" (Eventos para Blueprints)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBatteryChanged, float, NewBatteryLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUsageChanged, int32, NewUsageLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBatteryDrained);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FNAF_API UPowerManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPowerManagerComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Funciones para modificar el uso de energía
	UFUNCTION(BlueprintCallable, Category = "Power Management")
	void IncreaseUsage();

	UFUNCTION(BlueprintCallable, Category = "Power Management")
	void DecreaseUsage();

	// Eventos que podremos escuchar en Blueprints (UI o Game Over)
	UPROPERTY(BlueprintAssignable, Category = "Power Management|Events")
	FOnBatteryChanged OnBatteryChanged;

	UPROPERTY(BlueprintAssignable, Category = "Power Management|Events")
	FOnUsageChanged OnUsageChanged;

	UPROPERTY(BlueprintAssignable, Category = "Power Management|Events")
	FOnBatteryDrained OnBatteryDrained;

protected:
	// Variables configurables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power Management")
	float MaxBattery = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power Management")
	float BaseDrainRate = 0.15f; // Cuánta batería se gasta por segundo por cada nivel de uso

private:
	float CurrentBattery;
	int32 CurrentUsageLevel; // 1 = Solo oficina, 2 = Oficina + Monitor, etc.
	bool bIsPowerDead; // Para saber si ya perdimos
};