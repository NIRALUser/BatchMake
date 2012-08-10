#
# Check the speed of the main CPU
#
# VARIABLE - variable to store the result to
#

macro(BatchMake_GET_CPU_INFO CPU_SPEED_MHZ_VAR CPU_NB_PHYSICAL_VAR CPU_NB_LOGICAL_VAR )

  if(NOT DEFINED HAVE_${CPU_SPEED_MHZ_VAR})

    if(NOT BatchMake_GET_CPU_INFO_EXE)
	  message(FATAL_ERROR "BatchMake_GET_CPU_INFO_EXE is not defined!")
    else(NOT BatchMake_GET_CPU_INFO_EXE)

      message(STATUS "Check the speed of the main CPU...")

      execute_process(COMMAND ${BatchMake_GET_CPU_INFO_EXE}
        OUTPUT_VARIABLE OUTPUT
        RESULT_VARIABLE RESULT)

      set(HAVE_${CPU_SPEED_MHZ_VAR} ${RESULT} CACHE INTERNAL "")

      if(NOT OUTPUT OR NOT RESULT)
	    message(FATAL_ERROR "GET_CPU_INFO ERROR:\n${OUTPUT}")
      else(NOT OUTPUT OR NOT RESULT)

        # CPU speed
        string(REGEX MATCH "CPU_SPEED_MHZ=[A-Z0-9]+" OUTPUT_CPU ${OUTPUT})
        if(OUTPUT_CPU)
          string(LENGTH  ${OUTPUT_CPU} OUTPUT_CPU_LENGTH)
          math(EXPR CPU_SIZE "${OUTPUT_CPU_LENGTH}-14")
          string(SUBSTRING ${OUTPUT_CPU} 14 ${CPU_SIZE} ${CPU_SPEED_MHZ_VAR})
        else(OUTPUT_CPU)
          set(${CPU_SPEED_MHZ_VAR} 0)
        endif(OUTPUT_CPU)

        # Number of physical CPUs
        string(REGEX MATCH "CPU_PHYSICAL_CPUS=[A-Z0-9]+" OUTPUT_CPU ${OUTPUT})
        if(OUTPUT_CPU)
          string(LENGTH  ${OUTPUT_CPU} OUTPUT_CPU_LENGTH)
          math(EXPR CPU_SIZE "${OUTPUT_CPU_LENGTH}-18")
          string(SUBSTRING ${OUTPUT_CPU} 18 ${CPU_SIZE} ${CPU_NB_PHYSICAL_VAR})
        else(OUTPUT_CPU)
          set(${CPU_NB_PHYSICAL_VAR} 0)
        endif(OUTPUT_CPU)

        # Number of logical CPUs
        string(REGEX MATCH "CPU_LOGICAL_CPUS=[A-Z0-9]+" OUTPUT_CPU ${OUTPUT})
        if(OUTPUT_CPU)
          string(LENGTH  ${OUTPUT_CPU} OUTPUT_CPU_LENGTH)
          math(EXPR CPU_SIZE "${OUTPUT_CPU_LENGTH}-17")
          string(SUBSTRING ${OUTPUT_CPU} 17 ${CPU_SIZE} ${CPU_NB_LOGICAL_VAR})
        else(OUTPUT_CPU)
          set(${CPU_NB_LOGICAL_VAR} 0)
        endif(OUTPUT_CPU)

      endif(NOT OUTPUT OR NOT RESULT)

    endif(NOT BatchMake_GET_CPU_INFO_EXE)

  endif(NOT DEFINED HAVE_${CPU_SPEED_MHZ_VAR})

endmacro(BatchMake_GET_CPU_INFO)
