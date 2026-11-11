# Procedural-Animation
Procedural Animation is Movement based on logic, Physics and maths using code. In this project, I'm making a procedural walk animation where a character walks based on its environment.

Right now I'm working on the files MyControlRig.h and MyControlRig.cpp.
Current Progress: I created a Struct in cpp and created a variable with the struct as the variable type in Blueprint. I created variables like cycle time, start time, endtime and so on.

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

I created a function named UpdateTime which just increases CycleTime every frame by delta time,
