#include <memory>
#include <esp32_player.hpp>
#include <player/pcm_device.hpp>
#include <freertos/task.h>
#include <chrono>

void task_function(void *pv){
    Esp32Player *p = (Esp32Player *)pv;
    p->worker();
}

Esp32Player::Esp32Player(PcmDevice pcmDevice, std::shared_ptr<Stream> stream):
            Player(pcmDevice, stream)

{

}

void Esp32Player::start() {
    xTaskCreate(task_function, "player", 8192*2, this, 5, &worker_task);
}

void Esp32Player::worker(void){
    LOG(INFO) << "EspPlayer worker started";
    while(1){
        char buf[8192];
        chronos::usec delay(5);
        //stream_->getPlayerChunk((void *)buf, delay, 200UL);
        stream_->clearChunks();
        vTaskDelay(10/portTICK_RATE_MS);
    }
}