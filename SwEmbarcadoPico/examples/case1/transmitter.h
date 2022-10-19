/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <math.h>

#include "LoRa-RP2040.h"

#define MAX_SIZE_SEND 220
#define HEADER 6

#define MAX_OUTPUT  255
#define MIN_OUTPUT  0



// infos do dataset_test -- nasaa.py
#define MAX_BEARING_1 0.122483
#define MAX_BEARING_2 0.224439
#define MAX_BEARING_3 0.543770
#define MAX_BEARING_4 0.244897

#define MIN_BEARING_1 0.010065	
#define MIN_BEARING_2 0.002441
#define MIN_BEARING_3 0.003663	
#define MIN_BEARING_4 0.001428
         
#define MEAN_BEARING_1 0.063355
#define MEAN_BEARING_2 0.069354
#define MEAN_BEARING_3 0.075143 
#define MEAN_BEARING_4 0.052171

#define STD_BEARING_1 0.010065
#define STD_BEARING_2 0.020400	
#define STD_BEARING_3 0.051188
#define STD_BEARING_4 0.024280


int checksumCalc(uint8_t *data, int bytes);

void loraBegin();

void send_packet_full(uint8_t data[]);

void send_packet(uint8_t data[]);

void send_packet_inference(uint8_t data[]);

void debug_data_send(uint8_t buffer[]);

int map_to_send(float x, float in_min, float in_max, int out_min, int out_max);
float scaler_transform(float x, int index);