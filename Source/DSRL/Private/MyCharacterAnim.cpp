#include "MyCharacterAnim.h"
#include "MyCharacter.h"

UMyCharacterAnim::UMyCharacterAnim()
{
	IsDead = false;
	ConstructorHelpers::FObjectFinder<UAnimMontage> Attack_Montage(TEXT("AnimMontage'/Game/Blueprints/AM_MyCharacter.AM_MyCharacter'"));
	if (Attack_Montage.Succeeded())
	{
		AttackMontage = Attack_Montage.Object;
	}
}

void UMyCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();

	if (!::IsValid(Pawn)) return;

	auto Player = Cast<AMyCharacter>(Pawn);

	if (!IsDead)
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

void UMyCharacterAnim::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}

void UMyCharacterAnim::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UMyCharacterAnim::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UMyCharacterAnim::JumpToAttackMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

FName UMyCharacterAnim::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}