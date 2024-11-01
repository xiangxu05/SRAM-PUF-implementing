#ifndef LY68L6400_H
#define LY68L6400_H


#define LY68L6400_OK            (0)
#define LY68L6400_ERROR         (-1)

typedef enum {
  LY68L6400_SPI_MODE = 0,           /*!< 1-1-1 commands, Power on H/W default setting   */
  LY68L6400_QPI_MODE,               /*!< 4-4-4 commands                                 */
} LY68L6400_Interface_t;

#define DUMMY_CYCLES_READ       8U
#define DUMMY_CYCLES_READ_QSPI  6U

/***** READ/WRITE MEMORY Operations with 3-Byte Address ****************************/
#define LY68L6400_READ_CMD                      0x03U   /*!< Normal Read 3 Byte Address        (spi)1-1-1 */
#define LY68L6400_FAST_READ_CMD                 0x0BU   /*!< Fast Read 3 Byte Address          (spi)1-1-1 */
#define LY68L6400_QUAD_READ_CMD                 0xEBU   /*!< QUAD Read 3 Byte Address          (spi)1-4-4, (QPI)4-4-4 */
#define LY68L6400_WRITE_CMD                     0x02U   /*!< QUAD Write 3 Byte Address         (spi)1-1-1, (QPI)4-4-4 */
#define LY68L6400_QUAD_WRITE_CMD                0x38U   /*!< QUAD Write 3 Byte Address         (spi)1-4-4, (QPI)4-4-4 */
#define LY68L6400_ENTER_QSPI_CMD                0x35U   /*!< Enter Quad Mode                   (spi)1-0-0 */
#define LY68L6400_EXIT_QSPI_CMD                 0xF5U   /*!< Exit Quad Mode                    (QPI)4-0-0 */
#define LY68L6400_RESET_EN                      0x66U   /*!< Reset Enable                      (spi)1-0-0, (QPI)4-0-0 */
#define LY68L6400_RESET                         0x99U   /*!< Reset                             (spi)1-0-0, (QPI)4-0-0 */
#define LY68L6400_BURST_LEN                     0xC0U   /*!< Burst Length Config               (spi)1-0-0, (QPI)4-0-0 */
#define LY68L6400_READ_ID                       0x9FU   /*!< Read Chip ID                      (spi)1-1-1 */



/**
  * @brief  Enable memory mapped mode for the OSPI memory.
  *         SPI/QPI; 1-1-1/4-4-4
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  AddressSize Address size
  * @retval QSPI memory status
  */
int32_t LY68L6400_EnableMemoryMappedMode(OSPI_HandleTypeDef *Ctx, LY68L6400_Interface_t Mode);


/**
  * @brief  Reads an amount of data from the OSPI memory.
  *         SPI/QPI; 1-1-1/4-4-4
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  pData Pointer to data to be read
  * @param  ReadAddr Read start address
  * @param  Size Size of data to read
  * @retval QSPI memory status
  */
int32_t LY68L6400_Read(OSPI_HandleTypeDef *Ctx,  LY68L6400_Interface_t Mode, uint8_t *pData, uint32_t ReadAddr, uint32_t Size);

/**
  * @brief  Write an amount of data to the OSPI memory.
  *         SPI/QPI; 1-1-1/4-4-4
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  pData Pointer to data to be read
  * @param  WriteAddr Read start address
  * @param  Size Size of data to read
  * @retval QSPI memory status
  */
int32_t LY68L6400_Write(OSPI_HandleTypeDef *Ctx,  LY68L6400_Interface_t Mode, uint8_t *pData, uint32_t WriteAddr, uint32_t Size);

/**
  * @brief  Enter QSPI mode command
  *         SPI
  * @param  Ctx Component object pointer
  * @retval error status
  */
int32_t LY68L6400_EnterQSPIMode(OSPI_HandleTypeDef *Ctx);

/**
  * @brief  Exit QSPI mode command
  *         QPI
  * @param  Ctx Component object pointer
  * @retval error status
  */
int32_t LY68L6400_ExitQSPIMode(OSPI_HandleTypeDef *Ctx);

/**
  * @brief  Read chip ID command
  *         SPI
  * @param  Ctx Component object pointer
  * @retval error status
  */
int32_t LY68L6400_ReadID(OSPI_HandleTypeDef *Ctx, uint8_t *pData, uint32_t Size);

/**
  * @brief  Chip RESET command
  *         SPI,QPI
  * @param  Ctx Component object pointer
  * @retval error status
  */
int32_t LY68L6400_Reset(OSPI_HandleTypeDef *Ctx);
#endif