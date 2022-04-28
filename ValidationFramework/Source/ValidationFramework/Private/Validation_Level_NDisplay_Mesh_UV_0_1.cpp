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


#include "Validation_Level_NDisplay_Mesh_UV_0_1.h"

#include "ValidationBPLibrary.h"


UValidation_Level_NDisplay_Mesh_UV_0_1::UValidation_Level_NDisplay_Mesh_UV_0_1()
{
	ValidationName = "NDisplay - Check UVs 0 To 1";
	ValidationDescription = "Any meshes which are being used to build nDisplay setups should have their UVs in a 0-1 UV Space. "
							"Verts which fall outside of 0-1 UV space will cause issues and artifacts with nDisplay";
	FixDescription = "No Fix available artists will need to reimport the meshes with correct UVs";
	ValidationScope = EValidationScope::Level;
	ValidationApplicableWorkflows = {
		EValidationWorkflow::ICVFX
	};
}

FValidationResult UValidation_Level_NDisplay_Mesh_UV_0_1::Validation_Implementation()
{
#if PLATFORM_WINDOWS || PLATFORM_LINUX
	const UWorld* World = GetCorrectValidationWorld();
	const TFunction<EValidationStatus(UStaticMesh* StaticMesh, const int LodIndex, FString& Message)> ProcessFunction =
		&UValidation_Level_NDisplay_Mesh_UV_0_1::ValidateUVs;
	
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

FValidationFixResult UValidation_Level_NDisplay_Mesh_UV_0_1::Fix_Implementation()
{
#if PLATFORM_WINDOWS || PLATFORM_LINUX
	FValidationFixResult Result = FValidationFixResult(EValidationFixStatus::Fixed, "");
	
	const UWorld* World = GetCorrectValidationWorld();
	const TFunction<EValidationStatus(UStaticMesh* StaticMesh, const int LodIndex, FString& Message)> ProcessFunction =
		&UValidation_Level_NDisplay_Mesh_UV_0_1::ValidateUVs;
	
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

EValidationStatus UValidation_Level_NDisplay_Mesh_UV_0_1::ValidateUVs(UStaticMesh* StaticMesh, const int LodIndex,
	FString& Message)
{
	EValidationStatus Result = EValidationStatus::Pass;
	const uint32 NumUVChannels = StaticMesh->GetNumUVChannels(LodIndex);
	const FStaticMeshLODResources& LODResource = StaticMesh->GetRenderData()->LODResources[LodIndex];
	const uint32 NumVerts = LODResource.VertexBuffers.StaticMeshVertexBuffer.GetNumVertices();
	for (uint32 UV = 0; UV < NumUVChannels; UV++)
	{
		for (uint32 i = 0; i < NumVerts; i++)
		{
			const FVector2D UVPosition = LODResource.VertexBuffers.StaticMeshVertexBuffer.GetVertexUV(i, UV);
			if (UVPosition.X > 1 || UVPosition.X < 0 || UVPosition.Y > 1 || UVPosition.Y < 0)
			{
				Result = EValidationStatus::Fail;
				Message += StaticMesh->GetPathName() + " LOD " + FString::FromInt(LodIndex) + " Has UVs In Channel " + FString::FromInt(UV) + " Outside 0-1 Space\n";
				break;
			}
		}
	}

	return Result;
}
