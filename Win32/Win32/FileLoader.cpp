#include"FileLoader.h"

void FileLoader::LoadObjFile(std::string fpath, Mesh * pMesh)
{
	std::ifstream file(fpath);
	//if (!file.good())
	//{
	//	DEBUG_MSG1("Import OBJ : Open File failed!!");
	//	return FALSE;
	//}

	std::vector<FLOAT3> pointList;//xyz buffer
	std::vector<FLOAT2> texcoordList;//texcoord buffer
	std::vector<FLOAT3> VNormalList;//vertex normal buffer
	std::vector<OBJ_vertexInfoIndex> vertexInfoList;//indices combination

	std::string currString;

	//,...............
	while (!file.eof())
	{
		file >> currString;
		if (currString == "v")
		{
			FLOAT3 currPoint(0, 0, 0);
			file >> currPoint.x >> currPoint.y >> currPoint.z;
			pointList.push_back(currPoint);
		}
		if (currString == "vn")
		{
			FLOAT3 currNormal(0, 0, 0);
			file >> currNormal.x >> currNormal.y >> currNormal.z;
			VNormalList.push_back(currNormal);
		}

		//texture coordinate "vt 1.0000000 0.0000000"
		if (currString == "vt")
		{
			FLOAT2 currTexCoord(0, 0);
			file >> currTexCoord.x >> currTexCoord.y;
			texcoordList.push_back(currTexCoord);
		}
		if (currString == "f")
		{
			for (UINT i = 0; i < 3; ++i)
			{
				//static UINT currUniqueVertexCount = 0;
				OBJ_vertexInfoIndex currVertex = { 0,0,0 };
				std::stringstream tmpSStream;
				std::string tmpString;

				//then a "1/2/3" "4/1/3" like string will be input
				file >> tmpString;

				//replace "/" with SPACE ,so STD::STRINGSTREAM will automatically format
				//the string into 3 separate part
				for (auto& c : tmpString) { if (c == '/')c = ' '; }

				tmpSStream << tmpString;
				tmpSStream >> currVertex.vertexID >> currVertex.texcoordID >> currVertex.vertexNormalID;
				--currVertex.texcoordID;
				--currVertex.vertexID;
				--currVertex.vertexNormalID;

				//non-existed element will be created
				BOOL IsVertexExist = FALSE;
				UINT  existedVertexIndex = 0;
				for (UINT j = 0; j < vertexInfoList.size(); j++)
				{
					//in DEBUG mode ,[] operator will be a big performance overhead
					if (vertexInfoList[j] == currVertex)
					{
						IsVertexExist = TRUE;
						existedVertexIndex = j;
						break;
					}
				}
				if (!IsVertexExist)
				{
					vertexInfoList.push_back(currVertex);
					//the newest vertex.....
					pMesh->m_indexbuffer.push_back(vertexInfoList.size() - 1);
				}
				else
				{
					pMesh->m_indexbuffer.push_back(existedVertexIndex);
				}
			}
		}
	}
	file.close();
	pMesh->m_vertexbuffer.resize(vertexInfoList.size());
	for (UINT i = 0; i < pMesh->m_vertexbuffer.size(); ++i)
	{
		Vertex tmpVertex (0,0,0,0,ARGB(0.5,0.0f,0.0f,0.0f));

		//several indices which can retrieve vertex information
		OBJ_vertexInfoIndex& indicesCombination = vertexInfoList.at(i);
		tmpVertex.m_Position = QVector(pointList.at(indicesCombination.vertexID).x, pointList.at(indicesCombination.vertexID).y, pointList.at(indicesCombination.vertexID).z,0.0f);
		tmpVertex.m_Normal = VNormalList.at(indicesCombination.vertexNormalID);
		tmpVertex.m_UV = texcoordList.at(indicesCombination.texcoordID);


		//.......
		pMesh->m_vertexbuffer.at(i) = (tmpVertex);
	}
}
bool FileLoader::LoadObjPpm(std::string fpath, int width,int height,QVector ** pixelbuffer)
{
	std::ifstream fileIn(fpath, std::ios::binary);
	if (fileIn.good() == FALSE)
	{
		//MessageBoxA(L"Load PPM : File Open Failed!!");
		return false;
	}

	//file head (some fxxking ascii string)
	UINT maxColorIntensity;

	//skip the file Format "P6/n"
	fileIn.seekg(3);
	std::streampos filePos = fileIn.tellg();
	fileIn >> width >> height >> maxColorIntensity;

	//skip the '/r'
	fileIn.seekg(1, std::ios::cur);

	//read the main color chunk (binary)
	unsigned char* byteBuff = new unsigned char[width*height * 3];
	fileIn.read((char*)byteBuff, 3 * width*height);
	//there is no need to resize pixelbuffer,it's size is set when construction


	fileIn.close();

	//output
	for (UINT i = 0; i < width*height; ++i)
	{
		QVector tempvec;
		tempvec = { byteBuff[3 * i] / 255.0f,byteBuff[3 * i + 1] / 255.0f,byteBuff[3 * i + 2] / 255.0f ,1.0f};
		pixelbuffer[i / width][i%width] = tempvec;
	}

	return TRUE;
}