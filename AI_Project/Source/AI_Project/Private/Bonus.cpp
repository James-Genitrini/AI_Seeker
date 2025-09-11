#include "Bonus.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

ABonus::ABonus()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	RootComponent = BoxCollision;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);

	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetEnableGravity(true);
	StaticMesh->SetMobility(EComponentMobility::Movable);
}

void ABonus::BeginPlay()
{
	Super::BeginPlay();
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