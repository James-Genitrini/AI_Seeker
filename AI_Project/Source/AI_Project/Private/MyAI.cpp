#include "MyAI.h"

#include "Bonus.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyAI::AMyAI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	RootComponent = BoxCollision;
	
	BoxRadius = 64.f;
	
	BoxCollision->InitBoxExtent(FVector(BoxRadius));
	BoxCollision->SetCollisionProfileName("Trigger");
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	
	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = BoxCollision;
	
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyAI::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AMyAI::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> OverlappingActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABonus::StaticClass(), OverlappingActors);
	if (OverlappingActors.Num() > 0)
	{
		NextBonus = OverlappingActors[0];
	}
}

FVector AMyAI::Seek(FVector TargetLocation)
{
	FVector vDesiredLocation = TargetLocation - GetActorLocation();

	// On ignore X et Z
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

void AMyAI::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(ABonus::StaticClass()))
	{
		OtherActor->Destroy();;
    	
    	TArray<AActor*> OverlappingActors;
    	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABonus::StaticClass(), OverlappingActors);
    	if (OverlappingActors.Num() > 0)
    	{
    		NextBonus = OverlappingActors[0];
    	}
	}
}


// Called every frame
void AMyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (NextBonus)
	{
		MovementComponent->AddInputVector(Seek(NextBonus->GetActorLocation()));
	}
	
}

