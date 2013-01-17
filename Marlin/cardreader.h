#ifndef __CARDREADERH
#define __CARDREADERH

#ifdef SDSUPPORT

#include "SdFile.h"
enum LsAction {LS_SerialPrint,LS_Count,LS_GetFilename};
class CardReader
{
public:
  CardReader();
  
  void initsd();
  void write_command(char *buf);
  //files auto[0-9].g on the sd card are performed in a row
  //this is to delay autostart and hence the initialisaiton of the sd card to some seconds after the normal init, so the device is available quick after a reset

  void checkautostart(bool x); 
  void openFile(char* name,bool read);
  void closefile();
  void release();
  void startFileprint();
  void pauseSDPrint();
  void getStatus();
  void printingHasFinished();

  void getfilename(const uint8_t nr);
  uint16_t getnrfilenames();
  

  void ls();
  void chdir(const char * relpath);
  void updir();

  FORCE_INLINE bool eof() { return sdpos>=filesize ;};
  FORCE_INLINE int16_t get() {  sdpos = file.curPosition();return (int16_t)file.read();};
  FORCE_INLINE void setIndex(long index) {sdpos = index;file.seekSet(index);};
  FORCE_INLINE uint8_t percentDone(){if(!sdprinting) return 0; if(filesize) return sdpos*100/filesize; else return 0;};
  FORCE_INLINE char* getWorkDirName(){workDir.getFilename(filename);return filename;};

public:
  bool saving;
  bool sdprinting ;  
  bool cardOK ;
  char filename[11];
  bool filenameIsDir;
private:
  SdFile root,*curDir,workDir,workDirParent,workDirParentParent;
  Sd2Card card;
  SdVolume volume;
  SdFile file;
  uint32_t filesize;
  //int16_t n;
  unsigned long autostart_atmillis;
  uint32_t sdpos ;

  bool autostart_stilltocheck; //the sd start is delayed, because otherwise the serial cannot answer fast enought to make contact with the hostsoftware.
  
  LsAction lsAction; //stored for recursion.
  int16_t nrFiles; //counter for the files in the current directory and recycled as position counter for getting the nrFiles'th name in the directory.
  char* diveDirName;
  void lsDive(char *prepend,SdFile parent);
};
  

#else

#define dir_t bool 
class CardReader
{
public:
  FORCE_INLINE CardReader(){};
  
  FORCE_INLINE static void initsd(){};
  FORCE_INLINE static void write_command(char *buf){};
  
  FORCE_INLINE static void checkautostart(bool x) {}; 
  
  FORCE_INLINE static void openFile(char* name,bool read){};
  FORCE_INLINE static void closefile() {};
  FORCE_INLINE static void release(){};
  FORCE_INLINE static void startFileprint(){};
  FORCE_INLINE static void startFilewrite(char *name){};
  FORCE_INLINE static void pauseSDPrint(){};
  FORCE_INLINE static void getStatus(){};
  
  FORCE_INLINE static void selectFile(char* name){};
  FORCE_INLINE static void getfilename(const uint8_t nr){};
  FORCE_INLINE static uint8_t getnrfilenames(){return 0;};
  

  FORCE_INLINE static void ls() {};
  FORCE_INLINE static bool eof() {return true;};
  FORCE_INLINE static char get() {return 0;};
  FORCE_INLINE static void setIndex(){};
  FORCE_INLINE uint8_t percentDone(){return 0;};
};
#endif //SDSUPPORT
#endif