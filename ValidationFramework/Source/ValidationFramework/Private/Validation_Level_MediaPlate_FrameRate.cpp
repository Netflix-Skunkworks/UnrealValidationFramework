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

#include "Validation_Level_MediaPlate_FrameRate.h"

#include "MediaPlate.h"
#include "Kismet/GameplayStatics.h"
#include "MediaPlateComponent.h"
#include "MediaSource.h"
#include "MediaPlaylist.h"
#include "ImgMediaSource.h"
#include "ValidationBPLibrary.h"
#include "VFProjectSettingsBase.h"


UValidation_Level_MediaPlate_FrameRate::UValidation_Level_MediaPlate_FrameRate()
{
	ValidationName = "MediaPlate Frame Rate";
	ValidationDescription = "Ensures that the MediaPlate sources are set to a compatible shooting rate";
	FixDescription = "Sets the FrameRate of the ImgMediaSource to match the project settings, and ensures DX12 loaded";
	ValidationScope = EValidationScope::Level,
	ValidationApplicableWorkflows = {
		EValidationWorkflow::ICVFX,
		EValidationWorkflow::VAD,
		EValidationWorkflow::VRScouting,
		EValidationWorkflow::SimulCam
	};
}

TArray<UImgMediaSource*> UValidation_Level_MediaPlate_FrameRate::GetAllMediaSourcesFromLevel() const
{
	const UWorld* World = GetCorrectValidationWorld();

	TArray<UImgMediaSource*> ImgMediaSourceArray;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, AMediaPlate::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		const AMediaPlate* MediaPlateActor = Cast<AMediaPlate>(FoundActor);
		TInlineComponentArray<UMediaPlateComponent*> MediaPlateComponents;
		MediaPlateActor->GetComponents(MediaPlateComponents);
		for (const UMediaPlateComponent* MediaPlateComponent : MediaPlateComponents)
		{
			const TObjectPtr<UMediaPlaylist> MediaPlaylist = MediaPlateComponent->GetMediaPlaylist();
			for (int i = 0; i < MediaPlaylist->Num(); i++)
			{
				UMediaSource* MediaSource = MediaPlaylist->Get(i);
				UImgMediaSource* ImgMediaSource = Cast<UImgMediaSource>(MediaSource);
				ImgMediaSourceArray.AddUnique(ImgMediaSource);
				
			}
		}
	}
	return ImgMediaSourceArray;
}

FValidationResult UValidation_Level_MediaPlate_FrameRate::Validation_Implementation()
{
	FValidationResult ValidationResult = UValidation_Project_DX12::Validation_Implementation();
	FString Message = "";

	if (ValidationResult.Result == EValidationStatus::Pass)
	{
		ValidationResult.Message = Message;
	}

	const UObject* Settings = UValidationBPLibrary::GetValidationFrameworkProjectSettings();
	if (Settings == nullptr)
	{
		ValidationResult.Result = EValidationStatus::Fail;
		ValidationResult.Message += "\nNo Validation Framework Settings In Project";
		return ValidationResult;
	}

	const UVFProjectSettingsBase* ProjectSettings = Cast<UVFProjectSettingsBase>(Settings);
	const FFrameRate ProjectFrameRate = ProjectSettings->ProjectFrameRate;

	TArray<UImgMediaSource*> ImgMediaSourceArray = GetAllMediaSourcesFromLevel();
	for (const UImgMediaSource* ImgMediaSource : ImgMediaSourceArray)
	{
		const FFrameRate MediaFrameRate = ImgMediaSource->FrameRateOverride;
		const EFrameRateComparisonStatus Compatible = UValidationBPLibrary::CompareFrameRateCompatability(
			 MediaFrameRate, ProjectFrameRate);

		if (Compatible == EFrameRateComparisonStatus::InValid)
		{
			ValidationResult.Result = EValidationStatus::Fail;
			ValidationResult.Message += "\nInCompatible Frame Rate Set In ImageMediaSource Asset\n" + ImgMediaSource->GetPathName();
		}

		if (Compatible == EFrameRateComparisonStatus::ValidMultiple)
		{
			if (ValidationResult.Result > EValidationStatus::Warning)
			{
				ValidationResult.Result = EValidationStatus::Warning;
			}
			ValidationResult.Message += "\nFrame Rate Set In ImageMediaSource Asset Is Compatible Multiple. "
										"This May Be Intentional " + ImgMediaSource->GetPathName();;
		}
	}

	if (ValidationResult.Result == EValidationStatus::Pass)
	{
		ValidationResult.Message = "Valid";
	}
	else
	{
		Message += "\nUnable To Determine Frame Rates Of Sequences Needs Manually Setting\n";
		ValidationResult.Message += Message;
	}
	
	return ValidationResult;
}

FValidationFixResult UValidation_Level_MediaPlate_FrameRate::Fix_Implementation() 
{
	const FValidationResult ValidationResult = Validation_Implementation();
	FValidationFixResult ValidationFixResult = UValidation_Project_DX12::Fix_Implementation();
	

	if (ValidationResult.Result  != EValidationStatus::Pass)
	{
		ValidationFixResult.Result = EValidationFixStatus::ManualFix;
		ValidationFixResult.Message = "Unable To Determine Frame Rates Of Sequences Needs Manually Setting\n";
	}
	
	
	return ValidationFixResult;
}



