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
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CalculateLineTrace();
	// if physics handle attached: move currently holding object
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(LineEnd);
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	/// look for physics handle instance
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Physics Handle was not found for actor: %s"), *(GetOwner()->GetName()))
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Input Component for Owner not found"));
	}
}

void UGrabber::Grab() {
	// line trace to see if we hit any actors 
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	if (ActorHit) {
		if (!PhysicsHandle) { return;  }

		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() 
{
	/// setup query params
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	CalculateLineTrace();
	/// line-trace out to reach distance
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		LineStart,
		LineEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	// see what we hit
	AActor *ActorHit = HitResult.GetActor();
	if (ActorHit != NULL) {
		FString ActorName(ActorHit->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Hey, the actor wee hit is named: %s"), *ActorName);

	}

	return HitResult;
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Hey, hey, this should be a release!!"));

	// TODO: release physics handle
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::CalculateLineTrace()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	LineStart = PlayerViewPointLocation;
	LineEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

}