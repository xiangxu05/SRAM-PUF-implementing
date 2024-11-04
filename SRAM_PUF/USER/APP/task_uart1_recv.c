/*****************************************************
功能: 串口1接收解析
			
说明:	
******************************************************/
#include "task_uart1_recv.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "cmsis_armcc.h"
#include "usart.h"
#include "stdio.h"
#include "w25xx_SPI.h"
#include "sram_spi.h"
#include "spi.h"
#include <flashdb.h>
struct urt_aRxBuffer_t uart1_data;
int w25_id;
unsigned char buf_random[1024];
static char flg = 0;

__STATIC_INLINE void __set_FAULTMASK1(uint32_t faultMask)
{
  register uint32_t __regFaultMask       __ASM("faultmask");
  __regFaultMask = (faultMask & (uint32_t)1U);
}

//重复输入C重启，用于进入IAP更新程序
void check_goto_iap(unsigned char* buf)
{
		
		if(buf[0] == 'c' || buf[0] == 'C'){
				if(flg-- <= 0){
						__set_FAULTMASK1(1);
						NVIC_SystemReset();
				}
		}
}


//void encryption(....)
//{


//}

//#define FDB_LOG_TAG "[sample][tsdb]"
//static void lock(fdb_db_t db)
//{
//    __disable_irq();
//}

//static void unlock(fdb_db_t db)
//{
//    __enable_irq();
//}


//void test_flash_db()
//{
//		/* KVDB object */
//		static struct fdb_kvdb kvdb = { 0 };
//		static char buff[1024];
//		struct fdb_default_kv default_kv;
//		struct fdb_blob blob;
//		int temp_data = 36;
//	

//		fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_LOCK, (void *)lock);
//		fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_UNLOCK, (void *)unlock);
//		fdb_err_t result = fdb_kvdb_init(&kvdb, "env", "fdb_kvdb1", &default_kv, NULL);

//		if (result != FDB_NO_ERR) {
//				return;
//		}
//				
//		fdb_kv_print(&kvdb);
//		fdb_kv_set_blob(&kvdb, "temp1", fdb_blob_make(&blob, "temp1", sizeof("temp5")));
//		fdb_kv_set_blob(&kvdb, "temp2", fdb_blob_make(&blob, "temp2", sizeof("temp5")));
//		fdb_kv_set_blob(&kvdb, "temp3", fdb_blob_make(&blob, "temp3", sizeof("temp5")));
//		fdb_kv_set_blob(&kvdb, "temp4", fdb_blob_make(&blob, "temp4", sizeof("temp5")));
//		fdb_kv_set_blob(&kvdb, "temp5", fdb_blob_make(&blob, "temp5", sizeof("temp5")));
//		
//		FDB_INFO("get the 'boot_count' failed\n");
//		fdb_kv_get_blob(&kvdb, "temp1", fdb_blob_make(&blob, &buff, sizeof("temp5")));
//		if (blob.saved.len > 0) {
//				FDB_INFO("get the 'temp_data' value is %d %s\n", temp_data,buff);
//		} else {
//				FDB_INFO("get the 'boot_count' failed\n");
//		}
//		
//		fdb_kv_get_blob(&kvdb, "temp2", fdb_blob_make(&blob, &buff, sizeof("temp5")));
//		if (blob.saved.len > 0) {
//				FDB_INFO("get the 'temp_data' value is %d %s\n", temp_data,buff);
//		} else {
//				FDB_INFO("get the 'boot_count' failed\n");
//		}
//		
//		fdb_kv_get_blob(&kvdb, "temp3", fdb_blob_make(&blob, &buff, sizeof("temp5")));
//		if (blob.saved.len > 0) {
//				FDB_INFO("get the 'temp_data' value is %d %s\n", temp_data,buff);
//		} else {
//				FDB_INFO("get the 'boot_count' failed\n");
//		}
//		
//		fdb_kv_get_blob(&kvdb, "temp4", fdb_blob_make(&blob, &buff, sizeof("temp5")));
//		if (blob.saved.len > 0) {
//				FDB_INFO("get the 'temp_data' value is %d %s\n", temp_data,buff);
//		} else {
//				FDB_INFO("get the 'boot_count' failed\n");
//		}
//		
//		fdb_kv_get_blob(&kvdb, "temp5", fdb_blob_make(&blob, &buff, sizeof("temp5")));
//		if (blob.saved.len > 0) {
//				FDB_INFO("get the 'temp_data' value is %d %s\n", temp_data,buff);
//				fdb_kv_del(&kvdb, "temp5");
//		} else {
//				FDB_INFO("get the 'boot_count' failed\n");
//		}
//}




//串口数据接收解析
void task_uart1_rx(void const * argument)
{
//		w25_id = W25_Flash_ReadJedecID();
//		printf("W25q256 JedecID %x\r\n",w25_id);
//		sram_test();
//		test_flash_db();
    while(1){
        if( xQueueReceive( usart1Msg, &uart1_data, 1000) == pdPASS ){
						check_goto_iap(uart1_data.rx_buf);
						sram_read_random(0, buf_random,sizeof(buf_random),uart1_data.rx_buf[1]);  //获取随机数
						//dma_usart1_tx_start(uart1_data.rx_buf,uart1_data.rx_len);                 //发送数据	
					
						
        }  
        else{
            flg = 0;
        }
    }
  /* USER CODE END StartDefaultTask */
}
