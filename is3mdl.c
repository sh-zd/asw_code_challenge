// LIS3MDL Magnetometer Driver
// Author: Chayma Zaghdoudi


#include "i2c.h"

#define LIS3MDL_ADDR 0x1E

// Register addresses
#define LIS3MDL_REG_CTRL_REG1 0x20
#define LIS3MDL_REG_CTRL_REG2 0x21
#define LIS3MDL_REG_INT_CFG   0x30
#define LIS3MDL_REG_OUT_X_L 0x28


// Full-scale configuration
typedef enum {
    LIS3MDL_FS_4GAUSS,
    LIS3MDL_FS_8GAUSS,
    LIS3MDL_FS_12GAUSS,
    LIS3MDL_FS_16GAUSS
} LIS3MDL_FullScale;

// Output data rate
typedef enum {
    LIS3MDL_ODR_0_625_HZ,
    LIS3MDL_ODR_1_25_HZ,
    LIS3MDL_ODR_2_5_HZ,
    LIS3MDL_ODR_5_HZ,
    LIS3MDL_ODR_10_HZ,
    LIS3MDL_ODR_20_HZ,
    LIS3MDL_ODR_40_HZ,
    LIS3MDL_ODR_80_HZ
} LIS3MDL_OutputDataRate;

// Function prototypes
status_t lis3mdl_get_full_scale(LIS3MDL_FullScale *full_scale);
status_t lis3mdl_set_output_data_rate(LIS3MDL_OutputDataRate odr);
status_t lis3mdl_enable_interrupt_pin(void);
status_t lis3mdl_disable_interrupt_pin(void);
status_t lis3mdl_read_output_data(int16_t *x, int16_t *y, int16_t *z);

// Get the full-scale configuration
status_t lis3mdl_get_full_scale(LIS3MDL_FullScale *full_scale) {
    uint8_t reg_value;
    status_t status = i2c_read(LIS3MDL_ADDR, LIS3MDL_REG_CTRL_REG2, 1, &reg_value);
    if (status != STATUS_OK) return status;

    switch (reg_value & 0x60) {
        case 0x00: *full_scale = LIS3MDL_FS_4GAUSS; break;
        case 0x20: *full_scale = LIS3MDL_FS_8GAUSS; break;
        case 0x40: *full_scale = LIS3MDL_FS_12GAUSS; break;
        case 0x60: *full_scale = LIS3MDL_FS_16GAUSS; break;
        default: return STATUS_ERROR;
    }

    return STATUS_OK;
}

// Get and set the output data rate
status_t lis3mdl_set_output_data_rate(LIS3MDL_OutputDataRate odr) {
    uint8_t reg_value;
    status_t status = i2c_read(LIS3MDL_ADDR, LIS3MDL_REG_CTRL_REG1, 1, &reg_value);
    if (status != STATUS_OK) return status;

    reg_value &= ~0xF0; 
    reg_value |= (odr << 2); 

    return i2c_write(LIS3MDL_ADDR, LIS3MDL_REG_CTRL_REG1, 1, &reg_value);
}

// Enable and disable the interrupt pin
status_t lis3mdl_enable_interrupt_pin(void) {
    uint8_t reg_value;
    status_t status = i2c_read(LIS3MDL_ADDR,  LIS3MDL_REG_INT_CFG, 1, &reg_value);
    if (status != STATUS_OK) return status;
    
    reg_value |= 0x01;
    
    status = i2c_write(LIS3MDL_ADDR,  LIS3MDL_REG_INT_CFG, 1, &reg_value);
    return status;
}

status_t lis3mdl_disable_interrupt_pin(void) {
    uint8_t reg_value;
    status_t status = i2c_read(LIS3MDL_ADDR,  LIS3MDL_REG_INT_CFG, 1, &reg_value);
    if (status != STATUS_OK) return status;
    
    reg_value &= ~0x01; 
    
    status = i2c_write(LIS3MDL_ADDR,  LIS3MDL_REG_INT_CFG, 1, &reg_value);
    return status;
}

// Read the output data of a specified axis
status_t lis3mdl_read_output_data(int16_t *x, int16_t *y, int16_t *z) {
    uint8_t buffer[6];
    status_t status = i2c_read(LIS3MDL_ADDR, LIS3MDL_REG_OUT_X_L | 0x80, 6, buffer); 
    if (status != STATUS_OK) return status;

    *x = (int16_t)((buffer[1] << 8) | buffer[0]);
    *y = (int16_t)((buffer[3] << 8) | buffer[2]);
    *z = (int16_t)((buffer[5] << 8) | buffer[4]);

    return STATUS_OK;
}


