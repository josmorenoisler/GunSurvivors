// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Enemy.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);

	BulletSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BulletSprite"));
	BulletSprite->SetupAttachment(RootComponent);

	MovementDirection = FVector2D(1.0f, 0.0f);
	
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OverlapBegin);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsLaunched)
	{
		FVector2D DistanceToMove = MovementDirection * MovementSpeed * DeltaTime;

		FVector	NewLocation = GetActorLocation() + FVector(DistanceToMove.X, 0.0f, DistanceToMove.Y);

		SetActorLocation(NewLocation);
	}
}

void ABullet::Launch(FVector2D Direction, float Speed)
{
	if (bIsLaunched) return;

	bIsLaunched = true;

	MovementDirection = Direction;
	MovementSpeed = Speed;

	float DeleteTime = 10.0f;
	GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &ABullet::OnDeleteTimerTimeout, 1.0f, false, DeleteTime);
}

void ABullet::OnDeleteTimerTimeout()
{
	Destroy();
}

void ABullet::OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyINdex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);

	if (Enemy && Enemy->IsEnemyAlive())
	{
		DisableBullet();
		Enemy->Die();
	}
}

void ABullet::DisableBullet()
{
	if (bIsDisabled)
	{
		return;
	}

	bIsDisabled = true;
	SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BulletSprite->DestroyComponent();
}

