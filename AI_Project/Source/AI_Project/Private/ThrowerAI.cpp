#include "ThrowerAI.h"
#include "Bonus.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

AThrowerAI::AThrowerAI()
{
	PrimaryActorTick.bCanEverTick = true;

	// UE_LOG(LogTemp, Warning, TEXT("AThrowerAI CONSTRUCTEUR"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->bConstrainToPlane = true;
	MovementComponent->SetPlaneConstraintNormal(FVector(0,0,1));
}

void AThrowerAI::BeginPlay()
{
	Super::BeginPlay();

	// UE_LOG(LogTemp, Warning, TEXT("BeginPlay AThrowerAI lancé"));

	// if (BonusCounterWidgetInstance)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("BonusCounterWidgetInstance est déjà assigné depuis le Level Blueprint."));
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("BonusCounterWidgetInstance n'est pas assigné ! Assure-toi de le connecter dans le Level Blueprint."));
	// }
}

FVector AThrowerAI::Seek(FVector Target)
{
	FVector vDesiredLocation = Target - GetActorLocation();

	// Ignorer X et Z
	vDesiredLocation.X = 0.f;
	vDesiredLocation.Z = 0.f;

	vDesiredLocation.Normalize();
	vDesiredLocation *= MovementComponent->GetMaxSpeed();

	FVector vSteering = vDesiredLocation - MovementComponent->Velocity;
	vSteering.X = 0.f;
	vSteering.Z = 0.f;

	return vSteering.GetClampedToMaxSize(MovementComponent->GetMaxSpeed());
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
		
	}

	// Random de 1% de chance / tick
	if (FMath::RandRange(0, 100) <= 1)
	{
		if (BonusClass && FMath::RandBool())   // 50% de chance
		{
			FVector SpawnLocation = currentLocation + FVector(0.f, 0.f, -120.f);
			FRotator SpawnRotation = FRotator::ZeroRotator;
			
			ABonus* NewBonus = GetWorld()->SpawnActor<ABonus>(BonusClass, SpawnLocation, SpawnRotation);
			if (NewBonus)
			{
				NewBonus->OwnerThrower = this;
				ActiveBonuses.Add(NewBonus);

				// Appel de la fonction Blueprint sur le widget déjà assigné
				if (BonusCounterWidgetInstance)
				{
					if (UFunction* Func = BonusCounterWidgetInstance->FindFunction(TEXT("IncrementThrown")))
					{
						BonusCounterWidgetInstance->ProcessEvent(Func, nullptr);
						UE_LOG(LogTemp, Warning, TEXT("IncrementThrown appelé !"));
					}
				}
			}
		}
	}

	MovementComponent->AddInputVector(Seek(TargetLocation));
}