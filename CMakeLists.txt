cmake_minimum_required(VERSION 3.7...3.26)

# Allow IDE Source Tree Visualization
if(${CMAKE_VERSION} VERSION_LESS 3.26)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
else()
    cmake_policy(SET CMP0143 NEW)
endif()

project(UTTT)

set(SRCS
    "src/attacks.cpp"
    "src/attacks.h"
    "src/bench.cpp"
    "src/bench.h"
    "src/bitboard.h"
    "src/board.cpp"
    "src/board.h"
    "src/datagen.cpp"
    "src/datagen.h"
    "src/eval_constants.h"
    "src/evaluate.cpp"
    "src/evaluate.h"
    "src/main.cpp"
    "src/movegen.h"
    "src/move_picker.cpp"
    "src/move_picker.h"
    "src/search.cpp"
    "src/search.h"
    "src/time_man.cpp"
    "src/time_man.h"
    "src/tt.cpp"
    "src/tt.h"
    "src/types.h"
    "src/zobrist.h"

    "src/tuner/dataset.cpp"
    "src/tuner/dataset.h"
    "src/tuner/eval_fn.cpp"
    "src/tuner/eval_fn.h"
    "src/tuner/settings.h"
    "src/tuner/thread_pool.cpp"
    "src/tuner/thread_pool.h"
    "src/tuner/trace.h"
    "src/tuner/tune.cpp"
    "src/tuner/tune.h"

    "src/util/multi_array.h"
    "src/util/prng.h"
    "src/util/static_vector.h"
)

if("${UTTT_EXE_EXTENSION}" STREQUAL "")
    message("none")
    set(UTTT_EXE_NAME "uttt")
else()
    set(UTTT_EXE_NAME "uttt-${UTTT_EXE_EXTENSION}")
endif()

add_executable(${UTTT_EXE_NAME} ${SRCS})

target_compile_features(${UTTT_EXE_NAME} PRIVATE cxx_std_20)

# for Visual Studio/MSVC
set_target_properties(${UTTT_EXE_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})