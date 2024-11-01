#define userInfoStartAdd 520;//用int类型表示
#define fileInfoStartADD 4096;
#define MAXUSERADD 4096;
#define MAXFILEADD 268435456;//用int类型表示的256MB最大地址
struct userInfo{
	uint32_t userLabel[4];
	uint32_t firstAdd;
};

struct fileInfo{
	uint32_t fileLabel[4];
	uint32_t helpData[32];
	uint32_t nextAdd;
};
struct statuInfo{
	double userSpace;
	double fileSpace;
	uint8_t status;
	uint32_t userLabel[4];
	uint32_t fileLabel[4];
	uint32_t key[32];
};
typedef union{
	struct statuInfo data;
	uint8_t bytes[sizeof(struct statuInfo)];
}statuInfo_union;