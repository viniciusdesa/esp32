/**
 * @file CommonStorage.cpp
 * @author your name (you@domain.com)
 * @brief Realiza o controle do sistema de armazenamento.
 * @version 0.1
 * @date 2019-11-12
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "CommonStorage.h"

static SemaphoreHandle_t m_semaphoreStorageController;

/**
 * @brief Inicializar o objeto StorageControllerClass.
 * 
 */
StorageControllerClass::StorageControllerClass()
{
    m_writeErrorCount = 0;
    m_readErrorCount = 0;
    m_freeSpace = 0;
    m_lastCheck = 0;
    
    if (m_semaphoreStorageController == nullptr)
    {
        m_semaphoreStorageController = xSemaphoreCreateMutex();            
    }
}

/**
 * @brief Inicializar o gerenciador de armazenamento.
 * 
 * @param gpioNumber Número do GPIO onde o SD está conectado.
 * @return true Caso a inicialização tenha sucesso.
 * @return false Caso a inicialização falhe.
 */
bool StorageControllerClass::begin(gpio_num_t gpioNumber)
{
    m_gpioNumber = gpioNumber;

    CacheStatic.SystemStatus.SDWorking = SD.begin(gpioNumber, SDSPEED);

    if (!CacheStatic.SystemStatus.SDWorking)
    {
        // Falha ao montar o SD card.
        Serial.println(SCC_LOGE_0001);
    }
    else
    {
        getFreeSpace(false);
    }

    return CacheStatic.SystemStatus.SDWorking;
}

/**
 * @brief Incrementar o contador de erros de escrita.
 * 
 */
void StorageControllerClass::incrementWriteErrorCount()
{
    Serial.printf("[%lu] Erros de escrita: %d | Codigo Erro: %d\n", millis(), m_writeErrorCount + 1, SD.cardErrorCode());
    m_writeErrorCount++;

    if (m_writeErrorCount > 5)
    {
        resetStorage();
    }
}

/**
 * @brief Incrementar o contador de erros de leitura.
 * 
 */
void StorageControllerClass::incrementReadErrorCount()
{
    Serial.printf("[%lu] Erros de leitura: %d | Codigo Erro: %d\n", millis(), m_readErrorCount + 1, SD.cardErrorCode());
    m_readErrorCount++;

    if (m_readErrorCount > 5)
    {
        resetStorage();
    }
}

/**
 * @brief Retornar a quantidade de erros de escrita.
 * 
 * @return int 
 */
int StorageControllerClass::getWriteErrorCount()
{
    return m_writeErrorCount;
}

/**
 * @brief Retornar a quantidade de erros de leitura.
 * 
 * @return int 
 */
int StorageControllerClass::getReadErrorCount()
{
    return m_readErrorCount;
}

/**
 * @brief Resetar o contador de erros de escrita.
 * 
 */
void StorageControllerClass::resetWriteErrorCount()
{
    m_writeErrorCount = 0;
}

/**
 * @brief Resetar o contador de erros de leitura.
 * 
 */
void StorageControllerClass::resetReadErrorCount()
{
    m_readErrorCount = 0;
}

/**
 * @brief Retornar o espaço livre no microSD.
 * 
 * @return int Espaço livre, em MB.
 */
void StorageControllerClass::resetStorage()
{
    // Resetando o armazenamento.
    Serial.printf(SCC_LOGD_0001);

    if (!SD.begin(m_gpioNumber, SDSPEED))
    {
        // Falha ao montar o SD card.
        Serial.printf(SCC_LOGE_0001);
        CacheStatic.SystemStatus.SDWorking = false;
    }
    else
    {
        // Resetando o contador de erros.
        Serial.printf(SCC_LOGD_0002);
        resetWriteErrorCount();
    }
}

/**
 * @brief Retornar o espaço livre no microSD.
 * 
 * @param useCache Define se é para usar o cache ou ler do SD novamente.
 * 
 * @return int Espaço livre, em MB.
 */
int StorageControllerClass::getFreeSpace(bool useCache)
{
    if (useCache)
        return m_freeSpace;

    uint32_t volFree = SD.vol()->freeClusterCount();
    float freeSpace = 0.000512 * volFree * SD.vol()->blocksPerCluster();
    m_freeSpace = (int)freeSpace;

    return m_freeSpace;
}

bool StorageControllerClass::appendFile(String fileName, String content, size_t &bytesAppended)
{
    bool result = false;

    xSemaphoreTake(m_semaphoreStorageController, portMAX_DELAY);

    File file = SD.open(fileName, FILE_WRITE);
    if (!file)
    {
        Serial.printf(SCC_LOGE_0012, fileName.c_str());
        incrementWriteErrorCount();
        result = false;
    }
    else
    {
        resetWriteErrorCount();
        bytesAppended = file.print(content);
        if (bytesAppended <= 0)
            Serial.printf(SCC_LOGE_0003);

        file.close();
        result = true;
    }

    xSemaphoreGive(m_semaphoreStorageController);
    return result;
}

bool StorageControllerClass::writeFile(String fileName, String content, size_t &bytesWritten)
{
    bool result = false;

    xSemaphoreTake(m_semaphoreStorageController, portMAX_DELAY);

    File file = SD.open(fileName, FILE_WRITE);
    if (!file)
    {
        Serial.printf(SCC_LOGE_0012, fileName.c_str());
        incrementWriteErrorCount();
    }
    else
    {
        resetWriteErrorCount();
        bytesWritten = file.write(content.c_str());
        if (bytesWritten <= 0)
            Serial.printf(SCC_LOGE_0003);

        file.close();
        result = true;
    }

    xSemaphoreGive(m_semaphoreStorageController);
    return result;
}

bool StorageControllerClass::printFile(String fileName, String content, size_t &bytesWritten)
{
    bool result = false;

    xSemaphoreTake(m_semaphoreStorageController, portMAX_DELAY);

    File file = SD.open(fileName, FILE_WRITE);
    if (!file)
    {
        Serial.printf(SCC_LOGE_0012, fileName.c_str());
        incrementWriteErrorCount();
    }
    else
    {
        resetWriteErrorCount();
        bytesWritten = file.print(content.c_str());
        if (bytesWritten <= 0)
            Serial.printf(SCC_LOGE_0003);

        file.close();
        result = true;
    }

    xSemaphoreGive(m_semaphoreStorageController);
    return result;
}

bool StorageControllerClass::moveFile(String sourcePath, String destinationPath, bool overwrite)
{
    bool result = false;

    xSemaphoreTake(m_semaphoreStorageController, portMAX_DELAY);

    if (SD.exists(destinationPath.c_str()))
    {
        if (overwrite)
            SD.remove(destinationPath.c_str());
        else
        {
            Serial.printf("O arquivo %s nao pode ser sobrescrito.", destinationPath.c_str());
            xSemaphoreGive(m_semaphoreStorageController);
            return false;
        }
        
    }

    result = SD.rename(sourcePath.c_str(), destinationPath.c_str());

    xSemaphoreGive(m_semaphoreStorageController);

    return result;
}

bool StorageControllerClass::removeFile(String path)
{
    Serial.printf("Removendo o arquivo %s\n", path.c_str());
    
    waitOne(portMAX_DELAY);
    bool result = SD.remove(path.c_str());
    releaseMutex();

    return result;
}

bool StorageControllerClass::removeDirectory(String path)
{
    Serial.printf("Removendo o diretorio %s\n", path.c_str());
    
    waitOne(portMAX_DELAY);
    bool result = SD.rmdir(path.c_str());
    releaseMutex();

    return result;
}

bool StorageControllerClass::fileExists(String path)
{
    waitOne(portMAX_DELAY);
    bool result = SD.exists(path.c_str());
    releaseMutex();

    return result;
}

bool StorageControllerClass::createFolder(String path)
{
    bool result = false;

    waitOne(portMAX_DELAY);
    SdFile sdRoot;

    if (!sdRoot.open(path.c_str()))
        result = SD.mkdir(path.c_str());
    else 
        result = true;
    sdRoot.close();
    releaseMutex();

    return result;
}

void StorageControllerClass::waitOne(TickType_t timeout)
{
    xSemaphoreTake(m_semaphoreStorageController, timeout);
}

void StorageControllerClass::releaseMutex()
{
    xSemaphoreGive(m_semaphoreStorageController);
}

bool StorageControllerClass::cleanFolder(String path)
{
    Serial.printf("Excluindo arquivos da pasta %s\n", path.c_str());

    if (path.length() <= 0)
    {
        Serial.printf("Caminho invalido\n");
        return false;
    }
    
    if (!fileExists(path))
    {
        Serial.printf("O caminho nao existe\n");
        return false;
    }

    waitOne(portMAX_DELAY);

    SdFile root;
    SdFile file;
    
    if (!root.open(path.c_str(), O_READ))
    {
        releaseMutex();
        Serial.printf("Nao eh possivel abrir a pasta %s\n", path.c_str());
        return false;
    }
    
    root.rewind();

    while(file.openNext(&root, O_READ))
    {
        char fileName[64];
        memset(fileName, 0, sizeof(fileName));
        file.getName(fileName, sizeof(fileName));
        file.close();

        if (!path.endsWith("/"))
            path.concat("/");

        String fullPath = path + String(fileName);
     
        if (SD.exists(fullPath.c_str()))
            SD.remove(fullPath.c_str());
    }
    
    releaseMutex();

    return true;  
}

void StorageControllerClass::check()
{
    if ((millis() - m_lastCheck) > SD_CHECK_INTERVAL)
    {
        m_lastCheck = millis();
        if (!CacheStatic.SystemStatus.SDWorking)
        {
            Serial.printf("Tentando recuperar o microSD\n");
            StorageController.begin(GPIO_NUM_5);
        }
    }
}

StorageControllerClass StorageController;
SdFat SD;