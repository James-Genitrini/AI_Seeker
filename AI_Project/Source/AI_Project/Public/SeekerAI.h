// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
class ABonus;
#include "SeekerAI.generated.h"

UCLASS()
class AI_PROJECT_API ASeekerAI : public APawn
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(Category = Pawn, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPawnMovementComponent* MovementComponent;
	
	AActor* NextBonus;
	
	float BoxRadius;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	// Sets default values for this pawn's properties
	ASeekerAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector Seek(FVector TargetLocation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
