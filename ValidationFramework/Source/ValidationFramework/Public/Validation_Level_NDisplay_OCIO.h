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
#include "DisplayClusterConfigurationTypes_ICVFX.h"
#include "OpenColorIOColorSpace.h"
#include "ValidationBase.h"
#include "Components/DisplayClusterICVFXCameraComponent.h"
#include "Validation_Level_NDisplay_OCIO.generated.h"

/**
* Validation which ensures that the OCIO Configs are setup and being used correctly NDisplay, the reference comes from
* the ValidationProjectSettings set in the project
*/
UCLASS()
class VALIDATIONFRAMEWORK_API UValidation_Level_NDisplay_OCIO final : public UValidationBase
{
	GENERATED_BODY()
public:
	UValidation_Level_NDisplay_OCIO();

	/**
	* Compares two OCIO Color Conversion Settings and validates them to make
	* sure they are the same, comparing the source assets and the color spaces are also matching between source and
	* destination
	* @param ValidationResult - The validation status to be updated
	* @param OCIOSettings1 - The OCIO color conversion settings we want to compare with
	* @param OCIOSettings2 - The OCIO color conversion settings we want to compare with
	* @param OCIOObjectName - The name of the object and or component owning the settings
	* @param HardFail - How strict do we want to be, a hard fail causes a Fail other wise a warning
	*/
	void ValidateOCIOColorConversionSettings(FValidationResult& ValidationResult,
	                                         FOpenColorIOColorConversionSettings OCIOSettings1,
	                                         FOpenColorIOColorConversionSettings OCIOSettings2, FString OCIOObjectName,
	                                         bool HardFail = true);
	
	/**
	* Validates the OCIO setup for the outer frustum compared to the project setup
	* @param ValidationResult - The validation status to be updated
	* @param ProjectOCIOSettings - The OCIO Settings defined for the project in the validation framework settings
	* @param OCIOObjectName - The name of the object and or component owning the settings
	* @param StageSettings - The StageSettings we are validating
	*/
	void ValidateAllViewportOCIOSetups(FValidationResult& ValidationResult,
	                                    FOpenColorIOColorConversionSettings ProjectOCIOSettings, FString OCIOObjectName,
	                                    FDisplayClusterConfigurationICVFX_StageSettings StageSettings);
	
	/**
	* Validates the OCIO setup for the outer frustum per viewport overrides
	* @param ValidationResult - The validation status to be updated
	* @param ProjectOCIOSettings - The OCIO Settings defined for the project in the validation framework settings
	* @param OCIOObjectName - The name of the object and or component owning the settings
	* @param StageSettings - The StageSettings we are validating
	*/
	void ValidatePerViewportOCIOOverrideSetups(FValidationResult& ValidationResult,
	                                           FOpenColorIOColorConversionSettings ProjectOCIOSettings,
	                                           FString OCIOObjectName,
	                                           FDisplayClusterConfigurationICVFX_StageSettings StageSettings);

	/**
	* Validates the OCIO setup for the inner frustum for the given ICVFX camera
	* @param ValidationResult - The validation status to be updated
	* @param ProjectOCIOSettings - The OCIO Settings defined for the project in the validation framework settings
	* @param ComponentName - The name of the object and or component owning the settings
	* @param Icvfx_CameraSettings - The ICVFX Camera settings we are validating
	*/
	void ValidateInnerFrustumOCIOSetups(FValidationResult& ValidationResult,
	                                    FOpenColorIOColorConversionSettings ProjectOCIOSettings,
	                                    FString ComponentName,
	                                    FDisplayClusterConfigurationICVFX_CameraSettings Icvfx_CameraSettings);

	/**
	* Validates the OCIO setup for the inner frustum per node setups for the given ICVFX camera
	* @param ValidationResult - The validation status to be updated
	* @param ProjectOCIOSettings - The OCIO Settings defined for the project in the validation framework settings
	* @param ComponentName - The name of the object and or component owning the settings
	* @param Icvfx_CameraSettings - The ICVFX Camera settings we are validating
	*/
	void ValidateInnerFrustumOCIOPerNodeSetups(FValidationResult& ValidationResult,
										FOpenColorIOColorConversionSettings ProjectOCIOSettings,
										FString ComponentName,
										FDisplayClusterConfigurationICVFX_CameraSettings Icvfx_CameraSettings);

	virtual FValidationResult Validation_Implementation() override;
	
	virtual FValidationFixResult Fix_Implementation() override;
};


