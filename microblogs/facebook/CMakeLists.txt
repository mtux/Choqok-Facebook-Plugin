project(choqok-facebook)

cmake_minimum_required(VERSION 2.6)

find_package(KDE4 REQUIRED)
find_package(Choqok REQUIRED)

include (KDE4Defaults)
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(
    ${CHOQOK_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set( choqok_facebook_SRCS
facebookaccount.cpp
facebookediaccountwidget.cpp
facebookmicroblog.cpp
)

kde4_add_ui_files(choqok_facebook_SRCS
facebookeditaccount_base.ui
)

kde4_add_plugin(choqok_facebook ${choqok_facebook_SRCS})

target_link_libraries(choqok_facebook ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${QJSON_LIBRARY} ${CHOQOK_LIBRARIES})

# Install

install(TARGETS choqok_facebook DESTINATION ${PLUGIN_INSTALL_DIR})
install( FILES choqok_facebook.desktop  DESTINATION ${SERVICES_INSTALL_DIR})

add_subdirectory(icons)
