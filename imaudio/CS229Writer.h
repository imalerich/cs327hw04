#ifndef CS229WRITER_H
#define CS229WRITER_H

#include "iFileWriter.h"

using namespace std;

class CS229Writer : public iFileWriter {
public:
	virtual void writeFile(AudioFile &file, string filename);
	virtual void writeFile(AudioFile &file, ostream &os);

private:
};

#endif
