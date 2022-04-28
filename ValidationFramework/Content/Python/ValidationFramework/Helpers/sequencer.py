import unreal


@unreal.uclass()
class ValidationFrameworkSequencerHelpers(unreal.BlueprintFunctionLibrary):
    """ Gets all the level sequence actors from the given world, and checks their level sequences frame rates against the given project frame rate.
        Checks to ensure that frame rates match, if not that they are flagged as an error, or if they are a valid multiple they are flagged as a warning for the user to check

        Args:
            projectFrameRate: unreal.FrameRate - the frame rate we want to validate again
            world: unreal.world - the world we want to get the level sequence actors from

        Returns:
            unreal.ValidationResult - the result of the validation check
    """
    @unreal.ufunction(ret=unreal.ValidationResult, static=True, params=[unreal.FrameRate, unreal.World], meta=dict(Category="ValidationBPLibrary"))
    def validateSequencesAgainstFrameRate(projectFrameRate, world):
        actors = unreal.GameplayStatics.get_all_actors_of_class(world, unreal.LevelSequenceActor)
        validationStatus = unreal.ValidationStatus.PASS
        validationMessages = []

        if not actors:
            validationMessages.append("Valid As Not Sequences Found In Level")

        for actor in actors:
            sequence_validation = unreal.ValidationStatus.PASS
            sequence_validation_message = ""
            
            level_sequence = actor.load_sequence()
            display_rate = level_sequence.get_display_rate()
            
            # We check the level sequence display rate vs the project frame rate and if they do not match we fail
            if display_rate.numerator != projectFrameRate.numerator or display_rate.denominator != projectFrameRate.denominator:
                sequence_validation = unreal.ValidationStatus.FAIL
                sequence_validation_message = "{0} frame rate {1} does not match the project frame rate {2}".format(
                    actor.get_name(),
                    display_rate.numerator,
                    projectFrameRate.numerator
                )

                # It could be that they do not match but the display rate could be a multiple of the project, ie, 48fps against 24 fps. Often
                # not but the user should make sure this is expected for the workflow so we only warn
                if unreal.TimeManagementLibrary.is_valid_multiple_of(projectFrameRate, display_rate):
                    sequence_validation = unreal.ValidationStatus.WARNING
                    sequence_validation_message += " but is a valid multiple, please check this is expected".format(actor.get_name())
            
            if sequence_validation.value < validationStatus.value:
                validationStatus = sequence_validation
            
            validationMessages.append(sequence_validation_message)

        result = unreal.ValidationResult()
        result.result = validationStatus
        result.message = "\n".join(validationMessages)
        return result

    @unreal.ufunction(ret=unreal.ValidationFixResult, static=True, params=[unreal.FrameRate, unreal.World], meta=dict(Category="ValidationBPLibrary"))
    def fixSequencesAgainstFrameRate(projectFrameRate, world):
        """ Gets all the level sequence actors from the given world, and checks their level sequences frame rates against the given project frame rate.
            Sets the sequencers to match the project frame rate, unless its a multiplier of the project frame rate in which case we highlight to the user to fix manually or ignore

            Args:
                projectFrameRate: unreal.FrameRate - the frame rate we want to validate against and fix sequences
                world: unreal.world - the world we want to get the level sequence actors from

            Returns:
                unreal.ValidationFixResult - the result of the validation fixes
        """
        actors = unreal.GameplayStatics.get_all_actors_of_class(world, unreal.LevelSequenceActor)
        fix_status = unreal.ValidationFixStatus.FIXED
        fix_messages = []
        for actor in actors:
            sequence_validation = unreal.ValidationFixStatus.FIXED
            
            level_sequence = actor.load_sequence()
            display_rate = level_sequence.get_display_rate()
            
            # We check the level sequence display rate vs the project frame rate and if they do not match we flag to fix
            if display_rate.numerator != projectFrameRate.numerator or display_rate.denominator != projectFrameRate.denominator:
                sequence_validation = unreal.ValidationFixStatus.NOT_FIXED

                # If it is a potentisally valid multiple we flag this as needing a manual fix as we do not not for sure this is not
                # intentional
                if unreal.TimeManagementLibrary.is_valid_multiple_of(projectFrameRate, display_rate):
                    sequence_validation = unreal.ValidationFixStatus.MANUAL_FIX
                    fix_messages.append(
                    "{0} Frame Rate Should Be Checked Manually".format(level_sequence.get_name())
                )
                    
            # If the sequence is still flagged as not fixed we fix it because we know its not already Fixed and can not be left for manual inspection
            if sequence_validation == unreal.ValidationFixStatus.NOT_FIXED:
                level_sequence.set_display_rate(projectFrameRate)
                fix_messages.append(
                    "Set {0} Frame Rate To {1}".format(level_sequence.get_name(), projectFrameRate.numerator)
                )
        
            if sequence_validation.value < fix_status.value:
                fix_status = sequence_validation

        result = unreal.ValidationFixResult()
        result.result = fix_status
        result.message = "\n".join(fix_messages)
        return result





