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
#include "ValidationCommon.h"
#include "EditorUtilityTask.h"
#include "ValidationBase.generated.h"


/**
* The underlying base class from which all of the validations inherit, both from c++ implementations, and for blueprints
* implemented by artists/technical directors
*/
UCLASS(Blueprintable)
class VALIDATIONFRAMEWORK_API UValidationBase : public UEditorUtilityTask
{
	GENERATED_BODY()

public:
	UValidationBase();
	~UValidationBase();

	/**
	* The display name of the validation for UI and report generation
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ValidationBPLibrary")
	FString ValidationName;

	/**
	* A description of what the validation checks for and why its important
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="ValidationBPLibrary")
	FString ValidationDescription;

	/**
	* A description of what the validation fix will do
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="ValidationBPLibrary")
	FString FixDescription;

	/**
	* An array of the workflows which the validation is associated with
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="ValidationBPLibrary")
	TArray<EValidationWorkflow> ValidationApplicableWorkflows;

	/**
	* The scope the validation is related to, be it for project or level 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="ValidationBPLibrary")
	EValidationScope ValidationScope;

	/**
	* The blueprint event which should be implemented by the artist/td within blueprints, that deals with the checks
	* to define whether something is valid or not for the defined scope and workflow
	* @return A validation result containing the result status and any info messages from the validation
	*/
	UFUNCTION(BlueprintNativeEvent)
	FValidationResult Validation();

	/**
	* The function which should be overriden and implemented by inheriting classes in c++, that deal with the checks to
	* define whether something is valid or not for the defined scope and workflows
	* @return A validation result containing the result status and any info messages from the validation
	*/
	virtual FValidationResult Validation_Implementation();

	/**
	* Function which runs the validation regardless of whether the validation is implemented in code or
	* blueprints
	* @return A validation result containing the result status and any info messages from the validation
	*/
	UFUNCTION(BlueprintCallable,  Category="ValidationBPLibrary")
	FValidationResult RunValidation();

	/**
	* The blueprint event which should be implemented by the artist/td within blueprints, that deals with applying
	* fixes to ensure something is valid for this given scope and workflow
	* @return A validation fix result containing the fix status and any info messages from the validation fix
	*/
	UFUNCTION(BlueprintNativeEvent)
	FValidationFixResult Fix();

	/**
	* The function which should be overriden and implemented by inheriting classes in c++, that applies the fixes
	* fixes to ensure something is valid for this given scope and workflow
	* @return A validation fix result containing the fix status and any info messages from the validation fix
	*/
	virtual FValidationFixResult Fix_Implementation();

	/**
	* Function which runs the fix regardless of whether the validation is implemented in code or
	* blueprints
	* @return A validation fix result containing the fix status and any info messages from the validation fix
	*/
	UFUNCTION(BlueprintCallable,  Category="ValidationBPLibrary")
	FValidationFixResult RunFix();

	/**
	* Ensures that we always get the correct world as often we are running within UI widgets and this is not easily
	* accessible
	* @return A UWorld object representing the editor world we want to be working in
	*/
	UFUNCTION(BlueprintCallable,  Category="ValidationBPLibrary")
	static UWorld* GetCorrectValidationWorld();

	/**
	* Gets the name of the undo context for the given validation
	* @return An FString of the name to use for the undo or transaction contexts
	*/
	UFUNCTION(BlueprintCallable, Category = "ValidationBPLibrary")
	FString ValidationUndoContextName();
	
};
