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


#include "Validation_PP_Project_Exposure.h"

#include "Engine/RendererSettings.h"

UValidation_PP_Project_Exposure::UValidation_PP_Project_Exposure()
{
	ValidationName = "PP - Disable Auto Exposure";
	ValidationDescription = "This is to ensure that we disable auto exposure and ensure it is set to Manual fo ICVFX "
							"as not to break the color pipeline";
	FixDescription = "Disables auto exposure, sets auto exposure to be manual and sets the bias to 0";
	ValidationScope = EValidationScope::Project;
	ValidationApplicableWorkflows = {
		EValidationWorkflow::ICVFX
	};
}

FValidationResult UValidation_PP_Project_Exposure::Validation_Implementation()
{
	FValidationResult ValidationResult = FValidationResult(EValidationStatus::Pass, "");
	FString Message = "";

	const URendererSettings* Settings = GetMutableDefault<URendererSettings>();
	if (Settings->bDefaultFeatureAutoExposure)
	{
		ValidationResult.Result = EValidationStatus::Fail;
		Message += "Auto Exposure Is Turned On In Project Settings\n";
		
	}

	if (Settings->DefaultFeatureAutoExposure != EAutoExposureMethodUI::AEM_Manual)
	{
		ValidationResult.Result = EValidationStatus::Fail;
		Message += "Auto Exposure Is Not Set To Manual In Project Settings\n";
		
	}
	
	if (Settings->DefaultFeatureAutoExposureBias != 0.0)
    {
    	ValidationResult.Result = EValidationStatus::Fail;
    	Message += "Auto Exposure Bias Should Be Set To 0.0\n";
    	
    }

	if (ValidationResult.Result == EValidationStatus::Pass)
	{
		Message = "Valid";
	}
	
	ValidationResult.Message = Message;
	return ValidationResult;
}

FValidationFixResult UValidation_PP_Project_Exposure::Fix_Implementation() 
{
	FString Message = "";

	URendererSettings* Settings = GetMutableDefault<URendererSettings>();
	if (Settings->bDefaultFeatureAutoExposure)
	{
		Settings->bDefaultFeatureAutoExposure = false;
		Message += "Auto Exposure Turned Off In Project Settings\n";
		
	}

	if (Settings->DefaultFeatureAutoExposure != EAutoExposureMethodUI::AEM_Manual)
	{
		Settings->DefaultFeatureAutoExposure = EAutoExposureMethodUI::AEM_Manual;
		Message += "Auto Exposure Set To Manual In Project Settings\n";
		
	}
	
	if (Settings->DefaultFeatureAutoExposureBias != 0.0)
	{
		Settings->DefaultFeatureAutoExposureBias = 0.0;
		Message += "Auto Exposure Bias Set To 0.0\n";
    	
	}
	Settings->SaveConfig();
	
	return FValidationFixResult( EValidationFixStatus::Fixed, Message);
}


