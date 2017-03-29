cmake_minimum_required(VERSION 2.8)
 
project(slice_dice_MIRTK)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(MIRTK REQUIRED)


 INCLUDE_DIRECTORIES(
	"~/Documents/Code/third_party/mirtk/bin/include" 
	"~/Documents/Code/third_party/mirtk/src/Modules/Common/include"
	"~/Documents/Code/third_party/mirtk/src/Modules/Image/include"
	"~/Documents/Code/third_party/mirtk/src/Modules/IO/include"
	"~/Documents/Code/third_party/mirtk/src/Modules/Numerics/include"
	"~/Documents/Code/third_party/mirtk/src/Modules/PointSet/include"
	"~/Documents/Code/third_party/mirtk/src/Modules/Registration/include"
	"~/Documents/Code/third_party/mirtk/src/Modules/Transformation/include"
	"/usr/local/include/"
 )

 LINK_DIRECTORIES(
	"~/Documents/Code/third_party/mirtk/bin/lib/"
	"/usr/local/lib/"
	"~/Documents/Code/third_party/zlib/src/"
 )

add_executable(slice_dice_mirtk slice_dice_mirtk.cxx)

target_link_libraries(slice_dice_mirtk ${VTK_LIBRARIES} LibCommon LibNumerics LibImage LibIO)

