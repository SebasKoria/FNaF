#include "PowerManagerComponent.h"

UPowerManagerComponent::UPowerManagerComponent()
{
	// Activar el Tick para que la batería baje constantemente
	PrimaryComponentTick.bCanEverTick = true;
	
	// Valores por defecto
	CurrentUsageLevel = 1; // Siempre empezamos con nivel 1 (luz de la oficina)
	bIsPowerDead = false;
}

void UPowerManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Iniciar la batería al máximo
	CurrentBattery = MaxBattery;

	// Avisar a la UI de los valores iniciales
	OnBatteryChanged.Broadcast(CurrentBattery);
	OnUsageChanged.Broadcast(CurrentUsageLevel);
}

void UPowerManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Si ya no hay energía, no hacemos nada
	if (bIsPowerDead) return;

	// Calcular cuánto drenar en este frame (Rate * Nivel de Uso * DeltaTime)
	float DrainAmount = BaseDrainRate * CurrentUsageLevel * DeltaTime;
	CurrentBattery -= DrainAmount;

	// Transmitir el nuevo valor de batería a la UI
	OnBatteryChanged.Broadcast(CurrentBattery);

	// Chequear si se acabó la batería
	if (CurrentBattery <= 0.0f)
	{
		CurrentBattery = 0.0f;
		bIsPowerDead = true;
		
		// Disparar el evento de Derrota / Apagón
		OnBatteryDrained.Broadcast(); 
	}
}

void UPowerManagerComponent::IncreaseUsage()
{
	if (bIsPowerDead) return;

	CurrentUsageLevel++;
	// Avisar a la UI (para encender otra rayita verde de uso)
	OnUsageChanged.Broadcast(CurrentUsageLevel); 
}

void UPowerManagerComponent::DecreaseUsage()
{
	if (bIsPowerDead) return;

	// Evitar que el uso baje de 1 (la oficina siempre gasta)
	if (CurrentUsageLevel > 1)
	{
		CurrentUsageLevel--;
		OnUsageChanged.Broadcast(CurrentUsageLevel);
	}
}