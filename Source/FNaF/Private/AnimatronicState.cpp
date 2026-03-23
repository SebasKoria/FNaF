// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimatronicState.h"

void UAnimatronicState::EnterState(AAnimatronic* OwnerAnimatronic)
{
	Animatronic = OwnerAnimatronic;
}

void UAnimatronicState::UpdateState(float DeltaTime)
{
}

void UAnimatronicState::ExitState()
{
}
