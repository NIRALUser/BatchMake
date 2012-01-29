#
# Check the total physical memory available on the system
#
# VARIABLE - variable to store the result to
#

macro(BatchMake_GET_MEMORY_INFO MEMORY_TOTAL_PHYSICAL_MB_VAR)

  if(NOT DEFINED HAVE_${MEMORY_TOTAL_PHYSICAL_MB_VAR})

    if(NOT BatchMake_GET_MEMORY_INFO_EXE)
	  message(FATAL_ERROR "BatchMake_GET_MEMORY_INFO_EXE is not defined!")
    else(NOT BatchMake_GET_MEMORY_INFO_EXE)

      message(STATUS
        "Check the total amount of memory on the system...")

      execute_process(COMMAND ${BatchMake_GET_MEMORY_INFO_EXE}
        OUTPUT_VARIABLE OUTPUT
        RESULT_VARIABLE RESULT)

      set(HAVE_${MEMORY_TOTAL_PHYSICAL_MB_VAR} ${RESULT} CACHE INTERNAL "")

      if(NOT OUTPUT OR NOT RESULT)
	    message(FATAL_ERROR "GET_MEMORY_INFO ERROR:\n${BatchMake_GET_MEMORY_INFO_EXE}\n${OUTPUT}")
      else(NOT OUTPUT OR NOT RESULT)

        string(REGEX MATCH "PHYSICAL_MEMORY_MB=[-A-Z0-9]+" OUTPUT_MEM ${OUTPUT})
        string(LENGTH ${OUTPUT_MEM} OUTPUT_MEM_LENGTH)
        math(EXPR MEM_SIZE "${OUTPUT_MEM_LENGTH}-19")
        string(SUBSTRING ${OUTPUT_MEM} 19 ${MEM_SIZE} ${MEMORY_TOTAL_PHYSICAL_MB_VAR})
      endif(NOT OUTPUT OR NOT RESULT)

    endif(NOT BatchMake_GET_MEMORY_INFO_EXE)

  endif(NOT DEFINED HAVE_${MEMORY_TOTAL_PHYSICAL_MB_VAR})

endmacro(BatchMake_GET_MEMORY_INFO)
