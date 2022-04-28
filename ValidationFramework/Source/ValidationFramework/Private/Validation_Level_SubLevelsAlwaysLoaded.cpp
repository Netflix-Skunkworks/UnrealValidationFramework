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


#include "Validation_Level_SubLevelsAlwaysLoaded.h"
#include "EditorLevelUtils.h"
#include "Engine/LevelStreamingAlwaysLoaded.h"


UValidation_Level_SubLevelsAlwaysLoaded::UValidation_Level_SubLevelsAlwaysLoaded()
{
	ValidationName = "SubLevels - Always Loaded";
	ValidationDescription = "Working in Virtual Production any SubLevels need to have their Streaming Method set to Always Loaded";
	FixDescription = "Sets any SubLevels to have their Streaming Method set to Always Loaded";
	ValidationScope = EValidationScope::Level;
	ValidationApplicableWorkflows = {
		EValidationWorkflow::ICVFX,
		EValidationWorkflow::VAD
	};
}

FValidationResult UValidation_Level_SubLevelsAlwaysLoaded::Validation_Implementation()
{
	FValidationResult ValidationResult = FValidationResult(EValidationStatus::Pass, "");
	FString Message = "";

	const UWorld* CurrentWorld = GetCorrectValidationWorld();

	// Add models for each streaming level in the world
	for (const ULevelStreaming* StreamingLevel : CurrentWorld->GetStreamingLevels())
	{
		if (StreamingLevel)
		{
			const bool AlwaysLoaded = StreamingLevel->ShouldBeAlwaysLoaded();
			if (!AlwaysLoaded)
			{
				ValidationResult.Result = EValidationStatus::Fail;
				FName PackageName = StreamingLevel->GetWorldAssetPackageFName();
				Message += PackageName.ToString() + "\n";
			}
		}
	}
	

	if (ValidationResult.Result == EValidationStatus::Pass)
	{
		ValidationResult.Message = "Valid";
	}
	else
	{
		ValidationResult.Message = "Error - The Following Sub Levels Are Not Set To Always Loaded\n" + Message;
	}
	return ValidationResult;
}

FValidationFixResult UValidation_Level_SubLevelsAlwaysLoaded::Fix_Implementation()
{


	FValidationFixResult ValidationFixResult = FValidationFixResult();
	ValidationFixResult.Result = EValidationFixStatus::Fixed;
	FString Message = "";

	const UWorld* CurrentWorld = GetCorrectValidationWorld();
	
	// Add models for each streaming level in the world
	for (ULevelStreaming* StreamingLevel : CurrentWorld->GetStreamingLevels())
	{
		if (StreamingLevel)
		{
			const bool AlwaysLoaded = StreamingLevel->ShouldBeAlwaysLoaded();
			if (!AlwaysLoaded)
			{
				ValidationFixResult.Result = EValidationFixStatus::Fixed;
				UEditorLevelUtils::SetStreamingClassForLevel(
					StreamingLevel, ULevelStreamingAlwaysLoaded::StaticClass());
				FName PackageName = StreamingLevel->GetWorldAssetPackageFName();
				Message += PackageName.ToString() + " Set To Always Loaded\n";
				
			}
		}
	}

	ValidationFixResult.Message = Message;

	return ValidationFixResult;
}

