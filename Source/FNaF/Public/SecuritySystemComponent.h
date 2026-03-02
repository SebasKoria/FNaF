// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SecuritySystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FNAF_API USecuritySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USecuritySystemComponent();

	// Funciones principales
	void OpenMonitor(APlayerController* PC, AActor* OfficePawn);
	void CloseMonitor(APlayerController* PC, AActor* OfficePawn);
	
	UFUNCTION(BlueprintCallable, Category = "Security")
	void SwitchToCamera(APlayerController* PC, int32 CameraIndex);

protected:
	// Lista de cámaras en el nivel que asignaremos desde el Editor
	UPROPERTY(EditAnywhere, Category = "Security Cameras")
	TArray<AActor*> Cameras;

private:
	// Para recordar en qué cámara estábamos antes de cerrar el monitor
	int32 CurrentCameraIndex = 0;
};
