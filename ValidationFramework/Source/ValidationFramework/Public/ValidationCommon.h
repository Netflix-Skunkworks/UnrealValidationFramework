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
#include "ValidationCommon.generated.h"

#define VALIDATION_FRAMEWORK_UNDO_CAT "Validation_Framework_Undo"

/**
* A range of general usage structs, enums and classes used as foundational building blocks within the wider framework
*/

/**
* An enum to represent the different outcomes of a validation
*/
UENUM(BlueprintType)
enum class EValidationStatus : uint8
{
	Pass = 2		UMETA(DisplayName = "Pass"),
	Warning = 1		UMETA(DisplayName = "Warning"),
	Fail = 0		UMETA(DisplayName = "Fail"),
};

/**
* An enum to represent the different outcomes of a validation fix
*/
UENUM(BlueprintType)
enum class EValidationFixStatus : uint8
{
	Fixed = 2		UMETA(DisplayName = "Fixed"),
	ManualFix = 1		UMETA(DisplayName = "FixManually"),
	NotFixed = 0		UMETA(DisplayName = "NotFixed"),
};

/**
* An enum to represent the different outcomes of a frame rate comparison
*/
UENUM(BlueprintType)
enum class EFrameRateComparisonStatus : uint8
{
	Valid = 2		UMETA(DisplayName = "Valid"),
	ValidMultiple = 1		UMETA(DisplayName = "ValidMultiple"),
	InValid = 0		UMETA(DisplayName = "InValid"),
};

/**
* A struct to hold the results of a validation
*/
USTRUCT(BlueprintType)
struct FValidationResult
{
	GENERATED_BODY()

	/**
	* The status of the result represented as an enum
	*/
	UPROPERTY(BlueprintReadWrite,  Category="ValidationBPLibrary")
	EValidationStatus Result;

	/**
	* A message conveying any useful information about the validation which was run
	*/
	UPROPERTY(BlueprintReadWrite,  Category="ValidationBPLibrary")
	FString Message;

	FValidationResult()
	{
		Result = EValidationStatus::Pass;
		Message = "";
	}

	FValidationResult(const EValidationStatus ResultIn, FString MessageIn)
	{
		Result = ResultIn;
		Message = MessageIn;
	}
};

/**
* A struct to hold the results of a validation fix
*/
USTRUCT(BlueprintType)
struct FValidationFixResult
{
	GENERATED_BODY()

	/**
	* The status of the fix outcome represented as an enum
	*/
	UPROPERTY(BlueprintReadWrite,  Category="ValidationBPLibrary")
	EValidationFixStatus Result;

	/**
	* A message conveying any useful information about the fix which was run
	*/
	UPROPERTY(BlueprintReadWrite,  Category="ValidationBPLibrary")
	FString Message;

	FValidationFixResult()
	{
		Result = EValidationFixStatus::Fixed;
		Message = "";
	}

	FValidationFixResult(const EValidationFixStatus ResultIn, FString MessageIn)
	{
		Result = ResultIn;
		Message = MessageIn;
	}
};

/**
* An enum representing all of the available workflows which validations can belong too.
* Simply extending this enum exposes new options in the workflow selection from the UI, as well as
* allowing validations to register to this new workflow option
*/
UENUM(BlueprintType)
enum class EValidationWorkflow : uint8
{
	ICVFX			UMETA(DisplayName = "ICVFX"),
	VRScouting		UMETA(DisplayName = "VR Scouting"),
	SimulCam		UMETA(DisplayName = "SimulCam"),
	VAD				UMETA(DisplayName = "VAD")
};

/**
* An enum representing whether the validations are related to the Project or to a Level, this provides better granularity
* when filtering all of the validations in UIs or via code
*/
UENUM(BlueprintType)
enum class EValidationScope : uint8
{
	Project		UMETA(DisplayName = "ProjectValidation"),
	Level		UMETA(DisplayName = "LevelValidation")
};

/**
* A struct to represent the rows within a data table, used to generate a validation report
*/
USTRUCT()
struct FValidationReportRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	/**
	* The name of the validation for this row
	*/
	UPROPERTY()
	FString Name;

	/**
	* The description of the validation for this row
	*/
	UPROPERTY()
	FString Description;

	/**
	* The string representation of the validation result 
	*/
	UPROPERTY()
	FString Result;

	/**
	* Any messages which came out of the validation execution
	*/
	UPROPERTY()
	FString Message;

	/**
	* The description of the fix which can be applied
	*/
	UPROPERTY()
	FString Fix;
};

/**
* Class to represent a DataTable specifically to generate validation reports
*/
UCLASS()
class UValidationReportDataTable final: public UDataTable
{
	GENERATED_BODY()

};
