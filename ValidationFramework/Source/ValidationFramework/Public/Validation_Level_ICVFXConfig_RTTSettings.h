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

#pragma once
#include "CoreMinimal.h"
#include "ValidationBase.h"
#include "Components/DisplayClusterICVFXCameraComponent.h"

#if PLATFORM_WINDOWS || PLATFORM_LINUX
#include "DisplayClusterConfigurationTypes_ICVFX.h"
#include "DisplayClusterRootActor.h"
#include "CineCameraActor.h"
#endif
#include "Validation_Level_ICVFXConfig_RTTSettings.generated.h"

/**
 *Validation to ensure the the ICVFX rendering settings on the NDisplayRootCluster
 *are correctly aligned with with camera settings of an associated CineCameraActor
 */
UCLASS()
class VALIDATIONFRAMEWORK_API UValidation_Level_ICVFX_RTTSettings final: public UValidationBase
{
	GENERATED_BODY()
	
public:

	UValidation_Level_ICVFX_RTTSettings();

	//Class Overrides
	virtual FValidationResult Validation_Implementation() override;
	virtual FValidationFixResult Fix_Implementation() override;

#if PLATFORM_WINDOWS || PLATFORM_LINUX

	static void ValidateICVFXAspectRatio (
		FValidationResult& Result,
		FString& ActorMessages,
		const ACineCameraActor* TargetCineCamera,
		const UDisplayClusterICVFXCameraComponent* IcvfxCamera);

	static void FixICVFXAspectRatio(
		FValidationFixResult& ValidationFixResult,
		FString& Result,
		UDisplayClusterICVFXCameraComponent* IcvfxCamera,
		ADisplayClusterRootActor* DisplayClusterRoot,
		const ACineCameraActor* TargetCineCamera
	);

#endif
	
	
};
