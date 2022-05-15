/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <cstdlib>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/pwm.h"

/// \tag::hello_uart[]

#define UART_ID uart0
#define BAUD_RATE 115200

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

enum class MessageCodes : uint8_t{
    MSP_IDENT = 100,
    MSP_STATUS,
    MSP_RAW_IMU,
    MSP_SERVO,
    MSP_MOTOR,
    MSP_RC,
    MSP_RAW_GPS,
    MSP_COMP_GPS,
    MSP_ATTITUDE,
    MSP_ALTITUDE,
    MSP_BAT,
    MSP_RC_TUNING,
    MSP_PID,
    MSP_BOX,
    MSP_MISC,
    MSP_MOTOR_PINS,
    MSP_BOXNAMES,
    MSP_PIDNAMES,
    MSP_WP,
    MSP_DEBUG
};

void msp_request(MessageCodes code, uint8_t * data, uint8_t length)
{
    //preamble
    uart_puts(UART_ID, "$M<");
    //data length
    uart_putc(UART_ID, length);
    //code
    uart_putc(UART_ID, (uint8_t)code);
    //data
    uart_puts(UART_ID, (char*)data);

    char crc = length ^ (uint8_t)code;
    for (int i = 0; i < length; ++i)
    {
        crc = crc ^ data[i];
    }
    //checksom
    uart_putc(UART_ID, crc);
}

bool msp_response(uint8_t *& data, uint8_t & length) {

    uint8_t start[5] = {0};
    uart_read_blocking(UART_ID,start, 5);

    length = start[3];
    uint8_t code = start[4];

    data = (uint8_t*)calloc(1, length + 1);
    uart_read_blocking(UART_ID, data, length + 1);
    uint8_t crc = data[length];

    crc = crc ^ length ^ code;
    for (int i = 0; i < length; ++i)
    {
        crc = crc ^ data[i];
    }

    return crc == 0;
}

void init_servo()
{
    // Tell GPIO 0 and 1 they are allocated to the PWM
    gpio_set_function(2, GPIO_FUNC_PWM);
    gpio_set_function(3, GPIO_FUNC_PWM);
    // Find out which PWM slice is connected to GPIO 2 (it's slice 1)
    uint slice_num = pwm_gpio_to_slice_num(2);

    // Get some sensible defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range (0 to 2**16-1)
    pwm_config config = pwm_get_default_config();
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 250.f);
    pwm_config_set_wrap(&config, 10000);

    // Load the configuration into our PWM slice, and set it running.
    pwm_init(slice_num, &config, true);
}
void set_servo_angle(int angle, uint channel)
{
    angle = angle % 360;
    uint slice_num = pwm_gpio_to_slice_num(2);
    pwm_set_chan_level(slice_num, channel, 750 + 8000/360 * angle);
}
int main() {
    stdio_init_all();
    sleep_ms(5000);
    printf("starting");

    init_servo();

    int angle = 0;
    while(true)
    {
        set_servo_angle(angle, PWM_CHAN_A);
        set_servo_angle(angle, PWM_CHAN_B);
        angle += 1;
        sleep_ms(10);
    }

    printf("finished");
//
//    // Set up our UART with the required speed.
//    uart_init(UART_ID, BAUD_RATE);
//
//    // Set the TX and RX pins by using the function select on the GPIO
//    // Set datasheet for more information on function select
//    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
//    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
//
//    printf("press enter");
//    char line[5];
//    scanf("%s", line);
//
//    uint8_t * data;
//    msp_request(MessageCodes::MSP_SERVO, data, 0);
//
//    uint8_t length = 0;
//    if (!msp_response(data, length))
//    {
//        printf("crc didn't checkout");
//    }
//
//    for (int i = 0; i < length; ++i)
//    {
//        printf("%d ",data[i]);
//    }
}

/// \end::hello_uart[]
