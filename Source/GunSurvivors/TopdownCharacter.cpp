// Fill out your copyright notice in the Description page of Project Settings.


#include "TopdownCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATopdownCharacter::ATopdownCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	CharacterFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CharacterFlipbook"));
	CharacterFlipbook->SetupAttachment(RootComponent);

	GunParent = CreateDefaultSubobject<USceneComponent>(TEXT("GunParent"));
	GunParent->SetupAttachment(RootComponent);

	GunSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("GunSprite"));
	GunSprite->SetupAttachment(GunParent);

	BulletSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnPosition"));
	BulletSpawnPosition->SetupAttachment(GunSprite);
}

// Called when the game starts or when spawned
void ATopdownCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		PlayerController->SetShowMouseCursor(true);

		UEnhancedInputLocalPlayerSubsystem* InputPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (InputPlayerSubsystem)
		{
			// Registering the input mapping context
			InputPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Called every frame
void ATopdownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// Player Movement
	if (bCanMove)
	{
		if (MovementDirection.Length() > 0.0f)
		{
			if (MovementDirection.Length() > 1.0f)
			{
				MovementDirection.Normalize();
			}

			FVector2D DistanceToMove = MovementDirection * MovementSpeed * DeltaTime;
			FVector CurrentLocation = GetActorLocation();
			FVector NewLocation = CurrentLocation + FVector(DistanceToMove.X, 0.0f, 0.0f);
			if (!IsInMapBoundsHorizontal(NewLocation.X))
			{
				NewLocation -= FVector(DistanceToMove.X, 0.0f, 0.0f);
			}
			
			NewLocation += FVector(0.0f, 0.0f, DistanceToMove.Y);
			if (!IsInMapBoundsVertical(NewLocation.Z))
			{
				NewLocation -= FVector(0.0f, 0.0f, DistanceToMove.Y);
			}

			SetActorLocation(NewLocation);
		}
	}

	// Gun Rotation
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		FVector MouseWorldLocation, MouseWorldDirection;
		PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

		FVector CurrentLoc = GetActorLocation();
		FVector StartLoc = FVector(CurrentLoc.X, 0.0f, CurrentLoc.Z);
		FVector TargetLoc = FVector(MouseWorldLocation.X, 0.0f, MouseWorldLocation.Z);
		FRotator GunParentRotator = UKismetMathLibrary::FindLookAtRotation(StartLoc, TargetLoc);

		GunParent->SetRelativeRotation(GunParentRotator);
	}
}

// Called to bind functionality to input
void ATopdownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopdownCharacter::MoveTriggered);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ATopdownCharacter::MoveCompleted);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &ATopdownCharacter::MoveCompleted);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ATopdownCharacter::Shoot); // able to shoot once
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ATopdownCharacter::Shoot); // able to keep shooting while holding down the button
	}
}

void ATopdownCharacter::MoveTriggered(const FInputActionValue& Value)
{
	FVector2D MoveActionValue = Value.Get<FVector2D>();

	if (bCanMove)
	{
		MovementDirection = MoveActionValue;
		CharacterFlipbook->SetFlipbook(RunFlipbook);

		FVector FlipbookScale = CharacterFlipbook->GetComponentScale();
		if (MovementDirection.X < 0.0f && FlipbookScale.X > 0.0f)
		{
			CharacterFlipbook->SetWorldScale3D(UNIT_VECTOR_MINUSX);
		}
		if (MovementDirection.X > 0.0f && FlipbookScale.X < 0.0f)
		{
			CharacterFlipbook->SetWorldScale3D(UNIT_VECTOR);
		}

	}
}

void ATopdownCharacter::MoveCompleted(const FInputActionValue& Value)
{
	MovementDirection = FVector2D(0.0f, 0.0f);
	CharacterFlipbook->SetFlipbook(IdleFlipbook);
}

void ATopdownCharacter::Shoot(const FInputActionValue& Value)
{
	if (bCanShoot)
	{
		bCanShoot = false;

		// Spawn bullet actor
		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletActorToSpawn, BulletSpawnPosition->GetComponentLocation(), FRotator(0.0f, 0.0f, 0.0f));
		check(Bullet); //check used only for development purposes

		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		check(PlayerController);
		FVector MouseWorldLocation, MouseWorldDirection;
		PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);

		// Start calculate directions/positions
		FVector CurrentLocation = GetActorLocation();
		FVector2D BulletDirection = FVector2D(MouseWorldLocation.X - CurrentLocation.X, MouseWorldLocation.Z - CurrentLocation.Z);
		BulletDirection.Normalize();

		// Launch the bullet
		float BulletSpeed = 200.0f;
		Bullet->Launch(BulletDirection, BulletSpeed);

		GetWorldTimerManager().SetTimer(ShootCDTimer, this, &ATopdownCharacter::OnShootCDTimerTimeout, 1.0f, false, ShootCDDurationSec);
	}
}

bool ATopdownCharacter::IsInMapBoundsHorizontal(float xPos)
{
	return (xPos > HorizontalLimits.X && xPos < HorizontalLimits.Y);
}

bool ATopdownCharacter::IsInMapBoundsVertical(float zPos)
{
	return (zPos > VerticalLimits.X && zPos < VerticalLimits.Y);
}

void ATopdownCharacter::OnShootCDTimerTimeout()
{
	bCanShoot = true;
}

