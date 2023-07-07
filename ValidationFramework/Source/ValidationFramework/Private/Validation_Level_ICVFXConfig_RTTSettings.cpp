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

#include "Validation_Level_ICVFXConfig_RTTSettings.h"
#include "Kismet/GameplayStatics.h"
#if PLATFORM_WINDOWS
#include "DisplayClusterRootActor.h"
#include "CineCameraActor.h"
#include "Components/DisplayClusterICVFXCameraComponent.h"
#endif

#define ASPECT_RATIO_TOLERANCE 0.09f


UValidation_Level_ICVFX_RTTSettings::UValidation_Level_ICVFX_RTTSettings()
{
	ValidationName = "NDisplay - ICVFX/RTT Settings";
	ValidationDescription = "When rendering an ICVFX camera, nDisplay outputs a texture (RTT) at a defined resolution "
							"This texture is then projected onto the display surface "
							"using the associated CineCameraActor as a reference. Notably the texture is conformed "
							"into the same shape as the CineCamera's filmback/sensor settings. If the RTT and filmback "
							"have the same aspect ratio, a clean scaling occurs. If the RTT and filmback aspect ratios "
							"are misaligned then a distortion in scaling occurs"; 
	FixDescription = "Updates the ICVFX custom resolution to the correct aspect ratio by "
						"adjusting the height.";
	ValidationScope = EValidationScope::Level;
	ValidationApplicableWorkflows = {
		EValidationWorkflow::ICVFX
	};
}

FValidationResult UValidation_Level_ICVFX_RTTSettings::Validation_Implementation()
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
		const ADisplayClusterRootActor* NDCRootActor = Cast<ADisplayClusterRootActor>(FoundActor);
		if (NDCRootActor)
		{
			TArray<UDisplayClusterICVFXCameraComponent*> ICVFXComponentList;
			NDCRootActor->GetComponents<UDisplayClusterICVFXCameraComponent>(ICVFXComponentList);
			for (UDisplayClusterICVFXCameraComponent* IcvfxCameraComponent : ICVFXComponentList)
			{
				if (IcvfxCameraComponent)
				{
					if (ACineCameraActor* IcvfxCineCameraActor = Cast<ACineCameraActor>(IcvfxCameraComponent->GetCameraComponent()->GetAttachParentActor()))
					{
						ValidateICVFXAspectRatio(ValidationResult,ActorMessages, IcvfxCineCameraActor,IcvfxCameraComponent);
					}
				}
			}
		}
		if (ActorMessages.Len())
		{
			Message +=  NDCRootActor->GetName();
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

FValidationFixResult UValidation_Level_ICVFX_RTTSettings::Fix_Implementation()
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
		ADisplayClusterRootActor* NDCRootActor = Cast<ADisplayClusterRootActor>(FoundActor);
		if (NDCRootActor)
		{
			TArray<UDisplayClusterICVFXCameraComponent*> ICVFXComponentList;
			NDCRootActor->GetComponents<UDisplayClusterICVFXCameraComponent>(ICVFXComponentList);
			for (UDisplayClusterICVFXCameraComponent* IcvfxCameraComponent : ICVFXComponentList)
			{
				UCameraComponent* IcvfxCineCamera = IcvfxCameraComponent->GetCameraComponent();
				if (IcvfxCameraComponent)
				{
					if (ACineCameraActor* IcvfxCineCameraActor = Cast<ACineCameraActor>(IcvfxCameraComponent->GetCameraComponent()->GetAttachParentActor()))
					{
						FixICVFXAspectRatio(ValidationFixResult,ActorMessages,IcvfxCameraComponent,NDCRootActor,IcvfxCineCameraActor);
					}
				}
			}
		}
		if (ActorMessages.Len())
		{
			Message +=  NDCRootActor->GetName();
			Message += "\n";
			Message += ActorMessages;
		}
	}

	ValidationFixResult.Message = Message;
	
	return ValidationFixResult;

#endif

#if PLATFORM_MAC	
	FValidationFixResult ValidationResult = FValidationResult();
	ValidationResult.Result = EValidationStatus::Warning;
	ValidationResult.Message = "Ndisplay Validations Not Valid On OSX";
	return ValidationResult;
#endif
}

#if PLATFORM_WINDOWS || PLATFORM_LINUX

void UValidation_Level_ICVFX_RTTSettings::ValidateICVFXAspectRatio(
	FValidationResult& Result,
	FString& ActorMessages,
	const ACineCameraActor* TargetCineCamera,
	const UDisplayClusterICVFXCameraComponent* IcvfxCamera)
{
	
	const FDisplayClusterConfigurationICVFX_CustomSize ICVFXCustomSize_Settings = IcvfxCamera->CameraSettings.RenderSettings.CustomFrameSize;

	float ICVFX_AR;
	if (ICVFXCustomSize_Settings.CustomWidth == 2560 && ICVFXCustomSize_Settings.CustomHeight == 0) //Weird corner case where the default custom height is zero
		ICVFX_AR = 2560 / 1440;
	else
		ICVFX_AR = static_cast<float>(ICVFXCustomSize_Settings.CustomWidth) / static_cast<float>(ICVFXCustomSize_Settings.CustomHeight);

	
	const float CineCamera_AR = TargetCineCamera->GetCineCameraComponent()->Filmback.SensorWidth / TargetCineCamera->GetCineCameraComponent()->Filmback.SensorHeight;

	//Compare the aspect ratios and allow for a small variance
	if (FGenericPlatformMath::Abs(ICVFX_AR - CineCamera_AR) > ASPECT_RATIO_TOLERANCE)
	{
		//Validation Failed
		Result.Result = EValidationStatus::Fail;

		TArray< FStringFormatArg > Args;
		Args.Add( FStringFormatArg( FString::SanitizeFloat(ICVFX_AR) ) );
		Args.Add( FStringFormatArg( FString::SanitizeFloat(CineCamera_AR) ) );
		ActorMessages += FString::Format
				(
					TEXT( "Found mismatched ICVFX/RTT ({0}) and CineCamera ({1}) aspect ratios.\n" ),Args
				);
	}
	else 
	{
		//Validation passed
		Result.Result = EValidationStatus::Pass;
		ActorMessages +=  TEXT( "Found matching RTT and film back aspect ratios");
	}
}

void UValidation_Level_ICVFX_RTTSettings::FixICVFXAspectRatio(
	FValidationFixResult& ValidationFixResult,
	FString& Result,
	UDisplayClusterICVFXCameraComponent* IcvfxCamera,
	ADisplayClusterRootActor* DisplayClusterRoot,
	const ACineCameraActor* TargetCineCamera)
{
	ValidationFixResult.Result = EValidationFixStatus::NotFixed;

	float const ICVFX_AR = static_cast<float>(IcvfxCamera->CameraSettings.RenderSettings.CustomFrameSize.CustomWidth) / static_cast<float>(IcvfxCamera->CameraSettings.RenderSettings.CustomFrameSize.CustomHeight);
	float const CineCamera_AR = TargetCineCamera->GetCineCameraComponent()->Filmback.SensorWidth / TargetCineCamera->GetCineCameraComponent()->Filmback.SensorHeight;

	//Compare the aspect ratios and allow for a small variance
	if (FGenericPlatformMath::Abs(ICVFX_AR - CineCamera_AR) > ASPECT_RATIO_TOLERANCE)
	{
		int const TargetHeight = FMath::FloorToFloat(static_cast<float>(IcvfxCamera->CameraSettings.RenderSettings.CustomFrameSize.CustomWidth) / CineCamera_AR);
	
		//Begin the editor transaction
		GEngine->BeginTransaction(*FString(TEXT(VALIDATION_FRAMEWORK_UNDO_CAT)), FText::FromString(TEXT("Correct ICVFX aspect ratio")), DisplayClusterRoot);
			IcvfxCamera->Modify();
			IcvfxCamera->CameraSettings.RenderSettings.CustomFrameSize.bUseCustomSize = true;
			IcvfxCamera->CameraSettings.RenderSettings.CustomFrameSize.CustomHeight = TargetHeight;
		GEngine->EndTransaction();

		TArray< FStringFormatArg > Args;
		Args.Add( FStringFormatArg( IcvfxCamera->GetName() ) );
		Args.Add( FStringFormatArg( IcvfxCamera->CameraSettings.RenderSettings.CustomFrameSize.CustomWidth ));
		Args.Add( FStringFormatArg( TargetHeight));
		Result = FString::Format(
			TEXT("Updated ICVFX Component {0) and set custom resolution to {1} x {2}"),
			Args
			);
	}

	ValidationFixResult.Result = EValidationFixStatus::Fixed;
}


#endif





