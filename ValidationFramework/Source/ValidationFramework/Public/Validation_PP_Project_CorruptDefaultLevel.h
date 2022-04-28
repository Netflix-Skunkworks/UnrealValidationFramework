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
#include "Validation_PP_Project_CorruptDefaultLevel.generated.h"

/**
* Validation which checks that the default levels defined in the unreal project actually exist within the project, and
* are not soft reference, corrupt paths to none existent assets within the project. This causes a hard crash with
* nDisplay
*/
UCLASS()
class VALIDATIONFRAMEWORK_API UValidation_PP_Project_CorruptDefaultLevel final : public UValidationBase
{
	GENERATED_BODY()
public:
	UValidation_PP_Project_CorruptDefaultLevel();
	virtual FValidationResult Validation_Implementation() override;
	
	virtual FValidationFixResult Fix_Implementation() override;
};
