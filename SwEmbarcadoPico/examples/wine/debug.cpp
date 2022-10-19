#include "debug.h"
#include <stdio.h>

void model_summary(tflite::MicroInterpreter* interpreter) {

  size_t size;

  TfLiteTensor* model_input = interpreter->input(0);
  TfLiteTensor* model_output = interpreter->output(0);

  printf("----------------------------------------\n");
  printf("- Model Summary\n");
  printf("----------------------------------------\n\n");

  tflite::TfLiteTypeSizeOf(model_input->type, &size);

  printf("- %-20s --> %s (%d)\n",
          "Input Type",
          TfLiteTypeGetName(model_input->type),
          size
      );

  printf("- %-20s --> %d\n",
          "Input Size",
          model_input->bytes / size
      );

  printf("- %-20s --> %s\n\n",
          "Input Quantization",
          (model_input->quantization.type) ? "TRUE" : "FALSE"
      );

  if (model_input->quantization.type)
  {
  printf("- %-20s --> x_int8 = ( x / %f ) + ( %d )\n\n",
          "Input Equation",
          model_input->params.scale,
          model_input->params.zero_point
          );
  }

  tflite::TfLiteTypeSizeOf(model_output->type, &size);

  printf("- %-20s --> %s (%d)\n",
          "Output Type",
          TfLiteTypeGetName(model_output->type),
          size
      );

  printf("- %-20s --> %d\n",
          "Output Size",
          model_output->bytes / size
      );

  printf("- %-20s --> %s\n\n",
          "Output Quantization",
          (model_output->quantization.type) ? "TRUE" : "FALSE"
      );

  if (model_output->quantization.type)
  {
  printf("- %-20s --> y = ( y_int8 - (%d) ) x %f\n\n",
          "Output Equation",
          model_output->params.zero_point,
          model_output->params.scale
          );
  }

  printf("----------------------------------------\n\n");

  printf("Press ENTER key to continue...\n");
//   getchar();
}