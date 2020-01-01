#pragma once

#include <LibGUI/GJsonArrayModel.h>
#include <LibGUI/GWidget.h>

class CTimer;
class GJsonArrayModel;
class GTableView;

class ProcessMemoryMapWidget final : public GWidget {
    C_OBJECT(ProcessMemoryMapWidget);
public:
    explicit ProcessMemoryMapWidget(GWidget* parent);
    virtual ~ProcessMemoryMapWidget() override;

    void set_pid(pid_t);
    void refresh();

private:
    bool m_refresh_failed = false;
    pid_t m_pid { -1 };
    RefPtr<GTableView> m_table_view;
    RefPtr<GJsonArrayModel> m_json_model;
    RefPtr<GModel> m_stock_model;
    RefPtr<CTimer> m_timer;
};
