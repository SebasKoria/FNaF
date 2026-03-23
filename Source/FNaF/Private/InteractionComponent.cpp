// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractionComponent.h"
#include "Interactable.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::TryInteract(APlayerController* PC)
{
	if (PC)
	{
		FHitResult HitResult;
		if (PC->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(TraceChannel), true, HitResult))
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor)
			{
				if (HitActor->Implements<UInteractable>())
				{
					UE_LOG(LogTemp, Log, TEXT("InteractionSystem::Clicked an InteractableActor"));
					IInteractable::Execute_Interact(HitActor);
				}
			}
		}
	}
}
