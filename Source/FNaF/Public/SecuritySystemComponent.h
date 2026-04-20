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

	void OpenMonitor(APlayerController* PC, AActor* OfficePawn);
	void CloseMonitor(APlayerController* PC, AActor* OfficePawn);
	bool GetIsMonitorOpen() const { return bIsMonitorOpen; }
	
	UFUNCTION(BlueprintCallable, Category = "Security")
	void SwitchToCamera(APlayerController* PC, int32 CameraIndex);

protected:
	
	UPROPERTY(EditAnywhere, Category = "Security Cameras")
	TArray<AActor*> Cameras;

private:
	
	bool bIsMonitorOpen = false;
	int32 CurrentCameraIndex = 0;
};
