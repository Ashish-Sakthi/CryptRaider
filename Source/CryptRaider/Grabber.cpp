// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle != nullptr) {
		UE_LOG(LogTemp, Display, TEXT("%s"),*PhysicsHandle->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();	
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}


void UGrabber::Grab()
{
	UE_LOG(LogTemp, Display, TEXT("Grabbed"));
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;
	}
	
	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);
	if (HasHit) 
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		AActor* HitActor = HitResult.GetActor();
		HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			HitResult.GetComponent()->GetComponentRotation()
		);
		HitResult.GetActor()->Tags.Add("Grabbed");
	}
	
}
void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Released"));
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		AActor* Actor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		Actor->Tags.Remove("Grabbed"); 
		PhysicsHandle->ReleaseComponent();
	}

}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("This needs a phyicsHandle"));
	}
	return Result; 
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector start = GetComponentLocation();
	FVector end = start + MaxGrabDistance * GetForwardVector();
	DrawDebugLine(GetWorld(), start, end, FColor::Blue);
	//DrawDebugSphere(GetWorld(), end, 10, 10, FColor::Red, false, 5);

	FCollisionShape sphere = FCollisionShape::MakeSphere(GrabRadius);
	bool HasHit = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		start, end,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		sphere
	);

	return HasHit;
}
