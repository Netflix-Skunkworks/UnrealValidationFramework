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


#include "Validation_PP_Project_WorkingColorSpace.h"

#include "Engine/RendererSettings.h"

UValidation_PP_Project_WorkingColorSpace::UValidation_PP_Project_WorkingColorSpace()
{
	ValidationName = "PP - Aces CG Working Color Space";
	ValidationDescription = "Checks the project to see if it is using AcesCG/AP1 as this is the recommended best practises\n More Info from Epic\n"
							"http://tiny.cc/w529vz";
	FixDescription = "Warns the user the project is not using AcesCG and reccommends reading http://tiny.cc/w529vz";
	ValidationScope = EValidationScope::Project;
	ValidationApplicableWorkflows = {
		EValidationWorkflow::ICVFX,
		EValidationWorkflow::VAD
	};
}

FValidationResult UValidation_PP_Project_WorkingColorSpace::Validation_Implementation()
{
	FValidationResult ValidationResult = FValidationResult(EValidationStatus::Pass, "Valid");

	const URendererSettings* Settings = GetMutableDefault<URendererSettings>();
	EWorkingColorSpace::Type CurrentWorkingSpace = Settings->WorkingColorSpaceChoice;
	
	if (CurrentWorkingSpace != EWorkingColorSpace::ACESAP1)
	{

		UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWorkingColorSpace"), true);
		FText DisplayName = EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(CurrentWorkingSpace));
		FString CurrenColorSpace = DisplayName.ToString();
		ValidationResult.Result = EValidationStatus::Warning;
		ValidationResult.Message = "Working Color Space Is\n" + CurrenColorSpace + "\nNot\nAP1/Aces CG";

	}

	return ValidationResult;
}

FValidationFixResult UValidation_PP_Project_WorkingColorSpace::Fix_Implementation()
{
	FValidationFixResult FixResult = FValidationFixResult(EValidationFixStatus::Fixed, "");

	FValidationResult Result = Validation_Implementation();
	if (Result.Result != EValidationStatus::Pass)
	{
		FixResult.Result = EValidationFixStatus::ManualFix;
		FixResult.Message = "No Automatic Fix Available";

	}

	return FixResult;
}


