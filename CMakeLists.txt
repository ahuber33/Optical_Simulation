#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
#
project(SN_OpticalSim)

set(CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
find_package(ROOT)
list(APPEND libs ${ROOT_LIBRARIES})
include_directories(${ROOT_INCLUDE_DIR})

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
find_package(Geant4 REQUIRED ui_all vis_all)
else()
find_package(Geant4 REQUIRED)

endif()

#
# Add specific modules
#

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#include_directories(${PROJECT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIR})
#include_directories(${ROOT_INCLUDE_DIR})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs

# IMPORTANT NOTE !!!!!!!!!!!!!!!!!!!!
# There is different file for Geometry, you need to choose only one of this file and comment the others :
# N3EC : NEMO3 bloc EC
# SNXW : SuperNEMO XWALL
# N3DC : NEMO3 Direct Coupling (EC geometry without light guide)
# 5inchesMW : SuperNEMO Main Wall 5 inches
# N3L4 : NEMO3 bloc L4

set(PROJECT_SRC
	${CMAKE_CURRENT_SOURCE_DIR}/src/Coupling.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/PMT.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/Scintillator.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/SN_OpticalSimEventAction.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/SN_OpticalSimTrackingAction.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/SN_OpticalSimVisManager.cc
	#${CMAKE_CURRENT_SOURCE_DIR}/src/SN_OpticalSimGeometry_N3EC.cc
	#${CMAKE_CURRENT_SOURCE_DIR}/src/SN_OpticalSimGeometry_SNXW.cc
	#${CMAKE_CURRENT_SOURCE_DIR}/src/SN_OpticalSimGeometry_N3DC.cc
	#${CMAKE_CURRENT_SOURCE_DIR}/src/SN_OpticalSimGeometry_N3DC_8inch.cc
	#${CMAKE_CURRENT_SOURCE_DIR}/src/SN_OpticalSimGeometry_5inchesMW.cc
	#${CMAKE_CURRENT_SOURCE_DIR}/src/SN_OpticalSimGeometry_8inchesMW.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/SN_OpticalSimGeometry_8inchesMW_test.cc
	#${CMAKE_CURRENT_SOURCE_DIR}/src/SN_OpticalSimGeometry_N3L4.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/SN_OpticalSimMaterials.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/SN_OpticalSimPhysics.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/SN_OpticalSimPrimaryGeneratorAction.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/SN_OpticalSimRunAction.cc
	${CMAKE_CURRENT_SOURCE_DIR}/src/SN_OpticalSimSteppingAction.cc
	#${CMAKE_CURRENT_SOURCE_DIR}/src/NEMO3_OpticalSimSteppingAction.cc
	#${CMAKE_CURRENT_SOURCE_DIR}/src/N3DC_OpticalSimSteppingAction.cc	
	${CMAKE_CURRENT_SOURCE_DIR}/src/SN_OpticalSimTrackInformation.cc
    )

set(PROJECT_HEADER
	${CMAKE_CURRENT_SOURCE_DIR}/include/Coupling.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/PMT.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/Scintillator.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/SN_OpticalSimEventAction.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/SN_OpticalSimTrackingAction.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/SN_OpticalSimVisManager.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/SN_OpticalSimGeometry.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/SN_OpticalSimMaterials.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/SN_OpticalSimPhysics.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/SN_OpticalSimPrimaryGeneratorAction.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/SN_OpticalSimRunAction.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/SN_OpticalSimSteppingAction.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/SN_OpticalSimTrackInformation.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/SN_OpticalSimCaloMap.hh
	${CMAKE_CURRENT_SOURCE_DIR}/include/SN_OpticalSimUniformityPMT.hh
    )

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(SN_OpticalSim SN_OpticalSim.cc ${PROJECT_HEADER} ${PROJECT_SRC})
#
target_link_libraries(SN_OpticalSim ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} )

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
#install(TARGETS SN_OpticalSim DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/bin)
