
MACRO(COMPILE_SHADERS shaders_in shaders_out)
    IF(VulkanSDK_FOUND)
        FOREACH(file ${shaders_in})
            GET_FILENAME_COMPONENT(shader_name ${file} NAME_WLE)
            GET_FILENAME_COMPONENT(shader_path ${file} DIRECTORY)
            STRING(REPLACE 
                ${CMAKE_CURRENT_SOURCE_DIR}
                ${CMAKE_CURRENT_BINARY_DIR}
                shader_out_path
                ${shader_path}
            )

            FILE(MAKE_DIRECTORY ${shader_out_path})

            SET(shader_cflags "")
            FOREACH(dir ${SHADER_INCLUDE_PATH} ${shader_path})
                SET(shader_cflags ${shader_cflags} -I${dir})
            ENDFOREACH()

            SET(shader_out "${shader_out_path}/${shader_name}.spv")

            GET_FILENAME_COMPONENT(shader_type ${shader_name} LAST_EXT)
            STRING(SUBSTRING ${shader_type} 1 -1 shader_type)

            IF(shader_type STREQUAL "inc")
                CONTINUE()
            ENDIF()

            ADD_CUSTOM_COMMAND(
                OUTPUT ${shader_out}
                COMMAND ${VulkanSDK_glslc_PROGRAM} -fshader-stage=${shader_type} ${shader_cflags} -o ${shader_out} ${file}
                DEPENDS ${file}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
            
            LIST(APPEND ${shaders_out} ${shader_out})
        ENDFOREACH()
    ENDIF()
ENDMACRO()