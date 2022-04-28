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

#if PLATFORM_WINDOWS || PLATFORM_LINUX
#include "DisplayClusterConfigurationTypes_ICVFX.h"
#include "DisplayClusterRootActor.h"
#endif
#include "Validation_Level_ICVFXConfig_ColorGrading.generated.h"

/**
* Validation to ensure that any of the color grading settings as part of the NDisplayRoot Cluster are not affecting the
* imaging pipeline, some of the settings similar to the post processing settings if enabled be it as part of the inner frustum.
* per viewport or full cluster will cause either artifacts or perform invalid color transformations.
*/
UCLASS()
class VALIDATIONFRAMEWORK_API UValidation_Level_ICVFXConfig_ColorGrading final: public UValidationBase
{
	GENERATED_BODY()

public:
	UValidation_Level_ICVFXConfig_ColorGrading();

	virtual FValidationResult Validation_Implementation() override;
	
	virtual FValidationFixResult Fix_Implementation() override;
	
#if PLATFORM_WINDOWS || PLATFORM_LINUX
	/**
	* Validates the color grading settings for the whole cluster, this includes the exposure settings, blue correction
	* and expand gamut.
	* @param Result - The validation result object we want to collect the results in
	* @param ActorMessages - The overall collection of all the messages returned from the validations.
	* @param StageSettings - The stage settings from the NDisplay actor and ICVFX camera
	*/
	static void ValidateEntireClusterColorGrading(
		FValidationResult& Result, FString& ActorMessages,
		FDisplayClusterConfigurationICVFX_StageSettings StageSettings);

	/**
	* Validates the color grading settings which are set per viewport, this includes the exposure settings, blue correction
	* and expand gamut.
	* @param Result - The validation result object we want to collect the results in
	* @param ActorMessages - The overall collection of all the messages returned from the validations.
	* @param StageSettings - The stage settings from the NDisplay actor and ICVFX camera
	*/
	void ValidatePerViewPortColorGrading(
		FValidationResult& Result,
		FString& ActorMessages,
		FDisplayClusterConfigurationICVFX_StageSettings StageSettings) const;

	/**
	* Validates the color grading settings for the inner frustum, this includes the blue correction
	* and expand gamut.
	* @param Result - The validation result object we want to collect the results in
	* @param ActorMessages - The overall collection of all the messages returned from the validations.
	* @param Actor - The Actor representing the NDisplay setup that we need to validate
	*/	
	void ValidateInnerFrustumColorGrading(
		FValidationResult& Result,
		FString& ActorMessages,
		const ADisplayClusterRootActor* Actor) const;

	/**
	* Fixes the color grading settings for the whole cluster, this includes the blue correction
	* and expand gamut.
	* @param ActorMessages - The overall collection of all the messages returned from the validations.
	* @param StageSettings - The stage settings from the NDisplay actor and ICVFX camera
	*/
	static void FixEntireClusterColorGrading(
		FString& ActorMessages,
		FDisplayClusterConfigurationICVFX_StageSettings& StageSettings);

	/**
	* Fixes the color grading settings that are set per viewport, this includes the blue correction
	* and expand gamut.
	* @param ActorMessages - The overall collection of all the messages returned from the validations.
	* @param StageSettings - The stage settings from the NDisplay actor and ICVFX camera
	*/
	void FixPerViewportColorGrading(
		FString& ActorMessages,
		FDisplayClusterConfigurationICVFX_StageSettings& StageSettings) const;

	/**
	* Fixes the color grading settings that are set per viewport, this includes the blue correction
	* and expand gamut.
	* @param Result - The result of the fixes applied
	* @param ActorMessages - The overall collection of all the messages returned from the validations.
	* @param Actor - The Actor representing the NDisplay setup that we need to validate
	*/
	void FixInnerFrustumColorGrading(
		FValidationFixResult& Result, FString& ActorMessages,
		const ADisplayClusterRootActor* Actor) const;

#endif
};


