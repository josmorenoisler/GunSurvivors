// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("EnemyCapsuleComponent"));
	SetRootComponent(EnemyCapsuleComponent);

	EnemyFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("EnemyFlipbook"));
	EnemyFlipbook->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsEnemyAlive() && bCanFollow && Player)
	{
		// Enemy Movement towards player
		FVector CurrLoc = GetActorLocation();
		FVector PlayerLoc = Player->GetActorLocation();

		FVector PlayerDirection = PlayerLoc - CurrLoc;
		float DistToPlayer = PlayerDirection.Length();

		if (DistToPlayer >= StopDistance)
		{
			PlayerDirection.Normalize();
			FVector NewLoc = CurrLoc + (PlayerDirection * MovSpeed * DeltaTime);
			SetActorLocation(NewLoc);
		}

		// Enemy facing towards player
		CurrLoc = GetActorLocation();
		float FlipbookXScale = EnemyFlipbook->GetComponentScale().X;

		if ((PlayerLoc.X - CurrLoc.X) >= 0.0f) // PLayer is on the right side of the enemy
		{
			if (FlipbookXScale < 0.0f)
			{
				EnemyFlipbook->SetWorldScale3D(UNIT_VECTOR);
			}
		}
		else // PLayer is on the left side of the enemy
		{
			if (FlipbookXScale > 0.0f)
			{
				EnemyFlipbook->SetWorldScale3D(UNIT_VECTOR_MINUSX);
			}
		}
	}
}

void AEnemy::Die()
{
	if (!IsEnemyAlive())
	{
		return;
	}

	bIsAlive = false;
	bCanFollow = false;

	EnemyFlipbook->SetFlipbook(EnemyDeadFlipbook);
	EnemyFlipbook->SetTranslucentSortPriority(-5);

	float TTD = 10.0f;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AEnemy::OnDestroyTimerTimeout, 1.0f, false, TTD);
}

void AEnemy::OnDestroyTimerTimeout()
{
	Destroy();
}

bool AEnemy::IsEnemyAlive()
{
	return bIsAlive;
}

