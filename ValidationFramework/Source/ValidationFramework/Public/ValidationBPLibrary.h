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
#include "ValidationCommon.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ValidationBPLibrary.generated.h"

/**
* A Library of helper functions which are used in either c++ validations directly, or as blueprint nodes
* within validation blueprints to simplify the logic & cleanliness of the node networks
* 
*/
UCLASS()
class UValidationBPLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	/**
	* Gets all of the ValidationObjects which are defined in code or via blueprint
	* @return An array of UObjects representing all of the validations in the project
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static TArray<UObject*> GetAllValidations();

	/**
	* Gets all of the ValidationObjects which are defined via blueprints
	* @return An array of UObjects representing all of the blueprint validations in the project
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static TArray<UObject*> GetAllValidationsFromBlueprints();

	/**
	* Gets all of the validations which are defined in C++ and derive from ValidationBase
	* @return An array of UObjects representing all of the C++ validations in the project
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static TArray<UObject*> GetAllValidationsFromCode();

	/**
	* Get The ValidationFramework project settings for the UE Project
	* @return The UObject set in the project settings derived from UVFProjectSettingsBase
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static UObject* GetValidationFrameworkProjectSettings();

	/**
	* Gets the value of the default lens flare setting within the project
	* @return whether the lens flare default is on or off
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static bool GetLensFlareProjectSetting();

	/**
	* Sets the value of the default lens flare setting within the project
	* @param bValue - the value we want to use to enable or disable the setting
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static void SetLensFlareProjectSetting(bool bValue);

	/**
	* Gets the value of the default bloom setting within the project
	* @return whether the bloom default is on or off
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static bool GetBloomProjectSetting();

	/**
	* Sets the value of the default bloom setting within the project
	* @param bValue - the value we want to use to enable or disable the setting
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static void SetBloomProjectSetting(bool bValue);

	/**
	* Fixes the given post process settings to have their exposure settings correctly set for ICVFX
	* @param ObjectName - The name of the UObject which the settings relate too
	* @param Settings - The postprocessing settings we want to correct the settings within
	* @param FixResult - The validation fix result describing the outcome
	* @param IsCamera - Whether the post processing settings come from a camera or from a volume
	* @param PostProcessVolumeInLevel - Whether there is another post processing volume in the level or not
	* @return Returns the corrected post processing settings
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FPostProcessSettings FixPostProcessExposureSettings(
		FString ObjectName, FPostProcessSettings Settings, FValidationFixResult& FixResult,
		const bool IsCamera, const bool PostProcessVolumeInLevel);

	/**
	* Validates the exposure settings for the given post process settings for ICVFX workflows
	* @param ObjectName - The name of the UObject which the settings relate too
	* @param Settings - The postprocessing settings we want to correct the settings within
	* @param IsCamera - Whether the post processing settings come from a camera or from a volume
	* @param PostProcessVolumeInLevel - Whether there is another post processing volume in the level or not
	* @return Returns ValidationResult for these post process settings
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FValidationResult ValidatePostProcessExposureSettings(
		FString ObjectName, FPostProcessSettings Settings, const bool IsCamera, const bool PostProcessVolumeInLevel);

	/**
	* Validates the bloom settings for the given post process settings for ICVFX workflows
	* @param ObjectName - The name of the UObject which the settings relate too
	* @param Settings - The postprocessing settings we want to correct the settings within
	* @return Returns ValidationResult for these post process settings
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FValidationResult ValidatePostProcessBloomSettings(
		const FString ObjectName, const FPostProcessSettings Settings);

	/**
	* Fixes the bloom settings for the given post process settings for ICVFX workflows
	* @param ObjectName - The name of the UObject which the settings relate too
	* @param Settings - The postprocessing settings we want to correct the settings within
	* @param FixResult - The results of the fix to ensure it was a success etc
	* @return Returns the fixed and updated post process settings
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FPostProcessSettings FixPostProcessBloomSettings(
		FString ObjectName, FPostProcessSettings Settings, FValidationFixResult& FixResult);

	/**
	* Validates the blue correction settings for the given post process settings for ICVFX workflows
	* @param ObjectName - The name of the UObject which the settings relate too
	* @param Settings - The postprocessing settings we want to correct the settings within
	* @return Returns ValidationResult for these post process settings
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FValidationResult ValidatePostProcessBlueCorrectionSettings(
		const FString ObjectName, const FPostProcessSettings Settings);

	/**
	* Fixes the blue correction settings for the given post process settings for ICVFX workflows
	* @param ObjectName - The name of the UObject which the settings relate too
	* @param Settings - The postprocessing settings we want to correct the settings within
	* @param FixResult - The results of the fix to ensure it was a success etc
	* @return Returns the fixed and updated post process settings
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FPostProcessSettings FixPostProcessBlueCorrectionSettings(
		FString ObjectName, FPostProcessSettings Settings, FValidationFixResult& FixResult);

	/**
	* Validates the grain jitter and intensity settings for the given post process settings for ICVFX workflows
	* @param ObjectName - The name of the UObject which the settings relate too
	* @param Settings - The postprocessing settings we want to correct the settings within
	* @return Returns ValidationResult for these post process settings
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FValidationResult ValidatePostProcessGrainSettings(
		const FString ObjectName, const FPostProcessSettings Settings);

	/**
	* Fixes the grain jitter and intensity settings for the given post process settings for ICVFX workflows
	* @param ObjectName - The name of the UObject which the settings relate too
	* @param Settings - The postprocessing settings we want to correct the settings within
	* @param FixResult - The results of the fix to ensure it was a success etc
	* @return Returns the fixed and updated post process settings
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FPostProcessSettings FixPostProcessGrainSettings(
		FString ObjectName, FPostProcessSettings Settings, FValidationFixResult& FixResult);

	/**
	* Validates the lens flare settings for the given post process settings for ICVFX workflows
	* @param ObjectName - The name of the UObject which the settings relate too
	* @param Settings - The postprocessing settings we want to correct the settings within
	* @return Returns ValidationResult for these post process settings
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FValidationResult ValidatePostProcessLensFlareSettings(
		const FString ObjectName, const FPostProcessSettings Settings);
	/**
	* Fixes the lens flare settings for the given post process settings for ICVFX workflows
	* @param ObjectName - The name of the UObject which the settings relate too
	* @param Settings - The postprocessing settings we want to correct the settings within
	* @param FixResult - The results of the fix to ensure it was a success etc
	* @return Returns the fixed and updated post process settings
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FPostProcessSettings FixPostProcessLensFlareSettings(
		FString ObjectName, FPostProcessSettings Settings, FValidationFixResult& FixResult);

	/**
	* Validates the vignette settings for the given post process settings for ICVFX workflows
	* @param ObjectName - The name of the UObject which the settings relate too
	* @param Settings - The postprocessing settings we want to correct the settings within
	* @return Returns ValidationResult for these post process settings
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FValidationResult ValidatePostProcessVignetteSettings(
		const FString ObjectName, const FPostProcessSettings Settings);

	/**
	* Fixes the vignette settings for the given post process settings for ICVFX workflows
	* @param ObjectName - The name of the UObject which the settings relate too
	* @param Settings - The postprocessing settings we want to correct the settings within
	* @param FixResult - The results of the fix to ensure it was a success etc
	* @return Returns the fixed and updated post process settings
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FPostProcessSettings FixPostProcessVignetteSettings(
		FString ObjectName, FPostProcessSettings Settings, FValidationFixResult& FixResult);

	/**
	* Validates the expand gamut settings for the given post process settings for ICVFX workflows
	* @param ObjectName - The name of the UObject which the settings relate too
	* @param Settings - The postprocessing settings we want to correct the settings within
	* @return Returns ValidationResult for these post process settings
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FValidationResult ValidatePostProcessExpandGamutSettings(
		const FString ObjectName, const FPostProcessSettings Settings);

	/**
	* Fixes the expand gamut setting for the given post process settings for ICVFX workflows
	* @param ObjectName - The name of the UObject which the settings relate too
	* @param Settings - The postprocessing settings we want to correct the settings within
	* @param FixResult - The results of the fix to ensure it was a success etc
	* @return Returns the fixed and updated post process settings
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FPostProcessSettings FixPostProcessExpandGamutSettings(
		FString ObjectName, FPostProcessSettings Settings, FValidationFixResult& FixResult);

	/**
	* Validates the tone curve settings for the given post process settings for ICVFX workflows
	* @param ObjectName - The name of the UObject which the settings relate too
	* @param Settings - The postprocessing settings we want to correct the settings within
	* @return Returns ValidationResult for these post process settings
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FValidationResult ValidatePostProcessToneCurveSettings(
		const FString ObjectName, const FPostProcessSettings Settings);

	/**
	* Fixes the tone curve setting for the given post process settings for ICVFX workflows
	* @param ObjectName - The name of the UObject which the settings relate too
	* @param Settings - The postprocessing settings we want to correct the settings within
	* @param FixResult - The results of the fix to ensure it was a success etc
	* @return Returns the fixed and updated post process settings
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FPostProcessSettings FixPostProcessToneCurveSettings(
		FString ObjectName, FPostProcessSettings Settings, FValidationFixResult& FixResult);

	/**
	* Validates the chromatic aberration settings for the given post process settings for ICVFX workflows
	* @param ObjectName - The name of the UObject which the settings relate too
	* @param Settings - The postprocessing settings we want to correct the settings within
	* @return Returns ValidationResult for these post process settings
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FValidationResult ValidatePostProcessChromaticAberrationSettings(
		const FString ObjectName, const FPostProcessSettings Settings);

	/**
	* Fixes the chromatic aberration setting for the given post process settings for ICVFX workflows
	* @param ObjectName - The name of the UObject which the settings relate too
	* @param Settings - The postprocessing settings we want to correct the settings within
	* @param FixResult - The results of the fix to ensure it was a success etc
	* @return Returns the fixed and updated post process settings
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FPostProcessSettings FixPostProcessChromaticAberrationSettings(
		FString ObjectName, FPostProcessSettings Settings, FValidationFixResult& FixResult);

	/**
	* Helper function to mark the current open levels as dirty at the end of validation fixes
	* @return whether the operation was a success or not
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static bool MarkCurrentLevelDirty();

	/**
	* Filters the meshes within the NDisplay setups for known meshes which are not actually led walls
	* @return whether the mesh should be validated or not
	*/
	static bool ExcludeMeshFromNDisplayValidation(const UStaticMesh* Mesh);

	/**
	* Helper function which handles the generic logic for the validation of a mesh associated to an NDisplay setup
	* @param World - The UWorld we are operating within
	* @param InputValidationFunction - The function we want to run
	* @return The ValidationResult for the given mesh and checks
	*/
	static FValidationResult NDisplayMeshSettingsValidation(
		const UWorld* World,
		const TFunction<EValidationStatus(UStaticMesh* StaticMesh, const int LodIndex, FString& Message)> InputValidationFunction
		);

	/**
	* Exports The Validation Report To The Given Folder With A Given Suffix, This exports level and project validations
	* in both csv * json format
	* @param ValidationReportDataTable - The ValidationReport DataTable which we want to export
	* @param ReportPath - The function we want to run
	* @param Suffix - The function we want to run
	* @return Whether this was successful or not
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static bool ExportValidationReport(UValidationReportDataTable* ValidationReportDataTable, FString ReportPath="", const FString Suffix="");

	/**
	* Creates a new ValidationReport DataTable
	* @return An empty ValidationReport DataTable UObject ready to be populated
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static UValidationReportDataTable* CreateValidationDataTable();

	/**
	* Adds the results of a given validation to the validation report data table
	* @param ValidationReportDataTable - The data table we want to add the result too
	* @param Validation - The validation object the results relate too
	* @param ValidationResult - The results of the actual validation
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static void AddValidationResultToReport(
		UValidationReportDataTable* ValidationReportDataTable,
		UValidationBase* Validation, FValidationResult ValidationResult);

	/**
	* For a given level and workflow we generate a validation report in the given report path.
	* This runs all validations for both level & project, for the provided workflow and stores the results as csv & json
	* @param LevelPath - The path to the level we want to validate
	* @param Workflow - The workflow we want to validate for
	* @param ReportPath - The folder we want the validation reports to be written too
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static bool GenerateValidationReport(const FString LevelPath, const EValidationWorkflow Workflow, const FString ReportPath="");

	/**
	* Checks that any Sequences found in the world, have frame rates which match the given frame rate, or are valid
	* multiples
	* @param World - The world in which we want to find sequences within 
	* @param Rate - The frame rate we want to check the sequence against
	* @return Returns ValidationResult
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FValidationResult ValidateSequencesAgainstFrameRate(
		const UWorld* World,
		const FFrameRate Rate);

	/**
	* Checks that any Sequences found in the world, any frame rates which do not match the given frame rate, are set to
	* match unless its a valid multiple, ie 48 against 24. In this case this could be valid so we do not touch but report
	* to the user to check
	* @param World - The world in which we want to find sequences within 
	* @param Rate - The frame rate we want to check the sequence against
	* @return Returns ValidationFixResult
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static FValidationFixResult FixSequencesAgainstFrameRate(
		const UWorld* World,
		const FFrameRate Rate);

	/**
	* Checks To See If The Default RHI Is Set To DirectX
	* @return Returns true or false
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static bool CheckDefaultRHIIsDirectX12();
	
	/**
	* Sets The Default RHI to DirectX12 If Not Set
	* @return Returns true or false
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static bool SetProjectRHIDirectX12();

	/**
	* Causes The Editor To Raise The Restart Editor Warning
	*/
	UFUNCTION(BlueprintCallable, Category="ValidationBPLibrary")
	static void WarnAboutRestart();
	
};
