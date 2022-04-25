#include "cAssimpModel.h"
//#include "ErrorLog.h"

cAssimpModel::cAssimpModel(void)
{
	this->m_pImporter = new Assimp::Importer();

	return;
}

cAssimpModel::~cAssimpModel(void)
{
	delete this->m_pImporter;

	return;
}

bool cAssimpModel::LoadModelFromFile(const std::string& file)
{
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most import ant aspect for you - you'll
	// probably to request more postprocessing that we do in thsi example.
	this->m_pScene = this->m_pImporter->ReadFile(file,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	// If the import failed, report it
	//if (this->m_pScene == false)
	//{
	//	ErrorLog::GetInstance()->WriteErrorMessageToLogFile(this->m_pImporter->GetErrorString());
	//	return false;
	//}

	// We're done.  Everything will be cleaned up by the importer destructor
	return true;
}
