#include "FileUtil.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <io.h>
#include <fcntl.h>
#include <algorithm>


AppendFile::AppendFile(String_view filename)
  : fp_(::fopen(filename.data(), "a")),  // in linux, it should be 'ae' for O_CLOEXEC
    writtenBytes_(0)
{
  assert(fp_);
}

AppendFile::~AppendFile()
{
	flush();
  ::fclose(fp_);
}

void AppendFile::append(const char* logline, const size_t len)
{
  size_t n = write(logline, len);
  size_t remain = len - n;
  while (remain > 0)
  {
    size_t x = write(logline + n, remain);
    if (x == 0)
    {
      int err = ferror(fp_);
      if (err)
      {
        fprintf(stderr, "AppendFile::append() failed\n");
      }
      break;
    }
    n += x;
    remain = len - n; // remain -= x
  }

  writtenBytes_ += len;
}

void AppendFile::flush()
{
  ::fflush(fp_);
}

size_t AppendFile::write(const char* logline, size_t len)
{
  // #undef fwrite_unlocked
  return ::fwrite(logline, 1, len, fp_);
}

off_t AppendFile::writtenBytes() const
{
	return writtenBytes_;
}

//ReadSmallFile::ReadSmallFile(std::string_view filename)
//  : fd_(_open(filename.data(), _O_RDONLY | _O_EXCL)),
//    err_(0)
//{
//  buf_[0] = '\0';
//  if (fd_ < 0)
//  {
//    err_ = errno;
//  }
//}
//
//ReadSmallFile::~ReadSmallFile()
//{
//  if (fd_ >= 0)
//  {
//    ::_close(fd_); // FIXME: check EINTR
//  }
//}

// return errno
//int ReadSmallFile::readToString(int maxSize,std::string* content, int64_t* fileSize, int64_t* modifyTime, int64_t* createTime)
//{
//  //static_assert(sizeof(off_t) == 8, "_FILE_OFFSET_BITS = 64"); //in windows off_t=4byes
//  assert(content != NULL);
//  int err = err_;
//  if (fd_ >= 0)
//  {
//    content->clear();
//
//    if (fileSize)
//    {
//      struct stat statbuf;
//      if (::fstat(fd_, &statbuf) == 0)
//      {
//        if (S_IFREG&(statbuf.st_mode))
//        {
//          *fileSize = statbuf.st_size;
//          content->reserve(static_cast<int>(std::min<int64_t>(static_cast<int64_t>(maxSize), *fileSize)));
//        }
//        else if (S_IFDIR&(statbuf.st_mode))
//        {
//          err = EISDIR;
//        }
//        if (modifyTime)
//        {
//          *modifyTime = statbuf.st_mtime;
//        }
//        if (createTime)
//        {
//          *createTime = statbuf.st_ctime;
//        }
//      }
//      else
//      {
//        err = errno;
//      }
//    }
//
//    while (content->size() <static_cast<size_t>(maxSize))
//    {
//      size_t toRead = std::min<size_t>(static_cast<size_t>(maxSize) - content->size(), sizeof(buf_));
//      size_t n = ::_read(fd_, buf_, toRead);
//      if (n > 0)
//      {
//        content->append(buf_, n);
//      }
//      else
//      {
//        if (n < 0)
//        {
//          err = errno;
//        }
//        break;
//      }
//    }
//  }
//  return err;
//}

//int ReadSmallFile::readToBuffer(int* size)
//{
//  int err = err_;
//  if (fd_ >= 0)
//  {
//    size_t n =_read(fd_, buf_, sizeof(buf_)-1);
//    if (n >= 0)
//    {
//      if (size)
//      {
//        *size = static_cast<int>(n);
//      }
//      buf_[n] = '\0';
//    }
//    else
//    {
//      err = errno;
//    }
//  }
//  return err;
//}

