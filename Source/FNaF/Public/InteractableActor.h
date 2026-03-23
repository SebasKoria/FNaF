// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS()
class FNAF_API AInteractableActor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();
	virtual void Interact_Implementation() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void Lock();

protected:
	virtual void BeginPlay() override;
};


UCLASS()
class FNAF_API ADoorActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door State")
	bool bIsClosed = false;
};


UCLASS()
class FNAF_API ALightActor : public AInteractableActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light State")
	bool bIsOn = false;
};

