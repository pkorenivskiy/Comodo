// dllmain.h : Declaration of module class.

class CScanSrvModule : public ATL::CAtlDllModuleT< CScanSrvModule >
{
public :
	DECLARE_LIBID(LIBID_ScanSrvLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SCANSRV, "{1283B86B-4357-45D9-B5EE-01165B73BA2E}")
};

extern class CScanSrvModule _AtlModule;
