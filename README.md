# Unreal Engine - VP Workflow Validation Framework
##### Supported Versions
UE4.27+: Supported: :white_check_mark:\
UE5.0+: Testing: :heavy_exclamation_mark:

## 1. Intro
<img src="/docs/images/UI.png" alt="Validation UI">
The Validation Framework is designed as an extendible and customizable framework to host and manage automated validation checks and fixes which can identify and help address problems within specific workflows.

Initially focused on virtual production workflows, and primarily ICVFX, the goal is to help production teams to catch and resolve common issues which can cause unexpected render results, data outputs and represent performance challenges when working with Unreal Engine.

Given that some issues are not obvious and some might not even be visible to the human eye, and with the pressure and complexity of operating on live production and virtual production stages, it is very easy for human error to occur and oversee some of these important settings.

Having a simple toolkit which can be used to run pre-flight checks, acts as a safety net to the operators and the production.

With this in mind, the system can easily be extended to encompass custom workflows, adding custom validations and fixes, allowing teams to check their own custom steps.

The Validation Framework is not solely for use on a stage, it can also be utilized during the prep and content build phases, ensuring issues are handled before moving down the production pipeline ahead of time.

The tool is shipped with a set of the most common and required checks and fixes for a ICVFX production which cause challenges on a regular basis. This set comes as a result of a collaboration between the Virtual Production teams at Netflix and Epic Games and the collected knowledge and experience in ICVFX productions.

The system can also be integrated into pipelines and additional tooling around CI/CD to generate validation reports.



## Contents
1. [Intro](#1-intro)
2. [Installation](#2-installation)\
2.1 [Binaries](#21-binaries)\
2.2 [Source](#22-source)
3. [Quick Start](#3-quick-start)
4. [UI](#4-ui)\
4.1 [Menu Bar](#41-menu-bar)\
4.2 [Validation Framework UI](#42-validation-framework-ui)\
4.3 [Workflow](#43-workflow)\
4.4 [Level Validation](#44-level-validation)\
4.5 [Project Validation](#45-project-validation)\
4.6 [Refresh](#46-refresh)\
4.7 [Run All Validations](#47-run-all-validations)\
4.8 [Run All Fixes](#48-run-all-fixes)\
4.9 [Name](#49-name)\
4.10 [Description](#410-description)\
4.11 [Run](#411-run)\
4.12 [Validation Status](#412-validation-status)\
4.13 [Validation Message](#413-validation-message)\
4.14 [Fix](#414-fix)\
4.15 [Fix Description](#415-fix-description)
5. [Validation](#5-validations)\
5.1 [Workflows](#51-workflows)\
5.2 [Scopes](#52-scopes)\
5.3 [Validation](#53-validation)\
5.4 [Validation Status](#54-validationstatus)\
5.5 [Fixes](#55-fix)\
5.6 [Validation Fix Status](#56-validationfixstatus)
6. [Validation Project Settings](#6-validation-project-settings)\
6.1 [Setting Up Project Settings](#61-setting-up-project-settings)
7. [CI/CD](#7-cicd)\
7.1 [Reports](#71-reports)
8. [Extending & Customizing](#8-extending--customizing)\
8.1 [Adding New Validation](#81-adding-new-validations)\
8.2 [Adding New Workflows](#82-adding-new-workflows)

## 2. Installation
The Validation Framework comes as a self contained plugin with no external dependencies aside from the Unreal Engine itself.

The plugin consists of a C++ set of validations, utilities, base classes and helpers.

It also contains Content in the form of blueprints and other .uassets.

The plugin can simply be added to the Unreal Project>Plugins folder as either a pre compiled binary or can be compiled from source code.

### 2.1 Binaries
1. Download the relevant zip file for the given releases, unreal version and platform.
[Releases](https://github.com/Netflix-Skunkworks/UnrealValidationFramework/releases)

2. Unzip the plugin & place the folder inside the Plugins Folder within the Unreal Project
<img src="/docs/images/plugin_folder.png" alt="Plugin Folder">

3. Open the unreal project in Unreal and from the plugins menu ensure that the plugin is loaded.
<img src="/docs/images/plugin_load.png" alt="Plugin Load">

### 2.2 Source
1. Clone the repository and either copy or symlink the *ValidationFramework* Folder into the UnrealProject>Plugins folder.

2. Right Click The .uproject and select **Generate Visual Studio project files**

3. Once the solution is generated, open the solution and compile the project.

4. Once compiled, open the unreal project in unreal and from the plugins menu ensure that the plugin is loaded.

## 3. Quick Start
**TODO INSERT LINK TO QUICKSTART VIDEO/S**
## 4. UI
The UI is built entirely via Blueprints as an EditorUtilityWidget.

*ValidationFramework>Content>Blueprints>VP_ValidationFramework.uasset*

The blueprint contains no specific logic in regards to the validations, all of the logic is available via API, and any logic in the Blueprint is related to updating the UI itself.

### 4.1 Menu Bar
The main UI can be loaded from the menu item within the Netflix menu.

<img src="/docs/images/menubar.png" alt="Validation Menu Bar">

### 4.2 Validation Framework UI
The main UI for manual user interaction with the validation framework.

<img src="/docs/images/UI.png" alt="Validation UI">

### 4.3 Workflow
Drop down box which allows the user to select a specific workflow we want to run validations for.

This filters the validations visible and runnable from the UI.

### 4.4 Level Validation
Toggle to select all of the Level-based validations for the selected workflow.

This filters the validations visible and runnable from the UI

### 4.5 Project Validation
Toggle to select all of the Project-based validations for the selected workflow

This filters the validations visible and runnable from the UI

### 4.6 Refresh
Refresh button acts as an overall reset/refresh of the UI.
This clears and resets all the existing states of the validations.
As some validations are implemented via Blueprints, these may be registered after the UI and the engine has loaded: We can’t know about Blueprints until they compile, unlike c++ which we know is compiled ahead of time.
Refresh ensures any new or updated validations are all populated into the UI.

### 4.7 Run All Validations
Runs all of the validations for the selected Workflow and Scope, in the current Level and/or Project.

### 4.8 Run All Fixes
Runs all of the validation fixes for the selected Workflow and Scope, in the current Level and/or Project

### 4.9 Name
The short hand name of the validation

### 4.10 Description
A detailed description of what the validation is going to check for and why.

### 4.11 Run
Run buttons exist for each validation, this runs the validation check for this specific validation.

### 4.12 Validation Status
A simple graphical display of the validations result, this shows whether a validation was a Success, a Fail or has Warnings that the user should inspect further.

### 4.13 Validation Message
Description of the outcome from the validation which was run.
Descriptions often identify offending actors, objects and their incompatible settings so a user could choose to resolve manually if required.

### 4.14 Fix
Fix buttons exist for each validation, this runs the validation fix, for the specific validation.

Not all issues can be automatically fixed, however the user will be notified of this from the validation itself.

### 4.15 Fix Description
Description of what applying the fix will do to the Level or Project.

## 5. Validations
Validations can be implemented via Blueprints or by C++, they all inherit from a ValidationBase class, which provides a few key features which we outline below. 

### 5.1 Workflows
Validations are tagged/associated with specific workflows. This allows us to filter validations and only execute the ones which are appropriate.

Validations can be associated with **multiple** workflows.

Currently there are several in-built workflows
 * ICVFX
 * SIMULCAM
 * VAD
 * VR SCOUTING

 Custom workflows can be added with ease see [Adding New Workflows](#82-adding-new-workflows)

### 5.2 Scopes
Validations can also be tagged/associated with a scope, this is another level of filtering to ensure we are running the correct set of validations.

Validations can only be part of a **single** scope.

There are 2 scopes [Level](#521-level) & [Project](#522-project)
#### 5.2.1 Level
Level Scope indicates that the validation will run on the currently open level, (including sublevels).

Any fixes applied will be applied to the level.

**NOTE:** Changes are not auto saved, a user will still need to save any changes made to the levels for them to persist.

#### 5.2.2 Project
Project Scope indicates that the validation will run on the current unreal project.

These are cross level validations which often relate to the Project Settings themselves.

### 5.3 Validation
Validations must all implement a check, blueprint validations override this function whilst c++ validations implement it directly.

This is the function/logic which is run when the user requests a validation to be run.

### 5.4 ValidationStatus
Validations all return a ValidationStatus, this comprises of user information regarding any issues which have been detected, as well as a status indicating the level of severity.

There are 3 status's - [Success](#541-success), [Warning](#542-warning) & [Fail](#543-fail) 

#### 5.4.1 Success
Validations which pass indicate that no issues where detected and report a success\
<img src="/docs/images/success.png" alt="Fail Icon" width="50" height="50">

#### 5.4.2 Warning
Validations sometimes return warnings. There are some situations which may or may not be valid, however it is not possible to tell without knowledge of the production. These require the user to investigate and decide if this is indeed an issue or not for their production and workflow.\
<img src="/docs/images/warning-fill.png" alt="Fail Icon" width="50" height="50">

#### 5.4.3 Fail
Validations return a failure when something has been detected which is known to critically break the workflow. It is important to note that some of these issues may not be visible to the naked eye, and only appear much later in the production workflow.

Users are encourage to either fix manually based on the description, or apply the automated fixes when applicable.\
<img src="/docs/images/failure-fill.png" alt="Fail Icon" width="50" height="50">

### 5.5 Fix
Validations must all implement a fix, blueprint validations override this function whilst c++ validations implement it directly.

This is the function/logic which is run when the user requests a validation fix the detected issues.

This logic should apply any changes to the UObjects or UEProject as appropriate.

Sometimes its not possible to fix things automatically, in which case
the fix still needs implementing but this is as simple as returning a [ValidationFixStatus](#56-validationfixstatus) with status [ManualFix](#562-manualfix).

### 5.6 ValidationFixStatus
Fix Status's are returned from the execution of any fix logic. These carry both informative messages which can be relayed back to the user, along with an overall status related to the success.

Status can be [Fixed](#561-fixed), [ManualFix](#562-manualfix) or [NotFixed](#563-notfixed)

#### 5.6.1 Fixed
The fix or fixes were applied successfully.

#### 5.6.2 ManualFix
There is no automated fix available or this requires more context relating to the production and requires a user to manually fix.

#### 5.6.3 NotFixed
The fix was unable to be applied or failed and the issue still persists

## 6. Validation Project Settings
In order to keep the validations themselves flexible and reusable they sometimes need information about the overall workflow.

For instance validations may want to know the frame rate for the production. 

Rather than have individual validations for each possible frame rate, we store this value as a project setting from which the validations can then refer too.

Should a different project have different requirements the validations then do not need to change project to project.

### 6.1 Setting Up Project Settings

1. Create a new blueprint class from the parent class **VFProjectSettingsBase**. Given these are project related settings, its recommended to keep this within the UnrealProject rather than a Content Plugin. (Unless you use the same settings over and over again)
<img src="/docs/images/project_settings_example/1_vfprojectsettingsbase.png" alt="Create Project Settings">

2. Within the blueprint we set the default inherited values for the project frame rate and the OCIO Config we expect to be using.
<img src="/docs/images/project_settings_example/2_vfprojectsettings_bp_defaults.png" alt="Project Settings Defaults">

3. If you are extending the framework with custom validations, as this is all based on blueprints its possible to add custom values to the VFProjectSettings blueprint.
<img src="/docs/images/project_settings_example/2_vfprojectsettings_custom_values.png" alt="Create Project Settings">

4. Once you are happy with the VFProjectSettings blueprint, compile and save, then these need to be set into the Unreal Project Settings
<img src="/docs/images/project_settings_example/3_set_projectsettings.png" alt="Create Project Settings">

5. With this set, within the validations we can now use the **GetValidationFrameworkProjectSettings** node to retrieve them. You can then either cast to the base type to access just the inherited values such as frame rate, or if you are building custom workflows you can cast to your custom settings class and retrieve your custom settings.
<img src="/docs/images/project_settings_example/4_use_project_settings.png" alt="Create Project Settings">


## 7. CI/CD
Whilst running validations via UI is advantageous for users during prep and live operation in productions.

It can be also useful to execute these validations as part of a larger pipeline as an automated step. This could be as part of an asset publish, or a CI/CD build pipeline etc.

Whilst this framework does not provide the entire pipeline and framework for establishing the CI/CD and asset pipelines themselves.

It does provide the API building blocks for teams to integrate into their systems.

``` c++
static bool GenerateValidationReport(const FString LevelPath, const EValidationWorkflow Workflow, const FString ReportPath="");
```
Available as both blueprint node and c++ function, this api call allows the user or pipeline to specify a level, a workflow, and an output path in which to generate validation reports which can then be consumed by applications outside of unreal engine. 

Validations of both [Project](#522-project) & [Level](#521-level) Scope are executed.

### 7.1 Reports
Reports are generated both via the API call above, but also via the UI when [Run All Validations](#47-run-all-validations) is executed.

This serializes the results which are often displayed in the UI into reports which are stored along side the project folder. (default, unless report path is specified via the API)

UnrealProject>ValidationReports>NameOfLevel

These are stored as both csv or json format so are easily consumed by other applications, or simply sent back as part of support requests to supervisors or technical help groups.
<img src="/docs/images/reports.png" alt="Reports Folder">

## 8. Extending & Customizing
The initial version of the validation framework aimed at providing a framework for virtual production workflows, along with a base set of validations mainly targeting ICVFX to ensure the integrity of the color pipeline and maintaining the image chain as a linear workflow.

There will be many more validations which are needed in future as new issues and or standards are discovered/implemented.

Many new workflows both within, and outside of virtual production field will emerge and teams, products and productions may also have their own set of standards for very bespoke workflows and applications.

With this in mind we ensured that the framework can be extended with both new workflows and new validations by both technical artists and developers alike.

### 8.1 Adding New Validations
Validations can be added either via C++ or via Blueprints.

Whilst the validations can live anywhere within an unreal project it is recommended to keep them within a custom content plugin.

This allows companies, teams and productions to store and transport their own validations between projects without tying themselves to a single Unreal Project or populating the validation framework with none generic workflows

#### 8.1.1 Blueprints

Not everyone wants to or is able write c++, so we provide the ability to implement your own validations and fixes via blueprints.

1. Create a new blueprint class from the right click menu in the content browser,
as mentioned earlier these can live anywhere in the project, however it is recommended to keep them in a dedicated content plugin so they can be easily shared between projects
<img src="/docs/images/blueprint_example/1_create_blueprint.png" alt="Create Blueprint">

2. Select the **ValidationBase** from the All Classes section to act as the parent class
<img src="/docs/images/blueprint_example/2_select_baseclass.png" alt="Base Class Select">

3. Within the blueprint editor, select the class defaults and on the right we set the default values for the validation such as its name, a detailed description of the check and what it does, a detailed description of the fix and what it will do.
Also set the workflows that this validation is applicable to, and the scope of the validation.
<img src="/docs/images/blueprint_example/3_setconstructordefaults.png" alt="Set Defaults">

4. There are then two functions which need to be overridden one called **Validation** and one called **Fix**. We need to implement both.
<img src="/docs/images/blueprint_example/4_implementfunctions.png" alt="Implement Overrides">

5. Within the **Validation** function we can implement whatever logic we need to determine whether something is valid or not. We need to return a *ValidationResult* from the function, with the status and any messages that should be presented to the user in either the UI or the reports.
<img src="/docs/images/blueprint_example/5_implement_validation.png" alt="Implement Validation">

6. Similarly within the **Fix** function we can implement whatever logic we need to apply any fixes that can be done. We need to return a *ValidationFixResult* from the function which holds the status, and any messages we want to pass back to the user
<img src="/docs/images/blueprint_example/6_implement_fix.png" alt="Implement Fix">

7. With the blueprint finished, save and compile the blueprint before returning to the ValidationFramework UI. Select the workflow and scope your new validation is applicable to and hit refresh. You will now see the new validation appear in the UI with the details supplied to the class defaults.
<img src="/docs/images/blueprint_example/7_refreshUI.png" alt="Refresh UI">

8. We can now hit run on this validation, which currently does not do anything but we can see is functioning as expected as the status in this case returns a Success and our message is displayed in the UI.
<img src="/docs/images/blueprint_example/8_runvalidation.png" alt="Run Validation">

#### 8.1.2 C++

Sometimes the logic to run the validation or fix is to complex to be described in a blueprint, more over as the complexity of the validation and fix increases the messier or more complex the blueprint becomes.

Often its just easier to implement a new validation in C++.

1. Create a new class called *CustomValidation* which inherits from *UValidationBase*

``` c++
#pragma once

#include "CoreMinimal.h"
#include "ValidationBase.h"
#include "CustomValidation.generated.h"

/**
* Example Validation We Want To Implement
*/
UCLASS()
class VALIDATIONFRAMEWORK_API UCustomValidation final : public UValidationBase
{
	GENERATED_BODY()

public:
	UCustomValidation();
	virtual FValidationResult Validation_Implementation() override;
	
	virtual FValidationFixResult Fix_Implementation() override;
};
```

2. In the .cpp within the constructor we set the information we want to provide to the user such as name, description, fix description.

We also set which workflows the validation is going to be valid for and what scope it should be used for.

``` c++
#include "CustomValidation.h"


UCustomValidation::UCustomValidation()
{
	ValidationName = "Custom Validation Name";
	ValidationDescription = "Useful Description Of What Im Checking For And Why";
	FixDescription = "Useful Description Of What Im Going To Do To Fix It";
	ValidationScope = EValidationScope::Project;
	ValidationApplicableWorkflows = {
		EValidationWorkflow::ICVFX
	};
}
```

3. We then need to implement the validation, here we implement our logic to detect the problem we are trying to check for. We return a *FValidationResult* which contains any information we want along with the overall status. 
```c++
FValidationResult UCustomValidation::Validation_Implementation()
{
    FValidationResult ValidationResult = FValidationResult(EValidationStatus::Pass, "");

    // Insert logic to check for problems
    //
    FValidationResult.Result = EValidationStatus::Fail;
    FValidationResult.Message = "I Deliberately Failed This Validation";
	return ValidationResult;
}
```
4. Finally implement any logic which can be done to fix the issue. Care and consideration should be taken to consider if a fix can be done safely and cover all eventualities.
```c++
FValidationFixResult UCustomValidation::Fix_Implementation()
{
    FValidationFixResult ValidationFixResult = FValidationFixResult();

    // Insert logic to fix  problems
    //
    ValidationFixResult.Result = EValidationFixStatus::ManualFix;
    ValidationFixResult.Message = "Needs manual fix we don't even know what we are checking for";
    return ValidationFixResult;
}
```

### 8.2 Adding New Workflows
There are many workflows in the world, especially within an engine which is as open and flexible as Unreal.

With this in mind, users or teams may wish to extend the framework with their own workflows for their own uses.

This is as simple as extending a single enum and recompiling the plugin.

*ValidationFramework/Source/ValidationFramework/Public/ValidationCommon.h*

```c++
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
	VAD			UMETA(DisplayName = "VAD")
};
```

Adding an additional enum value such as **MyWorkflow** is as simple as

```c++
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
	VAD			UMETA(DisplayName = "VAD")
	MYWORKFLOW		UMETA(DisplayName = "MyWorkflow")
};
```

After recompile this option will display in the workflow selection dialog, and will be available to associate validations with.
