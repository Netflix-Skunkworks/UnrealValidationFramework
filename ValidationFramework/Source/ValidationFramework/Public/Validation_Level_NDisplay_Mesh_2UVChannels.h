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
#include "Validation_Level_NDisplay_Mesh_2UVChannels.generated.h"

/**
* Validation to ensure that any meshes which are used as part of an NDisplay setup have 2 UV channels.
* This is often missed by tds and causes problems when adding in the green screen, or inner frustums.
* As much as we cant check the UVS are correct, we can tell if we do not have the correct number of uv channels.
* This is documented in the unreal engine ICVFX guide.
*/
UCLASS()
class VALIDATIONFRAMEWORK_API UValidation_Level_NDisplay_Mesh_2UVChannels final : public UValidationBase
{
	GENERATED_BODY()

public:
	UValidation_Level_NDisplay_Mesh_2UVChannels();
	
	virtual FValidationResult Validation_Implementation() override;
	virtual FValidationFixResult Fix_Implementation() override;

	/**
	* Function to run the actual validation to check the mesh has exactly 2 uv channels.
	* This function is passed to a generic NDisplay mesh validation. 
	* @param StaticMesh - The mesh we want to validate
	* @param LodIndex - The lod of the static mesh that we want to validate
	* @param Message - Any Messages which we want to store from within the validation
	* @return The ValidationStatus of the actual check.
	*/
	static EValidationStatus ValidateMeshesWithout2UVChannels(UStaticMesh* StaticMesh, const int LodIndex, FString& Message);

};



