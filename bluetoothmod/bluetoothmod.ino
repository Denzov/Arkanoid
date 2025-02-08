//void uart_init(uint16_t baud_rate){
//  uint16_t ubrr_value = F_CPU / 16 / baud_rate - 1;
//  UBRR0H = (ubrr_value >> 8);
//  UBRR0L = (ubrr_value & 0xFF);
//
//  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
//
//  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
//}
//
//void uart_send_byte(uint8_t data) {
//  while (!(UCSR0A & (1 << UDRE0)));
//  UDR0 = data;
//}
//
//uint8_t uart_receive_byte() {
//  while (!(UCSR0A & (1 << RXC0)));
//  return UDR0;
//}

#define DATA_BYTES_VALUE 8
#define PRE_BYTES_VALUE 2
#define ID_BYTES_VALUE 1
#define PACKET_BYTES_VALUE (PRE_BYTES_VALUE + DATA_BYTES_VALUE + ID_BYTES_VALUE)

void setup() {
   Serial.begin(115200);
  Serial1.begin(115200);
}

uint8_t pac[PACKET_BYTES_VALUE];

void loop() {
  if(Serial1.available()) {
    Serial.println(Serial1.read());


  }


}
