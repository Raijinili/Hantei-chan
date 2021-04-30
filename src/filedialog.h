#include <string>

namespace fileType
{
enum {
	HA6,
	CG
};
}

std::string FileDialog(int fileType = -1);
