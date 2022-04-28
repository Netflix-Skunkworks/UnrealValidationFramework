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


#include "Validation_PP_Project_CorruptDefaultLevel.h"
#include "EditorAssetLibrary.h"
#include "GameMapsSettings.h"


UValidation_PP_Project_CorruptDefaultLevel::UValidation_PP_Project_CorruptDefaultLevel()
{
	ValidationName = "Corrupt Default Game & Editor Level";
	ValidationDescription = "Sometimes it is possible to set the default levels to an asset which is no longer in the "
							"project, this then crashes NDisplay";
	FixDescription = "Needs a manual fix from the artist as we do not know what their default levels should be.";
	ValidationScope = EValidationScope::Project;
	ValidationApplicableWorkflows = {
		EValidationWorkflow::ICVFX
	};
}

FValidationResult UValidation_PP_Project_CorruptDefaultLevel::Validation_Implementation()
{
	FValidationResult ValidationResult = FValidationResult(EValidationStatus::Pass, "");
	FString Message = "";

	const UGameMapsSettings* Settings = GetMutableDefault<UGameMapsSettings>();
	const FString DefaultGameMap = Settings->GetGameDefaultMap();
	if (DefaultGameMap.Len() != 0)
	{
		const bool bGameMapExists = UEditorAssetLibrary::DoesAssetExist(DefaultGameMap);
		if(!bGameMapExists){
			ValidationResult.Result = EValidationStatus::Fail;
			Message += "The GameDefaultMap Is Set To An Asset Which Does Not Exist In The Project\n";
			
		}
	}

	const FSoftObjectPath EditorStartupMap = Settings->EditorStartupMap;
	const FString EditorStartupMapString = EditorStartupMap.GetAssetPathString();
	if (EditorStartupMapString.Len() != 0)
	{
		const bool bEditorMapExists = UEditorAssetLibrary::DoesAssetExist(EditorStartupMapString);
		if(!bEditorMapExists){
			ValidationResult.Result = EValidationStatus::Fail;
			Message += "The EditorStartupMap Is Set To An Asset Which Does Not Exist In The Project\n";
			
		}
	}

	if (ValidationResult.Result == EValidationStatus::Pass)
	{
		ValidationResult.Message = "Valid";
	}
	else
	{
		ValidationResult.Message = "Error -Needs A Manual Fix\n" + Message;
	}
	return ValidationResult;
}

FValidationFixResult UValidation_PP_Project_CorruptDefaultLevel::Fix_Implementation()
{
	FValidationFixResult ValidationFixResult = FValidationFixResult();
	ValidationFixResult.Result = EValidationFixStatus::ManualFix;
	FString Message = "Needs manual fix as we do not know what the default levels should be";
	return ValidationFixResult;
}
