/**
 * @file CacheStatic.h
 * @author Vinicius de Sa (vinicius@vido-la.com)
 * @brief Cache de aplicação.
 * @version 0.1
 * @date 17/10/2019
 * 
 * @copyright Copyright VIDO.LA 2019: Todos os direitos reservados.
 */
#pragma once

/**
 * @brief Estado dos componentes do sistema.
 * 
 */
struct SystemStatus_t
{
public:
    SystemStatus_t()
    {
        MPUWorking = true;
        SDWorking = true;
        GPSWorking = true;
        AudioWorking = true;
        GSMWorking = true;
        VersionManagerWorking = true;
        ConfigurationManagerWorking = true;
        LogManagerWorking = true;
        ThreadEngineWorking = true;
        ThreadGSMMonitorWorking = true;
        ThreadTrackerMonitorWorking = true;
    }

    bool MPUWorking;    // MPU funcionando
    bool SDWorking;     // SD funcionando
    bool SPIFFSWorking; // SPIFFS funcionando
    bool GPSWorking;    // GPS funcionando
    bool AudioWorking;  // Audio funcionando
    bool GSMWorking;    // GSM funcionando
    bool VersionManagerWorking;
    bool ConfigurationManagerWorking;
    bool LogManagerWorking;
    bool ThreadTrackerMonitorWorking;
    bool ThreadGSMMonitorWorking;
    bool ThreadEngineWorking;
    bool EngineStarted;
    bool ShouldIdentify;
};

/**
 * @brief Cache da aplicação.
 * 
 */
class CacheStaticClass
{
    public:
        /**
         * @brief Inicializar o objeto CacheStaticClass.
         * 
         */
        CacheStaticClass();
        /**
         * @brief Indica se o engine está habilitado.
         * 
         */
        volatile bool IsEngineEnabled;
        /**
         * @brief Indica o status atual do sistema.
         * 
         */
        SystemStatus_t SystemStatus;
};

extern CacheStaticClass CacheStatic;