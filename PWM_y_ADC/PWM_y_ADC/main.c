/*
 * PWM_y_ADC.c
 *
 * Created: 29/7/2025 11:27:10
 * Author : Santiago
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "pwm.h"
#include "uart.h"

volatile char seleccion = '1';   // Default: rojo
volatile uint8_t nueva_opcion = 0;

//Prototipos de funciones
void mostrarMenu(void);

int main(void)
{
	PWM0_init();
	ADC_init();
	UART_Init();
	ADC_start(3);                 // Primera conversi�n

	sei();
	
	UART_EnviarString("\r\nSistema de Control por Colores - 8 Opciones\r\n");
	mostrarMenu();

	while(1)
	{
		if (nueva_opcion) {
			UART_EnviarString("\r\nOpci�n seleccionada: ");
			UART_EnviarString(seleccion);
			nueva_opcion = 0;
			mostrarMenu();
		}
	}
}

void mostrarMenu(void) {
	UART_EnviarString("\r\n=== MENU DE COLORES ===");
	UART_EnviarString("\r\n1. Rojo");
	UART_EnviarString("\r\n2. Verde");
	UART_EnviarString("\r\n3. Azul");
	UART_EnviarString("\r\n4. Cian");
	UART_EnviarString("\r\n5. Amarillo");
	UART_EnviarString("\r\n6. Magenta");
	UART_EnviarString("\r\n7. Blanco");
	UART_EnviarString("\r\n8. Negro\r\n");
}

ISR(USART_RX_vect)
{
	seleccion = UDR0;   // Guardar �ltima tecla recibida
	nueva_opcion = 1;   // Avisar al main que hay nueva opci�n
}

ISR(ADC_vect)
{
	uint16_t valor_adc = ADC;
	uint8_t brillo = (valor_adc * 100ul) / 1023;  // Escalar a porcentaje (0�100) (0-1)

	uint8_t r = 0, g = 0, b = 0;

	switch(seleccion)
	{
		case '1': r = 255; g = 0; b = 0; break;                          // Rojo
		case '2': r = 0; g = 255; b = 0; break;                          // Verde
		case '3': r = 0; g = 0; b = 255; break;                          // Azul
		case '4': r = 0; g = 255; b = 255; break;                        // Cian
		case '5': r = 255; g = 255; b = 0; break;                        // Amarillo
		case '6': r = 255; g = 0; b = 255; break;                        // Magenta
		case '7': r = 255; g = 255; b = 255; break;                      // Blanco
		case '8': default: r = 0; g = 0; b = 0; break;                   // Negro
	}

	// Aplicar brillo (0�100%) al color base
	uint8_t pwm_r = (r * brillo) / 255;
	uint8_t pwm_g = (g * brillo) / 255;
	uint8_t pwm_b = (b * brillo) / 255;
	//uint8_t pwm_r = (r * brillo);
	//uint8_t pwm_g = (g * brillo);
	//uint8_t pwm_b = (b * brillo);

	PWM0_dca(pwm_r, NO_INVERTING);   // Rojo ? PB1
	PWM0_dcb(pwm_g, NO_INVERTING);   // Verde ? PB2
	PWM0_dcsw(pwm_b);                // Azul ? PB5 (PWM software)

	ADC_start(3);  // Iniciar pr�xima conversi�n
}
