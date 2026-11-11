// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "ControlRig.h"
#include "MyControlrig.generated.h"

USTRUCT(BlueprintType)
struct FControlStruct
{
	GENERATED_BODY()
	UPROPERTY()
	TArray<FTransform> FootLockedTransform;
	
	UPROPERTY()
	TArray<FTransform> FootTargetTransform;
	
	UPROPERTY()
	TArray<FTransform> FootOutputTransform;
	
	UPROPERTY()
	TArray<FRigElementKey> FootControls;
	
	UPROPERTY()
	TArray<float> CycleTime;
	
	UPROPERTY()
	TArray<float> EndTime;
	
	UPROPERTY()
	TArray<float> StartTime;
	
	UPROPERTY()
	TArray<bool> IsStepping;
};

USTRUCT()
struct FRigUnitMutable_UpdateCycleTime : public FRigUnitMutable
{
	GENERATED_BODY()
	RIGVM_METHOD()
	void Execute();
	
	UPROPERTY(meta=(input, output))
	FControlStruct RigStruct;
};

USTRUCT()
struct FRigUnitMutable_UpdateFoot : public FRigUnitMutable
{
	GENERATED_BODY()
	RIGVM_METHOD()
	void Execute();
	
	UPROPERTY(meta=(input, output))
	FControlStruct RigStruct;
	
	UPROPERTY(meta=(input))
	FVector Velocity;
	
	UPROPERTY(meta=(input))
	float ScaleFactor;
};

USTRUCT()
struct FRigUnitMutable_UpdateOutput : public FRigUnitMutable
{
	GENERATED_BODY()
	RIGVM_METHOD()
	void Execute();
	
	UPROPERTY(meta=(input, output))
	FControlStruct RigStruct;
	
	UPROPERTY(meta=(input))
	float StepHeight;
};


UCLASS(Blueprintable)
class PROCEDURALANIM_API UMyControlrig : public UControlRig
{
	GENERATED_BODY()

public:
	
	
protected:
	virtual void Initialize(bool bRequestInit = true) override;
	
};
