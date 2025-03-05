// Fill out your copyright notice in the Description page of Project Settings.


#include "TiggerComponent.h"

UTiggerComponent::UTiggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTiggerComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("Component Alive"));
}

void UTiggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetAcceptableActor() == nullptr )
	{
		Mover->SetShouldMove(false);
	}
	else
	{
		AActor* Actor = GetAcceptableActor();
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		
		if (Component != nullptr)
		{
			Component->SetSimulatePhysics(false);
		}
		
		Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		Mover->SetShouldMove(true);
	}
	
}

void UTiggerComponent::Setmover(UMover* NewMover)
{
	Mover = NewMover;
}

AActor* UTiggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	for (AActor* Actor : Actors)
	{
		bool isAcceptableTag = Actor->ActorHasTag(UnlockingTag);
		bool isGrabbed = Actor->ActorHasTag("Grabbed");
		if (isAcceptableTag && !isGrabbed)
		{
			return Actor;
		}
	}

	return nullptr;
}