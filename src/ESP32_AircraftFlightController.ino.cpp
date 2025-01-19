# 1 "C:\\Users\\feder\\AppData\\Local\\Temp\\tmpauryxxx6"
#include <Arduino.h>
# 1 "C:/Users/feder/Documents/GitHub_Repositories/ESP32_FlightController/src/ESP32_AircraftFlightController.ino"
#include <Arduino.h>
#include "Aircraft.h"
#include "SystemController.h"
#include "FlightController.h"
#include "Logger.h"
#include "WiFiManager.h"


const char *ssid = "TIM-19028281";
const char *password = "casa12345";

Aircraft *aircraft = nullptr;
FlightController *flightController = nullptr;

SystemController controller = SystemController();

static unsigned long tPrev = 0;
static const unsigned long loopInterval = 10;
void setup();
void loop();
#line 20 "C:/Users/feder/Documents/GitHub_Repositories/ESP32_FlightController/src/ESP32_AircraftFlightController.ino"
void setup()
{

    Serial.begin(115200);

    WiFiManager::getInstance().begin(ssid, password);
    WiFiManager::getInstance().startServerCheckTask();
    delay(1000);

    aircraft = new Aircraft();
    flightController = new FlightController(aircraft->receiver_data, aircraft->imu_data, aircraft->output);

    Logger::getInstance().log(LogLevel::INFO, "Setup complete.");
}

void loop()
{
    unsigned long t = millis();

    if (t - tPrev >= loopInterval)
    {
        double dt = (t - tPrev) / 1000.0;
        tPrev = t;

        aircraft->read_imu(controller.error);
        aircraft->read_receiver(controller.error);
        controller.update_state(aircraft->receiver_data);
        controller.update_modes(aircraft->receiver_data, aircraft->imu.isSetupComplete);
        controller.check_errors();
        flightController->compute_data(dt, aircraft->receiver_data, aircraft->imu_data, aircraft->output, controller.assist_mode, controller.state, controller.error, controller.controller_mode);
        flightController->control(dt, aircraft->imu_data, aircraft->receiver_data, aircraft->output, controller.assist_mode, controller.state, controller.calibration_target);
        controller.set_output(aircraft->output, aircraft->receiver_data, aircraft->imu.isSetupComplete);
        aircraft->update_leds(controller.assist_mode, controller.state);

    }
}