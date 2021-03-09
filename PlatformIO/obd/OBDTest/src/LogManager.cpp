/**
 * @file LogManager.cpp
 * @author Vinicius de Sa (vinicius@vido-la.com)
 * @brief Classe que gerencia o armazenamento em logs.
 * @version 0.2
 * @date 16/07/2018
 * 
 * @copyright Copyright VIDO.LA 2019: Todos os direitos reservados.
 */
#include <Preferences.h>
#include "LogManager.h"

/**
 * @brief Inicializar o objeto LogManagerClass.
 * 
 */
LogManagerClass::LogManagerClass()
{
}

/**
 * @brief Inicializar o objeto com as configurações adequadas.
 * 
 * @param settings 
 */
void LogManagerClass::begin()
{
    m_lastTry = 0;
    m_remainingBytes = MAX_LOG_FILE_SIZE;
    m_lastLogTimeStamp = millis();

    // "Inicializando o sistema de log."
    //ESP_LOGD(TAG_LOG, LOG_LOGD_0005);

    CacheStatic.SystemStatus.LogManagerWorking = loadLogFile();
}

/**
 * @brief Logar dados de telemetria.
 * 
 * @param gpsContext Contexto do GPS.
 * @param mpuContext Contexto do acelerômetro.
 */
void LogManagerClass::logData(String data)
{
    String result = data + LOG_LINE_END;

    writeLog(result);
}

/**
 * @brief Carregar o arquivo de log atual a partir do que foi armazenado em memória.
 * 
 */
bool LogManagerClass::loadLogFile()
{
    // #define LOG_LOGD_0001 "Carregando o arquivo de log."
    //ESP_LOGD(TAG_LOG, LOG_LOGD_0001);

    if (CacheStatic.SystemStatus.SDWorking == false)
        return false;    

    Preferences pref;
    // "log"
    pref.begin(NAMESPACE_LOG, false);
    // "file_number"
    m_fileNumber = pref.getULong(KEY_LOG_FILE_NUMBER, 0);

    // "Numero do arquivo de log: %lu"
    //ESP_LOGD(TAG_LOG, LOG_LOGD_0002, m_fileNumber);

    if (m_fileNumber == 0) // eh o primeiro, joga pra 1
    {
        m_fileNumber = 1;
        pref.putULong(KEY_LOG_FILE_NUMBER, m_fileNumber);
    }

    pref.end();

    // montar o nome do arquivo
    String fileName = getLogFileName();

    StorageController.waitOne(portMAX_DELAY);
    if (SD.exists(fileName.c_str()))
    {
        File file = SD.open(fileName);
        m_remainingBytes = MAX_LOG_FILE_SIZE - file.size();

        // #define LOG_LOGD_0004 "Arquivo de log aberto: %s (%lu bytes livres)"
        Serial.printf(LOG_LOGD_0004, fileName.c_str(), m_remainingBytes);

        file.close();
    }
    else
    {
        // #define LOG_LOGE_0001 "O arquivo de log %s nao existe."
        Serial.printf(LOG_LOGE_0001, fileName.c_str());
    }
    StorageController.releaseMutex();

    return true;
}

/**
 * @brief Retornar o nome do arquivo de log.
 * 
 * @return String 
 */
String LogManagerClass::getLogFileName()
{
    String fileName = DEFAULT_LOG_FOLDER + String(m_fileNumber) + LOG_FILE_EXTENSION;
    return fileName;
}

/**
 * @brief Criar um novo arquivo de log.
 * 
 * @return String 
 */
String LogManagerClass::createLogFile()
{
    // incrementar o numero do log
    m_fileNumber++;
    // #define LOG_LOGD_0006 "Criando um arquivo de log numero %lu."
    Serial.printf(LOG_LOGD_0006, m_fileNumber);

    // salvar no preferences
    Preferences pref;
    pref.begin(NAMESPACE_LOG, false);
    pref.putULong(KEY_LOG_FILE_NUMBER, m_fileNumber);
    pref.end();

    // atualizar o m_remainingBytes e o m_fileNumber
    m_remainingBytes = MAX_LOG_FILE_SIZE;

    // retornar o nome do novo arquivo de log
    return getLogFileName();
}

/**
 * @brief Escrever um conteúdo no arquivo de log atual.
 * 
 * @param content Conteúdo a ser escrito.
 */
void LogManagerClass::writeLog(String content)
{
    String fileName = getLogFileName();

    if (!CacheStatic.SystemStatus.SDWorking)
    {
        if ((millis() - m_lastTry) > LOG_ERROR_MESSAGE_INTERVAL)
        {
            m_lastTry = millis();
            Serial.printf(LOG_LOGE_0003); // "Log desabilitado. SD com problemas."
        }
        return;
    }

    size_t bytesAppended = 0;
    bool result = StorageController.appendFile(fileName, content, bytesAppended);
    if ((result == false) || (bytesAppended == 0))
        Serial.printf(LOG_LOGE_0002);
    else
    {
        Serial.printf("%s", content.c_str());
        
        if (m_remainingBytes < content.length())
            fileName = createLogFile();

        m_remainingBytes = m_remainingBytes - bytesAppended;
    }
}

LogManagerClass LogManager;