#include <stdio.h>
#include "driver/i2s_pdm.h"

#define DATA_PIN 2
#define CLK_PIN 47
#define SAMPLE_SIZE 1024

void app_main(void)
{
    i2s_chan_handle_t rx_handle = NULL;
    static int16_t buf[SAMPLE_SIZE];
    size_t bytes_read;

    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, NULL, &rx_handle));

    i2s_pdm_rx_config_t pdm_rx_cfg = {
        .clk_cfg = I2S_PDM_RX_CLK_DEFAULT_CONFIG(44100),
        .slot_cfg = I2S_PDM_RX_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
        .gpio_cfg = {
            .clk = CLK_PIN,
            .din = DATA_PIN,
            .invert_flags = {
                .clk_inv = false,
            },
        },
    };

    printf("Initializing...\n");
    ESP_ERROR_CHECK(i2s_channel_init_pdm_rx_mode(rx_handle, &pdm_rx_cfg));
    ESP_ERROR_CHECK(i2s_channel_enable(rx_handle));

    printf("Listening...\n");
    char sound_bar[80] = "--------------------------------------------------\r\0";

    while (1)
    {
        if (i2s_channel_read(rx_handle, (char *)buf, SAMPLE_SIZE, &bytes_read, 1000) == ESP_OK)
        {
            uint16_t max_val = 0;
            for (uint32_t i = 0; i < bytes_read; i++)
            {
                uint16_t val = abs(buf[i]);
                if (val > max_val)
                {
                    max_val = val;
                }
            }

            uint16_t n = max_val / 10;
            for (int i = 0; i < 50; i++)
            {
                sound_bar[i] = n > i ? '#' : '-';
            }

            printf(sound_bar);
        }
        else
        {
            printf("Read Failed!\n");
        }
    }
}
