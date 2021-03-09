/**
 * @file QMC5883L.cpp
 * @author Vinicius de Sá (vinicius@vido-la.com)
 * @brief 
 * @version 0.1
 * @date 28/07/2020
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "QMC5883L.h"

void QMC5883LClass::setAddress(uint8_t address)
{
    m_address = address;
}

bool QMC5883LClass::writeRegister(uint8_t registerAddress, uint8_t value)
{
    Wire.beginTransmission(m_address);

    if (Wire.write(registerAddress) == 0)
        return false;

    if (Wire.write(value) == 0)
        return false;

    uint8_t error = Wire.endTransmission();
    if (error != I2C_ERROR_OK)
    {
        Serial.printf("Erro I2C: %02X\n", error);
        return false;
    }

    return true;
}

/**
 * @brief Inicializar o objeto QMC5883L.
 * 
 * @return true 
 * @return false 
 */
bool QMC5883LClass::begin()
{
    resetCalibration();

    writeRegister(QMC_REG_RESET, 0x01); // Sempre escrever 0x01 no Registrador de SET/RESET Period

    if (!setMode(QMC_MODE_CONTINUOUS, false))
        return false;
    if (!setSamplingRate(QMC_ODR_50Hz, false))
        return false;
    if (!setRange(QMC_RNG_2G, false))
        return false;
    if (!setOversampling(QMC_OSR_512, false))
        return false;

    return reconfig();
}

void QMC5883LClass::softReset()
{
    writeRegister(QMC_REG_CONFIG2, 0x80);
}

int QMC5883LClass::read(int16_t &x, int16_t &y, int16_t &z)
{
    if (!readRegister(QMC_REG_XOUT_LSB, 6))
        return 0;

    x = Wire.read() | (Wire.read() << 8);
    y = Wire.read() | (Wire.read() << 8);
    z = Wire.read() | (Wire.read() << 8);

    return 6;
}

int QMC5883LClass::read(int16_t &x, int16_t &y, int16_t &z, int &heading)
{
    int result = read(x, y, z);
    if (!result)
        return 0;

    /* Update the observed boundaries of the measurements */
    if (x < m_xlow)
        m_xlow = x;

    if (x > m_xhigh)
        m_xhigh = x;

    if (y < m_ylow)
        m_ylow = y;

    if (y > m_yhigh)
        m_yhigh = y;

    /* Bail out if not enough data is available. */
    if (m_xlow == m_xhigh || m_ylow == m_yhigh)
        return 0;

    /* Recenter the measurement by subtracting the average */
    float x1 = x - ((m_xhigh + m_xlow) / 2);
    float y1 = y - ((m_yhigh + m_ylow) / 2);

    /* Rescale the measurement to the range observed. */
    float fx = (float)x1 / (m_xhigh - m_xlow);
    float fy = (float)y1 / (m_yhigh - m_ylow);

    heading = 180.0 * atan2(fy, fx) / M_PI;
    
    if (heading <= 0)
        heading += 360;

    return result;

    // a = azimuth(y, x);
    // return result;
}

int QMC5883LClass::read(int16_t &x, int16_t &y, int16_t &z, float &a)
{
    int result = read(x, y, z);
    a = azimuth(y, x);
    return result;
}

int QMC5883LClass::readHeading()
{
    int16_t x, y, z;

    if (!read(x, y, z))
        return 0;

    /* Update the observed boundaries of the measurements */
    if (x < m_xlow)
        m_xlow = x;

    if (x > m_xhigh)
        m_xhigh = x;
    if (y < m_ylow)
        m_ylow = y;
    if (y > m_yhigh)
        m_yhigh = y;

    /* Bail out if not enough data is available. */
    if (m_xlow == m_xhigh || m_ylow == m_yhigh)
        return 0;

    /* Recenter the measurement by subtracting the average */

    x -= (m_xhigh + m_xlow) / 2;
    y -= (m_yhigh + m_ylow) / 2;

    /* Rescale the measurement to the range observed. */

    float fx = (float)x / (m_xhigh - m_xlow);
    float fy = (float)y / (m_yhigh - m_ylow);

    int heading = 180.0 * atan2(fy, fx) / M_PI;
    
    if (heading <= 0)
        heading += 360;

    return heading;
}

float QMC5883LClass::azimuth(uint16_t a, uint16_t b)
{
    float azimuth = atan2((int)a, (int)b) * 180.0 / PI;
    return azimuth < 0 ? 360 + azimuth : azimuth;
}

bool QMC5883LClass::setOversampling(uint8_t osr, bool applyConfig)
{
    m_oversampling = osr;
    if (applyConfig)
        return reconfig();

    return true;
}

/**
 * @brief Configura a sensibilidade do sensor.
 * 
 * @param range Pode ser os seguintes valores:
 * QMC_RNG_2G: 2 Gauss
 * QMC_RNG_8G: 8 Gauss
 */
bool QMC5883LClass::setRange(int range, bool applyConfig)
{
    m_range = range;
    if (applyConfig)
        return reconfig();

    return true;
}

/**
 * @brief Definir a taxa de amostragem.
 * 
 * @param rate Pode ter os seguintes valores:
 * QMC_ODR_10Hz
 * QMC_ODR_50Hz
 * QMC_ODR_100Hz
 * QMC_ODR_200Hz
 */
bool QMC5883LClass::setSamplingRate(uint8_t rate, bool applyConfig)
{
    if ((rate != QMC_ODR_10Hz) &&
        (rate != QMC_ODR_50Hz) &&
        (rate != QMC_ODR_100Hz) &&
        (rate != QMC_ODR_200Hz))
        return false;

    m_rate = rate;
    if (applyConfig)
        return reconfig();

    return true;
}

/**
 * @brief Configurar o modo de funcionamento.
 * 
 * @param mode Modo de funcionamento.
 * @param reconfig Indica se é para aplicar as configurações.
 * @return true Em caso de sucesso.
 * @return false Em caso de falha.
 */
bool QMC5883LClass::setMode(uint8_t mode, bool applyConfig)
{
    m_mode = mode;
    if (applyConfig)
        return reconfig();

    return true;
}

bool QMC5883LClass::reconfig()
{
    return writeRegister(QMC_REG_CONFIG, (byte)(m_oversampling | m_range | m_rate | m_mode));
}

int QMC5883LClass::ready()
{
    if (readRegister(QMC_REG_STATUS, 1) == 0)
        return 0;

    uint8_t status = Wire.read();

    return status & QMC_STATUS_DRDY;
}

int QMC5883LClass::readRegister(int registerToRead, uint8_t size)
{
    Wire.beginTransmission(m_address);
    Wire.write(registerToRead);
    Wire.endTransmission();

    Wire.requestFrom(m_address, size);
    int n = Wire.available();
    if (n != size)
        return 0;

    return n;
}

void QMC5883LClass::resetCalibration()
{
    m_xhigh = m_yhigh = 0;
    m_xlow = m_ylow = 0;
}

QMC5883LClass QMC5883L;