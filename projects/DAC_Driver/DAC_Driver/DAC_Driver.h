#include <winioctl.h>
// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� DAC_DRIVER_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// DAC_DRIVER_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef DAC_DRIVER_EXPORTS
#define DAC_DRIVER_API __declspec(dllexport)
#else
#define DAC_DRIVER_API __declspec(dllimport)
#endif

// �����Ǵ� DAC_Driver.dll ������
class DAC_DRIVER_API CDAC_Driver {
public:
	CDAC_Driver(void);
	// TODO: �ڴ�������ķ�����
};

extern DAC_DRIVER_API int nDAC_Driver;

DAC_DRIVER_API int fnDAC_Driver(void);

// constants define
#define IOCTL_DAC_START \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 60, METHOD_BUFFERED, FILE_ANY_ACCESS) //60???
#define IOCTL_DAC_STOP \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 61, METHOD_BUFFERED, FILE_ANY_ACCESS)

