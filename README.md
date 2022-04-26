# Unreal Engine - VP Workflow Validation Framework
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
### 2.1 Binaries
### 2.2 Source

## 3. Quick Start

## 4. UI

### 4.1 Menu Bar
The Main UI can be loaded from the the menu bar

### 4.2 Validation Framework UI
The main UI for manual interaction with the validation framework.

### 4.3 Workflow
Drop down box which allows the user to select a specific workflow which we want to validate

### 4.4 Level Validation
Toggle to select all of the Level based validations for the selected workflow

### 4.5 Project Validation
Toggle to select all of the Project based validations for the selected workflow

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
Validations can be implemented via Blueprints or by C++, they comprise a few key features which we outline below. 

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
#### 5.2.1 Level
#### 5.2.2 Project
### 5.3 Validation
### 5.4 ValidationStatus
#### 5.4.1 Success
#### 5.4.2 Warning
#### 5.4.3 Fail
### 5.5 Fix
### 5.6 ValidationFixStatus
#### 5.6.1 Fixed
#### 5.6.2 ManualFix
#### 5.6.3 NotFixed

## 6. Validation Project Settings



## 7. CI/CD
### 7.1 Reports


## 8. Extending & Customizing
### 8.1 Adding New Validations
#### 8.1.1 Blueprints
#### 8.1.2 C++
### 8.2 Adding New Workflows
