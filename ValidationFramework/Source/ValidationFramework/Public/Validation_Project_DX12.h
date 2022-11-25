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
#include "Validation_Project_DX12.generated.h"

/**
* Validation which checks that unreal is using DX12 for ICVFX workflows, whilst not critical for all workflows
* It is a hard dependency for the EXR plate playback
*/
UCLASS()
class VALIDATIONFRAMEWORK_API UValidation_Project_DX12 final : public UValidationBase
{
	GENERATED_BODY()

public:
	UValidation_Project_DX12();
	virtual FValidationResult Validation_Implementation() override;
	virtual FValidationFixResult Fix_Implementation() override;
	
};


