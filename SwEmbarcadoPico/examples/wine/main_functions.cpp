/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "main_functions.h"


#define MODEL_TFLITE 1

#ifdef MODEL_TFLITE
  #include "tensorflow/lite/micro/all_ops_resolver.h"
  #include "model.h"
  #include "tensorflow/lite/micro/micro_error_reporter.h"
  #include "tensorflow/lite/micro/micro_interpreter.h"
  #include "tensorflow/lite/schema/schema_generated.h"
  #include "tensorflow/lite/version.h"
#endif

#include "wine.h"
#include "pico/stdlib.h"
#include "debug.h"
#include "transmitter.h"
#include "tempo.h"

#if MODEL_TFLITE
  // Globals, used for compatibility with Arduino-style sketches.
  namespace {
  tflite::ErrorReporter* error_reporter = nullptr;
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;

  TfLiteTensor* model_input = nullptr;
  TfLiteTensor* model_output = nullptr;
  uint8_t ui8_buffer[5];
  float aux;
  uint64_t from;
  uint64_t final;
  uint64_t tempo_inferencia;
#else
  uint8_t ui8_buffer[200];
#endif

size_t size;
int output_length;
int inference_count = 0;
uint8_t position = 0; 
int linhas = sizeof(wine)/sizeof(wine[0]);
int colunas = sizeof(wine[0])/sizeof(wine[0][0]);

float hit_rate;
// uint8_t rc_ok = 0x55;
  int i=0;

int hit_counter;
float class_value[1125], max_class_value,output_class;

#if MODEL_TFLITE
// Create an area of memory to use for input, output, and intermediate arrays.
// The size of this will depend on the model you're using, and may need to be
// determined by experimentation.
// constexpr int kTensorArenaSize = 2000;
constexpr int kTensorArenaSize = 60 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
}  // namespace
#endif
// The name of this function is important for Arduino compatibility.
void setup() {
  stdio_init_all();
  sleep_ms(5000);  //wait for serial connection

  // Clear Screen (Esc[2J)
  printf("%c%c%c%c",0x1B,0x5B,0x32,0x4A);

  printf("========================================\n");
  printf("= Anomaly detection on Bearing\n");
  printf("========================================\n\n");
  start_time();
  loraBegin();
  sleep_ms(2000);
#if MODEL_TFLITE
  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(g_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  
  // Build an interpreter to run the model with.
  static tflite::AllOpsResolver resolver;

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  
  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  model_input = interpreter->input(0);
  model_output = interpreter->output(0);

  
  tflite::TfLiteTypeSizeOf(model_output->type, &size);
  output_length = model_output->bytes / size;

  model_summary(interpreter);
#endif
  // Keep track of how many inferences we have performed.
  inference_count = 0;
  get_time();

}

// void loop(){
//   test_lora();
//   sleep_ms(5000); 

// }

// ///// The name of this function is important for Arduino compatibility.
void loop() {

#if MODEL_TFLITE 
  
  // printf("========================================\n");
  // printf("Loop %d/%d\n", int(inference_count)+1, linhas);
  if (inference_count == linhas) {
    for (int i = 0; i < linhas; i++)
    {
      printf("%f\n", class_value[i]);
    }
    sleep_ms(100);
    inference_count = 0;
    hit_counter = 0;
    getchar();
  }
  // printf("----------------------------------------\n");
  // printf("Input Class = %d\n", int(wine[inference_count][0]));
  // printf("----------------------------------------\n");


  // Model Input Data
  for (int i = 0 ; i < colunas ; i++) {
    if (model_input->quantization.type) { // Quantized model
      model_input->data.int8[i-1] = int8_t (wine[inference_count][i] / model_input->params.scale + model_input->params.zero_point);
    } else { // Default model
      model_input->data.f[i] = wine[inference_count][i],i; 
    }
    // printf("Input Data[%d] = %.6f\n", i, wine[inference_count][i]);
  }
  
  // Run inference, and report any error.
  from = time_us_64();
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed");
    sleep_ms(5000);
    return;
  }
  final = time_us_64();
  tempo_inferencia = final - from;
  sleep_ms(10);
  printf("%" PRId64 "\n", tempo_inferencia);
  aux = 0;
  // Model Output int post train
  if (model_input->quantization.type) { // Quantized model
    for (int i = 0; i < output_length; i++){
        aux = float (((model_output->data.int8[i]) - model_output->params.zero_point) * model_output->params.scale);
        if(aux>wine[inference_count][i])
          class_value[inference_count] += aux-wine[inference_count][i];
        else
          class_value[inference_count] += (aux-wine[inference_count][i])*-1;
    }
  }else{
    for (int i = 0 ; i < output_length ; i++) {  
      if (model_output->data.f[i]>wine[inference_count][i])
        class_value[inference_count] += model_output->data.f[i]-wine[inference_count][i];
      else
        class_value[inference_count] += (model_output->data.f[i]-wine[inference_count][i])*-1;
    }
  }
  class_value[inference_count] = float(class_value[inference_count]/4);
  // printf("MSE %d = %f\n", inference_count, class_value[inference_count]);
  if (class_value[inference_count] > 0.25)
  {
    memset(ui8_buffer,0,sizeof(ui8_buffer));
    // get_time();
    // map_to_inference();
    ui8_buffer[0] = map_to_send(wine[inference_count][0],MIN_BEARING_1,MAX_BEARING_1,MIN_OUTPUT,MAX_OUTPUT);
    ui8_buffer[1] = map_to_send(wine[inference_count][1],MIN_BEARING_2,MAX_BEARING_2,MIN_OUTPUT,MAX_OUTPUT);
    ui8_buffer[2] = map_to_send(wine[inference_count][2],MIN_BEARING_3,MAX_BEARING_3,MIN_OUTPUT,MAX_OUTPUT);
    ui8_buffer[3] = map_to_send(wine[inference_count][3],MIN_BEARING_4,MAX_BEARING_4,MIN_OUTPUT,MAX_OUTPUT); 
    ui8_buffer[4] = 2;
    send_packet_inference(ui8_buffer);
  }
  sleep_ms(10); // mudar para 1 sec quando testar bateria
  inference_count += 1;

#else
    if (position % 200 == 0 && position !=0) {
      get_time();
      send_packet_full(ui8_buffer);
      position = 0;      
      memset(ui8_buffer, 0, sizeof(ui8_buffer));
      printf("inference_count: %d\n",inference_count);
  }

  if (inference_count == 1125) {
      get_time();
      position = 0;      
      memset(ui8_buffer, 0, sizeof(ui8_buffer));
      printf("Fim da Inferencia\n");
      getchar();
  }

    ui8_buffer[position+0] = map_to_send(wine[inference_count][0],MIN_BEARING_1,MAX_BEARING_1,MIN_OUTPUT,MAX_OUTPUT);
    ui8_buffer[position+1] = map_to_send(wine[inference_count][1],MIN_BEARING_2,MAX_BEARING_2,MIN_OUTPUT,MAX_OUTPUT);
    ui8_buffer[position+2] = map_to_send(wine[inference_count][2],MIN_BEARING_3,MAX_BEARING_3,MIN_OUTPUT,MAX_OUTPUT);
    ui8_buffer[position+3] = map_to_send(wine[inference_count][3],MIN_BEARING_4,MAX_BEARING_4,MIN_OUTPUT,MAX_OUTPUT);  
    
    inference_count++;
    position = position + 4;
#endif
sleep_ms(10); // mudar para 1 sec quando testar bateria
  

}
