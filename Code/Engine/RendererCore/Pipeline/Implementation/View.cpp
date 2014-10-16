#include <RendererCore/PCH.h>
#include <RendererCore/Pipeline/RenderPipeline.h>
#include <RendererCore/Pipeline/View.h>

void ezView::SetName(const char* szName)
{
  m_sName.Assign(szName);

  ezStringBuilder sb = szName;
  sb.Append(".ExtractData");
  m_ExtractDataProfilingID = ezProfilingSystem::CreateId(sb.GetData());

  sb = szName;
  sb.Append(".Render");
  m_RenderProfilingID = ezProfilingSystem::CreateId(sb.GetData());
}

void ezView::ExtractData()
{
  EZ_ASSERT(IsValid(), "Cannot extract data from an invalid view");

  EZ_PROFILE(m_ExtractDataProfilingID);

  m_pRenderPipeline->ExtractData(*m_pWorld, *m_pLogicCamera);
}

void ezView::Render()
{
  EZ_ASSERT(IsValid(), "Cannot render an invalid view");

  EZ_PROFILE(m_RenderProfilingID);

  m_pRenderPipeline->Render(*m_pRenderCamera);
}
