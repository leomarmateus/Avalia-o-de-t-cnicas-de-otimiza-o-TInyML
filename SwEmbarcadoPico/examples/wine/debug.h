#ifndef DEBUG_FUNCTIONS_H_
#define DEBUG_FUNCTIONS_H_

#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/memory_helpers.h"

// Expose a C friendly interface for main functions.
#ifdef __cplusplus
extern "C" {
#endif

void model_summary(tflite::MicroInterpreter*);

#ifdef __cplusplus
}
#endif

#endif  // DEBUG_FUNCTIONS_H_