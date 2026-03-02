// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"

#include "OfficeMovementComponent.h"
#include "Public/SecuritySystemComponent.h"

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Mostrar el mouse y configurar input para UI y Juego
	bShowMouseCursor = true;
	const FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);

	// Buscar el componente de movimiento en el Pawn que controlamos
	OfficePawn = GetPawn();
	if (OfficePawn)
	{
		MovementComp = OfficePawn->FindComponentByClass<UOfficeMovementComponent>();
		SecurityComp = OfficePawn->FindComponentByClass<USecuritySystemComponent>(); 
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. Obtener tamaño de pantalla y posición del mouse
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	float MouseX, MouseY;
	if (!GetMousePosition(MouseX, MouseY) || ViewportSizeX == 0 || ViewportSizeY == 0)
	{
		return; // Salir si no hay mouse o la pantalla no es válida
	}

	// 2. Normalizar coordenadas (0.0 a 1.0)
	float NormalizedX = MouseX / (float)ViewportSizeX;
	float NormalizedY = MouseY / (float)ViewportSizeY;

	// 3. Máquina de Estados
	switch (CurrentState)
	{
	case EPlayerState::Idle:
		HandleIdleState(NormalizedX, NormalizedY, DeltaTime);
		break;

	case EPlayerState::UsingMonitor:
		HandleMonitorState(NormalizedY);
		break;

	case EPlayerState::WearingMask:
		// Lógica similar a Idle pero sin abrir monitor
		break;

	case EPlayerState::Transitioning:
		// No hacer nada, esperar a que termine la animación
		break;
	}
}

void AMainCharacter::HandleIdleState(float MouseX_Norm, float MouseY_Norm, float DeltaTime)
{
	// --- LÓGICA DE CÁMARA HORIZONTAL ---
	float TurnInput = 0.0f;
	if (MouseX_Norm < 0.2f)      TurnInput = -1.0f; // Izquierda
	else if (MouseX_Norm > 0.8f) TurnInput = 1.0f;  // Derecha

	// Enviar el input al componente
	if (MovementComp)
	{
		MovementComp->UpdateRotation(TurnInput, DeltaTime);
	}

	// --- LÓGICA DE MONITOR VERTICAL ---
	if (MouseY_Norm > 0.9f) // El mouse está casi tocando el borde inferior
	{
		if (CanToggleMonitor)
		{
			if (SecurityComp)
			{
				CanToggleMonitor = false;
				SecurityComp->OpenMonitor(this, OfficePawn);
				CurrentState = EPlayerState::UsingMonitor; // ¡Cambiamos de estado!
				
				OnMonitorOpened();
			}
		}
	}
	else
	{
		CanToggleMonitor = true;
	}
}

void AMainCharacter::HandleMonitorState(float MouseY_Norm)
{
	// En este estado no movemos la cámara. Solo escuchamos si quiere salir.
	if (MouseY_Norm > 0.9f) 
	{
		if (CanToggleMonitor)
		{
			if (SecurityComp)
			{
				CanToggleMonitor = false;
				SecurityComp->CloseMonitor(this, OfficePawn);
				CurrentState = EPlayerState::Idle; // ¡Volvemos a la oficina!
				
				OnMonitorClosed();
			}
		}
	}
	else
	{
		CanToggleMonitor = true;
	}
}
