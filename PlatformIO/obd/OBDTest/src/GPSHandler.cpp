/************************************************************************
* Arquivo: GPSHandler.cpp
* Autor: Vinicius de Sá
* Data: 04/12/2017
* Objetivo: Contém a classe GPS que provê funções para uso do dispositivo GPS.
* Exemplo de código:
  
  GPS _gps( PIN_GPS_TX , PIN_GPS_RX );
  
  void setup()
  {
    ...
    _gps.begin();
    ...
  }

  void loop()
  {
      Serial.println( "[GPS]" + _gps.getString() );
  }
************************************************************************/
#include "GPSHandler.h"

//#define SERIAL_ON_PC    // Descomentar isto se está usando a serial do PC como origem dos dados de rastreamento (SatGenNMEA)

#ifdef SERIAL_ON_PC
#define SerialGPS Serial
#else
#define SerialGPS Serial2
#endif

/* 
 *  @brief Construtor da classe GPS. 
*/
GPSClass::GPSClass()
{
}

/* 
* @brief    Iniciar a comunicação com o GPS. 
* 
* @param    gpsTX       pino TX do GPS.
* @param    gpsRX       pino RX do GPS.
* @param    baudRate    baud rate do GPS.
*/
void GPSClass::begin(gpio_num_t gpsTX, gpio_num_t gpsRX, GPSSettings_t settings)
{
    // ""Inicializando o GPS. Baudrate: %d, GPS-TX: %d, GPS-RX: %d"
    Serial.printf(GPS_LOGD_0001, m_settings.BaudRate, gpsTX, gpsRX);

    m_settings = settings;
    m_baudRate = m_settings.BaudRate;

#ifdef SERIAL_ON_PC
    SerialGPS.begin(m_baudRate, SERIAL_8N1);
#else
    m_gpsTX = gpsTX;
    m_gpsRX = gpsRX;
    SerialGPS.begin(m_baudRate, SERIAL_8N1, (int8_t)gpsTX, (int8_t)gpsRX);
#endif

    delay(500);

    m_isInitialized = true;
    m_hasFix = false;
    m_messagesReadToFix = 0;
    m_messagesReadToFixLost = 0;
    m_lastPosition.Altitude = 0;
    m_lastPosition.Bearing = 0;
    m_lastPosition.Date_Day = 1;
    m_lastPosition.Date_Month = 1;
    m_lastPosition.Date_Year = 1900;
    m_lastPosition.HDOP = 99;
    m_lastPosition.IsPositionUpdated = false;
    m_lastPosition.IsValid = false;
    m_lastPosition.Latitude = 0;
    m_lastPosition.Longitude = 0;
    m_lastPosition.Satellites = 0;
    m_lastPosition.Speed = 0;
    m_stationaryTime = 0;

#ifndef SERIAL_ON_PC
    setBaudrate();
    setSampleRate();
    setNavigationPlatform();
    setConstellation();
#endif

    // #define GPS_LOGD_0002 "GPS inicializado."
    //Serial.printf(GPS_LOGD_0002);
}

/**
 * @brief Configurar o baudrate do GPS.
 * 
 */
void GPSClass::setBaudrate()
{
    if (m_settings.Model == 1) // ATGM
    {
        if (m_baudRate != 115200)
            atgmSetBaudrate();
    }
}

/**
 * @brief Configurar o baudrate do GPS ATGM.
 * 
 */
void GPSClass::atgmSetBaudrate()
{
    // #define GPS_LOGD_0009 "Configurando o baudrate do ATGM de %u para 115200."
    //Serial.printf(GPS_LOGD_0009, SerialGPS.baudRate());

    writeCommand(ATGM_GPS_CMD_SET_BAUDRATE_115200);
    delay(250);

    m_baudRate = 115200;
    SerialGPS.begin(m_baudRate, SERIAL_8N1, (int8_t)m_gpsTX, (int8_t)m_gpsRX);
}

/**
 * @brief Configurar a taxa de amostragem do GPS.
 * 
 */
void GPSClass::setSampleRate()
{
    if (m_settings.Model == 0)
        ubloxSetSampleRate();
    else
        atgmSetSampleRate();
}

/**
 * @brief Configurar a taxa de amostragem do GPS.
 * 
 */
void GPSClass::ubloxSetSampleRate()
{
    char command[35] = {
        // changes the sampling to 250ms
        0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xFA, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x96,
        // save settings
        0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1D, 0xAB};

    SerialGPS.write((uint8_t *)command, sizeof(command));
    // "Taxa de amostragem do uBlox ajustada para: %d"
    //Serial.printf(GPS_LOGD_0005 , result );
}

/**
 * @brief Configurar a taxa de amostragem do GPS.
 * 
 */
void GPSClass::atgmSetSampleRate()
{
    // #define GPS_LOGD_0010 "Configurando a amostragem do ATGM para %d."
    //Serial.printf(GPS_LOGD_0010, m_settings.SampleRate);

    switch (m_settings.SampleRate)
    {
    case 1:
        writeCommand(ATGM_GPS_CMD_SET_SAMPLE_RATE_1_HZ);
        break;
    case 2:
        writeCommand(ATGM_GPS_CMD_SET_SAMPLE_RATE_2_HZ);
        break;
    case 4:
        writeCommand(ATGM_GPS_CMD_SET_SAMPLE_RATE_4_HZ);
        break;
    case 5:
        writeCommand(ATGM_GPS_CMD_SET_SAMPLE_RATE_5_HZ);
        break;
    case 10:
        writeCommand(ATGM_GPS_CMD_SET_SAMPLE_RATE_10_HZ);
        break;
    default:
        writeCommand(ATGM_GPS_CMD_SET_SAMPLE_RATE_5_HZ);
        break;
    }
}

/**
 * @brief Configurar a plataforma de navegação.
 * 
 */
void GPSClass::setNavigationPlatform()
{
    if (m_settings.Model == 0)
        ubloxSetNavigationPlatform();
    else
        atgmSetNavigationPlatform();
}

/**
 * @brief Configurar a plataforma de navegação.
 * 
 */
void GPSClass::atgmSetNavigationPlatform()
{
    // #define GPS_LOGD_0011 "Configurando a plataforma de navegacao do ATGM para Automotive."
    //Serial.printf(GPS_LOGD_0011);

    writeCommand(ATGM_GPS_CMD_SET_NAVIGATION_PLATFORM_AUTOMOTIVE);
}

/**
 * @brief Configurar a plataforma de navegação.
 * @remarks Configura o GPS para:
 * Dynamic Model: 4 - Automotive
 * Fix Mode: 3 - Auto 2D/3D
 * UTC Standard: 0 - Automatic
 * Navigation Input Filters:
 * {
 *     Min SV Elevation: 5
 *     C/N0 Threshold: 0 SVs / 0 dbHz
 * }
 * Navigation Output Filters:
 * {
 *      DR Timeout: 0
 *      PDOP Mask: 25.0
 *      TDOP Mask: 25.0
 *      P Acc Mask: 100
 *      P Acc ADR Mask: 0
 *      T Acc Mask: 300
 *      Static Hold Threshold: 0.00
 *      Static Hold Exit Dist: 0
 * }
 * DGNSS Timeout: 60
 * 0000  B5 62 06 24 24 00 FF FF 04 03 00 00 00 00 10  µb$$ÿÿ
 * 000F  27 00 00 05 00 FA 00 FA 00 64 00 2C 01 00 3C  'úúd,<
 * 001E  00 00 00 00 00 00 00 00 00 00 00 00 50 A4     P¤
 */
void GPSClass::ubloxSetNavigationPlatform()
{
    char command[] = {
        0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10,
        0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x3C,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0xA4,
        // save settings
        0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1D, 0xAB};

    int result = SerialGPS.write((uint8_t *)command, sizeof(command));
    // #define GPS_LOGD_0007 "Plataforma de navegação do uBlox ajustada para: %d"
    Serial.printf(GPS_LOGD_0007, result);
}

/**
 * @brief Configurar as constelacoes de navegacao do GPS.
 * 
 */
void GPSClass::setConstellation()
{
    if (m_settings.Model == 1) // ATGM
    {
        atgmSetConstellation();
    }
}

/**
 * @brief Configurar as constelacoes de navegacao do GPS.
 * 
 */
void GPSClass::atgmSetConstellation()
{
    writeCommand(ATGM_GPS_CMD_SET_PROTOCOLS_GPS_BDS_GLONASS);
}

/* 
*  @brief   Obter os dados do GPS.
*  @return  Objeto GPSContext contendo os dados do GPS.
*/
GPSContext_t GPSClass::getData()
{
    GPSContext_t context;
    bool result = false;
    unsigned long startTime = millis();

    context.Latitude = 0.0;
    context.Longitude = 0.0;
    context.Bearing = 0.0;
    context.Speed = 0.0;
    context.Altitude = 0.0;
    context.Time_Hours = 0;
    context.Time_Minutes = 0;
    context.Time_Seconds = 0;
    context.Time_Centiseconds = 0;
    context.Date_Year = 0;
    context.Date_Month = 0;
    context.Date_Day = 0;
    context.IsPositionUpdated = false;

    result = executeSmartDelay(GPS_READ_TIMEOUT);

    if (!result || ((millis() - startTime) > m_settings.Timeout && _tinyGPS.charsProcessed() < GPS_MIN_CHARS_PROCESSED))
    {
        // #define GPS_LOGE_0002 "Nenhum dado recebido. Verifique o cabeamento."
        Serial.printf(GPS_LOGE_0002);
        context.IsValid = false;
    }
    else
    {
        context.IsValid = true;
        context.Satellites = _tinyGPS.satellites.value();
        context.HDOP = _tinyGPS.hdop.hdop();
        context.Time_Hours = _tinyGPS.time.hour();
        context.Time_Minutes = _tinyGPS.time.minute();
        context.Time_Seconds = _tinyGPS.time.second();
        context.Time_Centiseconds = _tinyGPS.time.centisecond();
        context.Date_Year = _tinyGPS.date.year();
        context.Date_Month = _tinyGPS.date.month();
        context.Date_Day = _tinyGPS.date.day();
        context.Latitude = _tinyGPS.location.lat();
        context.Longitude = _tinyGPS.location.lng();
        context.Bearing = _tinyGPS.course.deg();
        context.Speed = _tinyGPS.speed.kmph();
        context.Altitude = _tinyGPS.altitude.meters();
        context.IsPositionUpdated = !((context.Latitude == 0.0) && (context.Longitude == 0.0) && (context.Speed == 0.0) && (context.Bearing == 0.0));

        // Verificar se a coordenada eh valida.
        // Se tem 4 ou mais satelites e se o hdop esta abaixo de 5.
        if ((context.Satellites >= m_settings.MinSatellites) && (context.HDOP < m_settings.MaxHDOP) && (context.Date_Year >= 2019))
        {
            m_messagesReadToFixLost = 0;

            if (m_hasFix == false)
            {
                ++m_messagesReadToFix;

                if (m_messagesReadToFix >= m_settings.MessagesReadToFix)
                {
                    m_hasFix = true;
                    m_messagesReadToFix = 0;
                    m_fixTime = millis();
                }
            }

            if (context.Speed == 0)
                m_stationaryTime = millis() - m_fixTime;
            else
                m_stationaryTime = 0;

            m_lastPosition = context;
        }
        else // context.Satellites >= 4
        {
            m_messagesReadToFix = 0;

            if (m_hasFix == true)
            {
                ++m_messagesReadToFixLost;
                if (m_messagesReadToFixLost >= m_settings.MessagesReadToFix)
                {
                    m_hasFix = false;
                    m_messagesReadToFixLost = 0;
                }
            }

            m_lastPosition.IsPositionUpdated = false;
        }
    }

    return context;
}

void GPSClass::printDateTime(TinyGPSDate date, TinyGPSTime time)
{
    // #define GPS_LOGD_0006 "Data: %02d/%02d/%04d %02d:%02d:%02d"
    Serial.printf(GPS_LOGD_0006, date.day(), date.month(), date.year(), time.hour(), time.minute(), time.second());
}

/* 
*  @brief   Obter uma string contendo os dados do GPS.
*  @return  Objeto String contendo os dados do GPS.
*/
String GPSClass::toString(GPSContext_t context)
{
    String result = String(context.Latitude, 5) + ",";
    result = result + String(context.Longitude, 4) + ",";
    result = result + String(context.Bearing, 2) + ",";
    result = result + String(context.Speed, 2) + ",";
    result = result + String(context.Satellites) + ",";
    result = result + String(context.HDOP, 2) + ",";
    result = result + String(context.Altitude) + ",";
    if (context.Date_Day < 10)
        result = result + "0";
    result = result + String(context.Date_Day);
    if (context.Date_Month < 10)
        result = result + "0";
    result = result + String(context.Date_Month);
    if (context.Date_Year < 10)
        result = result + "0";
    result = result + String(context.Date_Year) + ",";
    if (context.Time_Hours < 10)
        result = result + "0";
    result = result + String(context.Time_Hours);
    if (context.Time_Minutes < 10)
        result = result + "0";
    result = result + String(context.Time_Minutes);
    if (context.Time_Seconds < 10)
        result = result + "0";
    result = result + String(context.Time_Seconds);
    if (context.Time_Centiseconds < 100)
        result = result + "0";
    if (context.Time_Centiseconds < 10)
        result = result + "0";
    result = result + String(context.Time_Centiseconds);

    return result;
}

/*
* @brief    executar um delay aguardando mensagens do GPS.
* @param    unsigned long timeout   Tempo maximo de espera em milissegundos.
* @return   bool                    True para sucesso, false caso contrario.
*/
bool GPSClass::executeSmartDelay(unsigned long timeout)
{
    int incomingByte = 0;

    if (m_isInitialized == false)
        return false;

    unsigned long start = millis();
    String incomingText = "";

    do
    {
        while (SerialGPS.available())
        {
            incomingByte = SerialGPS.read();
            _tinyGPS.encode(incomingByte);
            incomingText.concat((char)incomingByte);
        }

        if ((_tinyGPS.time.isUpdated() == true) && (_tinyGPS.course.isUpdated()))
            break;
    } while ((millis() - start) < timeout);

    // Se ocorreu timeout...
    if ((millis() - start) >= timeout)
    {
        if (_tinyGPS.time.isUpdated() == true)
        {
            m_isPositionUpdated = false;
            // #"[%02d/%02d/%04d %02d:%02d:%02d] Posicao desatualizada."
            // Serial.printf(GPS_LOGE_0003, _tinyGPS.date.day(), _tinyGPS.date.month(), _tinyGPS.date.year(), _tinyGPS.time.hour(), _tinyGPS.time.minute(), _tinyGPS.time.second());
        }
        else
        {
            // #define GPS_LOGE_0004 "GPS sem sinal."
            Serial.printf(GPS_LOGE_0004);
            return false;
        }
    }

    return true;
}

/*
* @brief    Retornar a flag que indica se o GPS esta com sinal.
* @return   Boolean com true para GPS com sinal, false caso contrario.
*/
bool GPSClass::hasFix()
{
    return m_hasFix;
}

/**
 * @brief       Configurar o GPS (taxa de amostragem e protocolos).
 * @param       const char * command        Comando a ser enviado.
 * @return      Numero de bytes enviados.
 */
size_t GPSClass::writeCommand(const char *command)
{
    size_t bytesWritten = SerialGPS.write(command);
    SerialGPS.flush();
    delay(150);
    return bytesWritten;
}

/**
 * @brief Retornar o último posicionamento.
 * 
 * @return GPSContext_t Contexto do GPS.
 */
GPSContext_t GPSClass::getLastPosition()
{
    return m_lastPosition;
}

/**
 * @brief Retornar o tempo em que o GPS está parado (sem deslocamento), em milissegundos.
 * 
 * @return time_t Tempo em que o GPS está parado (sem deslocamento), em milissegundos.
 */
time_t GPSClass::getStationaryTime()
{
    return m_stationaryTime;
}

GPSClass GPS;