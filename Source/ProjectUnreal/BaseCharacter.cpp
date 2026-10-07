#include "BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "MyAttributeSet.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UMyAttributeSet>(TEXT("AttributeSet"));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	WeaponCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetupAttachment(GetMesh(), TEXT("sword_bottom"));
	WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponCollision->SetCollisionObjectType(ECC_WorldDynamic);
	WeaponCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	WeaponCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	WeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnWeaponOverlap);

	WeaponCollision->SetCapsuleSize(WeaponCollisionRadius, WeaponCollisionHalfHeight);
	WeaponCollision->SetRelativeLocation(WeaponCollisionCenter);
	WeaponCollision->SetRelativeRotation(WeaponCollisionRotation);

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCFinder(TEXT("/Game/Input/IMC_Default.IMC_Default"));
	if (IMCFinder.Succeeded())
	{
		DefaultMappingContext = IMCFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveFinder(TEXT("/Game/Input/IA_Move.IA_Move"));
	if (MoveFinder.Succeeded())
	{
		MoveAction = MoveFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> LookFinder(TEXT("/Game/Input/IA_Look.IA_Look"));
	if (LookFinder.Succeeded())
	{
		LookAction = LookFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpFinder(TEXT("/Game/Input/IA_Jump.IA_Jump"));
	if (JumpFinder.Succeeded())
	{
		JumpAction = JumpFinder.Object;
	}
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AController* C = GetController())
	{
		FRotator ControlRot = C->GetControlRotation();
		ControlRot.Pitch = DefaultCameraPitch;
		C->SetControlRotation(ControlRot);
	}

	if (AbilitySystemComponent && InitAttributesEffect)
	{
		FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
		Context.AddSourceObject(this);

		const FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(InitAttributesEffect, 1.f, Context);
		if (Spec.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
	}

	if (AbilitySystemComponent)
	{
		for (const TSubclassOf<UGameplayAbility>& AbilityClass : StartupAbilities)
		{
			if (AbilityClass)
			{
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, -1, this));
			}
		}
	}
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && DefaultMappingContext)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
		}
		if (LookAction)
		{
			EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
		}
		if (JumpAction)
		{
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ABaseCharacter::OnJumpInput);
			EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}
		if (AttackAction)
		{
			EnhancedInput->BindAction(AttackAction, ETriggerEvent::Started, this, &ABaseCharacter::OnAttackInput);
		}
	}
}

void ABaseCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!WeaponCollision)
	{
		return;
	}

	WeaponCollision->SetVisibility(bWeaponCollision);
	if (!bWeaponCollision)
	{
		return;
	}

	WeaponCollision->SetCapsuleSize(WeaponCollisionRadius, WeaponCollisionHalfHeight);
	WeaponCollision->SetRelativeLocation(WeaponCollisionCenter);
	WeaponCollision->SetRelativeRotation(WeaponCollisionRotation);
}

void ABaseCharacter::OnAttackInput()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Attack.Light"))));
	}
}

void ABaseCharacter::OnJumpInput()
{
	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.MoveLocked"))))
	{
		return;
	}

	Jump();
}

void ABaseCharacter::SetWeaponCollisionEnabled(bool bEnabled)
{
	if (bEnabled)
	{
		HitActorsThisSwing.Empty();
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		WeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABaseCharacter::OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || HitActorsThisSwing.Contains(OtherActor))
	{
		return;
	}
	HitActorsThisSwing.Add(OtherActor);

	FGameplayEventData Payload;
	Payload.Instigator = this;
	Payload.Target = OtherActor;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, FGameplayTag::RequestGameplayTag(FName("Event.Montage.Attack.Hit")), Payload);
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.MoveLocked"))))
	{
		return;
	}

	const FVector2D Axis = Value.Get<FVector2D>();
	if (!Controller)
	{
		return;
	}

	const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, Axis.Y);
	AddMovementInput(Right, Axis.X);
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Axis = Value.Get<FVector2D>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}
