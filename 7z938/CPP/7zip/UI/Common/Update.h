// Update.h

#ifndef __COMMON_UPDATE_H
#define __COMMON_UPDATE_H

#include "../../../Common/Wildcard.h"

#include "ArchiveOpenCallback.h"
#include "LoadCodecs.h"
#include "OpenArchive.h"
#include "Property.h"
#include "UpdateAction.h"
#include "UpdateCallback.h"

enum EArcNameMode
{
  k_ArcNameMode_Smart,
  k_ArcNameMode_Exact,
  k_ArcNameMode_Add,
};

struct CArchivePath
{
  UString OriginalPath;

  UString Prefix;   // path(folder) prefix including slash
  UString Name; // base name
  UString BaseExtension; // archive type extension or "exe" extension
  UString VolExtension;  // archive type extension for volumes

  bool Temp;
  FString TempPrefix;  // path(folder) for temp location
  FString TempPostfix;

  CArchivePath(): Temp(false) {};
  
  void ParseFromPath(const UString &path, EArcNameMode mode);
  UString GetPathWithoutExt() const { return Prefix + Name; }
  UString GetFinalPath() const;
  UString GetFinalVolPath() const;
  FString GetTempPath() const;
};

struct CUpdateArchiveCommand
{
  UString UserArchivePath;
  CArchivePath ArchivePath;
  NUpdateArchive::CActionSet ActionSet;
};

struct CCompressionMethodMode
{
  bool Type_Defined;
  COpenType Type;
  CObjectVector<CProperty> Properties;
  
  CCompressionMethodMode(): Type_Defined(false) {}
};

namespace NRecursedType { enum EEnum
{
  kRecursed,
  kWildcardOnlyRecursed,
  kNonRecursed
};}

struct CRenamePair
{
  UString OldName;
  UString NewName;
  bool WildcardParsing;
  NRecursedType::EEnum RecursedType;
  
  CRenamePair(): WildcardParsing(true), RecursedType(NRecursedType::kNonRecursed) {}

  bool Prepare();
  bool GetNewPath(bool isFolder, const UString &src, UString &dest) const;
};

struct CUpdateOptions
{
  CCompressionMethodMode MethodMode;

  CObjectVector<CUpdateArchiveCommand> Commands;
  bool UpdateArchiveItself;
  CArchivePath ArchivePath;
  EArcNameMode ArcNameMode;

  bool SfxMode;
  FString SfxModule;
  
  bool OpenShareForWrite;

  bool StdInMode;
  UString StdInFileName;
  bool StdOutMode;
  
  bool EMailMode;
  bool EMailRemoveAfter;
  UString EMailAddress;

  FString WorkingDir;
  NWildcard::ECensorPathMode PathMode;
  UString AddPathPrefix;

  CBoolPair NtSecurity;
  CBoolPair AltStreams;
  CBoolPair HardLinks;
  CBoolPair SymLinks;

  bool DeleteAfterCompressing;

  bool SetArcMTime;

  CObjectVector<CRenamePair> RenamePairs;

  bool InitFormatIndex(const CCodecs *codecs, const CObjectVector<COpenType> &types, const UString &arcPath);
  bool SetArcPath(const CCodecs *codecs, const UString &arcPath);

  CUpdateOptions():
    UpdateArchiveItself(true),
    SfxMode(false),
    StdInMode(false),
    StdOutMode(false),
    EMailMode(false),
    EMailRemoveAfter(false),
    OpenShareForWrite(false),
    ArcNameMode(k_ArcNameMode_Smart),
    PathMode(NWildcard::k_RelatPath),
    
    DeleteAfterCompressing(false),
    SetArcMTime(false)

      {};

  void SetActionCommand_Add()
  {
    Commands.Clear();
    CUpdateArchiveCommand c;
    c.ActionSet = NUpdateArchive::k_ActionSet_Add;
    Commands.Add(c);
  }

  CRecordVector<UInt64> VolumesSizes;
};

struct CErrorInfo
{
  DWORD SystemError;
  FString FileName;
  FString FileName2;
  UString Message;
  // UStringVector ErrorPaths;
  // CRecordVector<DWORD> ErrorCodes;
  CErrorInfo(): SystemError(0) {};
};

struct CUpdateErrorInfo: public CErrorInfo
{
};

#define INTERFACE_IUpdateCallbackUI2(x) \
  INTERFACE_IUpdateCallbackUI(x) \
  virtual HRESULT OpenResult(const wchar_t *name, HRESULT result, const wchar_t *errorArcType) x; \
  virtual HRESULT StartScanning() x; \
  virtual HRESULT ScanProgress(UInt64 numFolders, UInt64 numFiles, UInt64 totalSize, const wchar_t *path, bool isDir) x; \
  virtual HRESULT CanNotFindError(const wchar_t *name, DWORD systemError) x; \
  virtual HRESULT FinishScanning() x; \
  virtual HRESULT StartArchive(const wchar_t *name, bool updating) x; \
  virtual HRESULT FinishArchive() x; \

struct IUpdateCallbackUI2: public IUpdateCallbackUI
{
  INTERFACE_IUpdateCallbackUI2(=0)
};

HRESULT UpdateArchive(
    CCodecs *codecs,
    const CObjectVector<COpenType> &types,
    const UString &cmdArcPath2,
    NWildcard::CCensor &censor,
    CUpdateOptions &options,
    CUpdateErrorInfo &errorInfo,
    IOpenCallbackUI *openCallback,
    IUpdateCallbackUI2 *callback,
    bool needSetPath);

#endif
