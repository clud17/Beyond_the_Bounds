#include "HealthComponent.h"
#include "EnemyBase.h"
#include "PlayerCharacter.h"
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
		if (currentHealth <= 0.0f)
		{
			currentHealth = 0.0f;

			AActor* Owner = GetOwner();
			
			if (Owner) {
				// Owner가 AEnemyBase 타입인지 확인하고, 맞다면 사망 처리
				if (Owner->IsA(AEnemyBase::StaticClass()))
				{
					Owner->Destroy(); // 추후 destroy는 델리게이트를 통해 OnDeath 이벤트를 호출하도록 변경 예정
				}
				// Owner가 APlayerCharacter 타입인지 확인하고, 맞다면 사망 처리
				if (Owner->IsA(APlayerCharacter::StaticClass())) 
				{
					// 델리게이트를 통해 OnDeath 이벤트를 호출하도록 변경 예정
					// 사망 처리는 PlayerCharacter 클래스에서 구현
					// 사망 UI 처리는 PlayerController에서 구현
				}
			}
			
		}
		UE_LOG(LogTemp, Warning, TEXT("Damage taken: %f, Current Health: %f"), dmg, currentHealth);
	}
}
