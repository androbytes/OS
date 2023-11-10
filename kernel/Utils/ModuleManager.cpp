#include "ModuleManager.h"
#include "Utils.hpp"

ModuleManager GlobalFileManager{};

ModuleManager::ModuleManager(volatile limine_module_request* request){
	m_ModuleRequest = request;
}

limine_file* ModuleManager::GetFile(const char* filePath){
	limine_module_response* moduleResponse = m_ModuleRequest->response;
	for(size_t i = 0; i < moduleResponse->module_count; i++){
		limine_file* file = moduleResponse->modules[i];
		if(endswith(file->path, filePath))
			return file;
	}
	return NULL;
}
