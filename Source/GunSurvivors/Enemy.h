// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbookComponent.h"
#include "TopdownCharacter.h"
#include "Engine/TimerHandle.h"
#include "Enemy.generated.h"

UCLASS()
class GUNSURVIVORS_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* EnemyCapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperFlipbookComponent* EnemyFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPaperFlipbook* EnemyDeadFlipbook;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ATopdownCharacter* Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAlive = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bCanFollow = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovSpeed = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StopDistance = 25.0f;

	FTimerHandle DestroyTimer;

	bool IsEnemyAlive();

	// Sets default values for this actor's properties
	AEnemy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Die();

	void OnDestroyTimerTimeout();

};
