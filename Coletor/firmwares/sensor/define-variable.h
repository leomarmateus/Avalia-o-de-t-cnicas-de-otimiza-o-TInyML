/*******************************************************************************
* Definição de Variaveis
*******************************************************************************/

byte  data_read[256];
byte  data_write[256];
byte  cfg_array[CFG_SIZE-1];

// MPU9250 registers
const uint8_t ACCEL_OUT = 0x3B;
const uint8_t GYRO_OUT  = 0x43;
const uint8_t TEMP_OUT  = 0x41;

// Initial time
bool flag_acquisition = false;

bool flag_send_sec = false;
 
uint8_t ui8_ID_number;
uint8_t ui8_type;
uint16_t ui16_Time_aquisition;
uint8_t ui8_Time_transmition;

// Buffer de aquisição de dados
uint8_t ui8_Buf[14];

//indexador do buffer de envio
uint8_t ui8_index_buffer_send = 0;

// variável para enviar buffer de configuração inicialmente
bool flag_send_start = true;

//elementos para separar os 2bytes do acelerometro e gyroscopio
uint8_t ui8_most_significant = 0;
uint8_t ui8_less_significant = 0;

//buffer de envio criado para enviar as informações
uint8_t ui8_send_buffer[250];

//variáveis para definição da quantidade de informação
uint8_t ui8_size_data = 0;
uint8_t ui8_packet_length = 0;

tmElements_t tm;
uint8_t ui8_error = 0;
