cmake_minimum_required(VERSION 2.8)
project( odewithcmake )

include_directories ("${PROJECT_SOURCE_DIR}/Include")
include_directories ("${PROJECT_SOURCE_DIR}/Utils/Include")
include_directories ("${PROJECT_SOURCE_DIR}/eigen")


link_directories("/usr/local/lib")

add_library(lib ClippedGaussianSampling.cpp  DiagonalGMM.cpp  DynamicPdfSampler.cpp EigenGeometryUtils.cpp EigenMathUtils.cpp  FileUtils.cpp IndexRandomizer.cpp LinAlgUtilities.cpp NormalDistribution.cpp Optimization.cpp RandomForestSampler.cpp SamplingTree.cpp)


target_link_libraries( lib ode )
