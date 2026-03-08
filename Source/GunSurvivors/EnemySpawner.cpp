// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* PlayerActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATopdownCharacter::StaticClass());
	if (PlayerActor)
	{
		Player = Cast<ATopdownCharacter>(PlayerActor);
	}

	StartSpawn();
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::EnemySetup(AEnemy* Enemy)
{
	if (Enemy)
	{
		Enemy->Player = Player;
		Enemy->bCanFollow = true;
	}
}

void AEnemySpawner::StartSpawn()
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawner::OnSpawnTimerTimeout, SpawnTime, true, SpawnTime);
}

void AEnemySpawner::StopSpawn()
{
	GetWorldTimerManager().ClearTimer(SpawnTimer);
}

void AEnemySpawner::OnSpawnTimerTimeout()
{
	SpawnEnemy();
}

void AEnemySpawner::SpawnEnemy()
{
	// Enemy Spawner
	FVector2D RndmPos = FVector2d(FMath::VRand());
	RndmPos.Normalize();
	RndmPos *= SpawnDistance;

	FVector EnemyLoc = GetActorLocation() + FVector(RndmPos.X, 0.0f, RndmPos.Y);

	AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(EnemyActorToSpawn, EnemyLoc, FRotator::ZeroRotator);

	EnemySetup(Enemy);

	// Difficulty System
	TotalEnemyCount += 1;
	if (TotalEnemyCount % DifficultySpikeInterval == 0)
	{
		if (SpawnTime > SpawnTimeMinimumLimit)
		{
			SpawnTime -= DecreaseDiffSpawnByInterval;
			if (SpawnTime < SpawnTimeMinimumLimit)
			{
				SpawnTime = SpawnTimeMinimumLimit;
			}

			StopSpawn();
			StartSpawn();
		}
	}
}
