#prama once
#include "spi_controller.h"
#include <SPI.h>

//const u8int_t csA = idk;
//const u8int_t csB = idk;

#define REG_START 0x00
#define REG_END 0x1C


void spiControllerBegin(uint8_t csA, uint8_t csB) {
    pinMode(csA, OUTPUT);
    pinMode(csB, OUTPUT);
    digitalWrite(csA, HIGH);
    digitalWrite(csB, HIGH);
    SPI.begin();
}

void writeReg(uint8_t cs, uint8_t addr, uint8_t val) {
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(cs, LOW);

    SPI.transfer(CMD_WRITE(addr));
    SPI.transfer(val);
    digitalWrite(cs, HIGH);
    delayMicroseconds(INTERBYTE_US);
    SPI.endTransaction();
}

void writeBurst(uint8_t cs, uint8_t addr, const uint8_t *d, uint8_t n) {   
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(cs, LOW);

    SPI.transfer(CMD_WRITE(addr));
    SPI.transfer(d, n);
    digitalWRite(cs, HIGH);
    delayMicroseconds(INTERBYTE_US);

    SPI.endTransaction();
}

uint8_t readReg(uint8_t cs, uint8_t addr) {
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(cs, LOW);

    SPI.transfer(CMD_READ(addr));
    SPI.transfer(0x00); // dummy byte
    uint8_t result = SPI.transfer(0x00); // read the byte clocked back
    
    delayMicroseconds(INTERBYTE_US);
    SPI.endTransaction();
    return result;
}

void readResult(uint8_t cs, uint32_t out[N_RESULT_REGS]) {
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    digitalWrite(cs, LOW);

    SPI.transfer(CMD_READ(REG_TIME1));

    for (int i = 0; i < 40; i++) {
        SPI.transfer(0x00); // dummy bytes
        out[i] = SPI.transfer(0x00);
    }

    out = out & 0x7FFFFF; // mask off parity bit
    out = out & ((1 << 23) - 1);
    
    delayMicroseconds(INTERBYTE_US);
    SPI.endTransaction();
}