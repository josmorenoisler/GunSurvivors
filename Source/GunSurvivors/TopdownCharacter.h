// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "InputActionValue.h"
#include "Engine/TimerHandle.h"
#include "Sound/SoundBase.h"
#include "Bullet.h"
#include "TopdownCharacter.generated.h"

#define UNIT_VECTOR			FVector(1.0f, 1.0f, 1.0f)
#define UNIT_VECTOR_MINUSX	FVector(-1.0f, 1.0f, 1.0f)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDiedDelegate);

UCLASS()
class GUNSURVIVORS_API ATopdownCharacter : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* CharacterFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbook* IdleFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPaperFlipbook* RunFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* BulletShootSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundBase* DieSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* GunParent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* BulletSpawnPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent* GunSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D HorizontalLimits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D VerticalLimits;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABullet> BulletActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed = 100.0f;

	UPROPERTY(BlueprintReadWrite)
	FVector2D MovementDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanMove = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanShoot = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootCDDurationSec = 0.3f;

	FTimerHandle ShootCDTimer;

	FPlayerDiedDelegate PlayerDiedDelegate;
		
	// Sets default values for this pawn's properties
	ATopdownCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveTriggered(const FInputActionValue& Value);
	void MoveCompleted(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);

	bool IsInMapBoundsHorizontal(float xPos);
	bool IsInMapBoundsVertical(float zPos);

	void OnShootCDTimerTimeout();

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyINdex, bool bFromSweep, const FHitResult& SweepResult);
};
