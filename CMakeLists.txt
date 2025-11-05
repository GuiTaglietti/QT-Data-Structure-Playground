cmake_minimum_required(VERSION 3.5)

project(QTDataStructurePlayground VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql LinguistTools)

set(TS_FILES
    i18n/QTDataStructurePlayground_pt_BR.ts
    i18n/QTDataStructurePlayground_en_US.ts
    i18n/QTDataStructurePlayground_es_ES.ts
)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QTDataStructurePlayground
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        diagram_view.h
        diagram_view.cpp
        code_exporter.h
        code_exporter.cpp
        texts.h
        base_panel.h
        base_panel.cpp
        stack_panel.h
        stack_panel.cpp
        queue_panel.h
        queue_panel.cpp
        hash_panel.h
        hash_panel.cpp
        sll_panel.h
        sll_panel.cpp
        dll_panel.h
        dll_panel.cpp
        graph_panel.h
        graph_panel.cpp
        bst_panel.h
        bst_panel.cpp
        rbt_panel.h
        rbt_panel.cpp
        storage_manager.h
        storage_manager.cpp
    )
else()
    if(ANDROID)
        add_library(QTDataStructurePlayground SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(QTDataStructurePlayground
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QTDataStructurePlayground PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(QTDataStructurePlayground PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

qt_add_translation(QM_FILES ${TS_FILES})

qt_add_resources(QTDataStructurePlayground translations_autogen
    PREFIX "/i18n"
    FILES ${QM_FILES}
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.QTDataStructurePlayground)
endif()
set_target_properties(QTDataStructurePlayground PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS QTDataStructurePlayground
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QTDataStructurePlayground)
endif()
