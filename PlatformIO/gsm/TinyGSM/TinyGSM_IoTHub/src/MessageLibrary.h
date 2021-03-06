/*
 * messageLibrary.h
 *
 *  Created on: 4 de jun de 2018
 *      Author: Vinicius
 */

#ifndef MAIN_MESSAGELIBRARY_H_
#define MAIN_MESSAGELIBRARY_H_

#include <Arduino.h>

#define TAG_GPS "[GPS]"
#define TAG_MAIN "[MAIN]"
#define TAG_WP "[WP]"
#define TAG_MPU "[MPU6050]"
#define TAG_SDCARD "[SDCARD]"
#define TAG_LEDHANDLER "[LEDHANDLER]"
#define TAG_AUDIO "[AUDIO]"
#define TAG_GEOCOORDINATE "[GEOCOORDINATE]"
#define TAG_POIHANDLER "[POIHANDLER]"
#define TAG_POI "[POI]"
#define TAG_ALERTMANAGER "[ALERTMANAGER]"
#define TAG_CONNECTIONMANAGER "[CONNMGR]"

//#define SDCARD_ROOT "/sdcard/"
#define SDCARD_ROOT "/"
#define POI_FOLDER "POI/"

#define LOGE_0001 "Falha ao inicializar a MPU. Tentando novamente..."
#define LOGE_0002 "Falha critica ao inicializar a MPU. Reiniciando em 5 segundos...\n"
#define LOGE_0003 "Falha ao inicializar o MPU-6050.\n"
#define LOGE_0004 "Erro ao configurar os parametros do GPS.\n"
#define LOGE_0005 "Erro ao configurar os pinos do GPS.\n"
#define LOGE_0006 "Erro ao instalar o driver da serial do GPS.\n"
#define LOGE_0007 "Falha ao inicializar o GPS.\n"
#define LOGE_0008 "Falha ao montar o sistema de arquivos. Se voce deseja que o cartao seja formatado, defina format_if_mount_failed = true.\n"
#define LOGE_0009 "Falha ao inicializar o cartao %d. Garanta que os pinos do SD card estejam ligados com resistores de pull-up.\n"
#define LOGE_0010 "Erro ao abrir o arquivo %s no modo leitura.\n"
#define LOGE_0011 "Erro ao abrir o arquivo %s no modo escrita.\n"
#define LOGE_0012 "Erro ao abrir o arquivo %s no modo append.\n"
#define LOGE_0013 "Falha ao inicializar o SD CARD.\n"
#define LOGE_0014 "Falha ao inicializar o Gerenciador de LEDs.\n"
#define LOGE_0015 "Falha ao inicializar o Player de Audio.\n"
#define LOGE_0016 "Falha ao inicializar o Gerenciador de Botoes.\n"
#define LOGE_0017 "Erro ao abrir o arquivo de audio %s.\n"
#define LOGE_0018 "O LED %d ja foi ligado.\n"
#define LOGE_0019 "O LED %d ja foi desligado.\n"
#define LOGE_0020 "Falha ao adicionar os dados de telemetria no arquivo.\n"
#define LOGE_0021 "Falha ao montar o SD card."
#define LOGE_0022 "Nao ha cart??o SD conectado."
#define LOGE_0023 "Ponto de interesse de tipo %d nao identificado."
#define LOGE_0024 "GPS nao inicializado. Reiniciando o sistema em 5 segundos..."

#define LOGI_0001 "Iniciando o processamento."
#define LOGI_0002 "Parando o Worker Process."
#define LOGI_0003 "[%02d/%02d/%04d %02d:%02d:%02d.%03d] HDOP: %.2f | SAT: %d | LAT: %.6f | LON: %.6f"
#define LOGI_0004 "Accelerometer (X=%d, Y=%d, Z=%d) / Gyroscope (X=%d, Y=%d, Z=%d)"
#define LOGI_0005 "Desmontando unidade do SD card."
#define LOGI_0006 "Lendo o tamanho do arquivo %s"
#define LOGI_0007 "Lendo o tamanho do arquivo %s"
#define LOGI_0008 "Alocando %ld bytes para o arquivo %s."
#define LOGI_0009 "Abrindo o arquivo %s."
#define LOGI_0010 "Arquivo lido. Conteudo: %s."
#define LOGI_0011 "Gravando em %s: %s."
#define LOGI_0012 "Inicializando o Gerenciador de Leds."
#define LOGI_0013 "Reproduzindo audio a partir do arquivo"
#define LOGI_0014 "Lendo o cabecalho RIFF do audio."
#define LOGI_0015 "Cabecalho RIFF do audio lido corretamente."
#define LOGI_0016 "Inicializando o GPS."
#define LOGI_0017 "GPS inicializado."
#define LOGI_0018 "Lendo as propriedades do audio.\n"
#define LOGI_0019 "Propriedades do audio lidas corretamente.\n"
#define LOGI_0020 "Lendo o cabecalho de dados do audio."
#define LOGI_0021 "Cabecalho de dados do audio lido corretamente."
#define LOGI_0022 "Lendo o cabecalho data do audio."
#define LOGI_0023 "Cabecalho data do audio lido corretamente."
#define LOGI_0024 "Reproduzindo %d bytes de audio lidos."
#define LOGI_0025 "LEDs ligados: %d.\n"
#define LOGI_0026 "Inicializando o Gerenciador de Leds."
#define LOGI_0027 "Construtor da GeoCoordinate(lat=%.6f,lon=%.6f,azi=%.2f)\n"
#define LOGI_0028 "Obtendo POI para GeoCoordinate(lat=%.6f,lon=%.6f,azi=%.2f)\n"
#define LOGI_0029 "Executando auto-teste dos leds."
#define LOGI_0030 "Desligando todos os leds."
#define LOGI_0031 "Ligando todos os leds."
#define LOGI_0032 "Iniciando o SD card."
#define LOGI_0033 "Revisao do chip: %d"
#define LOGI_0034 "Frequencia do processador: %d MHz"
#define LOGI_0035 "Cart??o SD conectado: %s"
#define LOGI_0036 "Iniciando o timer dos alertas.\n"
#define LOGI_0037 "Alerta de curva a esquerda ja ativado.\n"
#define LOGI_0038 "Alerta de curva a esquerda. Vel.Max: %d km/h\n"
#define LOGI_0039 "Alerta de curva a direita ja ativado.\n"
#define LOGI_0040 "Alerta de curva a direita. Vel.Max: %d km/h\n"
#define LOGI_0041 "Contador alerta esquerda:"
#define LOGI_0042 "Contador alerta direita:"
#define LOGI_0043 "Contador alarme esquerda:"
#define LOGI_0044 "Contador alarme direita:"
#define LOGI_0045 "Alerta de curva a esquerda desligado.\n"
#define LOGI_0046 "Alerta de curva a direita desligado.\n"
#define LOGI_0047 "Alarme de curva a esquerda desligado.\n"
#define LOGI_0048 "Alarme de curva a direita desligado.\n"
#define LOGI_0049 "Alarme de curva a esquerda ja ativado.\n"
#define LOGI_0050 "Alarme de curva a esquerda. Vel.Max: "
#define LOGI_0051 "Alarme de curva a direita ja ativado.\n"
#define LOGI_0052 "Alarme de curva a direita. Vel.Max: "
#define LOGI_0053 "Header do audio encontrado. Lendo dados...\n"
#define LOGI_0054 "Inicializando o modem GSM\n"
#define LOGI_0055 "Modem:"
#define LOGI_0056 "Aguardando uma rede...\n"
#define LOGI_0057 "Tentando conectar em"
#define LOGI_0058 "Falha ao conectar o GPRS. Tentando novamente em 1s...\n"
#define LOGI_0059 "GPRS conectado!\n"
#define LOGI_0060 "Chip ID = %04X %08X"
#define LOGI_0061 "Versao do SDK: %s"
#define LOGI_0062 "Nenhum POI encontrado para (%.2f,%.2f)."
#define LOGI_0063 "Sistema Anjo ligado"
#define LOGI_0064 "Iniciando o Wave Player."
#define LOGI_0065 "Tamanho do SD Card: %lluMB"
#define LOGI_0066 "Tipo do SD Card: %s"
#define LOGI_0067 "Calibrando o acelerometro."
#define LOGI_0068 "Acelerometro calibrado."
#define LOGI_0069 "Iniciando o acelerometro."
#define LOGI_0070 "Acelerometro iniciado."
#define LOGI_0071 "Iniciando MPU. SDA: %d | SCL: %d"

#define LOG_PRESENTATION_1 "##################################################"
#define LOG_PRESENTATION_2 "##########   Anjo - Prova de Conceito   ##########"
#define LOG_PRESENTATION_3 "##################################################"

#define SD_CARD_TYPE_MMC "MMC"
#define SD_CARD_TYPE_SDSC "SDSC"
#define SD_CARD_TYPE_SDHC "SDHC"
#define SD_CARD_TYPE_UNKNOWN "DESCONHECIDO"

#define WAV_FILE_SYSTEM_IN_OPERATION "/systemop.wav"
#define WAV_FILE_SYSTEM_ON "/systemon.wav"

///////////////////////////////////////
// MENSAGENS PROTOCOLARES DO SISTEMA //
///////////////////////////////////////
// 01. GPS
// 01.999: Erros
#define MSG_ERROR_GPS_01 "#999,01,01\n" // GPS sem sinal
#define MSG_ERROR_GPS_02 "#999,01,02\n" // GPS timeout
#define MSG_ERROR_GPS_03 "#999,01,03\n" // "Nenhum dado recebido. Verifique o cabeamento."
#define MSG_ERROR_GPS_04 "#999,01,04\n" // GPS nao inicializado. Reiniciando o sistema em 5 segundos...
#define MSG_ERROR_GPS_05 "#999,01,05\n" // Posicao desatualizada.
// 02. PointOfInterestHandler
// 02.999: Erros
#define MSG_ERROR_POIHANDLER_01 "#999,02,01,%.4f,%.4f\n" // Nenhum POI encontrado

#endif /* MAIN_MESSAGELIBRARY_H_ */
