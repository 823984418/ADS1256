# ADS1256通用驱动


[官方数据手册](https://www.ti.com.cn/product/cn/ADS1256)

要使用此库
```c
#include "ads1256.h"
```
需要实现
```c
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
    /// \param e        是否选中
    void (*select)(bool_t e);
} ADS1256;
```
随后进行初始化
```c
ADS1256 ads1256 = (ADS1256) {
    .swap = framework_ADS1256_swap,
    .wait = framework_ADS1256_wait,
    .select = framework_ADS1256_select,
};
ADS1256_Init(&ads1256);
```
最简单的采样默认通道调用为
```c
ADS1256_Shot(&ads1256);
```
