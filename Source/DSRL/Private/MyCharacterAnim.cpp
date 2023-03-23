#include "MyCharacterAnim.h"
#include "MyCharacter.h"

void UMyCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto OwnerPawn = TryGetPawnOwner();
	auto Player = Cast<AMyCharacter>(OwnerPawn);

	if (Player)
	{
		FVector Velocity = Player->GetVelocity();
		FVector ForwardVector = Player->GetActorForwardVector();
		Speed = FVector::DotProduct(ForwardVector, Velocity);
		FVector RightVector = Player->GetActorRightVector();
		Direction = FVector::DotProduct(RightVector, Velocity);

		auto Movement = Player->GetCharacterMovement();
		IsInAir = Movement->IsFalling();
	}
}