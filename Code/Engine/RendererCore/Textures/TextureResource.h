#pragma once

#include <RendererCore/Basics.h>
#include <Core/ResourceManager/Resource.h>
#include <Core/ResourceManager/ResourceTypeLoader.h>
#include <Foundation/IO/MemoryStream.h>
#include <CoreUtils/Image/Image.h>
#include <RendererFoundation/Basics.h>

typedef ezResourceHandle<class ezTextureResource> ezTextureResourceHandle;

struct ezTextureResourceDescriptor
{
};

class EZ_RENDERERCORE_DLL ezTextureResource : public ezResource<ezTextureResource, ezTextureResourceDescriptor>
{
  EZ_ADD_DYNAMIC_REFLECTION(ezTextureResource);

public:
  ezTextureResource();

private:
  virtual ezResourceLoadDesc UnloadData(Unload WhatToUnload) override;
  virtual ezResourceLoadDesc UpdateContent(ezStreamReaderBase* Stream) override;
  virtual void UpdateMemoryUsage(MemoryUsage& out_NewMemoryUsage) override;
  virtual ezResourceLoadDesc CreateResource(const ezTextureResourceDescriptor& descriptor) override;

private:
  friend class ezRendererCore;

  const ezGALResourceViewHandle& GetGALTextureView() const { return m_hGALTexView; }
  const ezGALTextureHandle& GetGALTexture() const { return m_hGALTexture; }
  const ezGALSamplerStateHandle& GetGALSamplerState() const { return m_hSamplerState; }

private:
  ezGALTextureHandle m_hGALTexture;
  ezGALResourceViewHandle m_hGALTexView;
  ezGALSamplerStateHandle m_hSamplerState; // HACK
};


class EZ_RENDERERCORE_DLL ezTextureResourceLoader : public ezResourceTypeLoader
{
public:

  struct LoadedData
  {
    LoadedData() : m_Reader(&m_Storage) { }

    ezMemoryStreamStorage m_Storage;
    ezMemoryStreamReader m_Reader;
    ezImage m_Image;
  };

  virtual ezResourceLoadData OpenDataStream(const ezResourceBase* pResource) override;

  virtual void CloseDataStream(const ezResourceBase* pResource, const ezResourceLoadData& LoaderData) override;
};


