#ifndef CS229WRITER_H
#define CS229WRITER_H

#include "iFileWriter.h"

using namespace std;

class CS229Writer : public iFileWriter {
public:
	void writeFile(AudioFile &file, string filename) { iFileWriter::writeFile(file, filename); }
	virtual void writeFile(AudioFile &file, ostream &os);

private:
};

#endif
