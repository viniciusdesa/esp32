/**
 * @file QMC5883L.h
 * @author Vinicius de Sá (vinicius@vido-la.com)
 * @brief 
 * @version 0.1
 * @date 28/07/2020
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <Arduino.h>
#include <Wire.h>

#define QMC5883_ADDRESS 0x0D // Endereço I2C do módulo

//////////////////////////////////////////////////////
//                  REGISTRADORES                   //
//////////////////////////////////////////////////////
/**
 * @brief Modo de funcionamento Standby
 * @remarks É o modo padrão após o POR (Power On Reset) e o Soft Reset. Somente algumas funções estão ativadas neste modo,
 * que mantém o consumo o mais baixo possível. Neste estado, valores de registradores são mantidos por um
 * LDO de ultra baixo consumo. A interface I2C pode ser acordada lendo ou escrevendo em quaisquer registradores.
 * Não há medição neste modo. O clock interno também é interrompido.
 */
#define QMC_MODE_STANDBY 0b00000000
/**
 * @brief Modo de funcionamento Medição Contínua
 * @remarks Durante este modo (bits = 01),  o sensor magnético faz medições contínuas e coloca os dados medidos em
 * registradores de data output. Os registradores field range (ou sensitivity) e o data output rate também estão
 * localizados no registrador de controle (09H). Eles devem ser configurados apropriadamente para suas aplicações no
 * modo de Medição Contínua.
 * 
 * Por exemplo, se a aplicação requerir uma taxa de saída de dados de 50Hz, o bit ODR no registrador 09H deve ser 01.
 * Se o field range é +/- 8 Gauss, o RNG = 01.
 * O over sample rate é de uso opcional. O valor padrão OSR = 00. Se sua aplicação tem resolução suficiente e necessida
 * de um baixo consumo de energia, você deve reduzir o OSR para um número baixo, tal como OSR=10 (128) ou OSR=11 (64).
 * 
 * No modo de Medição Contínua, os dados do sensor magnético são automaticamente compensados para efeitos de
 * temperatura e offset. Os ganhos são calibrados na fábrica.
 */
#define QMC_MODE_CONTINUOUS 0b00000001

/* Valores para o registrador ODR */
#define QMC_ODR_10Hz 0b00000000     // 0x00
#define QMC_ODR_50Hz 0b00000100     // 0x04
#define QMC_ODR_100Hz 0b00001000    // 0x08
#define QMC_ODR_200Hz 0b00001100    // 0x0C

/* Valores para o registrador RNG */
#define QMC_RNG_2G 0b00000000       // 0x00
#define QMC_RNG_8G 0b00010000       // 0x10

/* Valores para o registrador OSR */
#define QMC_OSR_512 0b00000000      // 0x00
#define QMC_OSR_256 0b01000000      // 0x40
#define QMC_OSR_128 0b10000000      // 0x80
#define QMC_OSR_64 0b11000000       // 0xC0

/* Valores (bits) para o registrador STATUS */
#define QMC_STATUS_DRDY 1
#define QMC_STATUS_OVL 2
#define QMC_STATUS_DOR 4

/**
 * @brief Define os registradores utilizados no sistema.
 * 
 */
enum QMCRegisters_e
{
    /**
     * @brief Armazena a medição de cada eixo magnético no modo de Medição Contínua. Os dados são atualizados
     * periodicamente com base na taxa de atualização de dados ODR definida nos registradores de controle. Os dados 
     * permanecem os mesmos, independentemente da leitura de status até que novos dados os substituam.
     * Cada eixo tem um valor de 16 bits com complemento de dois, isto é, o MSB do 01H/03H/05H indica o sinal de cada eixo.
     * Os dados de cada canal saturam a -32768 a 32767.
     * 
     */
    QMC_REG_XOUT_LSB = 0x00, // Data output do eixo X (LSB)
    QMC_REG_XOUT_MSB = 0x01, // Data output do eixo X (MSB)
    QMC_REG_YOUT_LSB = 0x02, // Data output do eixo Y (LSB)
    QMC_REG_YOUT_MSB = 0x03, // Data output do eixo Y (MSB)
    QMC_REG_ZOUT_LSB = 0x04, // Data output do eixo Z (LSB)
    QMC_REG_ZOUT_MSB = 0x05, // Data output do eixo Z (MSB)
    /**
     * @brief Registrador de status.
     * Bit 0: Data Ready (DRDY), indica se os dados dos três eixos estão prontos para leitura 
     *        e os dados já foram carregados nos registradores de output. Se ler qqer registrador
     *        de dados, seu valor é setado para "0".
     * Bit 1: Overflow flag (OVL), indica se algum dado dos canais do sensor está fora do range.
     *        A saída de dados de cada eixo satura em -32768 e 32767, se qualquer um exceder este eixo,
     *        a flag OVL é setada em "1". Esta flag é zerada se a próxima medição voltar para o range correto.
     * Bit 2: Data Skip (DOR), indica se todos os canais dos registradores de output data estão sendo desconsiderados
     *        no modo de medição contínua. É resetado para "0" lendo quaisquer registradores de dados.     * 
     */
    QMC_REG_STATUS = 0x06,
    /**
     * @brief Registradores de Temperatura
     * @remarks Os registradores 07H e 08H armazenam os dados do sensor de temperatura. A saída é em complemento de dois.
     * O ganho do sensor de temperatura é calibrado em fábrica, mas seu ofset não é compensado, somente se a temperatura
     * relativa é precisa. O coeficiente de temperatura é por volta de 100 LSB/°C.
     * 
     */
    QMC_REG_TOUT_LSB = 0x07,
    QMC_REG_TOUT_MSB = 0x08,
    /**
     * @brief Registrador de Controle 1
     * @remarks Define os modos operacionais (MODE), taxa de atualização (ODR - output data rate),
     * faixa de medição do campo magnético ou sensibilidade dos sensores (RNG) e taxa de over sampling (OSR).
     * Bits 0-1: Mode (00 - Standard (default), 01 - Continuous, 10 - RFU, 11 - RFU)
     * Bits 2-3: ODR (00 - 10Hz, 01 - 50Hz, 10 - 100 Hz, 11 - 200Hz)
     * Bits 4-5: RNG (00 - 2 Gauss, 01 - 8 Gauss, 10 - RFU, 11 - RFU)
     * Bits 6-7: OSR (00 - 512, 01 - 256, 10 - 128, 11 - 64)
     */
    QMC_REG_CONFIG = 0x09,
    /**
     * @brief Registrador de Controle 2
     * @remarks Controla a habilitação do pino de interrupção (INT_ENB), função Point roll over (POL_PNT)
     * e soft reset (SOFT_RST).
     * Bit 0: INT_ENB (0 - habilita pino de interrupção, 1 - Desabilita pino de interrupção)
     * Bits 1-5: RFU
     * Bit 6: ROL_PNT (0 - normal, 1 - Point Roll Over habilitado)
     * Bit 7: SOFT_RST (0 - normal, 1 - Soft reset, restaura valor padrão de todos os registradores)
     */
    QMC_REG_CONFIG2 = 0x0A,
    /**
     * @brief Registrador de SET/RESET Period
     * @remarks Escrever 0x01
     */
    QMC_REG_RESET = 0x0B,
    QMC_REG_RFU_0C = 0x0C, // RFU
    QMC_REG_CHIP_ID = 0x0D     // RFU
};

/**
 * @brief Controla a bússola magnética de 3 eixos QST QMC5883L.
 * 
 */
class QMC5883LClass
{
public:
    /**
     * @brief Inicializar o QMC5883L.
     * 
     * @return true Em caso de sucesso.
     * @return false Em caso de falha.
     */
    bool begin();
    /**
     * @brief Configurar o endereço base do módulo.
     * 
     * @param address Endereço base do módulo.
     */
    void setAddress(uint8_t address);
    /**
     * @brief Configurar o funcionamento do módulo.
     * 
     * @param mode Pode ser QMC_MODE_CONTINUOUS ou QMC_MODE_STANDBY.
     * @param odr Pode ser QMC_ODR_10Hz, QMC_ODR_50Hz, QMC_ODR_100Hz ou QMC_ODR_200Hz.
     * @param rng Pode ser QMC_RNG_2G ou QMC_RNG_8G.
     * @param osr Pode ser QMC_OSR_64, QMC_OSR_128, QMC_OSR_256 ou QMC_OSR_512
     */
    bool setConfig(uint8_t mode, uint8_t odr, uint8_t rng, uint8_t osr);
    /**
     * @brief Realizar um soft reset.
     * @remarks Restaura os valores default dos registradores.
     * 
     */
    void softReset();
    /**
     * @brief Lê os eixos do magnetômetro.
     * 
     * @param x Eixo x raw.
     * @param y Eixo y raw.
     * @param z Eixo z raw.
     */
    int read(int16_t &x, int16_t &y, int16_t &z);
    /**
     * @brief Lê os eixos do magnetômetro.
     * 
     * @param x Eixo x raw.
     * @param y Eixo y raw.
     * @param z Eixo z raw.
     * @param heading Azimute calculado.
     */
    int read(int16_t &x, int16_t &y, int16_t &z, int &heading);
    /**
     * @brief Lê os eixos do magnetômetro.
     * 
     * @param x Eixo x raw.
     * @param y Eixo y raw.
     * @param z Eixo z raw.
     * @param heading Azimute calculado.
     */
    int read(int16_t &x, int16_t &y, int16_t &z, float &heading);
    /**
     * @brief Calcular o azimute com base no valor de dois eixos.
     * 
     * @param a Primeiro eixo.
     * @param b Segundo eixo.
     */
    float azimuth(uint16_t a, uint16_t b);
    /**
     * @brief Configura o registrador OSR (Oversampling).
     * 
     * @param osr Pode ter os seguintes valores:
     * QMC_OSR_64 - consumo de energia mínimo
     * QMC_OSR_128 - consumo de energia reduzido
     * QMC_OSR_256 - consumo de energia normal
     * QMC_OSR_512 - consumo de energia máximo
     * @param reconfig Indica se é para aplicar as configurações.
     * 
     * @return true Em caso de sucesso.
     * @return false Em caso de falha.
     
     * 
     * @remarks O OSR controla a largura de banda de um filtro digital interno.
     * Quanto maior o OSR menor o filtro de largura de banda, menor ruído in-band e maior consumo de energia.
     * Usado para balancear ruído e energia.
     */
    bool setOversampling(uint8_t osr, bool reconfig = true);
    /**
     * @brief Configura a sensibilidade do sensor.
     * 
     * @param range Pode ser os seguintes valores:
     * QMC_RNG_2G: 2 Gauss
     * QMC_RNG_8G: 8 Gauss
     * @param reconfig Indica se é para aplicar as configurações.
     * 
     * @return true Em caso de sucesso.
     * @return false Em caso de falha.     
     */
    bool setRange(int range, bool reconfig = true);
    /**
     * @brief Definir a taxa de amostragem.
     * 
     * @param rate Pode ter os seguintes valores:
     * QMC_ODR_10Hz
     * QMC_ODR_50Hz
     * QMC_ODR_100Hz
     * QMC_ODR_200Hz
     * @param reconfig Indica se é para aplicar as configurações.
     *      
     * @return true Em caso de sucesso.
     * @return false Em caso de falha.
     */
    bool setSamplingRate(uint8_t rate, bool reconfig = true);

    /**
     * @brief Configurar o modo de funcionamento.
     * 
     * @param mode Modo de funcionamento.
     * @param reconfig Indica se é para aplicar as configurações.
     * @return true Em caso de sucesso.
     * @return false Em caso de falha.
     */
    bool setMode(uint8_t mode, bool reconfig = true);

    int ready();

    void resetCalibration();

    int readHeading();
private:
    /**
     * @brief Escrever um valor em um determinado registrador.
     * 
     * @param registerAddress Registrador a ser escrito.
     * @param value Valor a ser escrito.
     */
    bool writeRegister(uint8_t registerAddress, uint8_t value);

    int readRegister(int registerToRead, uint8_t size);
    /**
     * @brief Reconfigurar o QMC5883L de acordo com as configurações ajustadas.
     * 
     */
    bool reconfig();
    /**
     * @brief Endereço I2C do módulo.
     * 
     */
    uint8_t m_address = QMC5883_ADDRESS;
    int16_t m_xhigh, m_xlow;
    int16_t m_yhigh, m_ylow;
    uint8_t m_addr;
    uint8_t m_mode;
    uint8_t m_rate;
    uint8_t m_range;
    uint8_t m_oversampling;
};

extern QMC5883LClass QMC5883L;