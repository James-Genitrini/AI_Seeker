#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SeekerAI.generated.h"

class ABonus;
class AThrowerAI;

UCLASS()
class AI_PROJECT_API ASeekerAI : public APawn
{
	GENERATED_BODY()

public:
	ASeekerAI();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	FVector Seek(const FVector& Target);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category="Effects")
	class UParticleSystem* CollectParticles;

protected:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
	class UFloatingPawnMovement* MovementComponent;

	AActor* NextBonus;

	AThrowerAI* Thrower = nullptr;
	
	float BoxRadius = 64.f;
};
