/**
 * @file main.cpp
 * @author Vinicius de Sa
 * @brief Este exemplo demonstra o funcionamento da SdFat.
 * @version 0.1
 * @date 04/06/2019
 * 
 * @copyright Copyright VIDO.LA 2019: Todos os direitos reservados.
 */


#include <SPI.h>
#include "SdFat.h"

#define SD_CS_PIN SS
#define SDSPEED SD_SCK_MHZ(20) // result: 101 tracks in 771 ms
bool cleanFolder(String path);

SdFat SD;

void setup()
{
    Serial.begin(115200);
    while (!Serial);

    ESP_LOGI("", "Inicializando o SD card...");

    if (!SD.begin(SD_CS_PIN, SDSPEED))
    {
        ESP_LOGE("", "Falha na inicializacao. Reiniciando...");
        ESP.restart();
    }
    
    //Serial.println("SD inicializado! Abrindo arquivo teste.txt para escrita...");

    // file = SD.open("/teste.txt", FILE_WRITE);

    // if (file)
    // {
    //     ESP_LOGI("Escrevendo no arquivo teste.txt...");

    //     file.println("Teste de escrita.");
    //     file.close();
        
    //     ESP_LOGI("", "Dados escritos!");
    // }
    // else
    // {
    //     ESP_LOGE("", "Falha ao abrir o arquivo teste.txt.");
    // }

    // file = SD.open("/teste.txt");
    // String content = "";
    // if (file)
    // {
    //     while (file.available())
    //     {
    //         content += (char)file.read();
    //     }
        
    //     ESP_LOGI("", "Conteudo do arquivo: %s", content.c_str());

    //     file.close();
    // }
    // else
    // {
    //     ESP_LOGE("", "Falha ao abrir o arquivo teste.txt.");
    // }

    // SdFile root;
    
    // if (root.open("/LOG", O_READ))
    // {
    //     ESP_LOGD("", "Pasta aberta. Eh pasta? %d | Ta aberta? %d | Eh root? %d", root.isDir(), root.isOpen(), root.isRoot());

    //     SdFile file;

    //     root.rewind();

    //     while(file.openNext(&root, O_READ))
    //     {
    //         char fileName[64];
    //         memset(fileName, 0, sizeof(fileName));
    //         file.getName(fileName, sizeof(fileName));
    //         ESP_LOGD("", "File: %s", fileName);
    //         file.close();
    //     }

    //     //root.ls(&Serial);

    //     ESP_LOGD("", "Fim dos arquivos...");
    // }
    // else
    // {
    //     ESP_LOGE("", "Erro ao abrir a pasta");
    // }

    cleanFolder("/LOG");
}

bool cleanFolder(String path)
{
    SdFile root;
    SdFile file;
    
    if (!root.open(path.c_str(), O_READ))
    {
        ESP_LOGE("", "Erro ao abrir a pasta");
        return false;
    }
    
    ESP_LOGD("", "Pasta aberta. Eh pasta? %d | Ta aberta? %d | Eh root? %d", root.isDir(), root.isOpen(), root.isRoot());
    root.rewind();

    while(file.openNext(&root, O_READ))
    {
        char fileName[64];
        memset(fileName, 0, sizeof(fileName));
        file.getName(fileName, sizeof(fileName));
        ESP_LOGD("", "Arquivo: %s", fileName);
        file.close();

        if (!path.endsWith("/"))
            path.concat("/");

        String fullPath = path + String(fileName);
        ESP_LOGD("", "Excluindo %s", fullPath.c_str());

        if (SD.exists(fullPath.c_str()))
            SD.remove(fullPath.c_str());
    }

    ESP_LOGD("", "Fim dos arquivos...");
    return true;    
}

void loop()
{
}