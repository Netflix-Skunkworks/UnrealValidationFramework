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
#include "VFProjectSettingsBase.h"
#include "VFProjectSettingsEditor.generated.h"

/**
 * Class which allows us to add and store ValidationFrameworkSettings as part of the UE Project
 */
UCLASS(config = ValidationFrameworkSettings)
class VALIDATIONFRAMEWORK_API UVFProjectSettingsEditor final: public UObject
{
	GENERATED_BODY()
public:
	UVFProjectSettingsEditor(const FObjectInitializer& obj);

	/**
	* Property to store a ValidationFrameworkProjectSettings asset within the UE Project settings
	*/
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Validation Framework Settings")
	TSubclassOf<UVFProjectSettingsBase> ValidationFrameworkSettings;
};
