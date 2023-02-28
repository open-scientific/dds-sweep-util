#ifdef DLL_API
#else
#define DLL_API extern "C" _declspec(dllimport)
#endif
#define WINAPI __stdcall

//////////////////////////////////////////////////////////

DLL_API int WINAPI DDSSearch();
DLL_API BOOL WINAPI DDSSetFrequency(int index, double frequency, int* wavePointNum, int* TNum);		
DLL_API BOOL WINAPI DDSGetMeasure(int index, BOOL bFreq, double* measure);	
DLL_API BOOL WINAPI DDSSetSingleWave(int index, BOOL bSingle);				
DLL_API BOOL WINAPI DDSResetCounter(int index);	
DLL_API BOOL WINAPI DDSSetTrigger(int index, BOOL bInter, BOOL bEdge);	
DLL_API BOOL WINAPI DDSGetDigitalIn(int index, unsigned short* In);				
DLL_API BOOL WINAPI DDSSetDigitalOut(int index, unsigned short Out);		
DLL_API BOOL WINAPI DDSDownload(int index, unsigned short* buf, int number);
DLL_API BOOL WINAPI DDSSetDIOMode(int index, BOOL mode);		
DLL_API BOOL WINAPI DDSCheck(int index);						
DLL_API BOOL WINAPI DDSSetPowerOnOutput(int index, BOOL bOpen);	

