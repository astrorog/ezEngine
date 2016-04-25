#include <CoreUtils/PCH.h>

#include <CoreUtils/Image/ImageConversion.h>

EZ_BEGIN_SUBSYSTEM_DECLARATION(CoreUtils, Image)

  //BEGIN_SUBSYSTEM_DEPENDENCIES
  //END_SUBSYSTEM_DEPENDENCIES

  ON_BASE_STARTUP
  {
    ezImageFormat::Startup();
  }

EZ_END_SUBSYSTEM_DECLARATION



EZ_STATICLINK_FILE(CoreUtils, CoreUtils_Image_Implementation_SubSystem);

