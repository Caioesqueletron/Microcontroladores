// bloco para definir os bits do display LCD
sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D4 at RB0_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;

sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;

// LED para marcar uma medida
sbit LED at PORTB.B7;

void main(){
     char medidaStr[15]; // armazena a medida no formato string
     char aux[5]; // armazena a medida no formato string formatado como se pede
     int i; // variavel do for
     float medida; // armazena o valor de tensão medido
     
     ADCON0 = 0b00000001; // chanel 0 (AN0), halita o conversor A/D
     ADCON1 = 0b00111011; // VRF- no AN2 e VRF+ no AN3 e AN0:AN3 são analógicas
     ADCON2 = 0b10010100; // justificado a direita, 4 TAD e FOSC/4
     
     Delay_us(40);
     
     ADCON0.B1 = 1; // inicia uma conversão
     
     TRISA = 0b00001111; // as portas RA 0, 1, 2 e 3 são saída, as demais entrada
     TRISB = 0b00000000; // todas portas são saída

     // este bloco é resposável por iniar e exibir a mensagem e tensão no LCD
     Lcd_Init();
     Lcd_Cmd(_LCD_CLEAR);
     Lcd_Cmd(_LCD_CURSOR_OFF);
     Lcd_Out(1,5,"TENSAO");

     while(1){
           // verifica se terminou de contar
           if(!ADCON0.B1){
                  // pega o valor contado e calculá-se o valor de tensão
                  medida = (float) ADRES * (5.0 / 1023.0);
                  
                  // este bloco formata a saida como se pede (0.000)
                  if(medida < 1.0) {
                        medida = medida * 1000.0;
                        FloatToStr(medida, medidaStr);
                        aux[0] =  '0';
                        aux[1] =  '.';
                        aux[2] =  medidaStr[0];
                        aux[3] =  medidaStr[1];
                        aux[4] =  medidaStr[2];
                  } else if(medida > 1.0){
                        FloatToStr(medida, medidaStr);

                        for(i = 0; i < 5; i++){
                              aux[i] =  medidaStr[i];
                        }
                  }
                  if(medida < 0.1){
                        aux[0] = '0';
                        aux[1] = '.';
                        aux[2] = '0';
                        aux[3] = '0';
                        aux[4] = '0';
                  }

                  // exibe o valor de tensão no LCD
                  Lcd_Out(2, 4, aux);
                  Lcd_Out(2, 11, "V");

                  ADCON0 = 0b00000001; // chanel 0 (AN0), halita o conversor A/D
                  ADCON1 = 0b00111011; // VRF- no AN2 e VRF+ no AN3 e AN0:AN3 são analógicas
                  ADCON2 = 0b10010100; // justificado a direita, 4 TAD e FOSC/4

                  Delay_us(500);

                  // acende ou apaga o LED
                  if(LED){
                        LED = 0;
                  } else {
                        LED = 1;
                  }
                  
                  ADCON0.B1 = 1; // inicia uma conversão
           }
     }
}