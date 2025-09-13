#include "SeekerAI.h"
#include "Bonus.h"
#include "ThrowerAI.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"

ASeekerAI::ASeekerAI()
{
    PrimaryActorTick.bCanEverTick = true;

    BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
    RootComponent = BoxCollision;
    BoxCollision->InitBoxExtent(FVector(BoxRadius));
    BoxCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic")); // overlap pour déclencher OnOverlapBegin
    BoxCollision->SetGenerateOverlapEvents(true);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(RootComponent);

    MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
    MovementComponent->UpdatedComponent = RootComponent;

    BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ASeekerAI::OnOverlapBegin);
}

void ASeekerAI::BeginPlay()
{
    Super::BeginPlay();
    Thrower = Cast<AThrowerAI>(UGameplayStatics::GetActorOfClass(GetWorld(), AThrowerAI::StaticClass()));
}

FVector ASeekerAI::Seek(const FVector& Target)
{
    FVector vDesiredLocation = Target - GetActorLocation();
    vDesiredLocation.X = 0.f;
    vDesiredLocation.Z = 0.f;
    vDesiredLocation.Normalize();
    vDesiredLocation *= MovementComponent->GetMaxSpeed();

    FVector vSteering = vDesiredLocation - MovementComponent->Velocity;
    vSteering.X = 0.f;
    vSteering.Z = 0.f;

    return vSteering.GetClampedToMaxSize(MovementComponent->GetMaxSpeed());
}

void ASeekerAI::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ABonus* Bonus = Cast<ABonus>(OtherActor);
    if (Bonus)
    {
        if (CollectParticles)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollectParticles, Bonus->GetActorLocation(), FRotator::ZeroRotator, true);
        }
        
        Bonus->Destroy();
    }
}


void ASeekerAI::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Thrower && Thrower->ActiveBonuses.Num() > 0)
    {
        // Prendre juste le premier bonus de la liste
        NextBonus = Thrower->ActiveBonuses[0];
    } else
    {
        NextBonus = nullptr;
    }

    if (NextBonus && MovementComponent)
    {
        MovementComponent->AddInputVector(Seek(NextBonus->GetActorLocation()));
    } else
    {
        MovementComponent->AddInputVector(Seek(Thrower->GetActorLocation()));
    }
}