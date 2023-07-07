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
#include "Styling/SlateStyle.h"

#define VALIDATION_FRAMEWORK_UNDO_CAT "ValidationFramework"


class FValidationFrameworkModule final: public IModuleInterface
{
public:

	/** IModuleInterface implementation */

	/**
	* Startup Module handles the initializing of the Netflix Style Sheet, adds and registers the validation framework
	* project settings, adds and registers the menu
	*/
	virtual void StartupModule() override;

	/**
	* Shutdown Module handles the de registering of the Netflix Style Sheet amd project settings
	*/
	virtual void ShutdownModule() override;

private:
	/**
	* Holds the style style set for the menus initialized in the startup and destroyed in the shutdown
	*/
	TSharedPtr<FSlateStyleSet> StyleSet;

	/**
	* Adds the entries to the menu for each of the sub menu categories
	*/
	void AddMenuEntry(FMenuBuilder& MenuBuilder) const;

	/**
	* Populates the individual items to the sub menu categories
	*/
	void FillSubmenu(FMenuBuilder& MenuBuilder) const;

	/**
	* Call back for opening the Validation Framework UI which can be called from a menu entry
	*/
	void OpenValidationFrameworkCB() const;
};
