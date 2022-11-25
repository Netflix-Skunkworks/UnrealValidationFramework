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


#include "Validation_TimecodeProvider.h"

#include "ValidationBPLibrary.h"
#include "VFProjectSettingsBase.h"
#include "Engine/TimecodeProvider.h"


UValidation_TimecodeProvider::UValidation_TimecodeProvider()
{
	ValidationName = "Timecode Provider Validation";
	ValidationDescription = "Ensures the TimecodeProvider is valid for the project config";
	FixDescription = "Requires Manual Configuration Based On The Users Workflow Which Could Involve, Physical Hardware Changes";
	ValidationScope = EValidationScope::Project;
	ValidationApplicableWorkflows = {
		EValidationWorkflow::SimulCam,
		EValidationWorkflow::ICVFX
	};
}

FValidationResult UValidation_TimecodeProvider::Validation_Implementation()
{
	FValidationResult VFResult = FValidationResult();
	VFResult.Result = EValidationStatus::Pass;
	
	// Check that we have a timecode provider configured in the project
	const FSoftClassPath SoftClassPath = GetDefault<UEngine>()->TimecodeProviderClassName;
	if (SoftClassPath.ToString().Len() == 0)
	{
		VFResult.Result = EValidationStatus::Fail;
		VFResult.Message = "No TimeCode Provider Configured For Project";
	}

	// Check that if we are generating default timecode that it matches the validation project settings
	const FFrameRate GeneratedFrameRate = GetDefault<UEngine>()->GenerateDefaultTimecodeFrameRate;

	// Ensure we have validation framework settings configured in the project before continuing 
	UObject* Settings = UValidationBPLibrary::GetValidationFrameworkProjectSettings();
	if (Settings == nullptr)
	{
		VFResult.Result = EValidationStatus::Fail;
		VFResult.Message += "\nNo Validation Framework Settings In Project";
		return VFResult;
	}

	// Check That The Default Generated FrameRate Is Matching The FrameRate For The Project
	const UVFProjectSettingsBase* ProjectSettings = Cast<UVFProjectSettingsBase>(Settings);
	const FFrameRate ProjectFrameRate = ProjectSettings->ProjectFrameRate;

	const EFrameRateComparisonStatus GeneratedFrameRateComparison = UValidationBPLibrary::CompareFrameRateCompatability(
		GeneratedFrameRate, ProjectFrameRate);

	if (GeneratedFrameRateComparison == EFrameRateComparisonStatus::InValid)
	{
		VFResult.Result = EValidationStatus::Fail;
		VFResult.Message += "\nThe Default Generated Rate Does Not Match The Project Frame Rate";
	}
	

	// If We Have A Timecode Provider Check Its Rate Matches The Validation Framework Project Rate
	const UTimecodeProvider* TimecodeProvider = GEngine->GetTimecodeProvider();
	if (TimecodeProvider)
	{
		const FFrameRate TimecodeProviderRate = TimecodeProvider->GetFrameRate();
		const EFrameRateComparisonStatus TimecodeProviderRateComparison = UValidationBPLibrary::CompareFrameRateCompatability(
			TimecodeProviderRate, ProjectFrameRate);

		if (TimecodeProviderRateComparison == EFrameRateComparisonStatus::InValid)
		{
			VFResult.Result = EValidationStatus::Fail;
			VFResult.Message += "\nThe FrameRate Provided By The TimecodeProvider Does Not Match The Project Frame Rate";
		}
	}
	
	// If We Have A Timecode Provider Check Its Not A System Timecode Generator & Advise Not To Use It 
	if (TimecodeProvider)
	{
		if (TimecodeProvider->IsA(TimecodeProvider->StaticClass()))
		{
			if (VFResult.Result > EValidationStatus::Warning)
			{
				VFResult.Result = EValidationStatus::Warning;	
			}
			
        	VFResult.Message += "\nThe Timecode Provider Being Used Is Derived From System Time It Is Advisable To Use A";
			VFResult.Message += "\nPhysical Hardware Connection To A Timecode Generator Or Master Clock";
			VFResult.Message += "\nWhich Should Be Shared With Camera, Sound etc.";
			VFResult.Message += "\nOtherwise ensure that all the machine system clocks are being kept in sync with PTP";
			VFResult.Message += "\nTo ensure timecode generated on each machine is also accurate and in sync";
		}
	}
	

	if (VFResult.Result == EValidationStatus::Pass)
	{
		VFResult.Message = "Valid";
	}
	
	return VFResult;
}

FValidationFixResult UValidation_TimecodeProvider::Fix_Implementation() 
{
	UE_LOG(LogTemp, Warning, TEXT("Running UValidation_TimecodeProvider Fix") );
	FValidationFixResult ValidationFixResult = FValidationFixResult(
		EValidationFixStatus::ManualFix,
		"This Requires The User To Fix Manually The Project Settings For Their Workflow And Restart Unreal");
	return ValidationFixResult;
}