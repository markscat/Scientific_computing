cmake_minimum_required(VERSION 3.16)

project(Scientific_computing VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


# --- 新增：讓 AutoUic 知道去哪裡找 ui/ 資料夾裡的 .ui 檔案 ---
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES Scientific_computing_zh_TW.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6) 
    qt_add_executable(Scientific_computing
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        inc/ledcurrentlimit.h inc/Line_Width.h
        inc/ResCap_Conversion.h
        inc/UnitConverterHandler.h
        inc/via_current_cal.h
        inc/Voltage_Divider.h
        src/ledcurrentlimit.cpp
        src/Line_Width.cpp
        src/ResCap_Conversion.cpp
        src/UnitConverterHandler.cpp
        src/via_current_cal.cpp
        src/Voltage_Divider.cpp
        ui/ledcurrentlimit.ui
        ui/Line_Width.ui
        ui/ResCap_Conversion.ui
        ui/via_current_cal.ui
        ui/Voltage_Divider.ui
        resources/Scientific_computing.qrc
        #ui/Scientific_computing.qrc
        #ui/Scientific_computing.qrc
        #ui/Scientific_computing.qrc
        #ui/Scientific_computing.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Scientific_computing APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Scientific_computing SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Scientific_computing
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(Scientific_computing PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)


# --- 新增：加入標頭檔搜尋路徑，讓編譯器找到 inc/ 下的檔案以及生成的 ui_*.h ---
target_include_directories(Scientific_computing PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Scientific_computing)
endif()
set_target_properties(Scientific_computing PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Scientific_computing
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Scientific_computing)
endif()