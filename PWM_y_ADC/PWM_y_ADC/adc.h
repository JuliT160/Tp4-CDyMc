/*
 * adc.h
 *
 * Created: 29/7/2025 11:28:10
 *  Author: Santiago
 */ 

#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <stdint.h>

/**
 * @brief Inicializa el ADC con voltaje de referencia AVcc (5V) 
 *        y preescaler de 128 (125kHz @ 16MHz)
 */
void ADC_init(void);

/**
 * @brief Lee un valor analógico del canal especificado
 * @param channel Canal ADC (0-5 para ATmega328P)
 * @return Valor digital de 10 bits (0-1023)
 */
uint16_t ADC_read(uint8_t channel);

/**
 * @brief Inicia la conversion
 */
void ADC_start(uint8_t channel);

#endif /* ADC_H_ */
