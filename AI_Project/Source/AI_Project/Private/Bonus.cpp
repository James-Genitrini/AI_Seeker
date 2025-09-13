#include "Bonus.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ThrowerAI.h"
#include "Kismet/GameplayStatics.h"

ABonus::ABonus()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	RootComponent = BoxCollision;
	BoxCollision->InitBoxExtent(FVector(BoxRadius));
	BoxCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxCollision->SetGenerateOverlapEvents(true);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->SetEnableGravity(true);
	StaticMesh->SetMobility(EComponentMobility::Movable);
}


void ABonus::HandleDestroyed(AActor* DestroyedActor)
{
	if (OwnerThrower)
	{
		OwnerThrower->ActiveBonuses.Remove(this);
	}
}


void ABonus::BeginPlay()
{
	Super::BeginPlay();

	OnDestroyed.AddDynamic(this, &ABonus::HandleDestroyed);
}

void ABonus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If under catcher AI destroy the Actor
	float ZPos = GetActorLocation().Z;
	if (ZPos < MinZToDestroy)
	{
		Destroy();
	}
}