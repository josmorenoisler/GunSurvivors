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
	
	if (!Player)
	{
		AActor* PlayerActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATopdownCharacter::StaticClass());

		if (PlayerActor)
		{
			Player = Cast<ATopdownCharacter>(PlayerActor);
			bCanFollow = true;
		}
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAlive && bCanFollow && Player)
	{
		// Enemy Movement towards player
		FVector CurrLoc = GetActorLocation();
		FVector PlayerLoc = Player->GetActorLocation();

		FVector PlayerDirection = PlayerLoc - GetActorLocation();
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

