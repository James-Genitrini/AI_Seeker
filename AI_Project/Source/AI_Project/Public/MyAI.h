// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyAI.generated.h"

UCLASS()
class AI_PROJECT_API AMyAI : public APawn
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(Category = Pawn, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPawnMovementComponent* MovementComponent;
	
	AActor* NextBonus;

public:	
	// Sets default values for this actor's properties
	AMyAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	FVector Seek(FVector TargetLocation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
