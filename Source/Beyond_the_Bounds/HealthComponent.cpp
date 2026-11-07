#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Replication 활성화
	SetIsReplicatedByDefault(true);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, currentHealth);
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		currentHealth = maxHealth;
	}

}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::OnRep_CurrentHealth()
{
	// Replication된 currentHealth 값이 변경될 때 호출되는 함수
	// 여기서 UI 업데이트나 다른 로직을 처리할 수 있음
	UE_LOG(LogTemp, Warning, TEXT("OnRep: Current Health updated: %f"), currentHealth);
}

void UHealthComponent::TakeDamage(float dmg)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		currentHealth -= dmg;
		if (currentHealth < 0.0f)
		{
			currentHealth = 0.0f;
		}
		UE_LOG(LogTemp, Warning, TEXT("Damage taken: %f, Current Health: %f"), dmg, currentHealth);
	}
}
