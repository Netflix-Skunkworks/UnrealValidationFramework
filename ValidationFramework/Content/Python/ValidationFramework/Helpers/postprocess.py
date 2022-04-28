import unreal


@unreal.uclass()
class ValidationFrameworkPostProcessHelpers(unreal.BlueprintFunctionLibrary):
    
    @unreal.ufunction(ret=bool, static=True, params=[str, unreal.PostProcessSettings], meta=dict(Category="ValidationBPLibrary"))
    def GetOverrideForPostProcessingSetting(overrideName, postProcessSettings):
        return postProcessSettings.get_editor_property(overrideName)
    
    @unreal.ufunction(ret=unreal.PostProcessSettings, static=True, params=[str, bool, unreal.PostProcessSettings], meta=dict(Category="ValidationBPLibrary"))
    def SetOverrideForPostProcessingSetting(overrideName, value, postProcessSettings):
        postProcessSettings.set_editor_property(overrideName, value)
        return postProcessSettings
