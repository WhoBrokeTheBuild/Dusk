
MACRO(COMPILE_SHADERS shaders_in shaders_out)
    IF(VulkanSDK_FOUND)
        FOREACH(file ${shaders_in})
            GET_FILENAME_COMPONENT(shader_ext ${file} LAST_EXT)
            IF(shader_ext STREQUAL ".glsl")
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
                ELSE()
                    SET(shader_cflags ${shader_cflags} -fshader-stage=${shader_type})
                ENDIF()

                ADD_CUSTOM_COMMAND(
                    OUTPUT ${shader_out}
                    COMMAND ${VulkanSDK_glslc_PROGRAM} ${shader_cflags} -o ${shader_out} ${file}
                    DEPENDS ${file}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                )
                
                LIST(APPEND ${shaders_out} ${shader_out})
            ENDIF()
        ENDFOREACH()
    ENDIF()

    IF(WindowsSDK_FOUND)
        FOREACH(file ${shaders_in})
            GET_FILENAME_COMPONENT(shader_ext ${file} LAST_EXT)
            IF(shader_ext STREQUAL ".hlsl")
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

                SET(shader_out "${shader_out_path}/${shader_name}.dxc")

                GET_FILENAME_COMPONENT(shader_type ${shader_name} LAST_EXT)
                STRING(SUBSTRING ${shader_type} 1 -1 shader_type)

                # Use row-major matricies
                SET(shader_cflags ${shader_cflags} "-Zpc")

                IF(shader_type STREQUAL "inc")
                    CONTINUE()
                ELSEIF(shader_type STREQUAL "vert")
                    SET(shader_cflags ${shader_cflags} -T vs_6_0 -E VSMain)
                ELSEIF(shader_type STREQUAL "frag")
                    SET(shader_cflags ${shader_cflags} -T ps_6_0 -E PSMain)
                ELSEIF(shader_type STREQUAL "comp")
                    SET(shader_cflags ${shader_cflags} -T cs_6_0 -E CSMain)
                ELSEIF(shader_type STREQUAL "geom")
                    SET(shader_cflags ${shader_cflags} -T gs_6_0 -E GSMain)
                ENDIF()

                ADD_CUSTOM_COMMAND(
                    OUTPUT ${shader_out}
                    COMMAND ${WindowsSDK_dxc_PROGRAM} ${shader_cflags} -Fo ${shader_out} ${file}
                    DEPENDS ${file}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                )

                LIST(APPEND ${shaders_out} ${shader_out})
            ENDIF()
        ENDFOREACH()
    ENDIF()
ENDMACRO()