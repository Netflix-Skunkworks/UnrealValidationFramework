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
#include "Validation_Level_NDisplay_Mesh_FullPrecisionUVs.generated.h"

/**
* Validation to ensure that any meshes which are used as part of an NDisplay setup have full precision uvs enabled.
* Without this can cause issues with the seams between panels, and affect how the images are displayed.
*/
UCLASS()
class VALIDATIONFRAMEWORK_API UValidation_Level_NDisplay_Mesh_FullPrecisionUVs final: public UValidationBase
{
	GENERATED_BODY()

public:
	UValidation_Level_NDisplay_Mesh_FullPrecisionUVs();

	virtual FValidationResult Validation_Implementation() override;
	virtual FValidationFixResult Fix_Implementation() override;

	/**
	* Function to run the actual validation to check the mesh is using full precision uvs.
	* This function is passed to a generic NDisplay mesh validation. 
	* @param StaticMesh - The mesh we want to validate
	* @param LodIndex - The lod of the static mesh that we want to validate
	* @param Message - Any Messages which we want to store from within the validation
	* @return The ValidationStatus of the actual check.
	*/
	static EValidationStatus ValidateFullPrecisionUVs(UStaticMesh* StaticMesh, const int LodIndex, FString& Message);
	
};


