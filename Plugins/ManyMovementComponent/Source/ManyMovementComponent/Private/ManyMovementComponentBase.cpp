// Fill out your copyright notice in the Description page of Project Settings.


#include "ManyMovementComponentBase.h"

// Sets default values for this component's properties
UManyMovementComponentBase::UManyMovementComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UManyMovementComponentBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UManyMovementComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Ӧ��ÿ֡����������
	ApplyCalculateVectorForceEveryFrame(DeltaTime);

	// Ӧ��ÿ֡���ƶ�λ��
	ApplyMovingEveryFrame(DeltaTime);

	// ͨ��ÿ֡�ļ��ٶȼ��㲢Ӧ�õ��ٶ�
	ApplyVelocityByAccelerationEveryFrame(DeltaTime);

	// ͨ��ÿ֡���ۺ������������������㲢Ӧ�õ��ٶ�
	ApplyAccelerationEveryFrame(DeltaTime);
}

double UManyMovementComponentBase::GetVelocityLength()
{
	return Velocity.Length();
}

FVector UManyMovementComponentBase::CalculateVectorForce()
{
	return FVector();
}

void UManyMovementComponentBase::ApplyMovingEveryFrame(float DeltaTime)
{
	AActor* OwnerActor = GetOwner();
	OwnerActor->SetActorLocation(OwnerActor->GetActorLocation() + Velocity * 100 * DeltaTime);
}

void UManyMovementComponentBase::ApplyVelocityByAccelerationEveryFrame(float DeltaTime)
{
	Velocity += (Acceleration * DeltaTime);
}

void UManyMovementComponentBase::ApplyAccelerationEveryFrame(float DeltaTime)
{
	Acceleration = CalculateAcceleration(Mass, VectorForce);
}

FVector UManyMovementComponentBase::CalculateAcceleration(double _Mass, FVector _ForceVector)
{
	if (_Mass == 0)
	{
		return FVector(0, 0, 0);
	}
	return _ForceVector / _Mass;
}

void UManyMovementComponentBase::ApplyCalculateVectorForceEveryFrame(float DeltaTime)
{
}


