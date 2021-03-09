#include <Arduino.h>
#include "rom/miniz.h"
#include <stdio.h>

#define DATASIZE (1024*64)

void setup()
{    
    int x;
    char b;
    char *inbuf, *outbuf;
    tdefl_compressor *comp;
    tinfl_decompressor *decomp;
    tdefl_status status;
    size_t inbytes = 0, outbytes = 0, inpos = 0, outpos = 0, compsz;

    ESP_LOGI("", "Allocating data buffer and filling it with semi-random data");
    inbuf = (char *)malloc(DATASIZE);
    
    if (inbuf != NULL)
        ESP_LOGI("", "Conseguiu alocar inbuf");
    else
        ESP_LOGE("", "Nao conseguiu alocar inbuf");
    
    srand(0);

    for (x = 0; x < DATASIZE; x++)
    {
        inbuf[x] = (x & 1) ? rand() & 0xff : 0;
    }

    ESP_LOGI("", "Allocating compressor & outbuf (%d bytes)", sizeof(tdefl_compressor));
    comp = (tdefl_compressor *)malloc(sizeof(tdefl_compressor));

    if (comp != NULL)
        ESP_LOGI("", "Conseguiu alocar comp");
    else
        ESP_LOGE("", "Nao conseguiu alocar comp");

    outbuf = (char *)malloc(DATASIZE);
    if(outbuf != NULL)
        ESP_LOGI("", "Conseguiu alocar outbuf");
    else
        ESP_LOGE("", "Nao conseguiu alocar outbuf");

    ESP_LOGI("", "Compressing...");
    status = tdefl_init(comp, NULL, NULL, TDEFL_WRITE_ZLIB_HEADER | 1500);
    if (status == TDEFL_STATUS_OKAY)
        ESP_LOGI("", "Compressao iniciada");
    else
        ESP_LOGE("", "Compressao nao iniciada");
    
    
    while (inbytes != DATASIZE)
    {
        outbytes = DATASIZE - outpos;
        inbytes = DATASIZE - inpos;
        tdefl_compress(comp, &inbuf[inpos], &inbytes, &outbuf[outpos], &outbytes, TDEFL_FINISH);
        ESP_LOGI("", "...Compressed %d into %d bytes", inbytes, outbytes);
        inpos += inbytes;
        outpos += outbytes;
    }

    compsz = outpos;
    free(comp);

    //Kill inbuffer
    for (x = 0; x < DATASIZE; x++)
    {
        inbuf[x] = 0;
    }
    free(inbuf);

    inbuf = outbuf;
    
    outbuf = (char *)malloc(DATASIZE);
    if (outbuf != NULL)
        ESP_LOGI("", "Conseguiu alocar outbuf");
    else
        ESP_LOGE("", "Nao conseguiu alocar outbuf");

    ESP_LOGI("", "Reinflating...");
    decomp = (tinfl_decompressor *)malloc(sizeof(tinfl_decompressor));
    if (decomp != NULL)
        ESP_LOGI("", "Conseguiu alocar decomp");
    else
        ESP_LOGE("", "Nao conseguiu alocar decomp");

    tinfl_init(decomp);
    inpos = 0;
    outpos = 0;
    while (inbytes != compsz)
    {
        outbytes = DATASIZE - outpos;
        inbytes = compsz - inpos;
        tinfl_decompress(decomp, (const mz_uint8 *)&inbuf[inpos], &inbytes, (uint8_t *)outbuf, (mz_uint8 *)&outbuf[outpos], &outbytes, TINFL_FLAG_PARSE_ZLIB_HEADER);
        ESP_LOGI("", "...Decompressed %d into %d bytes", inbytes, outbytes);
        inpos += inbytes;
        outpos += outbytes;
    }

    ESP_LOGI("", "Checking if same...");
    srand(0);

    for (x = 0; x < DATASIZE; x++)
    {
        b = (x & 1) ? rand() & 0xff : 0;
        if (outbuf[x] != b)
        {
            ESP_LOGI("", "Pos %x: %hhx!=%hhx", x, outbuf[x], b);
        }
    }

    ESP_LOGI("", "Great Success!");
    free(inbuf);
    free(outbuf);
    free(decomp);

    while(1);
}

void loop()
{
    // put your main code here, to run repeatedly:
}