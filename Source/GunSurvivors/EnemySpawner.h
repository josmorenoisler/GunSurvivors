// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "Engine/TimerHandle.h"
#include "TopdownCharacter.h"
#include "EnemySpawner.generated.h"

UCLASS()
class GUNSURVIVORS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemy> EnemyActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnDistance = 400.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int TotalEnemyCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DifficultySpikeInterval = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnTimeMinimumLimit = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DecreaseDiffSpawnByInterval = 0.05f;

	ATopdownCharacter* Player;

	FTimerHandle SpawnTimer;

	// Sets default values for this actor's properties
	AEnemySpawner();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void EnemySetup(AEnemy* Enemy);

	void OnSpawnTimerTimeout();
	void StartSpawn();
	void StopSpawn();
	void SpawnEnemy();
};
