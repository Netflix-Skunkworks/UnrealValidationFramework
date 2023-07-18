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


#include "ValidationBase.h"

#include "Editor.h"


UValidationBase::UValidationBase()
{
}

UValidationBase::~UValidationBase()
{
}

FValidationResult UValidationBase::RunValidation()
{
	return Validation();
}

FValidationFixResult UValidationBase::RunFix()
{
	return Fix();
}

FValidationResult UValidationBase::Validation_Implementation()
{
	return FValidationResult(EValidationStatus::Pass, "Base Implementation");
}

FValidationFixResult UValidationBase::Fix_Implementation()
{
	return FValidationFixResult(EValidationFixStatus::Fixed, "Base Implementation fix");

}

UWorld* UValidationBase::GetCorrectValidationWorld()
{
	return GEditor ? GEditor->GetEditorWorldContext(false).World() : nullptr;
}

FString UValidationBase::ValidationUndoContextName()
{
	return FString(TEXT(VALIDATION_FRAMEWORK_UNDO_CAT)) + "_" + ValidationName;
}
