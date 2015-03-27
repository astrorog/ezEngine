#include <GuiFoundation/PCH.h>
#include <GuiFoundation/ActionViews/MenuBarActionMapView.moc.h>
#include <GuiFoundation/Action/ActionMapManager.h>
#include <GuiFoundation/Action/ActionManager.h>
#include <GuiFoundation/ActionViews/QtProxy.moc.h>
#include <GuiFoundation/ActionViews/MenuActionMapView.moc.h>

ezMenuBarActionMapView::ezMenuBarActionMapView(QWidget* parent) : QMenuBar(parent)
{
}

ezMenuBarActionMapView::~ezMenuBarActionMapView()
{
  ClearView();
}

ezResult ezMenuBarActionMapView::SetActionContext(const ezActionContext& context)
{
  auto pMap = ezActionMapManager::GetActionMap(context.m_sMapping);

  if (pMap == nullptr)
    return EZ_FAILURE;

  m_pActionMap = pMap;
  m_Context = context;

  CreateView();

  return EZ_SUCCESS;
}

void ezMenuBarActionMapView::ClearView()
{
  for (auto it = m_Proxies.GetIterator(); it.IsValid(); ++it)
  {
    ezQtProxy* pProxy = it.Value();
    pProxy->deleteLater();
  }
  m_Proxies.Clear();
}

void ezMenuBarActionMapView::CreateView()
{
  ClearView();

  auto pObject = m_pActionMap->GetRootObject();

  for (auto pChild : pObject->GetChildren())
  {
    auto pDesc = m_pActionMap->GetDescriptor(pChild);
    auto pAction = pDesc->m_hAction.GetDescriptor()->CreateAction(m_Context);

    ezQtProxy* pProxy = ezRttiMappedObjectFactory<ezQtProxy>::CreateObject(pAction->GetDynamicRTTI());
    m_Proxies[pChild->GetGuid()] = pProxy;
    pProxy->setParent(this);
    pProxy->SetAction(pAction);

    switch (pDesc->m_hAction.GetDescriptor()->m_Type)
    {
    case ezActionType::Action:
      {
        EZ_REPORT_FAILURE("Cannot map actions in a menubar view!");
      }
      break;

    case ezActionType::Category:
      {
        EZ_REPORT_FAILURE("Cannot map category in a menubar view!");
      }
      break;

    case ezActionType::Menu:
      {
        QMenu* pQtMenu = static_cast<ezQtMenuProxy*>(pProxy)->GetQMenu();
        addMenu(pQtMenu);

        ezMenuActionMapView::AddDocumentObjectToMenu(m_Proxies, m_Context, m_pActionMap, pQtMenu, pChild);
      }
      break;
    }
  }
}
