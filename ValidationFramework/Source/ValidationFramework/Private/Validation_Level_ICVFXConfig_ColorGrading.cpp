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

#include "Validation_Level_ICVFXConfig_ColorGrading.h"
#include "Kismet/GameplayStatics.h"
#if PLATFORM_WINDOWS
#include "DisplayClusterRootActor.h"
#include "DisplayClusterConfigurationTypes.h"
#include "Components/DisplayClusterICVFXCameraComponent.h"
#endif



UValidation_Level_ICVFXConfig_ColorGrading::UValidation_Level_ICVFXConfig_ColorGrading()
{
	ValidationName = "NDisplay - Color Grading";
	ValidationDescription = "This is to ensure that we do not have invalid settings on the color grading for the ICVFX "
							"entire cluster and per viewport as not to break the color pipeline, the existing post "
							"processing validations already check the ICVFX camera for additional invalid post processing"
							" setting";
	FixDescription = "Disables blue correction and expand gamut for both entire cluster and per viewport";
	ValidationScope = EValidationScope::Level;
	ValidationApplicableWorkflows = {
		EValidationWorkflow::ICVFX
	};
}

#if PLATFORM_WINDOWS
void UValidation_Level_ICVFXConfig_ColorGrading::ValidateEntireClusterColorGrading(
	FValidationResult& Result, FString& ActorMessages, const FDisplayClusterConfigurationICVFX_StageSettings StageSettings)
{
	if (StageSettings.EntireClusterColorGrading.ColorGradingSettings.bOverride_AutoExposureBias)
	{
		if (StageSettings.EntireClusterColorGrading.ColorGradingSettings.AutoExposureBias != 0.0)
		{
			Result.Result = EValidationStatus::Warning;
			ActorMessages += "Entire Cluster Exposure Compensation Enabled & Not 0.0 Could Be Artistic Choice\n";
		}
	}
	
	if (StageSettings.EntireClusterColorGrading.ColorGradingSettings.Misc.bOverride_BlueCorrection)
	{
		Result.Result = EValidationStatus::Fail;
		ActorMessages += "Entire Cluster Blue Correction Enabled\n";
			
	}

	if (StageSettings.EntireClusterColorGrading.ColorGradingSettings.Misc.BlueCorrection != 0.0)
	{
		Result.Result = EValidationStatus::Fail;
		ActorMessages += "Entire Cluster Blue Correction Not Set To 0.0\n";
	}

	if (StageSettings.EntireClusterColorGrading.ColorGradingSettings.Misc.bOverride_ExpandGamut)
	{
		Result.Result = EValidationStatus::Fail;
		ActorMessages += "Entire Cluster Expand Gamut Enabled\n";
			
	}

	if (StageSettings.EntireClusterColorGrading.ColorGradingSettings.Misc.ExpandGamut != 0.0)
	{
		Result.Result = EValidationStatus::Fail;
		ActorMessages += "Entire Cluster Expand Gamut Not Set To 0.0\n";
			
	}
}

void UValidation_Level_ICVFXConfig_ColorGrading::ValidatePerViewPortColorGrading(
	FValidationResult& Result, FString& ActorMessages, const FDisplayClusterConfigurationICVFX_StageSettings StageSettings) const
{
	for (int x=0; x<StageSettings.PerViewportColorGrading.Num(); x++)
	{
		TArray< FStringFormatArg > Args;
		Args.Add( FStringFormatArg( x ) );

		if (StageSettings.PerViewportColorGrading[x].ColorGradingSettings.bOverride_AutoExposureBias)
		{
			if (StageSettings.PerViewportColorGrading[x].ColorGradingSettings.AutoExposureBias != 0.0)
			{
				Result.Result = EValidationStatus::Warning;
				ActorMessages += FString::Format
				(
					TEXT( "PerViewportColorGrading {0} Exposure Compensation Enabled & Not 0.0 Could Be Ariststic Choice\n" ), Args
				);
				
			}
		}
		
		if (StageSettings.PerViewportColorGrading[x].ColorGradingSettings.Misc.bOverride_BlueCorrection)
		{
			Result.Result = EValidationStatus::Fail;
			ActorMessages += FString::Format
			(
				TEXT( "PerViewportColorGrading {0} Blue Correction Enabled\n" ), Args
			);
		}

		if (StageSettings.PerViewportColorGrading[x].ColorGradingSettings.Misc.BlueCorrection != 0.0)
		{
			Result.Result = EValidationStatus::Fail;
			ActorMessages += FString::Format
			(
				TEXT( "PerViewportColorGrading {0} Blue Correction Not Set To 0.0\n" ), Args
			);
		}

		if (StageSettings.PerViewportColorGrading[x].ColorGradingSettings.Misc.bOverride_ExpandGamut)
		{
			Result.Result = EValidationStatus::Fail;
			ActorMessages += FString::Format
			(
				TEXT( "PerViewportColorGrading {0} Expand Gamut Enabled\n" ), Args
			);
		}

		if (StageSettings.PerViewportColorGrading[x].ColorGradingSettings.Misc.ExpandGamut != 0.0)
		{
			Result.Result = EValidationStatus::Fail;
			ActorMessages += FString::Format
			(
				TEXT( "PerViewportColorGrading {0} Expand Gamut Not Set To 0.0\n" ), Args
			);
		}
	}
}

void UValidation_Level_ICVFXConfig_ColorGrading::ValidateInnerFrustumColorGrading(
	FValidationResult& Result, FString& ActorMessages, const ADisplayClusterRootActor* Actor) const
{
	TInlineComponentArray<UDisplayClusterICVFXCameraComponent*> IcvfxCameraComponents;
	Actor->GetComponents(IcvfxCameraComponents);
	for (const UDisplayClusterICVFXCameraComponent* IcvfxCameraComponent : IcvfxCameraComponents)
	{
		const bool BlueCorrectionEnabled = IcvfxCameraComponent->CameraSettings.AllNodesColorGrading.ColorGradingSettings.Misc.bOverride_BlueCorrection;
		const float BlueCorrection = IcvfxCameraComponent->CameraSettings.AllNodesColorGrading.ColorGradingSettings.Misc.BlueCorrection;

		const float ExpandGamutEnabled = IcvfxCameraComponent->CameraSettings.AllNodesColorGrading.ColorGradingSettings.Misc.bOverride_ExpandGamut;
		const float ExpandGamut = IcvfxCameraComponent->CameraSettings.AllNodesColorGrading.ColorGradingSettings.Misc.ExpandGamut;

		TArray< FStringFormatArg > Args;
		Args.Add( FStringFormatArg( IcvfxCameraComponent->GetName() ) );
			
		if (BlueCorrectionEnabled && BlueCorrection != 0.0)
		{
			Result.Result = EValidationStatus::Fail;
			ActorMessages += FString::Format(
				TEXT("{0}\nBlue Correction Is Enabled For Inner Frustum Color Grading & Not Set To 0.0\n"),
				Args
			);
		}

		if (ExpandGamutEnabled && ExpandGamut != 0.0)
		{
			Result.Result = EValidationStatus::Fail;
			ActorMessages += FString::Format(
				TEXT("{0}\nExpand Gamut Is Enabled For Inner Frustum Color Grading & Not Set To 0.0\n"),
				Args
			);
		}
	}
}

void UValidation_Level_ICVFXConfig_ColorGrading::FixEntireClusterColorGrading(
	FString& ActorMessages, FDisplayClusterConfigurationICVFX_StageSettings& StageSettings)
{
	if (StageSettings.EntireClusterColorGrading.ColorGradingSettings.Misc.bOverride_BlueCorrection)
	{
		StageSettings.EntireClusterColorGrading.ColorGradingSettings.Misc.bOverride_BlueCorrection = false;
		ActorMessages += "Entire Cluster Blue Correction Disabled\n";
			
	}

	if (StageSettings.EntireClusterColorGrading.ColorGradingSettings.Misc.BlueCorrection != 0.0)
	{
		StageSettings.EntireClusterColorGrading.ColorGradingSettings.Misc.BlueCorrection = 0.0;
		ActorMessages += "Entire Cluster Blue Correction Set To 0.0\n";
	}

	if (StageSettings.EntireClusterColorGrading.ColorGradingSettings.Misc.bOverride_ExpandGamut)
	{
		StageSettings.EntireClusterColorGrading.ColorGradingSettings.Misc.bOverride_ExpandGamut = false;
		ActorMessages += "Entire Cluster Expand Gamut Disabled\n";
			
	}

	if (StageSettings.EntireClusterColorGrading.ColorGradingSettings.Misc.ExpandGamut != 0.0)
	{
		StageSettings.EntireClusterColorGrading.ColorGradingSettings.Misc.ExpandGamut = 0.0;
		ActorMessages += "Entire Cluster Expand Gamut Set To 0.0\n";
	}
}

void UValidation_Level_ICVFXConfig_ColorGrading::FixPerViewportColorGrading(FString& ActorMessages, FDisplayClusterConfigurationICVFX_StageSettings& StageSettings) const
{
	for (int x=0; x<StageSettings.PerViewportColorGrading.Num(); x++)
	{
		TArray< FStringFormatArg > Args;
		Args.Add( FStringFormatArg( x ) );
			
		if (StageSettings.PerViewportColorGrading[x].ColorGradingSettings.Misc.bOverride_BlueCorrection)
		{
			StageSettings.PerViewportColorGrading[x].ColorGradingSettings.Misc.bOverride_BlueCorrection = false;
			ActorMessages += FString::Format
			(
				TEXT( "PerViewportColorGrading {0} Blue Correction Disabled\n" ), Args
			);
				
		}

		if (StageSettings.PerViewportColorGrading[x].ColorGradingSettings.Misc.BlueCorrection != 0.0)
		{
			StageSettings.PerViewportColorGrading[x].ColorGradingSettings.Misc.BlueCorrection = 0.0;
			ActorMessages += FString::Format
			(
				TEXT( "PerViewportColorGrading {0} Blue Correction Set To 0.0\n" ), Args
			);
				
		}

		if (StageSettings.PerViewportColorGrading[x].ColorGradingSettings.Misc.bOverride_ExpandGamut)
		{
			StageSettings.PerViewportColorGrading[x].ColorGradingSettings.Misc.bOverride_ExpandGamut = false;
			ActorMessages += FString::Format
			(
				TEXT( "PerViewportColorGrading {0} Expand Gamut Disabled\n" ), Args
			);
				
		}

		if (StageSettings.PerViewportColorGrading[x].ColorGradingSettings.Misc.ExpandGamut != 0.0)
		{
			StageSettings.PerViewportColorGrading[x].ColorGradingSettings.Misc.ExpandGamut = 0.0;
			ActorMessages += FString::Format
			(
				TEXT( "PerViewportColorGrading {0} Expand Gamut Set To 0.0\n" ), Args
			);
		}
			
	}
}

void UValidation_Level_ICVFXConfig_ColorGrading::FixInnerFrustumColorGrading(
	FValidationFixResult& Result, FString& ActorMessages, const ADisplayClusterRootActor* Actor) const
{
	TInlineComponentArray<UDisplayClusterICVFXCameraComponent*> IcvfxCameraComponents;
	Actor->GetComponents(IcvfxCameraComponents);
	for (UDisplayClusterICVFXCameraComponent* IcvfxCameraComponent : IcvfxCameraComponents)
	{
		const bool BlueCorrectionEnabled = IcvfxCameraComponent->CameraSettings.AllNodesColorGrading.ColorGradingSettings.Misc.bOverride_BlueCorrection;
		const float BlueCorrection = IcvfxCameraComponent->CameraSettings.AllNodesColorGrading.ColorGradingSettings.Misc.BlueCorrection;

		const float ExpandGamutEnabled = IcvfxCameraComponent->CameraSettings.AllNodesColorGrading.ColorGradingSettings.Misc.bOverride_ExpandGamut;
		const float ExpandGamut = IcvfxCameraComponent->CameraSettings.AllNodesColorGrading.ColorGradingSettings.Misc.ExpandGamut;

		TArray< FStringFormatArg > Args;
		Args.Add( FStringFormatArg( IcvfxCameraComponent->GetName() ) );
			
		if (BlueCorrectionEnabled || BlueCorrection != 0.0)
		{
			Result.Result = EValidationFixStatus::Fixed;
			IcvfxCameraComponent->CameraSettings.AllNodesColorGrading.ColorGradingSettings.Misc.bOverride_BlueCorrection = false;
			IcvfxCameraComponent->CameraSettings.AllNodesColorGrading.ColorGradingSettings.Misc.BlueCorrection = 0.0;
			ActorMessages += FString::Format(
				TEXT("{0}\nBlue Correction Is Disabled For Inner Frustum Color Grading & Set To 0.0\n"),
				Args
			);
		}

		if (ExpandGamutEnabled || ExpandGamut != 0.0)
		{
			Result.Result = EValidationFixStatus::Fixed;
			IcvfxCameraComponent->CameraSettings.AllNodesColorGrading.ColorGradingSettings.Misc.bOverride_ExpandGamut = false;
			IcvfxCameraComponent->CameraSettings.AllNodesColorGrading.ColorGradingSettings.Misc.ExpandGamut = 0.0;
			ActorMessages += FString::Format(
				TEXT("{0}\nExpand Gamut Is Disabled For Inner Frustum Color Grading & Set To 0.0\n"),
				Args
			);
		}
	}
}

#endif


FValidationResult UValidation_Level_ICVFXConfig_ColorGrading::Validation_Implementation()
{
#if PLATFORM_WINDOWS || PLATFORM_LINUX
	FValidationResult ValidationResult = FValidationResult(EValidationStatus::Pass, "Valid");
	FString Message = "";
	
	const UWorld* World = GetCorrectValidationWorld();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, ADisplayClusterRootActor::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		FString ActorMessages = "";
		const ADisplayClusterRootActor* MyActor = Cast<ADisplayClusterRootActor>(FoundActor);
		
		const UDisplayClusterConfigurationData* ConfigData = MyActor->GetConfigData();
		const FDisplayClusterConfigurationICVFX_StageSettings StageSettings = ConfigData->StageSettings;
		ValidateEntireClusterColorGrading(ValidationResult, ActorMessages, StageSettings);
		ValidatePerViewPortColorGrading(ValidationResult, ActorMessages, StageSettings);
		ValidateInnerFrustumColorGrading(ValidationResult, ActorMessages, MyActor);
		
		if (ActorMessages.Len())
		{
			Message +=  MyActor->GetName();
			Message += "\n";
			Message += ActorMessages;
		}
		
	}

	if (ValidationResult.Result != EValidationStatus::Pass)
	{
		ValidationResult.Message = Message;
	}
	
	return ValidationResult;
#endif

#if PLATFORM_MAC
	FValidationResult ValidationResult = FValidationResult();
	ValidationResult.Result = EValidationStatus::Warning;
	ValidationResult.Message = "Ndisplay Validations Not Valid On OSX";
	return ValidationResult;
#endif
}

FValidationFixResult UValidation_Level_ICVFXConfig_ColorGrading::Fix_Implementation()
{
	#if PLATFORM_WINDOWS || PLATFORM_LINUX
	FValidationFixResult ValidationFixResult = FValidationFixResult(EValidationFixStatus::Fixed, "");
	FString Message = "";
	
	const UWorld* World = GetCorrectValidationWorld();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, ADisplayClusterRootActor::StaticClass(), FoundActors);

	for (AActor* FoundActor : FoundActors)
	{
		FString ActorMessages = "";
		const ADisplayClusterRootActor* MyActor = Cast<ADisplayClusterRootActor>(FoundActor);
		
		UDisplayClusterConfigurationData* ConfigData = MyActor->GetConfigData();
		
		GEngine->BeginTransaction(*ValidationUndoContextName(), FText::FromString(ValidationDescription), ConfigData);
			ConfigData->Modify();

			FDisplayClusterConfigurationICVFX_StageSettings StageSettings = ConfigData->StageSettings;
			FixEntireClusterColorGrading(ActorMessages, StageSettings);
			FixPerViewportColorGrading(ActorMessages, StageSettings);
			FixInnerFrustumColorGrading(ValidationFixResult, ActorMessages, MyActor);
		
			ConfigData->StageSettings = StageSettings;
		GEngine->EndTransaction();
		
		if (ActorMessages.Len())
		{
			Message +=  MyActor->GetName();
			Message += "\n";
			Message += ActorMessages;
		}
		
	}
	ValidationFixResult.Message = Message;
	return ValidationFixResult;
	#endif

	#if PLATFORM_MAC
		FValidationFixResult ValidationFixResult = FValidationFixResult();
		ValidationFixResult.Result = EValidationFixStatus::NotFixed;
		ValidationFixResult.Message = "Ndisplay Validations Not Valid On OSX";
		return ValidationFixResult;
	#endif
}

