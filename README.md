# Unreal Engine - VP Workflow Validation Framework
##### Supported Versions
UE4.27+: Supported: :white_check_mark:\
UE5.0+: Testing: :heavy_exclamation_mark:

## 1. Intro

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
4.12 [Validation Status](#412-validationstatus)\
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
6. [Validation Project Settings](#6-validation-project-settings)
7. [CI/CD](#7-cicd)\
7.1 [Reports](#71-reports)
8. [Extending & Customizing](#8-extending--customizing)\
8.1 [Adding New Validation](#81-adding-new-validations)\
8.2 [Adding New Workflows](#82-adding-new-workflows)

## 2. Installation
The Validation Framework comes as a self contained plugin with no external dependencies aside from the Unreal Engine itself.

The Plugin consists of a C++ set of validations, utilities, base classes and helpers.

It also contains Content in the form of blueprints and other .uassets.

The plugin can simply be added to the Unreal Project>Plugins folder as either a pre compiled binary or can be compiled from source code.

### 2.1 Binaries
1. Download the relevant zip file for the given releases and unreal version.
<INSERT LINK>
2. Unzip the plugin & place the folder inside the UEProject>Plugins Folder within the Unreal Project
<img src="/docs/images/plugin_folder.png" alt="Plugin Folder">

3. Open the unreal project in Unreal and from the plugins menu ensure that the plugin is loaded.
<img src="/docs/images/plugin_load.png" alt="Plugin Load">

### 2.2 Source
1. Clone the repository and either copy or link the *ValidationFramework* into the UnrealProject>Plugins folder.

2. Right Click The .uproject and Generate Solution <Check Name>

3. Once the solution is generated open the solution and compile the project.

4. Once compiled open the unreal project in unreal and from the plugins menu ensure that the plugin is loaded.

## 3. Quick Start

## 4. UI
The UI is built entirely via Blueprints as an EditorUtilityWidget.

There is no logic here
<img src="/docs/images/UI.png" alt="Validation UI">

### 4.1 Menu Bar
The Main UI can be loaded from the the menu item within the Netflix menu.

<img src="/docs/images/menubar.png" alt="Validation Menu Bar">

### 4.2 Validation Framework UI
The main UI for manual interaction with the validation framework.

### 4.3 Workflow
Drop down box which allows the user to select a specific workflow which we want to validate for.

This filters the validations visible and runnable from the UI.

### 4.4 Level Validation
Toggle to select all of the Level based validations for the selected workflow.

This filters the validations visible and runnable from the UI

### 4.5 Project Validation
Toggle to select all of the Project based validations for the selected workflow

This filters the validations visible and runnable from the UI

### 4.6 Refresh
Refresh button acts as an overall reset/refresh of the UI.
This clears all the existing states of the validations to a clean state.
As some validations are implemented via Blueprints, these may be registered after the UI has loaded. (We cant know about Blueprints until they compile unlike c++ which we know is compiled ahead of time)
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
Run buttons exist for each validation, this runs the validation checks for this specific validation.

### 4.12 ValidationStatus
A simple graphical display of the validations result, this shows whether a validation was a Success, a Fail or has Warnings that the user should inspect further.

### 4.13 Validation Message
Description of the outcome from the validation which was run.
Descriptions often identify offending actors, objects and their incompatible settings so a user could choose to resolve manually if they so wish.

### 4.14 Fix
Fix buttons exist for each validation, this runs the validation fix, for the specific validation.

Not all issues can be automatically fixed, however the user will be notified of this.

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

Sometimes its not possible to fix this automatically, in which case
the fix still needs implementing but this is as simple as returning a [ValidationFixStatus](#56-validationfixstatus) with status [ManualFix](#562-manualfix).

### 5.6 ValidationFixStatus
Fix Status's are returned from the execution of any fix logic. These carry both informative messages which can be relayed back to the user, along with an overall status related to the success.

Status can be [Fixed](#561-fixed), [ManualFix](#562-manualfix) or [NotFixed](#563-notfixed)
#### 5.6.1 Fixed
The fix or fixes where applied successfully.

#### 5.6.2 ManualFix
There is no automated fix available or this requires more context relating to the production and requires a user to manually fix.

#### 5.6.3 NotFixed
The fix was unable to be applied or failed and the issue still persists

## 6. Validation Project Settings

## 7. CI/CD
Whilst running validations via UI is advantages for users and during live operation in productions.

It can be also useful to execute these validations as part of a larger pipeline as an automated step. This could be as part of an asset publish, or a CI/CD build pipeline etc.

Whilst this framework does not provide the entire pipeline and framework for establishing the CI/CD and asset pipelines themselves.

It does provide the API building blocks for teams to integrate into their systems.

``` c++
static bool GenerateValidationReport(const FString LevelPath, const EValidationWorkflow Workflow, const FString ReportPath="");
```
Available as both blueprint node and c++ function, this api call allows the user or pipeline to specify a Level, a workflow, and an output path in which to generate validation reports which can then be consumed by applications outside of unreal engine. 

Validations of both [Project](#522-project) & [Level](#521-level) Scope are executed.s

### 7.1 Reports
Reports are generated both via the API call above, but also via the UI when [Run All Validations](#47-run-all-validations) is executed.

This serializes the results which are often displayed in the UI into reports which are stored along side the project folder.

UnrealProject>ValidationReports>NameOfLevel

These are stored as both csv or json format so are easily consumed by other applications, or simply sent back as part of support requests to supervisors or technical help groups.
<img src="/docs/images/reports.png" alt="Reports Folder">



## 8. Extending & Customizing
### 8.1 Adding New Validations
#### 8.1.1 Blueprints
#### 8.1.2 C++
### 8.2 Adding New Workflows
