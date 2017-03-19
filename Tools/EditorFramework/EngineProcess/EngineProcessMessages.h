#pragma once

#include <EditorFramework/Plugin.h>
#include <EditorFramework/IPC/ProcessCommunication.h>
#include <ToolsFoundation/Reflection/ReflectedType.h>
#include <ToolsFoundation/Object/DocumentObjectMirror.h>
#include <Core/Application/Config/FileSystemConfig.h>
#include <Core/Application/Config/PluginConfig.h>
#include <RendererCore/Pipeline/Declarations.h>

///////////////////////////////////// ezProcessMessages /////////////////////////////////////

///////////////////////////////////// Curator /////////////////////////////////////


class EZ_EDITORFRAMEWORK_DLL ezProcessAsset : public ezProcessMessage
{
  EZ_ADD_DYNAMIC_REFLECTION(ezProcessAsset, ezProcessMessage);
public:
  ezUuid m_AssetGuid;
  ezString m_sAssetPath;
};

class EZ_EDITORFRAMEWORK_DLL ezProcessAssetResponse : public ezProcessMessage
{
  EZ_ADD_DYNAMIC_REFLECTION(ezProcessAssetResponse, ezProcessMessage);
public:
  bool m_bSuccess;
};

///////////////////////////////////// ezEditorEngineMsg /////////////////////////////////////

/// \brief Base class for all messages between editor and engine that are not bound to any document
class EZ_EDITORFRAMEWORK_DLL ezEditorEngineMsg : public ezProcessMessage
{
  EZ_ADD_DYNAMIC_REFLECTION(ezEditorEngineMsg, ezProcessMessage);

public:
  ezEditorEngineMsg()
  {
  }

};

class EZ_EDITORFRAMEWORK_DLL ezUpdateReflectionTypeMsgToEditor : public ezEditorEngineMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezUpdateReflectionTypeMsgToEditor, ezEditorEngineMsg);

public:
  ezReflectedTypeDescriptor m_desc;
};

class EZ_EDITORFRAMEWORK_DLL ezSetupProjectMsgToEngine : public ezEditorEngineMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezSetupProjectMsgToEngine, ezEditorEngineMsg);

public:
  ezString m_sProjectDir;
  ezApplicationFileSystemConfig m_FileSystemConfig;
  ezApplicationPluginConfig m_PluginConfig;
};

class EZ_EDITORFRAMEWORK_DLL ezProjectReadyMsgToEditor : public ezEditorEngineMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezProjectReadyMsgToEditor, ezEditorEngineMsg);

public:

};

class EZ_EDITORFRAMEWORK_DLL ezSimpleConfigMsgToEngine : public ezEditorEngineMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezSimpleConfigMsgToEngine, ezEditorEngineMsg);

public:
  ezString m_sWhatToDo;

};

///////////////////////////////////// ezEditorEngineDocumentMsg /////////////////////////////////////

/// \brief Base class for all messages that are tied to some document.
class EZ_EDITORFRAMEWORK_DLL ezEditorEngineDocumentMsg : public ezProcessMessage
{
  EZ_ADD_DYNAMIC_REFLECTION(ezEditorEngineDocumentMsg, ezProcessMessage);

public:
  ezUuid m_DocumentGuid;
};

class EZ_EDITORFRAMEWORK_DLL ezSyncWithProcessMsgToEngine : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezSyncWithProcessMsgToEngine, ezEditorEngineDocumentMsg);
public:
  ezUInt32 m_uiRedrawCount;
};

class EZ_EDITORFRAMEWORK_DLL ezSyncWithProcessMsgToEditor : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezSyncWithProcessMsgToEditor, ezEditorEngineDocumentMsg);
public:
  ezUInt32 m_uiRedrawCount;
};


class EZ_EDITORFRAMEWORK_DLL ezEditorEngineViewMsg : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezEditorEngineViewMsg, ezEditorEngineDocumentMsg);

public:
  ezEditorEngineViewMsg()
  {
    m_uiViewID = 0xFFFFFFFF;
  }

  ezUInt32 m_uiViewID;
};

/// \brief For very simple uses cases where a custom message would be too much
class EZ_EDITORFRAMEWORK_DLL ezDocumentConfigMsgToEngine : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezDocumentConfigMsgToEngine, ezEditorEngineDocumentMsg);

public:
  ezString m_sWhatToDo;
  int m_iValue;
  float m_fValue;
  ezString m_sValue;
};

class EZ_EDITORFRAMEWORK_DLL ezDocumentOpenMsgToEngine : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezDocumentOpenMsgToEngine, ezEditorEngineDocumentMsg);

public:
  ezDocumentOpenMsgToEngine()
  {
    m_bDocumentOpen = false;
  }

  bool m_bDocumentOpen;
  ezString m_sDocumentType;
};

class EZ_EDITORFRAMEWORK_DLL ezDocumentOpenResponseMsgToEditor : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezDocumentOpenResponseMsgToEditor, ezEditorEngineDocumentMsg);

public:
  ezDocumentOpenResponseMsgToEditor()
  {
  }
};

class EZ_EDITORFRAMEWORK_DLL ezViewDestroyedMsgToEngine : public ezEditorEngineViewMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezViewDestroyedMsgToEngine, ezEditorEngineViewMsg);

};

class EZ_EDITORFRAMEWORK_DLL ezViewRedrawMsgToEngine : public ezEditorEngineViewMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezViewRedrawMsgToEngine, ezEditorEngineViewMsg);

public:

  ezUInt64 m_uiHWND;
  ezUInt16 m_uiWindowWidth;
  ezUInt16 m_uiWindowHeight;
  bool m_bUpdatePickingData;
  bool m_bEnablePickingSelected;

  ezInt8 m_iCameraMode; ///< ezCameraMode::Enum
  float m_fNearPlane;
  float m_fFarPlane;
  float m_fFovOrDim;
  ezUInt8 m_uiRenderMode; ///< ezViewRenderMode::Enum
  ezCameraUsageHint::Enum m_CameraUsageHint;

  ezVec3 m_vPosition;
  ezVec3 m_vDirForwards;
  ezVec3 m_vDirUp;
  ezVec3 m_vDirRight;
  ezMat4 m_ViewMatrix;
  ezMat4 m_ProjMatrix;
};

class EZ_EDITORFRAMEWORK_DLL ezEntityMsgToEngine : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezEntityMsgToEngine, ezEditorEngineDocumentMsg);

public:
  ezObjectChange m_change;
};

class EZ_EDITORFRAMEWORK_DLL ezExportDocumentMsgToEngine : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezExportDocumentMsgToEngine, ezEditorEngineDocumentMsg);

public:
  ezExportDocumentMsgToEngine() : m_uiAssetHash(0), m_uiVersion(0) {}

  ezString m_sOutputFile;
  ezUInt64 m_uiAssetHash;
  ezUInt16 m_uiVersion;
};

class EZ_EDITORFRAMEWORK_DLL ezExportDocumentMsgToEditor : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezExportDocumentMsgToEditor, ezEditorEngineDocumentMsg);

public:
  ezExportDocumentMsgToEditor() : m_bOutputSuccess(false) {}

  bool m_bOutputSuccess;
};

class EZ_EDITORFRAMEWORK_DLL ezCreateThumbnailMsgToEngine : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezCreateThumbnailMsgToEngine, ezEditorEngineDocumentMsg);

public:
  ezCreateThumbnailMsgToEngine() : m_uiWidth(256), m_uiHeight(256) {}
  ezUInt16 m_uiWidth;
  ezUInt16 m_uiHeight;
};

class EZ_EDITORFRAMEWORK_DLL ezCreateThumbnailMsgToEditor : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezCreateThumbnailMsgToEditor, ezEditorEngineDocumentMsg);

public:
  ezCreateThumbnailMsgToEditor() {}
  ezDataBuffer m_ThumbnailData; ///< Raw 8-bit RGBA data (256x256x4 bytes)
};

class EZ_EDITORFRAMEWORK_DLL ezViewPickingMsgToEngine : public ezEditorEngineViewMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezViewPickingMsgToEngine, ezEditorEngineViewMsg);

public:

  ezUInt16 m_uiPickPosX;
  ezUInt16 m_uiPickPosY;
};

class EZ_EDITORFRAMEWORK_DLL ezViewPickingResultMsgToEditor : public ezEditorEngineViewMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezViewPickingResultMsgToEditor, ezEditorEngineViewMsg);

public:
  ezUuid m_ObjectGuid;
  ezUuid m_ComponentGuid;
  ezUuid m_OtherGuid;
  ezUInt32 m_uiPartIndex;

  ezVec3 m_vPickedPosition;
  ezVec3 m_vPickedNormal;
  ezVec3 m_vPickingRayStartPosition;
};

class ezEditorEngineConnection;

class EZ_EDITORFRAMEWORK_DLL ezViewHighlightMsgToEngine : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezViewHighlightMsgToEngine, ezEditorEngineDocumentMsg);

public:
  void SendHighlightObjectMessage(ezEditorEngineConnection* pConnection);

  ezUuid m_HighlightObject;
  // currently used for highlighting which object the mouse hovers over
  // extend this message if other types of highlighting become necessary
};

class EZ_EDITORFRAMEWORK_DLL ezLogMsgToEditor : public ezEditorEngineMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezLogMsgToEditor, ezEditorEngineMsg);

public:
  ezString m_sText;
  ezString m_sTag;
  ezInt8 m_iMsgType;
  ezUInt8 m_uiIndentation;
};


class EZ_EDITORFRAMEWORK_DLL ezEditorEngineSyncObjectMsg : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezEditorEngineSyncObjectMsg, ezEditorEngineDocumentMsg);

public:

  ezUuid m_ObjectGuid;
  ezString m_sObjectType;
  ezDataBuffer m_ObjectData;

  const ezDataBuffer& GetObjectData() const { return m_ObjectData; }
  void SetObjectData(const ezDataBuffer& s) { m_ObjectData = s; }
};

class EZ_EDITORFRAMEWORK_DLL ezObjectTagMsgToEngine : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezObjectTagMsgToEngine, ezEditorEngineDocumentMsg);

public:
  ezObjectTagMsgToEngine()
  {
    m_bSetTag = false;
    m_bApplyOnAllChildren = false;
  }

  ezUuid m_ObjectGuid;
  ezString m_sTag;
  bool m_bSetTag;
  bool m_bApplyOnAllChildren;
};

class EZ_EDITORFRAMEWORK_DLL ezObjectSelectionMsgToEngine : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezObjectSelectionMsgToEngine, ezEditorEngineDocumentMsg);

public:

  ezString m_sSelection;
};

class EZ_EDITORFRAMEWORK_DLL ezSceneSettingsMsgToEngine : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezSceneSettingsMsgToEngine, ezEditorEngineDocumentMsg);

public:
  ezSceneSettingsMsgToEngine()
  {
    m_bSimulateWorld = false;
    m_fSimulationSpeed = 1.0f;
    m_bRenderOverlay = false;
    m_bRenderShapeIcons = false;
    m_bRenderSelectionBoxes = false;
    m_bAddAmbientLight = false;
    m_fGizmoScale = 0.0f;
    m_fGridDensity = 0.0f;
  }

  bool m_bSimulateWorld;
  float m_fSimulationSpeed;
  bool m_bRenderOverlay;
  bool m_bRenderShapeIcons;
  bool m_bRenderSelectionBoxes;
  bool m_bAddAmbientLight;
  float m_fGizmoScale;
  float m_fGridDensity;
  ezVec3 m_vGridCenter;
  ezVec3 m_vGridTangent1;
  ezVec3 m_vGridTangent2;
};

class EZ_EDITORFRAMEWORK_DLL ezGameModeMsgToEngine : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION( ezGameModeMsgToEngine, ezEditorEngineDocumentMsg);

public:
  bool m_bEnablePTG;
};

class EZ_EDITORFRAMEWORK_DLL ezGameModeMsgToEditor : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezGameModeMsgToEditor, ezEditorEngineDocumentMsg);

public:
  bool m_bRunningPTG;
};

class EZ_EDITORFRAMEWORK_DLL ezQuerySelectionBBoxMsgToEngine : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezQuerySelectionBBoxMsgToEngine, ezEditorEngineDocumentMsg);
public:
  ezUInt32 m_uiViewID; /// passed through to ezQuerySelectionBBoxResultMsgToEditor
  ezInt32 m_iPurpose; /// passed through to ezQuerySelectionBBoxResultMsgToEditor
};

class EZ_EDITORFRAMEWORK_DLL ezQuerySelectionBBoxResultMsgToEditor : public ezEditorEngineDocumentMsg
{
  EZ_ADD_DYNAMIC_REFLECTION(ezQuerySelectionBBoxResultMsgToEditor, ezEditorEngineDocumentMsg);
public:
  ezVec3 m_vCenter;
  ezVec3 m_vHalfExtents;

  ezUInt32 m_uiViewID; /// passed through from ezQuerySelectionBBoxMsgToEngine
  ezInt32 m_iPurpose; /// passed through from ezQuerySelectionBBoxMsgToEngine
};

