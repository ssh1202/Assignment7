#include "SpartaPawn.h"
#include "SpartaPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASpartaPawn::ASpartaPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

}

void ASpartaPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaPawn::Move
				);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&ASpartaPawn::Look
				);
			}
		}
	}
}

void ASpartaPawn::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	FVector2D MoveValue = value.Get<FVector2D>();

	const FVector ForwardDir (1, 0, 0);
	const FVector RightDir (0, 1, 0);

	FVector DeltaLocation = ForwardDir * MoveValue.X + RightDir * MoveValue.Y;
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	AddActorLocalOffset(DeltaLocation * Speed * DeltaTime, true);
}

void ASpartaPawn::Look(const FInputActionValue& value)
{
	if (!Controller) return;

	FVector2D LookValue = value.Get<FVector2D>();
	
	FRotator DeltaRotationYaw(0.f, LookValue.X, 0.f);

	float DeltaTime = GetWorld()->GetDeltaSeconds();

	AddActorLocalRotation(DeltaRotationYaw * Speed * DeltaTime, false);
	
	FRotator SpringArmRotation = SpringArmComp->GetRelativeRotation();

	float SpringSpeed = 100.f;

	FRotator DeltaRotationPitch(LookValue.Y, 0.f, 0.f);

	SpringArmComp->AddLocalRotation(DeltaRotationPitch * SpringSpeed * DeltaTime);

	SpringArmRotation.Pitch = FMath::Clamp(SpringArmRotation.Pitch, -30.f, 30.f);

}


