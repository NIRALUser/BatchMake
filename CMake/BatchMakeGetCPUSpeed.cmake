#
# Check the speed of the main CPU
#
# VARIABLE - variable to store the result to
#

macro(BatchMake_GET_CPU_SPEED CPU_MIPS_VAR CPU_MFLOPS_VAR)

  if(NOT DEFINED HAVE_${CPU_MIPS_VAR})

    if(NOT BatchMake_GET_CPU_SPEED_EXE)
	  message(FATAL_ERROR "BatchMake_GET_CPU_SPEED_EXE is not defined!")
    else(NOT BatchMake_GET_CPU_SPEED_EXE)

      message(STATUS "Check the speed of the main CPU...")

      execute_process(COMMAND ${BatchMake_GET_CPU_SPEED_EXE}
        OUTPUT_VARIABLE OUTPUT
        RESULT_VARIABLE RESULT)

      set(HAVE_${CPU_MIPS_VAR} ${RESULT} CACHE INTERNAL "")

      if(NOT OUTPUT OR NOT RESULT)
	    message(FATAL_ERROR "GET_CPU_SPEED ERROR:\n${OUTPUT}")
      else(NOT OUTPUT OR NOT RESULT)

        # MIPS
        string(REGEX MATCH "CPU_MIPS=[A-Z0-9]+" OUTPUT_CPU ${OUTPUT})
        if(OUTPUT_CPU)
          string(LENGTH  ${OUTPUT_CPU} OUTPUT_CPU_LENGTH)
          math(EXPR CPU_SIZE "${OUTPUT_CPU_LENGTH}-9")
          string(SUBSTRING ${OUTPUT_CPU} 9 ${CPU_SIZE} ${CPU_MIPS_VAR})
        else(OUTPUT_CPU)
          set(${CPU_MIPS_VAR} 0)
        endif(OUTPUT_CPU)

        # MFLOPS
        string(REGEX MATCH "CPU_MFLOPS=[A-Z0-9]+" OUTPUT_CPU ${OUTPUT})
        if(OUTPUT_CPU)
          string(LENGTH  ${OUTPUT_CPU} OUTPUT_CPU_LENGTH)
          math(EXPR CPU_SIZE "${OUTPUT_CPU_LENGTH}-11")
          string(SUBSTRING ${OUTPUT_CPU} 11 ${CPU_SIZE} ${CPU_MFLOPS_VAR})
        else(OUTPUT_CPU)
          set(${CPU_MFLOPS_VAR} 0)
        endif(OUTPUT_CPU)

      endif(NOT OUTPUT OR NOT RESULT)

    endif(NOT BatchMake_GET_CPU_SPEED_EXE)

  endif(NOT DEFINED HAVE_${CPU_MIPS_VAR})

endmacro(BatchMake_GET_CPU_SPEED)
