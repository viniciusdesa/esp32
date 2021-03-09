/**
 * @file StructLibrary.h
 * @author Vinicius de Sá (vinicius@vido-la.com.br)
 * @brief 
 * @version 0.1
 * @date 28/01/2021
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <stdint.h>

struct GPSSettings_t
{
public:
    GPSSettings_t()
    {
        BaudRate = 9600;
        SampleRate = 5;
        MaxHDOP = 5;
        MinSatellites = 4;
        Timeout = 5000;
        MessagesReadToFix = 12;
        Model = 1;
    }

    /**
         * @brief Velocidade de comunicação, em bps.
         * 
         */
    int BaudRate;
    /**
         * @brief Taxa de amostragem. Pode ser 1, 2, 4, 5 ou 10 amostras por segundo.
         * 
         */
    int SampleRate;
    /**
         * @brief HDOP máximo para o equipamento identificar que tem sinal.
         * 
         */
    int MaxHDOP;
    /**
         * @brief Quantidade mínima de satélites para o equipamento identificar que tem sinal.
         * 
         */
    int MinSatellites;
    /**
         * @brief Timeout de resposta do GPS.
         * 
         */
    int Timeout;
    /**
         * @brief Número de mensagens que o GPS deve fornecer para que seja determinado que há sinal.
         * 
         */
    int MessagesReadToFix;
    /**
         * @brief 0 - uBlox Neo6-M / 1 - ATGM332D
         * 
         */
    int Model;
};

/**
 * @brief Estrutura que contem os dados do GPS.
 */
struct GPSContext_t
{
    /**
     * @brief Latitude, em graus decimais.
     * 
     */
    double Latitude;
    /**
     * @brief Longitude, em graus decimais.
     * 
     */
    double Longitude;
    /**
     * @brief Azimute, em graus.
     * 
     */
    double Bearing;
    /**
     * @brief Altitude, em metros.
     * 
     */
    double Altitude;
    /**
     * @brief Velocidade, em km/h.
     * 
     */
    double Speed;
    /**
     * @brief Horas.
     * 
     */
    uint8_t Time_Hours;
    /**
     * @brief Minutos.
     * 
     */
    uint8_t Time_Minutes;
    /**
     * @brief Segundos.
     * 
     */
    uint8_t Time_Seconds;
    /**
     * @brief Centisegundos.
     * 
     */
    uint8_t Time_Centiseconds;
    /**
     * @brief Ano.
     * 
     */
    uint16_t Date_Year;
    /**
     * @brief Mês.
     * 
     */
    uint8_t Date_Month;
    /**
     * @brief Dia.
     * 
     */
    uint8_t Date_Day;
    /**
     * @brief Número de satélites.
     * 
     */
    uint32_t Satellites;
    /**
     * @brief HDOP.
     * 
     */
    double HDOP;
    /**
     * @brief Flag que determina se o contexto é válido.
     * 
     */
    bool IsValid;
    /**
     * @brief Flag que determina se a posição está atualizada.
     * 
     */
    bool IsPositionUpdated;
};