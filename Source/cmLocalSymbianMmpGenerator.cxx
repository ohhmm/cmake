#include "cmLocalSymbianMmpGenerator.h"
#include "cmGlobalGenerator.h"
#include "cmMakefile.h"
#include "cmTarget.h"
#include "cmSourceFile.h"
#include "cmake.h"

#include <cctype>
#include <string>
#include <fstream>
#include <algorithm>

#define MMP_COLUMN_WIDTH 20
#define MMP_INDENT "  "

void cmLocalSymbianMmpGenerator::Generate()
{
  cmTargets::iterator i = Makefile->GetTargets().begin();
  for (; i != Makefile->GetTargets().end(); ++i)
    {
    if (i->second.GetType() == cmTarget::UTILITY)
      {
      writeMakefile(i->second);
      }
    else
      {
      writeMmp(i->second);
      }
    }
}

void cmLocalSymbianMmpGenerator::writeMmp(cmTarget& target)
{
  std::string targetName = target.GetName();
  std::string filename = Makefile->GetStartOutputDirectory();
  filename += "/" + targetName + ".mmp";
  std::ofstream mmp(filename.c_str());

  writeTargetType(target, mmp);
  addGenericOption(target, "UID", mmp);
  addGenericOption(target, "SECUREID", mmp);
  addGenericOption(target, "VENDORID", mmp);
  addGenericOption(target, "EPOCSTACKSIZE", mmp);
  addGenericOption(target, "EPOCHEAPSIZE", mmp);
  addGenericOption(target, "CAPABILITY", mmp);
  mmp << std::endl;

  addDefinitions(target, mmp);
  addResources(target, mmp);
  addIncludes(mmp);
  addSources(target, mmp);

  if (target.GetType() != cmTarget::STATIC_LIBRARY)
      addLibraries(target, mmp);
}

void cmLocalSymbianMmpGenerator::writeTargetType(cmTarget& target, std::ostream& mmp)
{
  std::string varName = target.GetName();
  varName += "_SYMBIAN_TARGETTYPE";
  const char* targettype = Makefile->GetDefinition(varName.c_str());

  if (!targettype)
    {
    switch (target.GetType())
      {
      case cmTarget::EXECUTABLE:
        targettype = "exe";
        break;
      case cmTarget::SHARED_LIBRARY:
        targettype = "dll";
        break;
      case cmTarget::STATIC_LIBRARY:
        targettype = "lib";
        break;
      }
    }

  mmp << keyword_with_param("TARGET") << target.GetName()
      << "." << targettype_extension(targettype) << std::endl;

  mmp << keyword_with_param("TARGETTYPE") << targettype << std::endl;
}

std::string cmLocalSymbianMmpGenerator::addGenericOption(cmTarget& target,
                                                         const std::string& option,
                                                         std::ostream& mmp)
{
  std::string varName = target.GetName();
  varName += "_SYMBIAN_" + option;
  const char* raw_value = Makefile->GetDefinition(varName.c_str());

  if (raw_value)
    {
    std::string value = raw_value;
    replaceSemicolonsWithSpaces(value);
    mmp << keyword_with_param(option) << value << std::endl;
    }

  return raw_value ? raw_value : "";
}

void cmLocalSymbianMmpGenerator::addIncludes(std::ostream& mmp)
{
  std::vector<std::string>& includes = Makefile->GetIncludeDirectories();
  std::vector<std::string>::iterator inc = includes.begin();
  for (; inc != includes.end(); ++inc)
    {
    std::string path = ConvertToRelativePath(StartOutputDirectoryComponents,
                                             inc->c_str());
    mmp << keyword_with_param("SYSTEMINCLUDE") << path << std::endl;
    }

  if (includes.size() > 0)
    {
       mmp << std::endl;
    }
}

void cmLocalSymbianMmpGenerator::addDefinitions(cmTarget& target, std::ostream& mmp)
{
  bool need_newline = false;

  if (writeMacros(mmp, Makefile->GetProperty("COMPILE_DEFINITIONS")))
      need_newline = true;

  if (writeMacros(mmp, target.GetProperty("COMPILE_DEFINITIONS")))
      need_newline = true;
  
  if (need_newline)
      mmp << std::endl;
}

void cmLocalSymbianMmpGenerator::addResources(cmTarget& target, std::ostream& mmp)
{
  const std::vector<cmSymbianResource>& resources = target.GetSymbianResources();
  std::vector<cmSymbianResource>::const_iterator res = resources.begin();
  for (; res != resources.end(); ++res)
    {
    if (res->type == cmSymbianResource::GENERIC)
      {
      writeGenericResource(*res, mmp);
      }
    else if (res->type == cmSymbianResource::BITMAP)
      {
      writeBitmap(*res, mmp);
      }
    }
}

void cmLocalSymbianMmpGenerator::addSources(cmTarget& target, std::ostream& mmp)
{
  std::vector<cmSourceFile*> const& sources = target.GetSourceFiles();
  std::vector<cmSourceFile*>::const_iterator src = sources.begin();

  for (; src != sources.end(); ++src)
    {
    std::vector<std::string>::const_iterator ext;
    ext = std::find(Makefile->GetHeaderExtensions().begin(),
                    Makefile->GetHeaderExtensions().end(),
                    (*src)->GetExtension());

    // skip headers
    if (ext != Makefile->GetHeaderExtensions().end())
      {
      continue;
      }
    
    mmp << keyword_with_param("SOURCE");
    mmp << ConvertToRelativePath(StartOutputDirectoryComponents,
                                 (*src)->GetFullPath().c_str());
    mmp << std::endl;
    }

  if (sources.size() > 0)
    {
    mmp << std::endl;
    }
}

void cmLocalSymbianMmpGenerator::addLibraries(cmTarget& target, std::ostream& mmp)
{
  cmTarget::LinkLibraryVectorType libraries = target.GetLinkLibraries();
  cmTarget::LinkLibraryVectorType::iterator lib = libraries.begin();
  for (; lib != libraries.end(); ++lib)
    {
    if (lib->second != cmTarget::IMPORT)
      {
      mmp << keyword_with_param("STATICLIBRARY");
      }
    else
      {
      mmp << keyword_with_param("LIBRARY");
      }
    mmp << lib->first;
    mmp << (cmSystemTools::StringEndsWith(lib->first.c_str(), ".lib") ? "" : ".lib");
    mmp << std::endl;
    }

  if (libraries.size() > 0)
    {
    mmp << std::endl;
    }
}

bool cmLocalSymbianMmpGenerator::writeMacros(std::ostream& mmp, const char* macros)
{
  if (! macros)
    return false;

  std::string values = macros;
  replaceSemicolonsWithSpaces(values);
  mmp << keyword_with_param("MACRO") << values << std::endl;
  return true;
}

void cmLocalSymbianMmpGenerator::writeGenericResource(const cmSymbianResource& res,
                                                      std::ostream& mmp)
{
  mmp << keyword_with_param("START RESOURCE");
  mmp << ConvertToRelativePath(StartOutputDirectoryComponents,
                               res.filename.c_str());
  mmp << std::endl;

  if (!res.target.empty())
    {
    mmp << MMP_INDENT << keyword_with_param("TARGET")
        << res.target << std::endl;
    }

  if (!res.targetpath.empty())
    {
    mmp << MMP_INDENT << keyword_with_param("TARGETPATH")
        << res.targetpath << std::endl;
    }

  if (res.header)
    {
    mmp << MMP_INDENT << "HEADER" << std::endl;
    }

  if (!res.lang.empty())
    {
    mmp << MMP_INDENT << keyword_with_param("LANG")
        << res.lang << std::endl;
    }

  if (!res.uid.empty())
    {
    mmp << MMP_INDENT << keyword_with_param("UID") << res.uid << std::endl;
    }

  mmp << "END" << std::endl << std::endl;
}

void cmLocalSymbianMmpGenerator::writeBitmap(const cmSymbianResource& res,
                                             std::ostream& mmp)
{
  mmp << keyword_with_param("START BITMAP") << res.target << std::endl;

  if (!res.targetpath.empty())
    {
    mmp << MMP_INDENT << keyword_with_param("TARGETPATH")
        << res.targetpath << std::endl;
    }

  if (res.header)
    {
    mmp << MMP_INDENT << "HEADER" << std::endl;
    }

  std::list<cmSymbianBitmapSource>::const_iterator img = res.images.begin();
  for (; img != res.images.end(); ++img)
    {
    mmp << MMP_INDENT << keyword_with_param("SOURCE");
    mmp << img->depth << " ";
    mmp << ConvertToRelativePath(StartOutputDirectoryComponents,
                                 img->filename.c_str());
    mmp << std::endl;
    }

  mmp << "END" << std::endl << std::endl;
}

std::string
cmLocalSymbianMmpGenerator::keyword_with_param(const std::string& option)
{
  std::string keyword = option;
  for (unsigned int i = 0; i < MMP_COLUMN_WIDTH - option.length() + 1; ++i)
    {
    keyword += ' ';
    }

  return keyword;
}

std::string
cmLocalSymbianMmpGenerator::targettype_extension(const std::string& targettype)
{
    return targettype;
}

void cmLocalSymbianMmpGenerator::writeMakefile(cmTarget& target)
{
  std::string targetName = target.GetName();
  std::string filename = Makefile->GetStartOutputDirectory();
  filename += "/" + targetName + ".mk";
  std::ofstream mk(filename.c_str());

  mk << "bld:" << std::endl;

  cmCustomCommand* cmd = target.GetSourceFiles()[0]->GetCustomCommand();

  if (cmd->GetWorkingDirectory())
    {
    std::string dir = cmd->GetWorkingDirectory();
#ifdef _WIN32
    for (unsigned int i = 0; i < dir.size(); ++i)
      {
      if (dir[i] == '/')
        {
        dir[i] = '\\';
        }
      }
#endif
      mk << "\t\tcd " <<  dir;
    }

  cmCustomCommandLines::const_iterator cmdline = cmd->GetCommandLines().begin();
  for (; cmdline != cmd->GetCommandLines().end(); ++cmdline)
    {
    mk << ";";
    std::vector<std::string>::const_iterator i = cmdline->begin();
    mk << *(i++);
    for (; i != cmdline->end(); ++i)
      {
      mk << " " << *i;
      }
    }

  mk << std::endl;
  mk << "makmake freeze lib cleanlib clean final resource savespace releaseables:";
  mk << std::endl << std::endl;
}

void cmLocalSymbianMmpGenerator::replaceSemicolonsWithSpaces(std::string& s)
{
  std::string::size_type semicolon_pos = s.find(';');
  while (semicolon_pos != std::string::npos)
    {
    s[semicolon_pos] = ' ';
    semicolon_pos = s.find(';', semicolon_pos);
    }
}
