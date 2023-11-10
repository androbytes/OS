#include "limine.h"

class ModuleManager{
private:
	volatile limine_module_request* m_ModuleRequest;
public:
	ModuleManager() = default;
	ModuleManager(volatile limine_module_request*);
	limine_file* GetFile(const char* filePath);
};

extern ModuleManager GlobalFileManager;