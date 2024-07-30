#pragma once
class kinterface_t {
private:
	__int64( __fastcall* fnAddr )( void*, void*, void* ) = nullptr;

	typedef enum _request_codes
	{
		request_drv = 0xFF << 1,
		request_read = 0xFF << 2,
		request_write = 0xFF << 3,
		request_procbase = 0xFF << 4,
		request_modbase = 0xFF << 5,

		request_success = 0xFF << 200,
		request_unique = 0xFF << 202,
	}request_codes, * prequest_codes;

	typedef struct _drv_request {
		uintptr_t secret_code;
	} drv_request, * pdrv_request;

	typedef struct _read_request {
		uint32_t pid;
		uint32_t mmcopy;
		uintptr_t address;
		void* buffer;
		size_t size;
		size_t ret_size;
	} read_request, * pread_request;

	typedef struct _write_request {
		uint32_t pid;
		uintptr_t address;
		void* buffer;
		size_t size;
	} write_request, * pwrite_request;

	typedef struct _proc_request {
		uint32_t pid;
		uintptr_t ProcessBase;
		uintptr_t ModuleBase;
		wchar_t lpModName [ 32 ];
	} proc_request, * pproc_request;

	typedef struct _request_data
	{
		uint32_t unique;
		request_codes code;
		uint32_t status;
		void* data;
	}request_data, * prequest_data;
public:
	int tPID;
	uintptr_t ProcessBase;
	uintptr_t ModuleBase;

	int PID( const char* name );
	int GetProcessThreadNumByID( DWORD dwPID );
	bool Initialize( );
	bool SendCMD( void* data, request_codes code );

	uintptr_t GetProcessBase( int PID );
	uintptr_t GetModuleBase( int PID, LPCWSTR ModName );

	bool ReadPhysMemory( const int pid, const std::uintptr_t address, void* buffer, const std::size_t size, bool mmcopy = false, PDWORD_PTR num_bytes = 0 );
	bool WritePhysMemory( const int pid, const std::uintptr_t address, void* buffer, const std::size_t size );
};
inline kinterface_t* kinterface = new kinterface_t( );

inline bool ReadPhysMemory( const std::uintptr_t address, void* buffer, size_t size  )
{
	return kinterface->ReadPhysMemory( kinterface->tPID, address, buffer, size );
}

template <typename T>
inline T ReadPhysMemory( const std::uintptr_t address )
{
	T response { };
	kinterface->ReadPhysMemory( kinterface->tPID, address, &response, sizeof( T ) );
	return response;
}

template <typename T>
inline T WritePhysMemory( const std::uintptr_t address, T value )
{
	return kinterface->WritePhysMemory( kinterface->tPID, address, &value, sizeof( T ) );
}