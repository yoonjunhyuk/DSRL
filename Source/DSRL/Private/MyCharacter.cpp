#include "MyCharacter.h"
#include "MyCharacterAnim.h"
#include "MyEnemyFSM.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempChar(TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));
	if (TempChar.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempChar.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	SpringArm->SetRelativeLocation(FVector(0, 0, 90));
	SpringArm->TargetArmLength = 800;
	SpringArm->bUsePawnControlRotation = true;

	Camera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	IsAttacking = false;
	MaxCombo = 3;
	AttackEndCombo();
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move();
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MyAnim = Cast<UMyCharacterAnim>(GetMesh()->GetAnimInstance());
	MyAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
	
	MyAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			AttackStartCombo();
			MyAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	MyAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackCheck);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMyCharacter::InputJump);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &AMyCharacter::InputRun);
	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &AMyCharacter::InputRun);
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AMyCharacter::InputAttack);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AMyCharacter::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AMyCharacter::InputVertical);
}

void AMyCharacter::Turn(float value)
{
	AddControllerYawInput(value);
}

void AMyCharacter::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void AMyCharacter::InputHorizontal(float value)
{
	Direction.Y = value;
}

void AMyCharacter::InputVertical(float value)
{
	Direction.X = value;
}

void AMyCharacter::Move()
{
	Direction = FTransform(GetControlRotation()).TransformVector(Direction);
	AddMovementInput(Direction);
	Direction = FVector::ZeroVector;
}

void AMyCharacter::InputJump()
{
	Jump();
}

void AMyCharacter::InputRun()
{
	auto Movement = GetCharacterMovement();
	if (Movement->MaxWalkSpeed > WalkSpeed)
	{
		Movement->MaxWalkSpeed = WalkSpeed;
	}
	else
	{
		Movement->MaxWalkSpeed = RunSpeed;
	}
}

void AMyCharacter::InputAttack()
{
	if (IsAttacking)
	{
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		AttackStartCombo();
		MyAnim->PlayAttackMontage();
		MyAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	AttackEndCombo();
}

void AMyCharacter::AttackStartCombo()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AMyCharacter::AttackEndCombo()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AMyCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(50.0f),
		Params);

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(1.0f, DamageEvent, GetController(), this);

			auto Enemy = HitResult.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (Enemy)
			{
				auto EnemyFSM = Cast<UMyEnemyFSM>(Enemy);
				EnemyFSM->OnDamageProcess();
			}
		}
	}
}

float AMyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Actor : %s"), *GetName());

	// 내 죽음 모션(체력?)
	if (FinalDamage > 0.0f)
	{
		MyAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	}

	return FinalDamage;
}