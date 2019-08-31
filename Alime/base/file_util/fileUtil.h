/*
	a mini logger modified from muduo,
	which we are using to recreate as a
	conponent in Alime. 
*/
#include <sys/types.h>  // for off_t
#include "alimeType.h"




class AppendFile
{
public:
	explicit AppendFile(String_view filename);

	~AppendFile();

	void append(const char* logline, size_t len);
	void flush();

	off_t writtenBytes() const;

private:
  size_t write(const char* logline, size_t len);

private:
  FILE* fp_;
  char buffer_[64*1024];
  off_t writtenBytes_;
};

