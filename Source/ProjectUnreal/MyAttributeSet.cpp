#include "MyAttributeSet.h"
#include "GameplayEffectExtension.h"

UMyAttributeSet::UMyAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitStamina(100.f);
	InitMaxStamina(100.f);
}

void UMyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}

void UMyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float LocalDamage = GetDamage();
		SetDamage(0.f);

		if (LocalDamage > 0.f)
		{
			SetHealth(FMath::Clamp(GetHealth() - LocalDamage, 0.f, GetMaxHealth()));
		}
	}
}
