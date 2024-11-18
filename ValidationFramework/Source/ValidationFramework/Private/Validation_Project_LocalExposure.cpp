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


#include "Validation_Project_LocalExposure.h"

#include "Engine/RendererSettings.h"

UValidation_Project_LocalExposure::UValidation_Project_LocalExposure()
{
	ValidationName = "Local Exposure";
	ValidationDescription = "Checks the local exposure is set to 1 as not to break the linear lighting chain";
	FixDescription = "Sets the local exposure settings to be 1 rather than 0.8 so defaults fall back to this";
	ValidationScope = EValidationScope::Project;
	ValidationApplicableWorkflows = {
		EValidationWorkflow::ICVFX,
		EValidationWorkflow::VAD
	};
}

FValidationResult UValidation_Project_LocalExposure::Validation_Implementation()
{
	FValidationResult ValidationResult = FValidationResult(EValidationStatus::Pass, "Valid");
	FString Message = "";

	const URendererSettings* Settings = GetMutableDefault<URendererSettings>();
	const float DefaultLocalExposureHighlight = Settings->DefaultFeatureLocalExposureHighlightContrast;
	if (DefaultLocalExposureHighlight != 1.0f)
	{
		ValidationResult.Result = EValidationStatus::Fail;
		Message = "Local Exposure Highlight Is Not Set To 1, This Can Cause Issues With Linear Lighting\n";
	}

	const float DefaultLocalExposureShadow = Settings->DefaultFeatureLocalExposureShadowContrast;
	if (DefaultLocalExposureShadow != 1.0f)
	{
		ValidationResult.Result = EValidationStatus::Fail;
		Message += "Local Exposure Shadow Is Not Set To 1, This Can Cause Issues With Linear Lighting\n";
	}

	if (ValidationResult.Result == EValidationStatus::Pass)
	{
		Message = "Valid";
	}

	ValidationResult.Message = Message;
	return ValidationResult;
}

FValidationFixResult UValidation_Project_LocalExposure::Fix_Implementation()
{
	FValidationFixResult FixResult = FValidationFixResult(EValidationFixStatus::Fixed, "");
	URendererSettings* Settings = GetMutableDefault<URendererSettings>();
	FString Message = "";

	const float DefaultLocalExposureHighlight = Settings->DefaultFeatureLocalExposureHighlightContrast;
	if (DefaultLocalExposureHighlight != 1.0f)
	{
		Settings->DefaultFeatureLocalExposureHighlightContrast = 1.0f;
		Message = "Set Local Exposure Highlight To 1, As This Can Cause Issues With Linear Lighting\n";
	}

	const float DefaultLocalExposureShadow = Settings->DefaultFeatureLocalExposureShadowContrast;
	if (DefaultLocalExposureShadow != 1.0f)
	{
		Settings->DefaultFeatureLocalExposureShadowContrast = 1.0f;
		Message += "Set Local Exposure Shadow To 1, This Can Cause Issues With Linear Lighting\n";
	}
	

	FixResult.Message = Message;
	return FixResult;
}
