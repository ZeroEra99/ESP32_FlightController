#include "Aircraft.h"
#include "pins.h"
#include "Logger.h"

// Costruttore della classe Aircraft
Aircraft::Aircraft() : esc(ESC_PIN),
                       servo_x(SERVO_PIN_X),
                       servo_y(SERVO_PIN_Y),
                       servo_z(SERVO_PIN_Z),
                       imu(),
                       receiver(IBUS_RX_PIN),
                       led_red(LED_PIN_RED),
                       led_green(LED_PIN_GREEN),
                       led_rgb(LED_PIN_RGB_RED, LED_PIN_RGB_GREEN, LED_PIN_RGB_BLUE)

{
    // Inizializza i dati del sistema
    receiver_data = {0};
    Logger::getInstance().log(LogLevel::INFO, "Aircraft setup complete.");
    led_green.set_state(LED_STATE::ON);
    led_red.set_state(BLINK_ON, BLINK_OFF);
    led_rgb.set_state(BLINK_ON, BLINK_OFF, COLOR::PURPLE);
}

void Aircraft::read_imu(Errors &error)
{
    if (!imu.isSetupComplete)
        return;

    bool imu_error = !imu.read(imu_data);

    if (error.IMU_ERROR != imu_error)
        error.IMU_ERROR = imu_error;

    if (!imu_error)
        imu.logData(imu_data);
}

void Aircraft::read_receiver(Errors &error)
{
    // Legge i dati dal ricevitore e rileva eventuali errori
    bool receiver_error = !receiver.read(receiver_data);

    if (error.RECEIVER_ERROR != receiver_error)
        error.RECEIVER_ERROR = receiver_error;

    // Logga i dati ricevuti dal ricevitore
    if (!receiver_error)
        receiver.logData(receiver_data);
}

void Aircraft::update_leds(ASSIST_MODE assist_mode, CONTROLLER_STATE state)
{
    // Aggiorna il colore del LED RGB in base alla modalità di assistenza
    switch (assist_mode)
    {
    case ASSIST_MODE::MANUAL:
        led_rgb.set_state(LED_STATE::ON, COLOR::RED); // Imposta il colore rosso
        break;
    case ASSIST_MODE::GYRO_STABILIZED:
        led_rgb.set_state(LED_STATE::ON, COLOR::BLUE); // Imposta il colore blu
        break;
    case ASSIST_MODE::ATTITUDE_CONTROL:
        led_rgb.set_state(LED_STATE::ON, COLOR::PURPLE); // Imposta il colore viola
        break;
    }

    // Aggiorna lo stato dei LED in base allo stato del controller
    switch (state)
    {
    case CONTROLLER_STATE::ARMED:
        led_green.set_state(BLINK_ON, BLINK_OFF / 5);    // Lampeggia velocemente
        led_red.set_state(LED_STATE::OFF);               // Spento
        led_rgb.set_state(LED_STATE::ON, led_rgb.color); // Acceso con il colore corrente
        break;
    case CONTROLLER_STATE::DISARMED:
        led_green.set_state(BLINK_ON / 5, BLINK_OFF); // Lampeggia lentamente
        led_red.set_state(LED_STATE::OFF);            // Spento
        led_rgb.set_state(LED_STATE::OFF);            // Spento
        break;
    case CONTROLLER_STATE::FAILSAFE:
        led_green.set_state(LED_STATE::OFF);                                   // Spento
        led_red.set_state(BLINK_ON / 5, BLINK_OFF / 5);                        // Lampeggia lentamente
        led_rgb.set_state(led_rgb.blink_on, led_rgb.blink_off, led_rgb.color); // Lampeggia con il colore corrente
        break;
    }

    // Aggiorna lo stato dei LED
    led_red.update();
    led_green.update();
    led_rgb.update();
}

void Aircraft::write_actuators()
{
    // Scrive i valori sugli attuatori
    servo_x.write(output.x);
    servo_y.write(output.y);
    servo_z.write(output.z);
    esc.write(output.throttle);
}