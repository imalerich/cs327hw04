#ifndef CS229WRITER_H
#define CS229WRITER_H

#include "iFileWriter.h"

using namespace std;

class CS229Writer : public iFileWriter {
public:
	void write_file(AudioFile &file, string filename) { iFileWriter::write_file(file, filename); }
	virtual void write_file(AudioFile &file, ostream &os);

private:
};

#endif
