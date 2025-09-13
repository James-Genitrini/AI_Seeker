// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ThrowerAI.generated.h"

class ABonus;

UCLASS()
class AI_PROJECT_API AThrowerAI : public APawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(Category = Pawn, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPawnMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, Category="Spawn")
	TSubclassOf<ABonus> BonusClass;

public:
	// Sets default values for this pawn's properties
	AThrowerAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector Seek(FVector TargetLocation);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
