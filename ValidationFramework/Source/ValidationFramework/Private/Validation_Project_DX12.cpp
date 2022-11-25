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


#include "Validation_Project_DX12.h"
#include "ValidationBPLibrary.h"


UValidation_Project_DX12::UValidation_Project_DX12()
{
	ValidationName = "Check DirectX 12";
	ValidationDescription = "Whilst not needed all the time, for some workflows such as EXR playback it is a"
		" requirement, new projects starting from 5.1 default to DX12 so is advisable regardless";
	FixDescription = "Sets The Targeted RHI to DirectX 12 in Project Settings, Requires Restart";
	ValidationScope = EValidationScope::Project;
	ValidationApplicableWorkflows = {
		EValidationWorkflow::ICVFX
	};
}

FValidationResult UValidation_Project_DX12::Validation_Implementation()
{
	FValidationResult ValidationResult = FValidationResult(EValidationStatus::Pass, "");
	FString Message = "";
	
#if PLATFORM_WINDOWS
	if (!UValidationBPLibrary::CheckDefaultRHIIsDirectX12())
	{
		ValidationResult.Result = EValidationStatus::Warning;
		Message += "Project Not Using DirectX 12, This Can Causes Errors With Certain ICVFX Workflows "
					"Such As EXR Playback\n";
		
	}
	
	if (ValidationResult.Result == EValidationStatus::Pass)
	{
		Message = "Valid";
	}
	
	ValidationResult.Message = Message;
	return ValidationResult;
#else
	ValidationResult.Result = EValidationStatus::Warning;
	ValidationResult.Message = "DirectX Only Available On Windows";
	return ValidationResult;
#endif
	
}


FValidationFixResult UValidation_Project_DX12::Fix_Implementation() 
{
#if PLATFORM_WINDOWS
	FString Message = "";

	if (!UValidationBPLibrary::CheckDefaultRHIIsDirectX12())
	{
		if(UValidationBPLibrary::SetProjectRHIDirectX12())
		{
			Message += "Setting The Default Graphics RHI To DirectX 12\n";	
		}
	}
	
	return FValidationFixResult( EValidationFixStatus::Fixed, Message);
#else
	return FValidationFixResult( EValidationFixStatus::NotFixed,  "DirectX Only Available On Windows");
#endif
	
}



