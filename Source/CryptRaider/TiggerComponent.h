// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mover.h"
#include "Components/BoxComponent.h"
#include "TiggerComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UTiggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UTiggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Setmover(UMover* NewMover);

private:
	UPROPERTY(EditAnywhere)
	FName UnlockingTag = "Unlock";

	AActor* GetAcceptableActor() const;

	UMover* Mover;
	
};
