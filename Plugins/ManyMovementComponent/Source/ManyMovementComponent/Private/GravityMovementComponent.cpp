// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityMovementComponent.h"

UGravityMovementComponent::UGravityMovementComponent(){}

void UGravityMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ��ʼʱ���������С
	GetOwner()->SetActorScale3D(FVector(Diameter, Diameter, Diameter));
}

void UGravityMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGravityMovementComponent::ApplyCalculateVectorForceEveryFrame(float DeltaTime)
{
	Super::ApplyCalculateVectorForceEveryFrame(DeltaTime);
	
	// ��������
	TSet<AActor*> OverlapActors;
	GetOwner()->GetOverlappingActors(OverlapActors, AActor::StaticClass());
	if (!OverlapActors.IsEmpty())
	{
		VectorForce = CalculateAllVectorForce(OverlapActors);
	}
}

FVector UGravityMovementComponent::CalculateVectorForce(AActor* OtherActor)
{
	FVector OwnerLocation = GetOwner()->GetActorLocation() * 0.01;
	FVector OtherLocation = OtherActor->GetActorLocation() * 0.01;
	UGravityMovementComponent* GravityMovementComponent = OtherActor->GetComponentByClass<UGravityMovementComponent>();
	if (GravityMovementComponent)
	{
		FVector TwoBodyVector = OtherLocation - OwnerLocation;
		double TwoBodyLength = TwoBodyVector.Length();
		if (TwoBodyLength < Diameter * 0.5 + GravityMovementComponent->Diameter * 0.5)
		{
			// �ں�����
			MergeDestroy(OtherActor);
		}
		double Force = CalculateForce(Mass, GravityMovementComponent->Mass, TwoBodyLength);
		return TwoBodyVector.GetSafeNormal() * Force;
	}
	return FVector(0, 0, 0);
}

double UGravityMovementComponent::CalculateForce(double MassA, double MassB, double Distance)
{
	return GravitationalConstant * MassA * MassB / (Distance * Distance);
}

FVector UGravityMovementComponent::CalculateAllVectorForce(TSet<AActor*> OtherActors)
{
	FVector AllVectorForce = FVector(0, 0, 0);
	for (AActor* OtherActor : OtherActors)
	{
		AllVectorForce += CalculateVectorForce(OtherActor);
	}
	return AllVectorForce;
}

void UGravityMovementComponent::MergeDestroy(AActor* OtherActor)
{
	UGravityMovementComponent* OtherGravityMovementComp = OtherActor->GetComponentByClass<UGravityMovementComponent>();

	AActor* OwnerActor = GetOwner();
	// ���������������λ��
	OwnerActor->SetActorLocation((OwnerActor->GetActorLocation() * Mass + OtherActor->GetActorLocation() * OtherGravityMovementComp->Mass) / (Mass + OtherGravityMovementComp->Mass));
	// �����µ����
	Diameter = FMath::Pow(FMath::Pow(Diameter, 3) + FMath::Pow(OtherGravityMovementComp->Diameter, 3), 1.0 / 3.0);
	OwnerActor->SetActorScale3D(FVector(Diameter, Diameter, Diameter));
	// ����������ٶ� Ŀǰ��֡���⣬��һЩ΢С���
	Velocity = (Mass * Velocity + OtherGravityMovementComp->Mass * OtherGravityMovementComp->Velocity) / (Mass + OtherGravityMovementComp->Mass);
	// ��������ļ��ٶ�
	Acceleration += OtherGravityMovementComp->Acceleration;
	// �������������
	Mass += OtherGravityMovementComp->Mass;
	// ����������ۺ�����
	VectorForce += OtherGravityMovementComp->VectorForce;
	MergeExtraEvent(OtherActor);
	OtherActor->Destroy();
}
