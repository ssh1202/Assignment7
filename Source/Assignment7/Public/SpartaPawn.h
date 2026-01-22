#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "SpartaPawn.generated.h"


class USpringArmComponent;
class UCameraComponent;

struct FInputActionValue;

UCLASS()
class ASSIGNMENT7_API ASpartaPawn : public APawn
{
	GENERATED_BODY()

public:
	ASpartaPawn();

protected:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Components|CapsuleCompoent")
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Components|SkeletalMeshComponent")
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* CameraComp;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);

	float Speed = 600.f;
};
