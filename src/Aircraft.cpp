#include "Aircraft.h"
#include "pins.h"

// Configura la porta seriale per il ricevitore

Aircraft::Aircraft() : esc(ESC_PIN),
                       servo_x(SERVO_PIN_X),
                       servo_y(SERVO_PIN_Y),
                       servo_z(SERVO_PIN_Z),
                       bno055(),
                       receiver(IBUS_RX_PIN),
                       led_red(LED_PIN_RED),
                       led_green(LED_PIN_GREEN),
                       led_rgb(LED_PIN_RGB_RED, LED_PIN_RGB_GREEN, LED_PIN_RGB_BLUE)
{
    // Inizializza i dati del sistema
    imu_data = {0};
    receiver_data = {0};
    output = {0};
    Serial.println("Aircraft initialized");
}

void Aircraft::read_imu(Errors &error)
{
    // Legge i dati dall'IMU e aggiorna lo stato degli errori
    bool isValid = bno055.read(imu_data);
    if (error.IMU_ERROR != isValid)
        error.IMU_ERROR = isValid;
}

void Aircraft::read_receiver(Errors &error)
{
    // Legge i dati dal ricevitore e aggiorna lo stato degli errori
    bool isValid = receiver.read(receiver_data);
    if (error.RECEIVER_ERROR != isValid)
        error.RECEIVER_ERROR = isValid;
}

void Aircraft::update_leds(ASSIST_MODE assist_mode, CONTROLLER_STATE state, Errors error)
{
    // Aggiorna lo stato dei LED in base al sistema
    switch (state)
    {
    case CONTROLLER_STATE::ARMED:
        led_green.set_state(BLINK_ON, BLINK_OFF / 5);
        led_red.set_state(LED_STATE::OFF);
        switch (assist_mode)
        {
        case ASSIST_MODE::MANUAL:
            led_rgb.set_state(LED_STATE::ON, COLOR::WHITE);
            break;
        case ASSIST_MODE::GYRO_STABILIZED:
            led_rgb.set_state(LED_STATE::ON, COLOR::LIGHT_BLUE);
            break;
        case ASSIST_MODE::ATTITUDE_CONTROL:
            led_rgb.set_state(LED_STATE::ON, COLOR::PURPLE);
            break;
        }
        break;
    case CONTROLLER_STATE::DISARMED:
        led_green.set_state(BLINK_ON / 5, BLINK_OFF);
        led_red.set_state(LED_STATE::OFF);
        led_rgb.set_state(LED_STATE::OFF);
        break;
    case CONTROLLER_STATE::FAILSAFE:
        led_green.set_state(LED_STATE::OFF);
        led_red.set_state(BLINK_ON / 5, BLINK_OFF / 3);
        if (error.RECEIVER_ERROR)
            led_rgb.set_state(BLINK_ON / 5, BLINK_OFF / 5, COLOR::WHITE);
        if (error.IMU_ERROR)
            led_rgb.set_state(BLINK_ON / 5, BLINK_OFF / 5, COLOR::LIGHT_BLUE);
        if (error.DATA_ERROR)
            led_rgb.set_state(BLINK_ON / 5, BLINK_OFF / 5, COLOR::PURPLE);
        break;
    }
}

void Aircraft::write_actuators()
{
    // Scrive i valori sugli attuatori
    servo_x.write(output.x);
    servo_y.write(output.y);
    servo_z.write(output.z);
    esc.write(output.throttle);
}
