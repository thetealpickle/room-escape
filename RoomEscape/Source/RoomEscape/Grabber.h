// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Reach = 100.0f;

	// Physics Handle - setup and init
	UPhysicsHandleComponent *PhysicsHandle = nullptr;
	void FindPhysicsHandleComponent();

	// Input Component - setup and init
	UInputComponent *InputComponent = nullptr;
	void SetupInputComponent();

	// ray-cast and grab the object in reach
	void Grab();

	// grab has been released
	void Release();

	FHitResult GetFirstPhysicsBodyInReach() const;

};
