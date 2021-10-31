//
// Created by 823984418@qq.com on 2021/8/8.
//

#include "ads1256.h"


#define WAKEUP      0x00
#define RDATA       0x01
#define RDATAC      0x03
#define SDATAC      0x0F
#define RREG        0x10
#define WREG        0x50
#define SELFCAL     0xF0
#define SELFOCAL    0xF1
#define SELFGCAL    0xF2
#define SYSOCAL     0xF3
#define SYSGCAL     0xF4
#define SYNC        0xFC
#define STANDBY     0xFD
#define RESET       0xFE
#define WAKEUP_     0xFF

void ADS1256_Init(ADS1256 *ads1256) {
    ads1256->select(TRUE);
    ADS1256_WREG(ads1256, ADS1256_STATUS, 5);
    uint8_t reg[5] = {
            ADS1256_STATUS_ACAL,
            ADS1256_MUX_PSEL_AIN0 | ADS1256_MUX_NSEL_AINCOM,
            ADS1256_ADCON_CLK_DIV_1,
            ADS1256_DRATE_1K,
            ADS1256_IO_DIR3 | ADS1256_IO_DIR2 | ADS1256_IO_DIR1,
    };
    ads1256->swap(reg, 5);
    ads1256->select(FALSE);
}

int32_t ADS1256_Read24i(ADS1256 *ads1256) {
    uint8_t data[3] = {0, 0, 0};
    ads1256->swap(data, 3);
    return (((uint32_t) data[0] << 24)
            | ((uint32_t) data[1] << 16)
            | ((uint32_t) data[2] << 8));
}

void ADS1256_WAKEUP(ADS1256 *ads1256) {
    uint8_t cmd = WAKEUP;
    ads1256->swap(&cmd, 1);
}

void ADS1256_RDATA(ADS1256 *ads1256) {
    uint8_t cmd = RDATA;
    ads1256->swap(&cmd, 1);
}

void ADS1256_RDATAC(ADS1256 *ads1256) {
    uint8_t cmd = RDATAC;
    ads1256->swap(&cmd, 1);
}

void ADS1256_SDATAC(ADS1256 *ads1256) {
    uint8_t cmd = SDATAC;
    ads1256->swap(&cmd, 1);
}

void ADS1256_RREG(ADS1256 *ads1256, uint8_t reg, uint8_t count) {
    uint8_t cmd[2] = {
            RREG | (reg & 0x0F),
            (count - 1) & 0x0F,
    };
    ads1256->swap(cmd, 2);
}

void ADS1256_WREG(ADS1256 *ads1256, uint8_t reg, uint8_t count) {
    uint8_t cmd[2] = {
            WREG | (reg & 0x0F),
            (count - 1) & 0x0F,
    };
    ads1256->swap(cmd, 2);
}

void ADS1256_SELFCAL(ADS1256 *ads1256) {
    uint8_t cmd = SELFCAL;
    ads1256->swap(&cmd, 1);
}

void ADS1256_SELFOCAL(ADS1256 *ads1256) {
    uint8_t cmd = SELFOCAL;
    ads1256->swap(&cmd, 1);
}

void ADS1256_SELFGCAL(ADS1256 *ads1256) {
    uint8_t cmd = SELFGCAL;
    ads1256->swap(&cmd, 1);
}

void ADS1256_SYSOCAL(ADS1256 *ads1256) {
    uint8_t cmd = SYSOCAL;
    ads1256->swap(&cmd, 1);
}

void ADS1256_SYSGCAL(ADS1256 *ads1256) {
    uint8_t cmd = SYSGCAL;
    ads1256->swap(&cmd, 1);
}

void ADS1256_SYNC(ADS1256 *ads1256) {
    uint8_t cmd = SYNC;
    ads1256->swap(&cmd, 1);
}

void ADS1256_STANDBY(ADS1256 *ads1256) {
    uint8_t cmd = STANDBY;
    ads1256->swap(&cmd, 1);
}

void ADS1256_RESET(ADS1256 *ads1256) {
    uint8_t cmd = RESET;
    ads1256->swap(&cmd, 1);
}

int32_t ADS1256_Wait_CS_Read(ADS1256 *ads1256) {
    ads1256->wait();
    ads1256->select(TRUE);
    int32_t v = ADS1256_Read24i(ads1256);
    ads1256->select(FALSE);
    return v;
}

int32_t ADS1256_Shot_MUX_ADCON(ADS1256 *ads1256, uint8_t MUX, uint8_t ADCON) {
    ads1256->select(TRUE);
    ADS1256_WREG(ads1256, ADS1256_MUX, 2);
    uint8_t data[2] = {MUX, ADCON};
    ads1256->swap(data, 2);
    ADS1256_SYNC(ads1256);
    ADS1256_WAKEUP(ads1256);
    ads1256->wait();
    ADS1256_RDATA(ads1256);
    int32_t v = ADS1256_Read24i(ads1256);
    ADS1256_STANDBY(ads1256);
    ads1256->select(FALSE);
    return v;
}

int32_t ADS1256_Shot_MUX(ADS1256 *ads1256, uint8_t MUX) {
    ads1256->select(TRUE);
    ADS1256_WREG(ads1256, ADS1256_MUX, 1);
    ads1256->swap(&MUX, 1);
    ADS1256_SYNC(ads1256);
    ADS1256_WAKEUP(ads1256);
    ads1256->wait();
    ADS1256_RDATA(ads1256);
    int32_t v = ADS1256_Read24i(ads1256);
    ADS1256_STANDBY(ads1256);
    ads1256->select(FALSE);
    return v;
}

int32_t ADS1256_Shot(ADS1256 *ads1256) {
    ads1256->select(TRUE);
    ADS1256_SYNC(ads1256);
    ADS1256_WAKEUP(ads1256);
    ads1256->wait();
    ADS1256_RDATA(ads1256);
    int32_t v = ADS1256_Read24i(ads1256);
    ADS1256_STANDBY(ads1256);
    ads1256->select(FALSE);
    return v;
}

void ADS1256_Mul_MUX_ADCON(ADS1256 *ads1256, const uint8_t *MUXs, const uint8_t *ADCONs,
                           int32_t *results, size_t count) {
    int i;
    ads1256->select(TRUE);
    uint8_t data[2];
    data[0] = MUXs[0];
    data[1] = ADCONs[0];
    ADS1256_WREG(ads1256, ADS1256_MUX, 2);
    ads1256->swap(data, 2);
    ADS1256_SYNC(ads1256);
    ADS1256_WAKEUP(ads1256);
    ads1256->wait();
    for (i = 1; i < count; i++) {
        data[0] = MUXs[0];
        data[1] = ADCONs[0];
        ADS1256_WREG(ads1256, ADS1256_MUX, 2);
        ads1256->swap(data, 2);
        ADS1256_SYNC(ads1256);
        ADS1256_WAKEUP(ads1256);
        ADS1256_RDATA(ads1256);
        results[i - 1] = ADS1256_Read24i(ads1256);
        ads1256->wait();
    }
    ADS1256_RDATA(ads1256);
    results[count - 1] = ADS1256_Read24i(ads1256);
    ADS1256_STANDBY(ads1256);
    ads1256->select(FALSE);
}

void ADS1256_Mul_MUX(ADS1256 *ads1256, const uint8_t *MUXs, int32_t *results, size_t count) {
    int i;
    ads1256->select(TRUE);
    uint8_t MUX = MUXs[0];
    ADS1256_WREG(ads1256, ADS1256_MUX, 1);
    ads1256->swap(&MUX, 1);
    ADS1256_SYNC(ads1256);
    ADS1256_WAKEUP(ads1256);
    ads1256->wait();
    for (i = 1; i < count; i++) {
        MUX = MUXs[i];
        ADS1256_WREG(ads1256, ADS1256_MUX, 1);
        ads1256->swap(&MUX, 1);
        ADS1256_SYNC(ads1256);
        ADS1256_WAKEUP(ads1256);
        ADS1256_RDATA(ads1256);
        results[i - 1] = ADS1256_Read24i(ads1256);
        ads1256->wait();
    }
    ADS1256_RDATA(ads1256);
    results[count - 1] = ADS1256_Read24i(ads1256);
    ADS1256_STANDBY(ads1256);
    ads1256->select(FALSE);
}
