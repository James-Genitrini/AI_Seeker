// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowerAI.h"
#include "Bonus.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
AThrowerAI::AThrowerAI()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->bConstrainToPlane = true;
	MovementComponent->SetPlaneConstraintNormal(FVector(0,0,1));
}

// Called when the game starts or when spawned
void AThrowerAI::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AThrowerAI::Seek(FVector Target)
{
	FVector vDesiredLocation = Target - GetActorLocation();

	// ON ingore X et Z
	vDesiredLocation.X = 0.f;
	vDesiredLocation.Z = 0.f;

	vDesiredLocation.Normalize();
	vDesiredLocation *= MovementComponent->GetMaxSpeed();

	FVector vSteering = vDesiredLocation - MovementComponent->Velocity;

	vSteering.X = 0.f;
	vSteering.Z = 0.f;

	vSteering = vSteering.GetClampedToMaxSize(MovementComponent->GetMaxSpeed());

	return vSteering;
}



// Called every frame
void AThrowerAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Elapsed += DeltaTime;

	FVector currentLocation = GetActorLocation();

	if (Elapsed >= 1.0f)
	{
		Elapsed = 0.f;

		float RandomY = FMath::RandRange(-400.f, 900.f);
		TargetLocation = FVector(currentLocation.X, RandomY, currentLocation.Z);

		if (BonusClass && FMath::RandBool())   // 50% de chance
		{
			FVector SpawnLocation = currentLocation + FVector(0.f, 0.f, -120.f);
			FRotator SpawnRotation = FRotator::ZeroRotator;
			
			ABonus* NewBonus = GetWorld()->SpawnActor<ABonus>(BonusClass, SpawnLocation, SpawnRotation);
			if (NewBonus)
			{
				NewBonus->OwnerThrower = this;
				ActiveBonuses.Add(NewBonus);
			}

		}
	}

	MovementComponent->AddInputVector(Seek(TargetLocation));
}
