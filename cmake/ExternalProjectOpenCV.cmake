find_package(Threads REQUIRED)
include(ExternalProject)


set(TMP_PATH ${CMAKE_BINARY_DIR}/3rd/external_files/tmp)
set(STAMP_PATH ${CMAKE_BINARY_DIR}/3rd/external_files/stamp)
set(LOG_PATH ${CMAKE_BINARY_DIR}/3rd/external_files/log)

set(OPENCV_URL https://github.com/opencv/opencv)

set(OPENCV_ROOT	${CMAKE_BINARY_DIR}/3rd/openCV)
set(OPENCV_BUILD ${CMAKE_BINARY_DIR}/3rd/openCV_BUILD)

make_directory(${CMAKE_OPENCV_INSTALL_DIR}/include)
make_directory(${CMAKE_OPENCV_INSTALL_DIR}/x64/vc16/lib)

include_directories(${CMAKE_OPENCV_INSTALL_DIR}/include)
link_directories(${CMAKE_OPENCV_INSTALL_DIR}/x64/vc16/lib)

set(OpenCV_LIBRARIES)

list(APPEND OpenCV_LIBRARIES
    ${CMAKE_SOURCE_DIR}/dependencies/x64/vc16/lib/opencv_calib3d450d.lib
    ${CMAKE_SOURCE_DIR}/dependencies/x64/vc16/lib/opencv_core450d.lib
    ${CMAKE_SOURCE_DIR}/dependencies/x64/vc16/lib/opencv_dnn450d.lib
    ${CMAKE_SOURCE_DIR}/dependencies/x64/vc16/lib/opencv_features2d450d.lib
    ${CMAKE_SOURCE_DIR}/dependencies/x64/vc16/lib/opencv_flann450d.lib
    ${CMAKE_SOURCE_DIR}/dependencies/x64/vc16/lib/opencv_gapi450d.lib
    ${CMAKE_SOURCE_DIR}/dependencies/x64/vc16/lib/opencv_highgui450d.lib
    ${CMAKE_SOURCE_DIR}/dependencies/x64/vc16/lib/opencv_imgcodecs450d.lib
    ${CMAKE_SOURCE_DIR}/dependencies/x64/vc16/lib/opencv_imgproc450d.lib
    ${CMAKE_SOURCE_DIR}/dependencies/x64/vc16/lib/opencv_ml450d.lib
    ${CMAKE_SOURCE_DIR}/dependencies/x64/vc16/lib/opencv_objdetect450d.lib
    ${CMAKE_SOURCE_DIR}/dependencies/x64/vc16/lib/opencv_photo450d.lib
    ${CMAKE_SOURCE_DIR}/dependencies/x64/vc16/lib/opencv_stitching450d.lib
    ${CMAKE_SOURCE_DIR}/dependencies/x64/vc16/lib/opencv_video450d.lib
    ${CMAKE_SOURCE_DIR}/dependencies/x64/vc16/lib/opencv_videoio450d.lib
    )

ExternalProject_Add(external_openCV
    PREFIX            ${OPENCV_ROOT}
    DOWNLOAD_DIR      ${OPENCV_ROOT}
    SOURCE_DIR        ${OPENCV_ROOT}
    BINARY_DIR        ${OPENCV_BUILD}
    INSTALL_DIR       ${CMAKE_OPENCV_INSTALL_DIR}
    TMP_DIR           ${TMP_PATH}
    STAMP_DIR         ${STAMP_PATH}
    LOG_DIR 	      ${LOG_PATH}
    CMAKE_ARGS        "-DCMAKE_INSTALL_PREFIX=${CMAKE_OPENCV_INSTALL_DIR}"
    GIT_REPOSITORY    ${OPENCV_URL}
    GIT_TAG           4.5.0
    GIT_PROGRESS      ON
    BUILD_COMMAND     ${CMAKE_COMMAND} --build ${OPENCV_BUILD} --config Release
    LOG_CONFIGURE     ON
    LOG_BUILD         ON
    LOG_INSTALL       ON
    BUILD_BYPRODUCTS  ${OpenCV_LIBRARIES}
    )

add_dependencies(vectorizer external_openCV)
set_property( TARGET vectorizer PROPERTY IMPORTED_LOCATION ${OpenCV_LIBRARIES}  )
