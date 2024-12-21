if (IMGUI)
    message("-- IMGUI support activated")
    target_compile_definitions(Reveal3d PUBLIC IMGUI=1)
    target_include_directories(Reveal3d PUBLIC extern/include/IMGUI)

    set(IMGUI_DIR extern/include/imgui)

    set(IMGUI
            ${IMGUI_DIR}/imgui.cpp
            ${IMGUI_DIR}/imgui.h
            ${IMGUI_DIR}/imconfig.h
            ${IMGUI_DIR}/imgui_draw.cpp
            ${IMGUI_DIR}/imgui_internal.h
            ${IMGUI_DIR}/imgui_tables.cpp
            ${IMGUI_DIR}/imgui_widgets.cpp
            ${IMGUI_DIR}/imstb_rectpack.h
            ${IMGUI_DIR}/imstb_textedit.h
            ${IMGUI_DIR}/imstb_truetype.h
    )

    set(IMGUI_BACKENDS
            ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
            ${IMGUI_DIR}/backends/imgui_impl_win32.h
            ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
            ${IMGUI_DIR}/backends/imgui_impl_glfw.h
            ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
            ${IMGUI_DIR}/backends/imgui_impl_opengl3.h
            ${IMGUI_DIR}/backends/imgui_impl_dx12.cpp
            ${IMGUI_DIR}/backends/imgui_impl_dx12.h
            #        ${IMGUI_DIR}/imgui_impl_dx11.cpp
            #        ${IMGUI_DIR}/imgui_impl_dx11.h
    )


    target_sources(Reveal3d PUBLIC ${IMGUI} ${IMGUI_BACKENDS})
endif ()
