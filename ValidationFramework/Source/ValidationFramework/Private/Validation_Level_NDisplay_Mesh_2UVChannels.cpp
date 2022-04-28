/**
Copyright 2022 Netflix, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "Validation_Level_NDisplay_Mesh_2UVChannels.h"
#include "ValidationBPLibrary.h"


UValidation_Level_NDisplay_Mesh_2UVChannels::UValidation_Level_NDisplay_Mesh_2UVChannels()
{
	ValidationName = "NDisplay - Check 2 UV Channels";
	ValidationDescription = "Any meshes which are being used to build nDisplay setups need to have two UV channels."
							"We do not check the UVs are correct but at least check to ensure its not been missed, "
							"See unreal docs for detailed info";
	FixDescription = "No Fix available for this as will need to be fixed manually by the artists";
	ValidationScope = EValidationScope::Level;
	ValidationApplicableWorkflows = {
		EValidationWorkflow::ICVFX
	};
}

FValidationResult UValidation_Level_NDisplay_Mesh_2UVChannels::Validation_Implementation()
{
#if PLATFORM_WINDOWS || PLATFORM_LINUX
	const UWorld* World = GetCorrectValidationWorld();
	const TFunction<EValidationStatus(UStaticMesh* StaticMesh, const int LodIndex, FString& Message)> ProcessFunction =
		&UValidation_Level_NDisplay_Mesh_2UVChannels::ValidateMeshesWithout2UVChannels;
	
	FValidationResult Result =  UValidationBPLibrary::NDisplayMeshSettingsValidation(
		World, ProcessFunction);

	if (Result.Result == EValidationStatus::Fail)
	{
		Result.Message += "Ensure LightMass UV Auto Generation Is Off & Meshes Have 2 UV Channels Only";
	}
	
	return Result;
#endif

#if PLATFORM_MAC
	FValidationResult ValidationResult = FValidationResult();
	ValidationResult.Result = EValidationStatus::Warning;
	ValidationResult.Message = "Ndisplay Validations Not Valid On OSX";
	return ValidationResult;
#endif
}

FValidationFixResult UValidation_Level_NDisplay_Mesh_2UVChannels::Fix_Implementation()
{
#if PLATFORM_WINDOWS || PLATFORM_LINUX
	FValidationFixResult Result = FValidationFixResult(EValidationFixStatus::Fixed, "");
	
	const UWorld* World = GetCorrectValidationWorld();
	const TFunction<EValidationStatus(UStaticMesh* StaticMesh, const int LodIndex, FString& Message)> ProcessFunction =
		&UValidation_Level_NDisplay_Mesh_2UVChannels::ValidateMeshesWithout2UVChannels;
	
	FValidationResult VResult =  UValidationBPLibrary::NDisplayMeshSettingsValidation(
		World, ProcessFunction);

	if (VResult.Result != EValidationStatus::Pass)
	{
		Result.Result = EValidationFixStatus::ManualFix;
		Result.Message = VResult.Message;
		Result.Message += "Ensure LightMass UV Auto Generation Is Off & Meshes Have 2 UV Channels Only";
	}
	return Result;
#endif

#if PLATFORM_MAC
	FValidationFixResult ValidationFixResult = FValidationFixResult();
	ValidationFixResult.Result = EValidationFixStatus::NotFixed;
	ValidationFixResult.Message = "Ndisplay Validations Not Valid On OSX";
	return ValidationFixResult;
#endif
}

EValidationStatus UValidation_Level_NDisplay_Mesh_2UVChannels::ValidateMeshesWithout2UVChannels(UStaticMesh* StaticMesh, const int LodIndex, FString& Message)
{
	const int NumUVChannels = StaticMesh->GetNumUVChannels(LodIndex);
	if (NumUVChannels != 2)
	{
		Message = StaticMesh->GetPathName() + " LOD " + FString::FromInt(LodIndex) + " Has " + FString::FromInt(NumUVChannels) +" UV Channels Instead Of 2\n";
		return EValidationStatus::Fail;
	}

	return EValidationStatus::Pass;
}

