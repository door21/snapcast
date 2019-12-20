#ifndef ESP32_PLAYER_H
#define ESP32_PLAYER_H
#include <player/player.hpp>
#include <player/pcm_device.hpp>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
class Esp32Player:public Player{
public:
Esp32Player(PcmDevice pcmDevice, std::shared_ptr<Stream> stream);
void start();
void worker(void);

private:
TaskHandle_t worker_task;

};
#endif