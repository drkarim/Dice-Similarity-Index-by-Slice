##############################################################################
# @file  CMakeLists.txt
# @brief Build configuration of MIRTK SliceDice tools.
##############################################################################

mirtk_add_executable(
  evaluate-slice-dice
  DEPENDS
    LibCommon
    LibNumerics
    LibImage
    LibIO
)
