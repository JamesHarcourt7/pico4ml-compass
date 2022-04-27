#include <stdio.h>

#include "pico/stdlib.h"
#include "lib/st7735.h"
#include "lib/ICM20948.h"

struct Vec {
    float x;
    float y;
    float z;
};

// Thanks plaaosert for the ST7735 and IMU guide :D

void SetupIMU() {
    // Apparently this is important ¯\_(ツ)_/¯
    i2c_init(i2c0, 400 * 1000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);
    gpio_pull_up(5);
    sleep_ms(1000);

    IMU_EN_SENSOR_TYPE enMotionSensorType;

    imuInit(&enMotionSensorType);
    if (IMU_EN_SENSOR_TYPE_ICM20948 != enMotionSensorType) {
        printf("Failed to initialise IMU...");
    }

    printf("IMU initialised!");
}

int main() {
    stdio_init_all();  // Initialise serial in/output
    setvbuf ( stdout , NULL , _IONBF , 0 );  // Disable line and block buffering on stdout (for serial comm.)
    sleep_ms(1000);

    ST7735_Init();
    ST7735_FillScreen(ST7735_BLACK);

    SetupIMU();

    struct Vec mag;
    // For calibration:
    struct Vec magmin;
    struct Vec magmax;

    int centre_x = ST7735_WIDTH / 2;
    int centre_y = ST7735_HEIGHT / 2;
    int point_x;
    int point_y;
    bool negative;
    float magnitude;
    int current;
    float heading;
    char arrayH[10];

    icm20948MagRead(&magmin.x, &magmin.y, &magmin.z);
    icm20948MagRead(&magmax.x, &magmax.y, &magmin.z);

    while (true) {
        // Read magnetometer values
        icm20948MagRead(&mag.x, &mag.y, &mag.z);

        // Calibrate values
        if (mag.x < magmin.x) magmin.x = mag.x;
        if (mag.x > magmax.x) magmax.x = mag.x;
        if (mag.y < magmin.y) magmin.y = mag.y;
        if (mag.y > magmax.y) magmax.y = mag.y;
        if (mag.z < magmin.z) magmin.z = mag.z;
        if (mag.z > magmax.z) magmax.z = mag.z;

        // Calibrate by removing offset of smallest seen value
        mag.x -= magmin.x;
        mag.y -= magmin.y;
        mag.z -= magmin.z;

        // Scale value based of highest values seen
        // Creates values between 0 and 1

        // Avoid Zero Division
        if (magmax.x - magmin.x > 0.001) mag.x /= magmax.x - magmin.x;
        if (magmax.y - magmin.y > 0.001) mag.y /= magmax.y - magmin.y;
        if (magmax.z - magmin.z > 0.001) mag.z /= magmax.z - magmin.z;

        // Shift values to -0.5 <= x <= 0.5
        mag.x -= 0.5;
        mag.y -= 0.5;
        mag.z -= 0.5;

        // Convert Gauss values to Radian heading
        heading = atan2(mag.x, mag.y);
        // Convert to positive heading (add a full radian turn)
        if (heading < 0) heading += 2 * 3.14159;

        // Worked out using counterclockwise rotation matrix
        point_x = centre_x + (20 * sin(heading));
        point_y = centre_y + ((-20) * cos(heading));

        // Draw compass needle
        ST7735_FillScreen(ST7735_BLACK);

        ST7735_DrawPixel(centre_x, centre_y, ST7735_RED);
        ST7735_DrawPixel(point_x, point_y, ST7735_RED);

        // Convert heading to degrees
        heading *= 180 / 3.14159;

        if (heading > 0) {
            sprintf(arrayH, "%.2f", heading);
        } else {
            sprintf(arrayH, "%.1f", heading);
        }
        ST7735_WriteString(80, 80, arrayH, Font_16x26, ST7735_RED, ST7735_BLACK);

        sleep_ms(100);
    }
}
