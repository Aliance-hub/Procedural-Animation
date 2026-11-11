#include "MyControlrig.h"

void UMyControlrig::Initialize(bool bRequestInit)
{
	Super::Initialize(bRequestInit);
}

FRigUnitMutable_UpdateCycleTime_Execute()
{
	for (int i = 0; i < RigStruct.FootControls.Num(); i++)
	{
		RigStruct.CycleTime[i] = RigStruct.CycleTime[i] + ExecuteContext.GetDeltaTime();
	}
}

FRigUnitMutable_UpdateFoot_Execute()
{
	URigHierarchy* RigHierarchy = ExecuteContext.Hierarchy;
	if (RigHierarchy == nullptr) return;

	const UWorld* World = ExecuteContext.GetWorld();
	if (World == nullptr) return;

	for (int FootIndex = 0; FootIndex < RigStruct.FootControls.Num(); FootIndex++)
	{
		FTransform FootRigTransform = RigHierarchy->GetGlobalTransform(RigStruct.FootControls[FootIndex]);
		FVector FootGlobalVector = ExecuteContext.ToWorldSpace(FootRigTransform.GetLocation());

		FVector PredictedVector = FootGlobalVector + (Velocity * ScaleFactor);
		
		FHitResult HitResult;
		FVector StartTrace = PredictedVector + FVector(0.0f, 0.0f, 350.0f);
		FVector EndTrace = PredictedVector + FVector(0.0f, 0.0f, -350.0f);
		FCollisionShape SphereShape = FCollisionShape::MakeSphere(10.0f);
		FCollisionQueryParams Params(SCENE_QUERY_STAT(HumanLegTrace), true);

		bool bTraceResult = World->SweepSingleByChannel(HitResult, StartTrace, EndTrace, FQuat::Identity,
		                                                ECollisionChannel::ECC_Visibility, SphereShape, Params);
		
		FVector FinalWorldTarget = bTraceResult ? FVector(HitResult.ImpactPoint) : PredictedVector;
		FVector FinalRigTarget = ExecuteContext.ToVMSpace(FinalRigTarget); 
		
		if (FVector::Distance(FinalWorldTarget, FootGlobalVector)> 100.0f && !RigStruct.IsStepping[FootIndex])
		{
			RigStruct.StartTime[FootIndex]= RigStruct.CycleTime[FootIndex];
			RigStruct.FootTargetTransform[FootIndex].SetLocation(FinalRigTarget);
			RigStruct.EndTime[FootIndex]= RigStruct.CycleTime[FootIndex]+ ScaleFactor;
			RigStruct.IsStepping[FootIndex] = true;			
		}
		
		if (RigStruct.CycleTime[FootIndex]>RigStruct.EndTime[FootIndex] && RigStruct.IsStepping[FootIndex])
		{
			RigStruct.IsStepping[FootIndex] = false;
			RigStruct.FootLockedTransform[FootIndex]= RigStruct.FootTargetTransform[FootIndex];
		}
		
	}
}

FRigUnitMutable_UpdateOutput_Execute()
{
		for (int FootIndex=0; FootIndex < RigStruct.FootControls.Num(); FootIndex++)
		{
			FVector StartVector = RigStruct.FootLockedTransform[FootIndex].GetLocation();
			FVector EndVector = RigStruct.FootTargetTransform[FootIndex].GetLocation();
			FQuat StartQuat = RigStruct.FootLockedTransform[FootIndex].GetRotation();
			FQuat EndQuat = RigStruct.FootTargetTransform[FootIndex].GetRotation();
			
			float Duration= RigStruct.EndTime[FootIndex] - RigStruct.CycleTime[FootIndex];
			float CurrentProgress= RigStruct.CycleTime[FootIndex]- RigStruct.StartTime[FootIndex];
			float InterpolateTime= FMath::Clamp(CurrentProgress/Duration, 0.0f, 1.0f);
			float HeightMultiplier =(FMath::Sin(PI* InterpolateTime)) * (FMath::Sin(PI* InterpolateTime));
			
			FVector TargetVector= FMath::Lerp(StartVector, EndVector, InterpolateTime);
			FQuat TargetQuat= FQuat::Slerp(StartQuat, EndQuat, InterpolateTime);
			
			TargetVector.Z+= HeightMultiplier * StepHeight;
			
			FVector FinalVector= ExecuteContext.ToVMSpace(TargetVector);
			FQuat FinalQuat= ExecuteContext.ToVMSpace(TargetQuat);			
			
			RigStruct.FootOutputTransform[FootIndex].SetLocation(FinalVector);
			RigStruct.FootOutputTransform[FootIndex].SetRotation(FinalQuat);
			
			UE_LOG(LogTemp,Warning, TEXT("Start Time: %f"), RigStruct.StartTime[FootIndex]);
			UE_LOG(LogTemp, Warning, TEXT("End Time: %f"), RigStruct.EndTime[FootIndex]);
			UE_LOG(LogTemp,Warning, TEXT("Cycle Time: %f"), RigStruct.CycleTime[FootIndex]);
		}
}