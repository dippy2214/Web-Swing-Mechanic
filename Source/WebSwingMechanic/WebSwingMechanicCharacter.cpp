// Copyright Epic Games, Inc. All Rights Reserved.

#include "WebSwingMechanicCharacter.h"
#include "WebSwingMechanicProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "CableComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AWebSwingMechanicCharacter

AWebSwingMechanicCharacter::AWebSwingMechanicCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	GrappleCableL = CreateDefaultSubobject<UCableComponent>(TEXT("Left Grappling Line"));
	GrappleCableL->SetupAttachment(FirstPersonCameraComponent);
	GrappleCableL->SetVisibility(false);

	GrappleCableR = CreateDefaultSubobject<UCableComponent>(TEXT("Right Grappling Line"));
	GrappleCableR->SetupAttachment(FirstPersonCameraComponent);
	GrappleCableR->SetVisibility(false);


}

void AWebSwingMechanicCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////// Input

void AWebSwingMechanicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWebSwingMechanicCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWebSwingMechanicCharacter::Look);

		// Interacting
		EnhancedInputComponent->BindAction(InteractRAction, ETriggerEvent::Triggered, this, &AWebSwingMechanicCharacter::InteractR);
		EnhancedInputComponent->BindAction(InteractRAction, ETriggerEvent::Completed, this, &AWebSwingMechanicCharacter::StopInteractR);

		EnhancedInputComponent->BindAction(InteractLAction, ETriggerEvent::Triggered, this, &AWebSwingMechanicCharacter::InteractL);
		EnhancedInputComponent->BindAction(InteractLAction, ETriggerEvent::Completed, this, &AWebSwingMechanicCharacter::StopInteractL);


	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AWebSwingMechanicCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	
	if (isGrapplingR)
	{
		//convert between local and world space with inverse transform position cause cable needs this to be consistent	
		GrappleCableR->EndLocation = GetActorTransform().InverseTransformPosition(GrapplePointR);
	
		if (GetActorLocation().Z < GrapplePointR.Z)
		{
			//distance from pivot to actor
			FVector PosVector = GetActorLocation() - GrapplePointR;

			//radial velocity (component of velocity that is going round the circle of rotation)
			FVector RadialVeclocity = (GetVelocity().Dot(PosVector) / (PosVector.Length() * PosVector.Length())) * PosVector;

			//tangential velocity (component of velocity that is just heading on forwards)
			FVector TangentialVelocity = GetVelocity() - RadialVeclocity;

			//Axis of rotation
			FVector Axis = RadialVeclocity.Cross(TangentialVelocity) / RadialVeclocity.Cross(TangentialVelocity).Length();

			//angle of rotation happening in this time step
			float Angle = (TangentialVelocity.Length() / PosVector.Length()) * deltaTime;

			//rotated tangnetial velocity (as per rodrigues' formula for rotation)
			FVector RotatedTangentialVelocity = TangentialVelocity * cosf(Angle) + (Axis.Cross(TangentialVelocity) * sinf(Angle));

			//make sure to preserve momentum to avoid unintended energy loss within the system
			RotatedTangentialVelocity = RotatedTangentialVelocity * GetVelocity().Length() / RotatedTangentialVelocity.Length();

			//rescale tangential velocity to make sure that the player is moving as quickly as we want them to
			double MinSpeed = 1000;
			double scale = 1;
			if ((RotatedTangentialVelocity/* + Gravity*/).Length() < MinSpeed)
			{
				scale = 1.2;
			}

			GetCharacterMovement()->Velocity = RotatedTangentialVelocity * scale; //+ Gravity;
		}
	}

	if (isGrapplingL)
	{	//convert between local and world space with inverse transform position cause cable needs this to be consistent	
		GrappleCableL->EndLocation = GetActorTransform().InverseTransformPosition(GrapplePointL);
		
		if (GetActorLocation().Z < GrapplePointL.Z)
		{
			//distance from pivot to actor
			FVector PosVector = GetActorLocation() - GrapplePointL;

			//radial velocity (component of velocity that is going round the circle of rotation)
			FVector RadialVeclocity = (GetVelocity().Dot(PosVector) / (PosVector.Length() * PosVector.Length())) * PosVector;

			//tangential velocity (component of velocity that is just heading on forwards)
			FVector TangentialVelocity = GetVelocity() - RadialVeclocity;

			//Axis of rotation
			FVector Axis = RadialVeclocity.Cross(TangentialVelocity) / RadialVeclocity.Cross(TangentialVelocity).Length();

			//angle of rotation happening in this time step
			float Angle = (TangentialVelocity.Length() / PosVector.Length()) * deltaTime;

			//rotated tangnetial velocity (as per rodrigues' formula for rotation)
			FVector RotatedTangentialVelocity = TangentialVelocity * cosf(Angle) + (Axis.Cross(TangentialVelocity) * sinf(Angle));

			//make sure to preserve momentum to avoid unintended energy loss within the system
			RotatedTangentialVelocity = RotatedTangentialVelocity * (GetVelocity().Length() / RotatedTangentialVelocity.Length());

			//rescale tangential velocity to make sure that the player is moving as quickly as we want them to
			double MinSpeed = 1000;
			//double scale = 1;
			if ((RotatedTangentialVelocity/* + Gravity*/).Length() < MinSpeed)
			{
				RotatedTangentialVelocity = RotatedTangentialVelocity * (MinSpeed / RotatedTangentialVelocity.Length());
			}

			GetCharacterMovement()->Velocity = RotatedTangentialVelocity;// + Gravity;
		}
	}
}

void AWebSwingMechanicCharacter::InteractR()
{
	FVector start = GetCapsuleComponent()->GetComponentLocation();
	FVector end = start + (MaxLineDistance * UKismetMathLibrary::GetForwardVector(FirstPersonCameraComponent->GetComponentRotation()));
	DrawDebugLine(GetWorld(), start, end, FColor::Emerald);

	FHitResult hitResult;

	//raycast and sweep by collision channel
	bool hasHit = GetWorld()->SweepSingleByChannel(hitResult, start, end, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(1.0f));

	if (hasHit)
	{
		if (!isGrapplingR)
		{
			isGrapplingR = true;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
			GrappleCableR->SetVisibility(true);
			GrapplePointR = hitResult.ImpactPoint;
			lengthR = (GetActorLocation()-GrapplePointR).Length();
		}
	}
}

void AWebSwingMechanicCharacter::InteractL()
{
	FVector start = GetCapsuleComponent()->GetComponentLocation();
	FVector end = start + (MaxLineDistance * UKismetMathLibrary::GetForwardVector(FirstPersonCameraComponent->GetComponentRotation()));
	DrawDebugLine(GetWorld(), start, end, FColor::Emerald);

	FHitResult hitResult;

	//raycast and sweep by collision channel
	bool hasHit = GetWorld()->SweepSingleByChannel(hitResult, start, end, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(1.0f));

	if (hasHit)
	{
		if (!isGrapplingL)
		{
			isGrapplingL = true;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
			GrappleCableL->SetVisibility(true);
			GrapplePointL = hitResult.ImpactPoint;
			lengthL = (GetActorLocation()-GrapplePointL).Length();
		}
	}
}

void AWebSwingMechanicCharacter::StopInteractR()
{
	if (isGrapplingR)
	{
		int mul = 0;
		if (GrapplePointR.Z - GetActorLocation().Z > 5)
		{
			mul = 70000;
		}
		else
		{
			mul = 0;
		}
		GetCharacterMovement()->AddImpulse(FVector(0, 0, 1) * mul);
	}
	isGrapplingR = false;

	//avoid floating weirdness by not overriding falling 
	if (!GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	}
	GrappleCableR->SetVisibility(false);
	
}

void AWebSwingMechanicCharacter::StopInteractL()
{
	if (isGrapplingL)
	{
		int mul = 0;
		if (GrapplePointL.Z - GetActorLocation().Z > 5)
		{
			mul = 70000;
		}
		else
		{
			mul = 0;
		}
		GetCharacterMovement()->AddImpulse(FVector(0, 0, 1) * mul);
	}
	isGrapplingL = false;

	//avoid floating weirdness by not overriding falling 
	if (!GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	}
	GrappleCableL->SetVisibility(false);
}


void AWebSwingMechanicCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AWebSwingMechanicCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}