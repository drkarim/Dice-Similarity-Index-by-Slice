cmake_minimum_required(VERSION 2.8)
 
project(slice_dice)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})


 INCLUDE_DIRECTORIES(
	"~/Documents/Code/irtk/src/image++/include/"
	"~/Documents/Code/irtk/src/common++/include/"
	"~/Documents/Code/irtk/src/contrib++/include/"
	"~/Documents/Code/irtk/src/geometry++/include/"
	"/usr/local/include/"
 )

 LINK_DIRECTORIES(
	"~/Documents/Code/irtk/bin/lib/"
	"/usr/local/lib/"
	"~/Documents/Code/third_party/zlib/src/"
 )

add_executable(slice_dice slice_dice.cxx)

target_link_libraries(slice_dice ${VTK_LIBRARIES} contrib++ image++ geometry++ common++ znz gsl niftiio z)

