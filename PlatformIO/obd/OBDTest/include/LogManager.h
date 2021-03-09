/**
 * @file LogManager.h
 * @author Vinicius de Sa (vinicius@vido-la.com)
 * @brief Classe que gerencia o armazenamento em logs.
 * @version 0.2
 * @date 16/07/2018
 * 
 * @copyright Copyright VIDO.LA 2019: Todos os direitos reservados.
 */
#pragma once

#include <Arduino.h>
#include "CacheStatic.h"
#include "CommonStorage.h"
#include "StructLibrary.h"

#define LOG_INTERVAL 100
#define LOG_ERROR_MESSAGE_INTERVAL 10000

#define MAX_LOG_FILE_SIZE 10485760UL // Tamanho maximo do arquivo, em MB
#define LOG_LINE_END "\r\n"
#define DEFAULT_LOG_FOLDER "/LOG/"
#define LOG_FILE_EXTENSION ".txt"
#define NAMESPACE_LOG "log"
#define KEY_LOG_FILE_NUMBER "file_number"

#define TAG_LOG "[LOG]"
#define LOG_LOGD_0001 "Carregando o arquivo de log.\n"
#define LOG_LOGD_0002 "Numero do arquivo de log: %lu\n"
#define LOG_LOGD_0003 "Abrindo arquivo de log %s\n"
#define LOG_LOGD_0004 "Arquivo de log aberto: %s (%u bytes livres)\n"
#define LOG_LOGD_0005 "Inicializando o sistema de log.\n"
#define LOG_LOGD_0006 "Criando um arquivo de log numero %u.\n"

#define LOG_LOGE_0001 "O arquivo de log %s nao existe.\n"
#define LOG_LOGE_0002 "Falha ao escrever o conteudo no arquivo de log.\n"
#define LOG_LOGE_0003 "Log desabilitado. SD com problemas.\n"

class LogManagerClass
{
public:
    /**
     * @brief Inicializar o objeto LogManagerClass.
     * 
     */
    LogManagerClass();
    /**
     * @brief Inicializar o objeto com as configurações adequadas.
     * 
     */
    void begin();
    /**
     * @brief Logar dados de telemetria.
     * 
     * @param data Dados a serem logados.
     */
    void logData(String data);

private:    
    uint32_t m_fileNumber;              // Número do arquivo de log atual.
    uint32_t m_remainingBytes;          // Número de bytes remanescentes no arquivo atual.
    unsigned long m_lastLogTimeStamp;   // Último timeStamp do log atual.
    unsigned long m_lastTry;

    /**
     * @brief Carregar o arquivo de log atual a partir do que foi armazenado em memória.
     * 
     */
    bool loadLogFile();
    /**
     * @brief Retornar o nome do arquivo de log.
     * 
     * @return String 
     */
    String getLogFileName();
    /**
     * @brief Criar um novo arquivo de log.
     * 
     * @return String 
     */
    String createLogFile();
    /**
     * @brief Escrever um conteúdo no arquivo de log atual.
     * 
     * @param content Conteúdo a ser escrito.
     */
    void writeLog(String content);
};

extern LogManagerClass LogManager;