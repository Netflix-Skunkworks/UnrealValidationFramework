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
#include "Validation_TimecodeProvider.generated.h"

/**
* Validation which checks that a Timecode Provider is set in the unreal project, and that the timecode rate it is
* receiving matches those set in the validation framework project settings, or is of a suitable multiplier  
*/
UCLASS()
class VALIDATIONFRAMEWORK_API UValidation_TimecodeProvider final : public UValidationBase 
{
	GENERATED_BODY()

public:
	UValidation_TimecodeProvider();

	virtual FValidationResult Validation_Implementation() override;
	
	virtual FValidationFixResult Fix_Implementation() override;
};
