/**
 * @file CommonStorage.h
 * @author Vinicius de Sa (vinicius@vido-la.com)
 * @brief Gerenciador de armazenamento.
 * @version 0.1
 * @date 18/04/2019
 * 
 * @copyright Copyright VIDO.LA 2019: Todos os direitos reservados.
 */
#pragma once

#include <Preferences.h>
#include <SPI.h>
#include "CacheStatic.h"
#include "SdFat.h"

#define SCC_LOGD_0001 "Resetando o armazenamento.\n"
#define SCC_LOGD_0002 "Resetando o contador de erros.\n"
#define SCC_LOGE_0001 "Falha ao montar o SD card.\n"
#define SCC_LOGE_0002 "Falha ao montar o SPIFFS.\n"
#define SCC_LOGE_0003 "Falha ao escrever o conteudo no arquivo.\n"

#define SCC_LOGE_0012 "Erro ao abrir o arquivo %s no modo append.\n"

#define TAG_SDCARD "[SDCARD]"

#define SDSPEED SD_SCK_MHZ(20) // result: 101 tracks in 771 ms
#define SD_CHECK_INTERVAL 120000

/**
 * @brief Gerenciador de armazenamento.
 * 
 */
class StorageControllerClass
{
    public:
        /**
         * @brief Inicializar o objeto StorageControllerClass.
         * 
         */
        StorageControllerClass();
        /**
         * @brief Inicializar o gerenciador de armazenamento.
         * 
         * @param gpioNumber Número do GPIO onde o SD está conectado.
         * @return true Caso a inicialização tenha sucesso.
         * @return false Caso a inicialização falhe.
         */
        bool begin(gpio_num_t gpioNumber);
        /**
         * @brief Incrementar o contador de erros de escrita.
         * 
         */
        void incrementWriteErrorCount();
        /**
         * @brief Retornar a quantidade de erros de escrita.
         * 
         * @return int 
         */
        int getWriteErrorCount();
        /**
         * @brief Resetar o contador de erros de escrita.
         * 
         */
        void resetWriteErrorCount();
        /**
         * @brief Incrementar o contador de erros de leitura.
         * 
         */
        void incrementReadErrorCount();
        /**
         * @brief Retornar a quantidade de erros de leitura.
         * 
         * @return int 
         */
        int getReadErrorCount();
        /**
         * @brief Resetar o contador de erros de leitura.
         * 
         */
        void resetReadErrorCount();
        /**
         * @brief Retornar o espaço livre no microSD.
         * 
         * @param useCache Define se é para usar o cache ou ler do SD novamente.
         * 
         * @return int Espaço livre, em MB.
         */
        int getFreeSpace(bool useCache);

        bool appendFile(String fileName, String content, size_t& bytesAppended);
        bool writeFile(String fileName, String content, size_t& bytesWritten);
        bool moveFile(String sourcePath, String destinationPath, bool overwrite);
        void waitOne(TickType_t timeout);
        void releaseMutex();
        bool fileExists(String path);
        bool removeFile(String path);
        bool removeDirectory(String path);
        bool createFolder(String path);
        bool printFile(String fileName, String content, size_t &bytesWritten);
        bool cleanFolder(String path);
        void check();
    private:
        void resetStorage();
        int m_writeErrorCount;
        int m_readErrorCount;
        gpio_num_t m_gpioNumber;
        int m_freeSpace;
        time_t m_lastCheck;
};

extern StorageControllerClass StorageController;
extern SdFat SD;