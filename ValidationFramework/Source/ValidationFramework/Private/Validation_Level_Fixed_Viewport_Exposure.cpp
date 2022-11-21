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


#include "Validation_Level_Fixed_Viewport_Exposure.h"



UValidation_Level_Fixed_Viewport_Exposure::UValidation_Level_Fixed_Viewport_Exposure()
{
	ValidationName = "Fixed Viewport Exposure";
	ValidationDescription = "Having A Fixed Viewport Exposure Overrides The Exposure Setup From Within The Level";
	FixDescription = "Sets The EV100 value to 1.0 and disables the setting so we revert to using the game logic";
	ValidationScope = EValidationScope::Level;
	ValidationApplicableWorkflows = {
		EValidationWorkflow::ICVFX,
		EValidationWorkflow::VAD
	};
}

FValidationResult UValidation_Level_Fixed_Viewport_Exposure::Validation_Implementation()
{
	FValidationResult ValidationResult = FValidationResult(EValidationStatus::Pass, "");
	
	const FEditorViewportClient* Client = static_cast<FEditorViewportClient*>(GEditor->GetActiveViewport()->GetClient());
	if (Client->ExposureSettings.bFixed)
	{
		ValidationResult.Result = EValidationStatus::Fail;
		ValidationResult.Message = "There Is A Fixed Exposure Being Driven By The Viewport";
	}
	

	if (ValidationResult.Result == EValidationStatus::Pass)
	{
		ValidationResult.Message = "Valid";
	}
	return ValidationResult;
}

FValidationFixResult UValidation_Level_Fixed_Viewport_Exposure::Fix_Implementation()
{


	FValidationFixResult ValidationFixResult = FValidationFixResult();
	ValidationFixResult.Result = EValidationFixStatus::Fixed;
	FString Message = "";

	FEditorViewportClient* Client = static_cast<FEditorViewportClient*>(GEditor->GetActiveViewport()->GetClient());
	Client->ExposureSettings.bFixed = true;
	Client->ExposureSettings.FixedEV100 = 1.0f;

	ValidationFixResult.Message = Message;

	return ValidationFixResult;
}


