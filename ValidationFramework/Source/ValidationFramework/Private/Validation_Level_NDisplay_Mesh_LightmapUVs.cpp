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


#include "Validation_Level_NDisplay_Mesh_LightmapUVs.h"
#include "ValidationBPLibrary.h"


UValidation_Level_NDisplay_Mesh_LightmapUVs::UValidation_Level_NDisplay_Mesh_LightmapUVs()
{
	ValidationName = "NDisplay - Check No LightMap UVs";
	ValidationDescription = "Any meshes which are being used to build nDisplay setups should not have light map generated UV channels. "
							"nDisplay Meshes should only have 2 UV Channels and additional LightMap UVs can cause confusion";
	FixDescription = "No Fix available artists will need to reimport the meshes with Generate LightMap UVs Disabled";
	ValidationScope = EValidationScope::Level;
	ValidationApplicableWorkflows = {
		EValidationWorkflow::ICVFX
	};
}

FValidationResult UValidation_Level_NDisplay_Mesh_LightmapUVs::Validation_Implementation()
{
#if PLATFORM_WINDOWS || PLATFORM_LINUX
	const UWorld* World = GetCorrectValidationWorld();
	const TFunction<EValidationStatus(UStaticMesh* StaticMesh, const int LodIndex, FString& Message)> ProcessFunction =
		&UValidation_Level_NDisplay_Mesh_LightmapUVs::ValidateLightmapUVs;
	
	FValidationResult Result =  UValidationBPLibrary::NDisplayMeshSettingsValidation(
		World, ProcessFunction);
	
	return Result;
#endif

#if PLATFORM_MAC
	FValidationResult ValidationResult = FValidationResult();
	ValidationResult.Result = EValidationStatus::Warning;
	ValidationResult.Message = "Ndisplay Validations Not Valid On OSX";
	return ValidationResult;
#endif
}

FValidationFixResult UValidation_Level_NDisplay_Mesh_LightmapUVs::Fix_Implementation()
{
#if PLATFORM_WINDOWS || PLATFORM_LINUX
	FValidationFixResult Result = FValidationFixResult(EValidationFixStatus::Fixed, "");
	
	const UWorld* World = GetCorrectValidationWorld();
	const TFunction<EValidationStatus(UStaticMesh* StaticMesh, const int LodIndex, FString& Message)> ProcessFunction =
		&UValidation_Level_NDisplay_Mesh_LightmapUVs::ValidateLightmapUVs;
	
	FValidationResult VResult =  UValidationBPLibrary::NDisplayMeshSettingsValidation(
		World, ProcessFunction);

	if (VResult.Result != EValidationStatus::Pass)
	{
		Result.Result = EValidationFixStatus::ManualFix;
		Result.Message = VResult.Message;
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

EValidationStatus UValidation_Level_NDisplay_Mesh_LightmapUVs::ValidateLightmapUVs(UStaticMesh* StaticMesh, const int LodIndex, FString& Message)
{
	const FStaticMeshSourceModel& LODModel = StaticMesh->GetSourceModel(LodIndex);
	if (LODModel.BuildSettings.bGenerateLightmapUVs)
	{
		Message = StaticMesh->GetPathName() + " LOD " + FString::FromInt(LodIndex) + " Has Lightmap UV Generation Enabled - Should Disable & Reimport\n";
		return EValidationStatus::Fail;
	}

	return EValidationStatus::Pass;
}