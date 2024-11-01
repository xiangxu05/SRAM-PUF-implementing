#include "octospi.h"
#include "main.h"
#include "ly68l6400.h"

/*
 *    < MPU config(importent):
 *        MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
 */

/**
  * @brief  Enable memory mapped mode for the OSPI memory.
  *         SPI/QPI; 1-1-1/4-4-4
  * @param  Ctx Component object pointer
  * @param  Mode Interface mode
  * @param  AddressSize Address size
  * @retval QSPI memory status
  */
int32_t LY68L6400_EnableMemoryMappedMode(OSPI_HandleTypeDef *Ctx, LY68L6400_Interface_t Mode)
{
  OSPI_RegularCmdTypeDef      s_command = {0};
  OSPI_MemoryMappedTypeDef s_mem_mapped_cfg = {0};

  s_command.FlashId            = HAL_OSPI_FLASH_ID_1;
  s_command.InstructionMode    = (Mode == LY68L6400_SPI_MODE) ? HAL_OSPI_INSTRUCTION_1_LINE : HAL_OSPI_INSTRUCTION_4_LINES;
  s_command.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
  s_command.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  s_command.AddressMode        = (Mode == LY68L6400_SPI_MODE) ? HAL_OSPI_ADDRESS_1_LINE : HAL_OSPI_ADDRESS_4_LINES;
  s_command.AddressSize        = HAL_OSPI_ADDRESS_24_BITS;
  s_command.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;
  s_command.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode           = (Mode == LY68L6400_SPI_MODE) ? HAL_OSPI_DATA_1_LINE : HAL_OSPI_DATA_4_LINES;
  s_command.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;
  s_command.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;
  s_command.Address            = 0;
  s_command.NbData             = 1;

  /* Initialize the read command */
  s_command.OperationType      = HAL_OSPI_OPTYPE_READ_CFG;
  s_command.Instruction        = (Mode == LY68L6400_SPI_MODE) ?  LY68L6400_FAST_READ_CMD : LY68L6400_QUAD_READ_CMD;
  s_command.DQSMode            = HAL_OSPI_DQS_DISABLE;
  s_command.DummyCycles        = (Mode == LY68L6400_SPI_MODE) ? DUMMY_CYCLES_READ : DUMMY_CYCLES_READ_QSPI;
  if (HAL_OSPI_Command(Ctx, &s_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return LY68L6400_ERROR;
  }
  
  /* Initialize the program command */
  s_command.OperationType      = HAL_OSPI_OPTYPE_WRITE_CFG;
  s_command.Instruction        = (Mode == LY68L6400_SPI_MODE) ? LY68L6400_WRITE_CMD : LY68L6400_QUAD_WRITE_CMD;
  s_command.DQSMode            = HAL_OSPI_DQS_ENABLE;
  s_command.DummyCycles        = 0U;

  if (HAL_OSPI_Command(Ctx, &s_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return LY68L6400_ERROR;
  }
  


  /* Configure the memory mapped mode */
  s_mem_mapped_cfg.TimeOutActivation = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;

  if (HAL_OSPI_MemoryMapped(Ctx, &s_mem_mapped_cfg) != HAL_OK)
  {
    return LY68L6400_ERROR;
  }

  return LY68L6400_OK;
}

/* Read/Write Array Commands (3 Byte Address Command Set) *********************/
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
int32_t LY68L6400_Read(OSPI_HandleTypeDef *Ctx,  LY68L6400_Interface_t Mode, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  OSPI_RegularCmdTypeDef s_command = {0};

  /* Initialize the read command */
  s_command.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
  s_command.FlashId            = HAL_OSPI_FLASH_ID_1;
  s_command.InstructionMode    = (Mode == LY68L6400_SPI_MODE) ? HAL_OSPI_INSTRUCTION_1_LINE : HAL_OSPI_INSTRUCTION_4_LINES;
  s_command.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
  s_command.Instruction        = (Mode == LY68L6400_SPI_MODE) ?  LY68L6400_FAST_READ_CMD : LY68L6400_QUAD_READ_CMD;
  s_command.AddressMode        = (Mode == LY68L6400_SPI_MODE) ? HAL_OSPI_ADDRESS_1_LINE : HAL_OSPI_ADDRESS_4_LINES;
  s_command.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressSize        = HAL_OSPI_ADDRESS_24_BITS;
  s_command.Address            = ReadAddr;
  s_command.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode           = (Mode == LY68L6400_SPI_MODE) ? HAL_OSPI_DATA_1_LINE : HAL_OSPI_DATA_4_LINES;
  s_command.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles        = (Mode == LY68L6400_SPI_MODE) ? DUMMY_CYCLES_READ : DUMMY_CYCLES_READ_QSPI;
  s_command.NbData             = Size;
  s_command.DQSMode            = HAL_OSPI_DQS_DISABLE;
  s_command.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if (HAL_OSPI_Command(Ctx, &s_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return LY68L6400_ERROR;
  }

  /* Reception of the data */
  if (HAL_OSPI_Receive(Ctx, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return LY68L6400_ERROR;
  }

  return LY68L6400_OK;
}

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
int32_t LY68L6400_Write(OSPI_HandleTypeDef *Ctx,  LY68L6400_Interface_t Mode, uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
  OSPI_RegularCmdTypeDef s_command = {0};

  /* Initialize the program command */
  s_command.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
  s_command.FlashId            = HAL_OSPI_FLASH_ID_1;
  s_command.InstructionMode    = (Mode == LY68L6400_SPI_MODE) ? HAL_OSPI_INSTRUCTION_1_LINE : HAL_OSPI_INSTRUCTION_4_LINES;
  s_command.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
  s_command.Instruction        = (Mode == LY68L6400_SPI_MODE) ? LY68L6400_WRITE_CMD : LY68L6400_QUAD_WRITE_CMD;
  s_command.AddressMode        = (Mode == LY68L6400_SPI_MODE) ? HAL_OSPI_ADDRESS_1_LINE : HAL_OSPI_ADDRESS_4_LINES;
  s_command.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressSize        = HAL_OSPI_ADDRESS_24_BITS;
  s_command.Address            = WriteAddr;
  s_command.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode           = (Mode == LY68L6400_SPI_MODE) ? HAL_OSPI_DATA_1_LINE : HAL_OSPI_DATA_4_LINES;
  s_command.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles        = 0U;
  s_command.NbData             = Size;
  s_command.DQSMode            = HAL_OSPI_DQS_DISABLE;
  s_command.SIOOMode           = HAL_OSPI_SIOO_INST_ONLY_FIRST_CMD;

  /* Configure the command */
  if (HAL_OSPI_Command(Ctx, &s_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return LY68L6400_ERROR;
  }

  /* Transmission of the data */
  if (HAL_OSPI_Transmit(Ctx, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return LY68L6400_ERROR;
  }

  return LY68L6400_OK;
}

/**
  * @brief  Enter QSPI mode command
  *         SPI
  * @param  Ctx Component object pointer
  * @retval error status
  */
int32_t LY68L6400_EnterQSPIMode(OSPI_HandleTypeDef *Ctx)
{
  OSPI_RegularCmdTypeDef s_command = {0};

  /* Initialize the resume command */
  s_command.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
  s_command.FlashId            = HAL_OSPI_FLASH_ID_1;
  s_command.InstructionMode    = HAL_OSPI_INSTRUCTION_1_LINE;
  s_command.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
  s_command.Instruction        = LY68L6400_ENTER_QSPI_CMD;
  s_command.AddressMode        = HAL_OSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode           = HAL_OSPI_DATA_NONE;
  s_command.DummyCycles        = 0U;
  s_command.DQSMode            = HAL_OSPI_DQS_DISABLE;
  s_command.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if (HAL_OSPI_Command(Ctx, &s_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return LY68L6400_ERROR;
  }

  return LY68L6400_OK;
}

/**
  * @brief  Exit QSPI mode command
  *         QPI
  * @param  Ctx Component object pointer
  * @retval error status
  */
int32_t LY68L6400_ExitQSPIMode(OSPI_HandleTypeDef *Ctx)
{
  OSPI_RegularCmdTypeDef s_command = {0};

  /* Initialize the resume command */
  s_command.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
  s_command.FlashId            = HAL_OSPI_FLASH_ID_1;
  s_command.InstructionMode    = HAL_OSPI_INSTRUCTION_4_LINES;
  s_command.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
  s_command.Instruction        = LY68L6400_EXIT_QSPI_CMD;
  s_command.AddressMode        = HAL_OSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode           = HAL_OSPI_DATA_NONE;
  s_command.DummyCycles        = 0U;
  s_command.DQSMode            = HAL_OSPI_DQS_DISABLE;
  s_command.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if (HAL_OSPI_Command(Ctx, &s_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return LY68L6400_ERROR;
  }

  return LY68L6400_OK;
}

/**
  * @brief  Read chip ID command
  *         SPI
  * @param  Ctx Component object pointer
  * @retval error status
  */
int32_t LY68L6400_ReadID(OSPI_HandleTypeDef *Ctx, uint8_t *pData, uint32_t Size)
{
  OSPI_RegularCmdTypeDef s_command = {0};

  /* Initialize the read command */
  s_command.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
  s_command.FlashId            = HAL_OSPI_FLASH_ID_1;
  s_command.InstructionMode    = HAL_OSPI_INSTRUCTION_1_LINE;
  s_command.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
  s_command.Instruction        = LY68L6400_READ_ID;
  s_command.AddressMode        = HAL_OSPI_ADDRESS_1_LINE;
  s_command.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;
  s_command.AddressSize        = HAL_OSPI_ADDRESS_24_BITS;
  s_command.Address            = 0;
  s_command.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode           = HAL_OSPI_DATA_1_LINE;
  s_command.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;
  s_command.DummyCycles        = 0;
  s_command.NbData             = Size;
  s_command.DQSMode            = HAL_OSPI_DQS_DISABLE;
  s_command.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if (HAL_OSPI_Command(Ctx, &s_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return LY68L6400_ERROR;
  }

  /* Reception of the data */
  if (HAL_OSPI_Receive(Ctx, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return LY68L6400_ERROR;
  }

  return LY68L6400_OK;
}

/**
  * @brief  Chip RESET command
  *         SPI,QPI
  * @param  Ctx Component object pointer
  * @retval error status
  */
int32_t LY68L6400_Reset(OSPI_HandleTypeDef *Ctx)
{
  OSPI_RegularCmdTypeDef s_command = {0};

  /* Initialize the resume command */
  s_command.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
  s_command.FlashId            = HAL_OSPI_FLASH_ID_1;
  s_command.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
  s_command.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
  s_command.AddressMode        = HAL_OSPI_ADDRESS_NONE;
  s_command.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode           = HAL_OSPI_DATA_NONE;
  s_command.DummyCycles        = 0U;
  s_command.DQSMode            = HAL_OSPI_DQS_DISABLE;
  s_command.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

  s_command.InstructionMode    = HAL_OSPI_INSTRUCTION_4_LINES;
  s_command.Instruction        = LY68L6400_RESET_EN;
  /* QSPI Send the command */
  if (HAL_OSPI_Command(Ctx, &s_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return LY68L6400_ERROR;
  }

  s_command.Instruction        = LY68L6400_RESET;
  /* QSPI  Send the command */
  if (HAL_OSPI_Command(Ctx, &s_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return LY68L6400_ERROR;
  }
  
  s_command.InstructionMode    = HAL_OSPI_INSTRUCTION_1_LINE;
  s_command.Instruction        = LY68L6400_RESET_EN;
  /* SPI Send the command */
  if (HAL_OSPI_Command(Ctx, &s_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return LY68L6400_ERROR;
  }
  
  s_command.Instruction        = LY68L6400_RESET;
  /* SPI Send the command */
  if (HAL_OSPI_Command(Ctx, &s_command, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return LY68L6400_ERROR;
  }  
  return LY68L6400_OK;
}
