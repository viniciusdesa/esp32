/**
 * @file GPSHandler.h
 * @author Vinicius de Sa (vinicius@vido-la.com)
 * @brief Contém a classe GPS que provê funções para uso do dispositivo GPS.
 * @version 0.1
 * @date 14/02/2017
 * 
 * @copyright Copyright VIDO.LA 2019: Todos os direitos reservados.
 */
#pragma once

#include <TinyGPS++.h>
#include "StructLibrary.h"

#define ATGM_GPS_CMD_SET_SAMPLE_RATE_1_HZ "$PCAS02,1000*2E\r\n"
#define ATGM_GPS_CMD_SET_SAMPLE_RATE_2_HZ "$PCAS02,500*1A\r\n"
#define ATGM_GPS_CMD_SET_SAMPLE_RATE_4_HZ "$PCAS02,250*18\r\n"
#define ATGM_GPS_CMD_SET_SAMPLE_RATE_5_HZ "$PCAS02,200*1D\r\n"
#define ATGM_GPS_CMD_SET_SAMPLE_RATE_10_HZ "$PCAS02,100*1E\r\n"

#define ATGM_GPS_CMD_SET_PROTOCOLS_GPS "$PCAS04,1*18\r\n"
#define ATGM_GPS_CMD_SET_PROTOCOLS_GPS_BDS "$PCAS04,3*1A\r\n"
#define ATGM_GPS_CMD_SET_PROTOCOLS_GPS_BDS_GLONASS "$PCAS04,7*1E\r\n"

#define ATGM_GPS_CMD_SET_BAUDRATE_115200 "$PCAS01,5*19\r\n"

#define ATGM_GPS_CMD_SET_NAVIGATION_PLATFORM_PORTABLE "$PCAS11,0*1D\r\n"
#define ATGM_GPS_CMD_SET_NAVIGATION_PLATFORM_PEDESTRIAN "$PCAS11,2*1F\r\n"
#define ATGM_GPS_CMD_SET_NAVIGATION_PLATFORM_AUTOMOTIVE "$PCAS11,3*1E\r\n"
#define ATGM_GPS_CMD_SET_NAVIGATION_PLATFORM_SEA "$PCAS11,4*19\r\n"

#define TAG_GPS "[GPS]"
#define GPS_LOGD_0001 "Inicializando o GPS. Baudrate: %d, GPS-TX: %d, GPS-RX: %d\n"
#define GPS_LOGD_0002 "GPS inicializado.\n"
#define GPS_LOGD_0003 "GPS nao inicializado\n"
#define GPS_LOGD_0004 "Dados de GPS recebidos mas invalidos.\n"
#define GPS_LOGD_0005 "Taxa de amostragem do uBlox ajustada para: %d\n"
#define GPS_LOGD_0006 "Data: %02d/%02d/%04d %02d:%02d:%02d\n"
#define GPS_LOGD_0007 "Plataforma de navegacao do uBlox ajustada para: %d\n"
#define GPS_LOGD_0008 "isUpdated (Speed: %d)\n"
#define GPS_LOGD_0009 "Configurando o baudrate do ATGM de %u para 115200.\n"
#define GPS_LOGD_0010 "Configurando a amostragem do ATGM para %d.\n"
#define GPS_LOGD_0011 "Configurando a plataforma de navegacao do ATGM para Automotive.\n"

#define GPS_LOGE_0001 "GPS nao inicializado. Reiniciando o sistema em 5 segundos...\n"
#define GPS_LOGE_0002 "Nenhum dado recebido. Verifique o cabeamento.\n"
#define GPS_LOGE_0003 "[%02d/%02d/%04d %02d:%02d:%02d] Posicao desatualizada.\n"
#define GPS_LOGE_0004 "GPS sem sinal.\n"

const uint8_t GPS_MIN_CHARS_PROCESSED = 10;
const uint16_t GPS_READ_TIMEOUT = 1100;

class GPSClass
{
    public:
        /* 
        *  @brief Construtor da classe GPS. 
        */
        GPSClass();
        /* 
        * @brief    Iniciar a comunicação com o GPS. 
        * 
        * @param    gpsTX       pino TX do GPS.
        * @param    gpsRX       pino RX do GPS.
        * @param    baudRate    baud rate do GPS.
        */
        void begin( gpio_num_t gpsTX, gpio_num_t gpsRX, GPSSettings_t settings);
        /* 
        *  @brief   Obter uma string contendo os dados do GPS.
        * 
        *  @param   GPSContext_t Contexto do GPS.
        *  @return  Objeto String contendo os dados do GPS.
        */
        String toString(GPSContext_t context);
        /* 
        *  @brief   Obter os dados do GPS.
        *  @return  Objeto GPSContext contendo os dados do GPS.
        */
        GPSContext_t getData();
        /*
        * @brief    Retornar a flag que indica se o GPS esta com sinal.
        * @return   Boolean com true para GPS com sinal, false caso contrario.
        */
        bool hasFix();
        /**
         * @brief Configurar a taxa de amostragem do GPS.
         * 
         */
        void setSampleRate();
        /**
         * @brief Configurar a plataforma de navegação.
         * 
         */
        void setNavigationPlatform();
        /**
         * @brief Configurar o baudrate do GPS.
         * 
         */
        void setBaudrate();
        /**
         * @brief Configurar as constelacoes de navegacao do GPS.
         * 
         */
        void setConstellation();
        /**
         * @brief Retornar o horário atual.
         * 
         * @param currentTime Horário atual.
         */
        void getCurrentTime(struct tm *currentTime);
        /**
         * @brief Retornar o último posicionamento válido.
         * 
         * @return GPSContext_t Contexto do GPS.
         */
        GPSContext_t getLastPosition();
        /**
         * @brief Retornar o tempo em que o GPS está parado (sem deslocamento), em milissegundos.
         * 
         * @return time_t Tempo em que o GPS está parado (sem deslocamento), em milissegundos.
         */
        time_t getStationaryTime();
    private:
        GPSSettings_t m_settings;
        TinyGPSPlus _tinyGPS;
        bool m_isInitialized;  
        bool m_hasFix;  
        int m_messagesReadToFix;    
        int m_messagesReadToFixLost;
        gpio_num_t m_gpsTX;    
        gpio_num_t m_gpsRX;
        unsigned long m_baudRate;
        GPSContext_t m_lastPosition;
        time_t m_stationaryTime;
        time_t m_fixTime;
        bool m_isPositionUpdated;
        //GPSContext m_lastContext;
        /**
         * @brief    executar um delay aguardando mensagens do GPS.
         * @param    unsigned long timeout   Tempo maximo de espera em milissegundos.
         * @return   bool                    True para sucesso, false caso contrario.
         */
        bool executeSmartDelay( unsigned long timeout );

        /**
         * @brief    Mostrar a data e hora provenientes do GPS.
         * @param    unsigned long timeout   Tempo maximo de espera em milissegundos.
         * @return   bool                    True para sucesso, false caso contrario.
         */
        void printDateTime(TinyGPSDate date, TinyGPSTime time);

        /**
         * @brief       Configurar o GPS (taxa de amostragem e protocolos).
         * @param       const char * command        Comando a ser enviado.
         * @return      Numero de bytes enviados.
         */
        size_t writeCommand(const char * command);
        /**
         * @brief Configurar a taxa de amostragem do GPS.
         * 
         */
        void ubloxSetSampleRate();
        /**
         * @brief Configurar a plataforma de navegação.
         * 
         */
        void ubloxSetNavigationPlatform();
        /**
         * @brief Configurar a taxa de amostragem do GPS.
         * 
         */
        void atgmSetSampleRate();
        /**
         * @brief Configurar a plataforma de navegação.
         * 
         */
        void atgmSetNavigationPlatform();
        /**
         * @brief Configurar o baudrate do GPS ATGM.
         * 
         */
        void atgmSetBaudrate();
        /**
         * @brief Configurar as constelacoes de navegacao do GPS.
         * 
         */
        void atgmSetConstellation();
        /**
         * @brief Configurar o horário atual.
         * 
         * @param context Contexto do GPS.
         */
        void setCurrentTime(GPSContext_t context);        
};

extern GPSClass GPS;