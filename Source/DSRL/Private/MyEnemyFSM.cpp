#include "MyEnemyFSM.h"
#include "MyCharacter.h"
#include "MyEnemy.h"

// Sets default values for this component's properties
UMyEnemyFSM::UMyEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	auto Actor = UGameplayStatics::GetActorOfClass(GetWorld(), AMyCharacter::StaticClass());
	Target = Cast<AMyCharacter>(Actor);
	Me = Cast<AMyEnemy>(GetOwner());
}


// Called every frame
void UMyEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (State)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
}

void UMyEnemyFSM::IdleState()
{
	//UE_LOG(LogTemp, Warning, TEXT("Idle"));

	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime > IdleDelayTime)
	{
		State = EEnemyState::Move;
		CurrentTime = 0;
	}
}

void UMyEnemyFSM::MoveState()
{
	//UE_LOG(LogTemp, Warning, TEXT("Move"));

	FVector Destination = Target->GetActorLocation();
	FVector Dir = Destination - Me->GetActorLocation();
	Me->AddMovementInput(Dir.GetSafeNormal());

	if (Dir.Size() < AttackRange)
	{
		State = EEnemyState::Attack;
	}

	float Distance = FVector::Distance(Target->GetActorLocation(), Me->GetActorLocation());
	if (Distance > AttackRange)
	{
		State = EEnemyState::Move;
	}

}

void UMyEnemyFSM::AttackState()
{
	//UE_LOG(LogTemp, Warning, TEXT("Attack"));

	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime > AttackDelay)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
		CurrentTime = 0;
	}

	float Distance = FVector::Distance(Target->GetActorLocation(), Me->GetActorLocation());
	if (Distance > AttackRange)
	{
		State = EEnemyState::Move;
	}
}

void UMyEnemyFSM::DamageState()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime > DamageDelay)
	{
		State = EEnemyState::Idle;
		CurrentTime = 0;

		float Distance = FVector::Distance(Target->GetActorLocation(), Me->GetActorLocation());
		if (Distance > AttackRange)
		{
			State = EEnemyState::Move;
		}
	}
}

void UMyEnemyFSM::DieState()
{
	Me->Destroy();
}

void UMyEnemyFSM::OnDamageProcess()
{
	HP--;
	if (HP > 0)
	{
		State = EEnemyState::Damage;
	}
	else
	{
		State = EEnemyState::Die;
	}
}