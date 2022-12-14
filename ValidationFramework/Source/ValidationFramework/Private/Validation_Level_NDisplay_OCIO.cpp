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

#include "Validation_Level_NDisplay_OCIO.h"
#include "Kismet/GameplayStatics.h"
#include "DisplayClusterRootActor.h"
#include "DisplayClusterConfigurationTypes.h"
#include "ValidationBPLibrary.h"
#include "VFProjectSettingsBase.h"
#include "Components/DisplayClusterICVFXCameraComponent.h"


UValidation_Level_NDisplay_OCIO::UValidation_Level_NDisplay_OCIO()
{
	ValidationName = "NDisplay - OCIO Setup";
	ValidationDescription = "OCIO ensures we have a color managed workflow, having mismatching or missing OCIO configs leads to content appearing incorrect";
	FixDescription = "Requires a manual fix as is often dependent on the make up of the led volumes, walls/viewports "
				  "may need different ocio configs which is not trivial to describe at this point";
	ValidationScope = EValidationScope::Level;
	ValidationApplicableWorkflows = {
		EValidationWorkflow::ICVFX,
		EValidationWorkflow::VAD
	};
}

void UValidation_Level_NDisplay_OCIO::ValidateOCIOColorConversionSettings(
	FValidationResult& ValidationResult,
	const FOpenColorIOColorConversionSettings OCIOSettings1,
	FOpenColorIOColorConversionSettings OCIOSettings2, FString OCIOObjectName, bool HardFail
	)
{

	EValidationStatus Status = EValidationStatus::Fail;
	if (!HardFail)
	{
		Status = EValidationStatus::Warning;
	}
	if (OCIOSettings2.ConfigurationSource != OCIOSettings1.ConfigurationSource)
	{
		if (ValidationResult.Result > Status)
		{
			ValidationResult.Result = Status;	
		}
		ValidationResult.Message += OCIOObjectName + " Has A Different OCIO Config To Those In The Validation Project Settings\n";
	}

	if (OCIOSettings2.SourceColorSpace != OCIOSettings1.SourceColorSpace)
	{
		if (ValidationResult.Result > Status)
		{
			ValidationResult.Result = Status;	
		}
		ValidationResult.Message += OCIOObjectName + " Has A Different Source Color Space To Those In The Validation Project Settings\n";
		ValidationResult.Message += OCIOSettings2.SourceColorSpace.ToString() + " v " + OCIOSettings1.SourceColorSpace.ToString() + "\n";
	}

	if (OCIOSettings2.DestinationColorSpace != OCIOSettings1.DestinationColorSpace)
	{
		if (ValidationResult.Result > Status)
		{
			ValidationResult.Result = Status;	
		}
		ValidationResult.Message += OCIOObjectName + " Has A Different Destination Color Space To Those In The Validation Project Settings\n";
		ValidationResult.Message += OCIOSettings2.DestinationColorSpace.ToString() + " v " + OCIOSettings1.DestinationColorSpace.ToString() + "\n";
	}
			
	if (OCIOSettings2.DestinationDisplayView != OCIOSettings1.DestinationDisplayView)
	{
		ValidationResult.Result = EValidationStatus::Fail;
		ValidationResult.Message += OCIOObjectName + " Has A Different Destination Display View To Those In The Validation Project Settings\n";
		ValidationResult.Message += OCIOSettings2.DestinationDisplayView.ToString() + " v " + OCIOSettings1.DestinationDisplayView.ToString() + "\n";
	}
}

void UValidation_Level_NDisplay_OCIO::ValidateAllViewportOCIOSetups(
	FValidationResult& ValidationResult,
	const FOpenColorIOColorConversionSettings ProjectOCIOSettings,
	FString OCIOObjectName, const FDisplayClusterConfigurationICVFX_StageSettings StageSettings)
{
	if (!StageSettings.bUseOverallClusterOCIOConfiguration)
	{
		ValidationResult.Result = EValidationStatus::Fail;
		ValidationResult.Message += OCIOObjectName +"\nUse OCIO Config For All Viewports Not Enabled\n";
	}
		
	FOpenColorIOColorConversionSettings AllViewPortsOCIOSettings = StageSettings.
	                                                                AllViewportsOCIOConfiguration.
	                                                                OCIOConfiguration.
	                                                                ColorConfiguration;
		
	if (!AllViewPortsOCIOSettings.IsValid())
	{
		ValidationResult.Result = EValidationStatus::Fail;
		ValidationResult.Message += OCIOObjectName +"\nAll Viewports OCIO Config Asset, Is Not Set or Has Sources or Destinations Which Do Not Exist In The OCIO Config File\n";
	
	}
	else
	{
		ValidateOCIOColorConversionSettings(
			ValidationResult, ProjectOCIOSettings, AllViewPortsOCIOSettings, OCIOObjectName);
	}
		
	
}

void UValidation_Level_NDisplay_OCIO::ValidatePerViewportOCIOOverrideSetups(
	FValidationResult& ValidationResult, const FOpenColorIOColorConversionSettings ProjectOCIOSettings,
	FString OCIOObjectName, const FDisplayClusterConfigurationICVFX_StageSettings StageSettings)
{
	for (int Viewport_Idx = 0; Viewport_Idx < StageSettings.PerViewportOCIOProfiles.Num(); Viewport_Idx++)
	{
		
		if (StageSettings.PerViewportOCIOProfiles[Viewport_Idx].bIsEnabled)
		{
				
			FOpenColorIOColorConversionSettings PerViewPortOCIOSettings = StageSettings.
			                                                              PerViewportOCIOProfiles[Viewport_Idx].
			                                                              OCIOConfiguration.
			                                                              ColorConfiguration;

			if (!PerViewPortOCIOSettings.IsValid())
			{
					
				ValidationResult.Result = EValidationStatus::Fail;
				ValidationResult.Message += OCIOObjectName +"\nPer Viewports OCIO Config Index " +
					FString::FromInt(Viewport_Idx) + 
					" Asset, Is Not Set or Has Sources or Destinations Which Do Not Exist In The OCIO Config File\n";
	
			}
			else
			{
				ValidateOCIOColorConversionSettings(ValidationResult, ProjectOCIOSettings,
					PerViewPortOCIOSettings, OCIOObjectName, false);
			}
		}
		else
		{
			if (ValidationResult.Result > EValidationStatus::Warning)
			{
				ValidationResult.Result = EValidationStatus::Warning;
			}
			ValidationResult.Message += OCIOObjectName +"\nPer Viewports OCIO Config Index " +
				FString::FromInt(Viewport_Idx) + 
				" Exists But Not Enabled\n";
				
		}
	}
}

void UValidation_Level_NDisplay_OCIO::ValidateInnerFrustumOCIOSetups(
	FValidationResult& ValidationResult, const FOpenColorIOColorConversionSettings ProjectOCIOSettings,
	FString ComponentName, FDisplayClusterConfigurationICVFX_CameraSettings Icvfx_CameraSettings)
{
	if (!Icvfx_CameraSettings.AllNodesOCIOConfiguration.bIsEnabled)
	{
		ValidationResult.Result = EValidationStatus::Fail;
		ValidationResult.Message += ComponentName + "\nInner Frustum OCIO Is Not Enabled\n";
	}
	FOpenColorIOColorConversionSettings InnerFrustumOCIOConfig = Icvfx_CameraSettings
		.AllNodesOCIOConfiguration
		.OCIOConfiguration
		.ColorConfiguration;
			
	if (!InnerFrustumOCIOConfig.IsValid())
	{
		ValidationResult.Result = EValidationStatus::Fail;
		ValidationResult.Message += ComponentName +"\nInner Frustum OCIO Config Asset, Is Not Set or Has Sources or Destinations Which Do Not Exist In The OCIO Config File\n";
	
	}
	else
	{
		ValidateOCIOColorConversionSettings(ValidationResult, ProjectOCIOSettings,
			InnerFrustumOCIOConfig, ComponentName);
	}
}

void UValidation_Level_NDisplay_OCIO::ValidateInnerFrustumOCIOPerNodeSetups(
	FValidationResult& ValidationResult,
	const FOpenColorIOColorConversionSettings ProjectOCIOSettings,
	FString ComponentName, FDisplayClusterConfigurationICVFX_CameraSettings Icvfx_CameraSettings)
{
	for (int PerNodeIndex = 0; PerNodeIndex < Icvfx_CameraSettings.PerNodeOCIOProfiles.Num(); PerNodeIndex++)
	{
		if(!Icvfx_CameraSettings.PerNodeOCIOProfiles[PerNodeIndex].bIsEnabled)
		{
			if(ValidationResult.Result > EValidationStatus::Warning)
			{
				ValidationResult.Result = EValidationStatus::Warning;	
			}
			ValidationResult.Message += ComponentName + "\nInner Frustum OCIO Per Node Config " +
				FString::FromInt(PerNodeIndex) + 
				" Exists But Is Not Enabled\n";
		}
		else
		{
			FOpenColorIOColorConversionSettings InnerFrustumPerNodeOCIOConfig = Icvfx_CameraSettings
			.PerNodeOCIOProfiles[PerNodeIndex]
			.OCIOConfiguration
			.ColorConfiguration;

			if (!InnerFrustumPerNodeOCIOConfig.IsValid())
			{
				ValidationResult.Result = EValidationStatus::Fail;
				ValidationResult.Message += ComponentName +"\nInner Frustum OCIO Per Node Config "
					+ FString::FromInt(PerNodeIndex)
					+ ", Asset Is Not Set or Has Sources or Destinations Which Do Not Exist In The OCIO Config File\n";
	
			}
			else
			{
				ValidateOCIOColorConversionSettings(
				ValidationResult,
				ProjectOCIOSettings,
				InnerFrustumPerNodeOCIOConfig,
				ComponentName, false);
			}
		}
		
	}

}

FValidationResult UValidation_Level_NDisplay_OCIO::Validation_Implementation()
{
	FValidationResult ValidationResult = FValidationResult(EValidationStatus::Pass, "");

	// Ensure we have validation framework settings configured in the project before continuing
	// TODO: Refactor into function as used in a number of places
	UObject* Settings = UValidationBPLibrary::GetValidationFrameworkProjectSettings();
	if (Settings == nullptr)
	{
		ValidationResult.Result = EValidationStatus::Fail;
		ValidationResult.Message += "\nNo Validation Framework Settings In Project";
		return ValidationResult;
	}

	const UVFProjectSettingsBase* ProjectSettings = Cast<UVFProjectSettingsBase>(Settings);
	const FOpenColorIOColorConversionSettings ProjectOCIOSettings = ProjectSettings->ProjectOpenIOColorConfig;

	if (!ProjectOCIOSettings.IsValid())
	{
		ValidationResult.Result = EValidationStatus::Fail;
		ValidationResult.Message += "\nValidation Framework Project OCIO Config Asset Is Not Set, or Has Sources or "
			"Destinations Which Do Not Exist In The OCIO Config File\nUnable To Validate Or Fix";
		return ValidationResult;
		
	}
	
	const UWorld* World = GetCorrectValidationWorld();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, ADisplayClusterRootActor::StaticClass(), FoundActors);
	for (AActor* FoundActor : FoundActors)
	{
		const ADisplayClusterRootActor* MyActor = Cast<ADisplayClusterRootActor>(FoundActor);
		const UDisplayClusterConfigurationData* ConfigData = MyActor->GetConfigData();
		const FDisplayClusterConfigurationICVFX_StageSettings StageSettings = ConfigData->StageSettings;
		ValidateAllViewportOCIOSetups(ValidationResult, ProjectOCIOSettings, FoundActor->GetName(), StageSettings);
		ValidatePerViewportOCIOOverrideSetups(ValidationResult, ProjectOCIOSettings, FoundActor->GetName(), StageSettings);
		
		TInlineComponentArray<UDisplayClusterICVFXCameraComponent*> IcvfxCameraComponents;
		MyActor->GetComponents(IcvfxCameraComponents);
		for (const UDisplayClusterICVFXCameraComponent* IcvfxCameraComponent : IcvfxCameraComponents)
		{

			FDisplayClusterConfigurationICVFX_CameraSettings Icvfx_CameraSettings = IcvfxCameraComponent->GetCameraSettingsICVFX();
			if(Icvfx_CameraSettings.bEnable)
			{
				const FString ComponentName = FoundActor->GetName() + " -> " + IcvfxCameraComponent->GetName(); 
				ValidateInnerFrustumOCIOSetups(ValidationResult, ProjectOCIOSettings, ComponentName,Icvfx_CameraSettings);
				ValidateInnerFrustumOCIOPerNodeSetups(ValidationResult, ProjectOCIOSettings, ComponentName,Icvfx_CameraSettings);
			}
		}
	}

	if (ValidationResult.Result ==  EValidationStatus::Pass)
	{
		ValidationResult.Message = "Valid";
	}
	
	return ValidationResult;
}

FValidationFixResult UValidation_Level_NDisplay_OCIO::Fix_Implementation()
{

	FValidationFixResult ValidationFixResult = FValidationFixResult();
	FValidationResult ValidationResult = Validation_Implementation();
	if (ValidationResult.Result == EValidationStatus::Pass)
	{
		ValidationFixResult.Result = EValidationFixStatus::Fixed;
		ValidationFixResult.Message = "Nothing To Fix";
	}
	else
	{
		ValidationFixResult.Result = EValidationFixStatus::ManualFix;
		ValidationFixResult.Message = "Requires Manual Fix";
	}

	return ValidationFixResult;
}



