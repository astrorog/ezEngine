#pragma once

#include <EditorFramework/Preferences/Preferences.h>
#include <Foundation/Strings/String.h>

/// \brief Stores project specific preferences that are shared among all users
class EZ_EDITORFRAMEWORK_DLL ezProjectPreferencesShared : public ezPreferences
{
  EZ_ADD_DYNAMIC_REFLECTION(ezProjectPreferencesShared, ezPreferences);

public:
  ezProjectPreferencesShared();

};

/// \brief Stores project specific preferences for the current user
class EZ_EDITORFRAMEWORK_DLL ezProjectPreferencesUser : public ezPreferences
{
  EZ_ADD_DYNAMIC_REFLECTION(ezProjectPreferencesUser, ezPreferences);

public:
  ezProjectPreferencesUser();

  ezString m_sRenderPipelines;
};

