//
// Created by 823984418@qq.com on 2021/8/8.
//

#ifndef ADS1256_H
#define ADS1256_H

#include <stdint.h>
#include <stddef.h>

typedef int_fast8_t bool_t;

#define FALSE ((bool_t) 0)
#define TRUE ((bool_t) 1)

#ifdef __cplusplus
extern "C" {
#endif

#define ADS1256_STATUS          0x00
#define ADS1256_MUX             0x01
#define ADS1256_ADCON           0x02
#define ADS1256_DRATE           0x03
#define ADS1256_IO              0x04
#define ADS1256_OFC0            0x05
#define ADS1256_OFC1            0x06
#define ADS1256_OFC2            0x07
#define ADS1256_FSC0            0x08
#define ADS1256_FSC1            0x09
#define ADS1256_FSC2            0x0A


#define ADS1256_STATUS_ID       0xF0
#define ADS1256_STATUS_ACAL     0x04
#define ADS1256_STATUS_BUFEN    0x02
#define ADS1256_STATUS_DRDY     0x01

#define ADS1256_MUX_PSEL        0xF0
#define ADS1256_MUX_NSEL        0x0F

#define ADS1256_MUX_PSEL_AIN0   0x00
#define ADS1256_MUX_PSEL_AIN1   0x10
#define ADS1256_MUX_PSEL_AIN2   0x20
#define ADS1256_MUX_PSEL_AIN3   0x30
#define ADS1256_MUX_PSEL_AIN4   0x40
#define ADS1256_MUX_PSEL_AIN5   0x50
#define ADS1256_MUX_PSEL_AIN6   0x60
#define ADS1256_MUX_PSEL_AIN7   0x70
#define ADS1256_MUX_PSEL_AINCOM 0x80

#define ADS1256_MUX_NSEL_AIN0   0x00
#define ADS1256_MUX_NSEL_AIN1   0x01
#define ADS1256_MUX_NSEL_AIN2   0x02
#define ADS1256_MUX_NSEL_AIN3   0x03
#define ADS1256_MUX_NSEL_AIN4   0x04
#define ADS1256_MUX_NSEL_AIN5   0x05
#define ADS1256_MUX_NSEL_AIN6   0x06
#define ADS1256_MUX_NSEL_AIN7   0x07
#define ADS1256_MUX_NSEL_AINCOM 0x08

#define ADS1256_ADCON_CLK       0x60
#define ADS1256_ADCON_SDCS      0x18
#define ADS1256_ADCON_PGA       0x07

#define ADS1256_ADCON_CLK_OFF   0x00
#define ADS1256_ADCON_CLK_DIV_1 0x20
#define ADS1256_ADCON_CLK_DIV_2 0x40
#define ADS1256_ADCON_CLK_DIV_4 0x60

#define ADS1256_ADCON_SDCS_OFF  0x00
#define ADS1256_ADCON_SDCS_0_5  0x08
#define ADS1256_ADCON_SDCS_2    0x10
#define ADS1256_ADCON_SDCS_10   0x18

#define ADS1256_ADCON_PGA_1     0x00
#define ADS1256_ADCON_PGA_2     0x01
#define ADS1256_ADCON_PGA_4     0x02
#define ADS1256_ADCON_PGA_8     0x03
#define ADS1256_ADCON_PGA_16    0x04
#define ADS1256_ADCON_PGA_32    0x05
#define ADS1256_ADCON_PGA_64    0x06

#define ADS1256_DRATE_30K       0xF0
#define ADS1256_DRATE_15K       0xE0
#define ADS1256_DRATE_7K500     0xD0
#define ADS1256_DRATE_3K750     0xC0
#define ADS1256_DRATE_2K        0xB0
#define ADS1256_DRATE_1K        0xA1
#define ADS1256_DRATE_500       0x92
#define ADS1256_DRATE_100       0x82
#define ADS1256_DRATE_60        0x72
#define ADS1256_DRATE_50        0x63
#define ADS1256_DRATE_30        0x53
#define ADS1256_DRATE_25        0x43
#define ADS1256_DRATE_15        0x33
#define ADS1256_DRATE_10        0x23
#define ADS1256_DRATE_5         0x13
#define ADS1256_DRATE_2_5       0x03

#define ADS1256_IO_DIR3         0x80
#define ADS1256_IO_DIR2         0x40
#define ADS1256_IO_DIR1         0x20
#define ADS1256_IO_DIR0         0x10
#define ADS1256_IO_DI0          0x0F

/// 一个ADS1256设备
typedef struct {
    /// 以3线SPI与ADS1256通信，同时发送和接受
    ///
    /// \param buffer   缓冲
    /// \param len      字节数
    void (*swap)(uint8_t *buffer, size_t len);

    /// 等待DRDY线拉低
    void (*wait)(void);

    /// 更改片选线 (中断 禁用/启用).
    /// (配置引脚 功能/方向)
    /// \param e    是否选中
    void (*select)(bool_t e);
} ADS1256;

/// 配置ADS1256寄存器为特定值
/// 开启自动校准
/// 测量 A0 - COM
/// 采样率1K
///
/// \param ads1256  设备
void ADS1256_Init(ADS1256 *ads1256);

/// 从设备中读取 24 位数据
/// 读取的结果存放为 0xAABBCC00
/// 可以除以0x7FFFFF00并乘以量程得到实际值
///
/// \param ads1256  设备
int32_t ADS1256_Read24i(ADS1256 *ads1256);

/// 发送 WAKEUP 指令
///
/// \param ads1256  设备
void ADS1256_WAKEUP(ADS1256 *ads1256);

/// 发送 RDATA 指令
///
/// \param ads1256  设备
void ADS1256_RDATA(ADS1256 *ads1256);

/// 发送 RDATAC 指令
///
/// \param ads1256  设备
void ADS1256_RDATAC(ADS1256 *ads1256);

/// 发送 SDATAC 指令
///
/// \param ads1256  设备
void ADS1256_SDATAC(ADS1256 *ads1256);

/// 发送 RREG 指令
///
/// \param ads1256  设备
/// \param reg      寄存器首地址
/// \param count    寄存器数
void ADS1256_RREG(ADS1256 *ads1256, uint8_t reg, uint8_t count);

/// 发送 WREG 指令
///
/// \param ads1256 设备
/// \param reg      寄存器首地址
/// \param count    寄存器数
void ADS1256_WREG(ADS1256 *ads1256, uint8_t reg, uint8_t count);

/// 发送 SELFCAL 指令
///
/// \param ads1256  设备
void ADS1256_SELFCAL(ADS1256 *ads1256);

/// 发送 SELFOCAL 指令
///
/// \param ads1256  设备
void ADS1256_SELFOCAL(ADS1256 *ads1256);

/// 发送 SELFGCAL 指令
///
/// \param ads1256  设备
void ADS1256_SELFGCAL(ADS1256 *ads1256);

/// 发送 SYSOCAL 指令
///
/// \param ads1256  设备
void ADS1256_SYSOCAL(ADS1256 *ads1256);

/// 发送 SYSGCAL 指令
///
/// \param ads1256  设备
void ADS1256_SYSGCAL(ADS1256 *ads1256);

/// 发送 SYNC 指令
///
/// \param ads1256  设备
void ADS1256_SYNC(ADS1256 *ads1256);

/// 发送 STANDBY 指令
///
/// \param ads1256  设备
void ADS1256_STANDBY(ADS1256 *ads1256);

/// 发送 RESET 指令
///
/// \param ads1256  设备
void ADS1256_RESET(ADS1256 *ads1256);

/// 等待 DRDY 后启用片选并读取
/// 这通常与 RDATAC 结合使用
///
/// \param ads1256  设备
/// \return 读取的 0xAABBCC00 结果
int32_t ADS1256_Wait_CS_Read(ADS1256 *ads1256);

/// 执行一次指定MUX和ADCON的单次转换，其他寄存器使用历史值
///
/// \param ads1256  设备
/// \param MUX      MUX 寄存器值
/// \param ADCON    ADCON 寄存器值
/// \return 读取的 0xAABBCC00 结果
int32_t ADS1256_Shot_MUX_ADCON(ADS1256 *ads1256, uint8_t MUX, uint8_t ADCON);

/// 执行一次指定MUX的单次转换，其他寄存器使用历史值
///
/// \param ads1256  设备
/// \param MUX      MUX 寄存器值
/// \return 读取的 0xAABBCC00 结果
int32_t ADS1256_Shot_MUX(ADS1256 *ads1256, uint8_t MUX);

/// 执行一次单次转换，寄存器使用历史值
///
/// \param ads1256  设备
/// \return 读取的 0xAABBCC00 结果
int32_t ADS1256_Shot(ADS1256 *ads1256);

/// 执行一次指定MUX和ADCON的多通道依次转换，其他寄存器使用历史值
///
/// \param ads1256  设备
/// \param MUXs     MUX 值数组
/// \param ADCONs   ADCON 值数组
/// \param results  接受 0xAABBCC00 结果的数组
/// \param count    数量
void ADS1256_Mul_MUX_ADCON(ADS1256 *ads1256, const uint8_t *MUXs, const uint8_t *ADCONs,
                           int32_t *results, size_t count);

/// 执行一次指定MUX的多通道依次转换，其他寄存器使用历史值
///
/// \param ads1256  设备
/// \param MUXs     MUX 值数组
/// \param results  接受 0xAABBCC00 结果的数组
/// \param count    数量
void ADS1256_Mul_MUX(ADS1256 *ads1256, const uint8_t *MUXs, int32_t *results, size_t count);

#ifdef __cplusplus
}
#endif

#endif //ADS1256_H
