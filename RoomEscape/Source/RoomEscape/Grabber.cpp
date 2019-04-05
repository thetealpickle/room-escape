// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Gameframework/PlayerController.h"
#include "DrawDebugHelpers.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	
	UE_LOG(LogTemp, Warning, TEXT("I AM A GRABBER, HEHE. My name is grabbie"));

	/// look for physics handle instance
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("Physics Handle was not found for actor: %s"), *(GetOwner()->GetName()))
	}

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Input Component was found for actor: %s"), *(GetOwner()->GetName()));

		/// bind the input axis
		InputComponent->BindAction("grab", IE_Pressed, this, &UGrabber::Grab);
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player view

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	/// ray-cast out to reach distance
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false, // persistence 
		0.f, // persistence lifetime
		0, // depth
		10.f // thickness
	);

	/// setup query params
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	/// line-trace out to reach distance
	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	// see what we hit
	AActor *ActorHit = Hit.GetActor();
	if (ActorHit != NULL) {
		FString ActorName(ActorHit->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Hey, the actor wee hit is named: %s"), *ActorName);

	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Hey, hey, this should be a grab!!"));
}
