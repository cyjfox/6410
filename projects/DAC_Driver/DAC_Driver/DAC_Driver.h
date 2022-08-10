#include <winioctl.h>
// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 DAC_DRIVER_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// DAC_DRIVER_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef DAC_DRIVER_EXPORTS
#define DAC_DRIVER_API __declspec(dllexport)
#else
#define DAC_DRIVER_API __declspec(dllimport)
#endif

// 此类是从 DAC_Driver.dll 导出的
class DAC_DRIVER_API CDAC_Driver {
public:
	CDAC_Driver(void);
	// TODO: 在此添加您的方法。
};

extern DAC_DRIVER_API int nDAC_Driver;

DAC_DRIVER_API int fnDAC_Driver(void);

// constants define
#define IOCTL_DAC_START \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 60, METHOD_BUFFERED, FILE_ANY_ACCESS) //60???
#define IOCTL_DAC_STOP \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 61, METHOD_BUFFERED, FILE_ANY_ACCESS)

