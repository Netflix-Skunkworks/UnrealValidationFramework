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

#ifndef WITH_OCIO
#define WITH_OCIO 0
#endif
#include "CoreMinimal.h"
#include "Misc/FrameRate.h"
#include "OpenColorIOConfiguration.h"


#include "VFProjectSettingsBase.generated.h"

/**
 * A simple class which stores project wide settings which are stored in the UE Project itself.
 * These are place holders for pieces of data which allow validations to be more dynamic, in that the validations can refer to these
 * settings and use these configurable values rather than hard coded values in the validations, or require duplicate validations.
 *
 * As an example, many validations want to know what the working frame rate of a project should be, rather than have
 * each validation store this, the validations can look this up themselves from a single place of truth.
 */
UCLASS(Blueprintable)
class VALIDATIONFRAMEWORK_API UVFProjectSettingsBase final: public UObject
{
	GENERATED_BODY()
public:
	UVFProjectSettingsBase();
	~UVFProjectSettingsBase();

	/**
	* The expected working frame rate of the project, allows validations to refer back to this expected frame
	* rate when validating other objects or settings within the project
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Validation Framework Settings")
	FFrameRate ProjectFrameRate;

	/**
	* The expected OCIO configuration to be used through the project, this allows validations to refer back to this
	* expected config, when validating other objects of settings within the project
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Validation Framework Settings")
	FOpenColorIOColorConversionSettings ProjectOpenIOColorConfig;
};
